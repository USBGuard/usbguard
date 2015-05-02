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
#include "LinuxDeviceManager.hpp"
#include "LinuxSysIO.hpp"
#include "Common/Logging.hpp"
#include <USB.hpp>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <stdexcept>
#include <fstream>
#include <unistd.h>

namespace usbguard {

  LinuxDevice::LinuxDevice(struct udev_device* dev)
  {
    //log->debug("Creating a new LinuxDevice instance");
    const char *name = udev_device_get_sysattr_value(dev, "product");
    if (name) {
      //log->debug("DeviceName={}", name);
      setDeviceName(name);
    }
    
    const char *id_vendor = udev_device_get_sysattr_value(dev, "idVendor");
    if (id_vendor) {
      //log->debug("VendorID={}", id_vendor);
      setVendorID(id_vendor);
    }

    const char *id_product = udev_device_get_sysattr_value(dev, "idProduct");
    if (id_product) {
      //log->debug("ProductID={}", id_product);
      setProductID(id_product);
    }

    const char *serial = udev_device_get_sysattr_value(dev, "serial");
    if (serial) {
      //log->debug("Serial={}", serial);
      setSerialNumber(serial);
    }

    const char *syspath = udev_device_get_syspath(dev);
    if (syspath) {
      //log->debug("Syspath={}", syspath);
      _syspath = syspath;
    } else {
      throw std::runtime_error("device wihtout syspath");
    }

    const char *sysname = udev_device_get_sysname(dev);
    if (sysname) {
      //log->debug("Sysname={}", sysname);
      setDevicePort(sysname);
    } else {
      throw std::runtime_error("device wihtout sysname");
    }

    //log->debug("DeviceHash={}", getDeviceHash());

    setTarget(Rule::Target::Unknown);

    std::ifstream descriptor_stream(_syspath + "/descriptors", std::ifstream::binary);
    if (!descriptor_stream.good()) {
      throw std::runtime_error("cannot load USB descriptors");
    }
    else {
      readDescriptors(descriptor_stream);
    }

    return;
  }

  void LinuxDevice::readDescriptors(std::istream& stream)
  {
    char buffer[sizeof(USBDeviceDescriptor)];
    stream.read(buffer, sizeof buffer);

    const USBDeviceDescriptor descriptor = \
      USBParseDeviceDescriptor(buffer, stream.gcount());
    loadDeviceDescriptor(&descriptor);

    for (size_t c = 0; c < descriptor.bNumConfigurations; ++c) {
      readConfiguration(c, stream);
    }

    return;
  }

  void LinuxDevice::readConfiguration(int c_num, std::istream& stream)
  {
    char buffer[sizeof(USBConfigurationDescriptor)];
    stream.read(buffer, sizeof buffer);

    const USBConfigurationDescriptor descriptor = \
      USBParseConfigurationDescriptor(buffer, stream.gcount());
    loadConfigurationDescriptor(c_num, &descriptor);

    for (size_t i = 0; i < descriptor.bNumInterfaces; ++i) {
      readInterfaceDescriptor(c_num, i, stream);
    }

    return;
  }

  void LinuxDevice::readInterfaceDescriptor(int c_num, int i_num, std::istream& stream)
  {
    char buffer[sizeof(USBInterfaceDescriptor)];
    stream.read(buffer, sizeof buffer);

    const USBInterfaceDescriptor descriptor = \
      USBParseInterfaceDescriptor(buffer, stream.gcount());
    loadInterfaceDescriptor(c_num, i_num, &descriptor);

    return;
  }

  const String& LinuxDevice::getSysPath() const
  {
    return _syspath;
  }

  bool LinuxDevice::isController() const
  {
    if (getPort().substr(0, 3) != "usb" || getInterfaceTypes().size() != 1) {
      return false;
    }

    const USBInterfaceType hub_interface("09:00:*");

    return hub_interface.appliesTo(getInterfaceTypes()[0]);
  }

  /*
   * Manager
   */
  LinuxDeviceManager::LinuxDeviceManager(DeviceManagerHooks& hooks)
    : DeviceManager(hooks),
      _thread(this, &LinuxDeviceManager::thread)
  {
    setDefaultBlockedState(/*state=*/true);

    if ((_event_fd = eventfd(0, 0)) < 0) {
      throw std::runtime_error("eventfd init error");
    }
    
    if ((_udev = udev_new()) == nullptr) {
      throw std::runtime_error("udev init error");
    }

    if ((_umon = udev_monitor_new_from_netlink(_udev, "udev")) == nullptr) {
      udev_unref(_udev);
      throw std::runtime_error("udev_monitor init error");
    }

    udev_monitor_filter_add_match_subsystem_devtype(_umon, "usb", "usb_device");
    return;
  }

