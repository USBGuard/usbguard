//
// Copyright (C) 2016 Red Hat, Inc.
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
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "SysFSDevice.hpp"

#include "usbguard/Device.hpp"
#include "usbguard/USB.hpp"

namespace usbguard
{
  class DeviceManagerBase;

  class DeviceBase : public Device, public USBDescriptorParserHooks
  {
  public:
    DeviceBase(DeviceManagerBase& device_manager, SysFSDevice& sysfs_device);

    SysFSDevice& sysfsDevice();
    const std::string& getSysPath() const;
    bool isController() const override;
    std::string getSystemName() const override;

  private:
    void parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
      USBDescriptor* descriptor_out) override;
    void loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor) override;
    bool isLinuxRootHubDeviceDescriptor(const USBDescriptor* descriptor);
    void updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* descriptor);

    SysFSDevice _sysfs_device;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
