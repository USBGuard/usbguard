//
// Copyright (C) 2015 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "DeviceBase.hpp"
#include "DeviceManagerBase.hpp"
#include "SysFSDevice.hpp"
#include "Common/FDInputStream.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Exception.hpp"
#include "usbguard/Logger.hpp"
#include "usbguard/USB.hpp"

namespace usbguard
{

  DeviceBase::DeviceBase(DeviceManagerBase& device_manager, SysFSDevice& sysfs_device)
    : Device(device_manager)
  {
    /*
     * Look for the parent USB device and set the parent id if we find one.
     */
    const std::string sysfs_parent_path(sysfs_device.getParentPath());
    const SysFSDevice sysfs_parent_device(sysfs_parent_path);

    if (sysfs_parent_device.getUEvent().getAttribute("DEVTYPE") == "usb_device") {
      setParentID(device_manager.getIDFromSysfsPath(sysfs_parent_path));
    }
    else {
      setParentID(Rule::RootID);
      setParentHash(hashString(sysfs_parent_path));
    }

    /*
     * Set name
     */
    setName(sysfs_device.readAttribute("product", /*strip_last_null=*/true, /*optional=*/true));
    /*
     * Set USB ID
     */
    const std::string id_vendor(sysfs_device.readAttribute("idVendor", /*strip_last_null=*/true));
    const std::string id_product(sysfs_device.readAttribute("idProduct", /*strip_last_null=*/true));
    const USBDeviceID device_id(id_vendor, id_product);
    setDeviceID(device_id);
    /*
     * Set serial number
     */
    setSerial(sysfs_device.readAttribute("serial", /*strip_last_null=*/true, /*optional=*/true));
    /*
     * Set USB port
     */
    setPort(sysfs_device.getName());
    /*
     * Sync authorization target
     */
    const std::string authorized_value(sysfs_device.readAttribute("authorized", /*strip_last_null=*/true));

    if (authorized_value == "0") {
      setTarget(Rule::Target::Block);
    }
    else if (authorized_value == "1") {
      setTarget(Rule::Target::Allow);
    }
    else {
      /*
       * Block the device if we get an unexpected value
       */
      setTarget(Rule::Target::Block);
    }

    /*
     * Set connect type
     */
    int retry = 0;
    std::string connect_type;
    /*
     * Host controllers are directly connected to SoC/PCI bus
     * hence do not have port/connect_type in sysfs.
     * Filter out host controllers to avoid unnecessary wait in the loop.
     */
    const auto before = std::chrono::steady_clock::now();

    if (!hasPrefix(getPort(), "usb")) {
      while (retry < 30) {
        connect_type = sysfs_device.readAttribute("port/connect_type", /*strip_last_null=*/true, /*optional=*/true);

        if (connect_type != "") {
          break;
        }

        USBGUARD_LOG(Trace) << "connect_type is empty, will attempt again";
        usleep(1000);
        retry++;
      }

      const auto after = std::chrono::steady_clock::now();
      const auto duration_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count();
      USBGUARD_LOG(Info) << "***Setting connect type=" << connect_type <<" after "<< retry << " retries (took " <<
        duration_nanoseconds << "ns)";

      if (connect_type == "") {
        USBGUARD_LOG(Warning) << "port/connect_type is empty for non host controller device " << getPort() << " after waiting for " <<
          duration_nanoseconds << "ns";
      }
    }

    setConnectType(connect_type);
    /*
     * Process USB descriptor data.
     *
     * FDInputStream (stdio_filebuf) is responsible for closing the file
     * descriptor returned by sysfs_device.openAttribute().
     *
     */
    FDInputStream descriptor_stream(sysfs_device.openAttribute("descriptors"));

    if (!descriptor_stream.good()) {
      throw ErrnoException("DeviceBase", sysfs_device.getPath(), errno);
    }

    initializeHash();
    USBDescriptorParser parser(*this);

    if (parser.parse(descriptor_stream) < sizeof(USBDeviceDescriptor)) {
      throw Exception("DeviceBase", sysfs_device.getPath(),
        "USB descriptor parser processed less data than the size of a USB device descriptor");
    }

    finalizeHash();
    /*
     * From now on we take ownership of the SysFSDevice instance.
     */
    _sysfs_device = std::move(sysfs_device);
  }

  SysFSDevice& DeviceBase::sysfsDevice()
  {
    return _sysfs_device;
  }

  const std::string& DeviceBase::getSysPath() const
  {
    return _sysfs_device.getPath();
  }

  bool DeviceBase::isController() const
  {
    if (getPort().substr(0, 3) != "usb" || getInterfaceTypes().size() != 1) {
      return false;
    }

    const USBInterfaceType hub_interface("09:00:*");
    return hub_interface.appliesTo(getInterfaceTypes()[0]);
  }

  std::string DeviceBase::getSystemName() const
  {
    return getSysPath();
  }

  void DeviceBase::parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out)
  {
    USBGUARD_LOG(Trace);
    USBDescriptorParserHooks::parseUSBDescriptor(parser, descriptor_raw, descriptor_out);

    if (isLinuxRootHubDeviceDescriptor(descriptor_out)) {
      updateHashLinuxRootHubDeviceDescriptor(descriptor_raw);
    }
    else {
      updateHash(descriptor_raw, static_cast<size_t>(descriptor_raw->bHeader.bLength));
    }
  }

  void DeviceBase::loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor)
  {
    const auto type = static_cast<USBDescriptorType>(descriptor->bHeader.bDescriptorType);

    switch (type) {
    case USBDescriptorType::Device:
      loadDeviceDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::Configuration:
      loadConfigurationDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::Interface:
      loadInterfaceDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::Endpoint:
      loadEndpointDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::AssociationInterface:
    case USBDescriptorType::Unknown:
    case USBDescriptorType::String:
    default:
      USBGUARD_LOG(Debug) << "Ignoring descriptor: type=" << (int)type
        << " size=" << descriptor->bHeader.bLength;
    }
  }

  bool DeviceBase::isLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);

    if (descriptor->bHeader.bDescriptorType != USB_DESCRIPTOR_TYPE_DEVICE) {
      return false;
    }

    const USBDeviceDescriptor* const device_descriptor = \
      reinterpret_cast<const USBDeviceDescriptor*>(descriptor);

    if (device_descriptor->idVendor == 0x1d6b /* Linux Foundation */) {
      switch (device_descriptor->idProduct) {
      case 0x0001: /* 1.1 root hub */
      case 0x0002: /* 2.0 root hub */
      case 0x0003: /* 3.0 root hub */
        return true;

      default:
        return false;
      }
    }

    return false;
  }

  void DeviceBase::updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);
    USBDeviceDescriptor descriptor_modified = *reinterpret_cast<const USBDeviceDescriptor*>(descriptor);
    descriptor_modified.bcdDevice = 0;
    updateHash(&descriptor_modified, sizeof descriptor_modified);
  }

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