  LinuxDeviceManager::~LinuxDeviceManager()
  {
    setDefaultBlockedState(/*state=*/false); // FIXME: Set to previous state
    stop();
    udev_monitor_unref(_umon);
    udev_unref(_udev);
    close(_event_fd);
    return;
  }

  void LinuxDeviceManager::setDefaultBlockedState(bool state)
  {
    sysioSetAuthorizedDefault(!state);
    return;
  }

  void LinuxDeviceManager::start()
  {
    // enumerate devices
    // broadcast present devices
    // start monitor thread
    _thread.start();
    return;
  }

  void LinuxDeviceManager::stop()
  {
    // stop monitor
    _thread.stop(/*do_wait=*/false);
    { /* Wakeup the device manager thread */
      const uint64_t one = 1;
      write(_event_fd, &one, sizeof one);
    }
    _thread.wait();
    return;
  }

  Pointer<Device> LinuxDeviceManager::allowDevice(uint32_t seqn)
  {
    //log->debug("Allowing device {}", seqn);
    Pointer<Device> device = applyDevicePolicy(seqn, Rule::Target::Allow);
    //log->debug("Calling DeviceAllowed hook");
    DeviceAllowed(device);
    return device;
  }

  Pointer<Device> LinuxDeviceManager::blockDevice(uint32_t seqn)
  {
    //log->debug("Blocking device {}", seqn);
    Pointer<Device> device = applyDevicePolicy(seqn, Rule::Target::Block);
    //log->debug("Calling DeviceBlocked hook");
    DeviceBlocked(device);
    return device;
  }

  Pointer<Device> LinuxDeviceManager::rejectDevice(uint32_t seqn)
  {
    //log->debug("Rejecting device {}", seqn);
    Pointer<Device> device = applyDevicePolicy(seqn, Rule::Target::Reject);
    //log->debug("Calling DeviceRejected hook");
    DeviceRejected(device);
    return device;
  }

  Pointer<Device> LinuxDeviceManager::applyDevicePolicy(uint32_t seqn, Rule::Target target)
  {
    //log->debug("Applying device policy {} to device {}", target, seqn);
    Pointer<LinuxDevice> device = std::static_pointer_cast<LinuxDevice>(getDevice(seqn));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());

    const char *target_file = nullptr;
    int target_value = 0;

    switch (target)
      {
      case Rule::Target::Allow:
	target_file = "authorized";
	target_value = 1;
	break;
      case Rule::Target::Block:
	target_file = "authorized";
	target_value = 0;
	break;
      case Rule::Target::Reject:
	target_file = "remove";
	target_value = 1;
	break;
      default:
	//log->critical("BUG: unknown rule target");
	throw std::runtime_error("Uknown rule target in applyDevicePolicy");
      }

    char sysio_path[SYSIO_PATH_MAX];
    snprintf(sysio_path, SYSIO_PATH_MAX, "%s/%s",
	     device->getSysPath().c_str(), target_file);
    /* FIXME: check that snprintf wrote the whole path */
    //log->debug("SysIO: writing '{}' to {}", target_value, sysio_path);
    sysioWrite(sysio_path, target_value);

    return std::move(device);
  }

  void LinuxDeviceManager::thread()
  {
    //log->debug("Entering LinuxDeviceManager thread");

    const int umon_fd = udev_monitor_get_fd(_umon);
    const int max_fd = umon_fd > _event_fd ? umon_fd : _event_fd;
    fd_set readset;

    udev_monitor_enable_receiving(_umon);
    udevEnumerateDevices();

    while (!_thread.stopRequested()) {
      struct timeval tv_timeout = { 5, 0 };
      
      FD_ZERO(&readset);
      FD_SET(umon_fd, &readset);
      FD_SET(_event_fd, &readset);
      
      switch (select(max_fd + 1, &readset, NULL, NULL, &tv_timeout)) {
      case 1: /* Device or event */
      case 2: /* Device and event */
	if (FD_ISSET(_event_fd, &readset)) {
	  //log->debug("Wakeup event received");
	  continue;
	}
	if (FD_ISSET(umon_fd, &readset)) {
	  //log->debug("Handling UDev read event");
	  udevReceiveDevice();
	}
	break;
      case 0: /* Timeout */
	continue;
      case -1: /* Error */
      default:
	//log->debug("Select failure: {}", errno);
	_thread.stop();
      }
    } /* Thread main loop */
    //log->debug("Returning from LinuxDeviceManager thread");
    return;
  }

  void LinuxDeviceManager::udevReceiveDevice()
  {
    struct udev_device *dev = udev_monitor_receive_device(_umon);

    if (!dev) {
      return;
    }

    const char *action_cstr = udev_device_get_action(dev);

    if (!action_cstr) {
      //log->warn("BUG? Device event witout action value.");
      udev_device_unref(dev);
      return;
    }

    if (strcmp(action_cstr, "add") == 0) {
      processDeviceInsertion(dev);
    }
    else if (strcmp(action_cstr, "remove") == 0) {
      processDeviceRemoval(dev);
    }
    else {
      //log->warn("BUG? Unknown device action value \"{}\"", action_cstr);
    }

    udev_device_unref(dev);
    return;
  }

  void LinuxDeviceManager::udevEnumerateDevices()
  {
    struct udev_enumerate *enumerate = udev_enumerate_new(_udev);

    if (enumerate == nullptr) {
      //log->debug("udev_enumerate_new returned NULL");
      throw std::runtime_error("udev_enumerate_new returned NULL");
    }

    udev_enumerate_add_match_subsystem(enumerate, "usb");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *dlentry = nullptr;

    udev_list_entry_foreach(dlentry, devices) {
      const char *syspath = udev_list_entry_get_name(dlentry);

      if (syspath == nullptr) {
	//log->warn("Received NULL syspath for en UDev enumerated device. Ignoring.");
	continue;
      }

      struct udev_device *device = udev_device_new_from_syspath(_udev, syspath);

      if (device == nullptr) {
	//log->warn("Cannot create a new device from syspath {}. Ignoring.", syspath);
	continue;
      }

      const char *devtype = udev_device_get_devtype(device);

      if (devtype == nullptr) {
	//log->warn("Cannot get device type for device at syspath {}. Ignoring.", syspath);
	udev_device_unref(device);
	continue;
      }

      if (strcmp(devtype, "usb_device") == 0) {
	processDevicePresence(device);
      }

      udev_device_unref(device);
    }

    udev_enumerate_unref(enumerate);
    return;
  }

  void LinuxDeviceManager::processDevicePresence(struct udev_device *dev)
  {
    //log->debug("Processing device presence");
    Pointer<LinuxDevice> device = makePointer<LinuxDevice>(dev);
    insertDevice(device);
    DevicePresent(device);
    return;
  }

  void LinuxDeviceManager::processDeviceInsertion(struct udev_device *dev)
  {
    //log->debug("Processing device insertion");
    Pointer<LinuxDevice> device = makePointer<LinuxDevice>(dev);
    insertDevice(device);
    DeviceInserted(device);
    return;
  }

  void LinuxDeviceManager::insertDevice(Pointer<Device> device)
  {
    DeviceManager::insertDevice(device);
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    _syspath_map[std::static_pointer_cast<LinuxDevice>(device)->getSysPath()] = device->getSeqn();
    return;
  }

  void LinuxDeviceManager::processDeviceRemoval(struct udev_device *dev)
  {
    //log->debug("Processing device removal");
    const char *syspath_cstr = udev_device_get_syspath(dev);
    if (!syspath_cstr) {
      return;
    }
    const String syspath(syspath_cstr);
    try {
      Pointer<Device> device = removeDevice(syspath);
      DeviceRemoved(device);
    } catch(...) {
      /* Ignore for now */
      //log->debug("Removal of an unknown device ignored.");
      return;
    }
    return;
  }

  Pointer<Device> LinuxDeviceManager::removeDevice(const String& syspath)
  {
    /* FIXME: device map locking */
    auto it = _syspath_map.find(syspath);
    if (it == _syspath_map.end()) {
      throw std::runtime_error("Unknown device, cannot remove from syspath map");
    }
    const uint32_t seqn = it->second;
    Pointer<Device> device = DeviceManager::removeDevice(seqn);
    _syspath_map.erase(it);
    return device;
  }

} /* namespace usbguard */
