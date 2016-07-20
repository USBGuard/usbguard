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
#pragma once

#include <Typedefs.hpp>
#include <Rule.hpp>
#include <USB.hpp>
#include <mutex>

namespace usbguard {
  class DevicePrivate;
  class DLL_PUBLIC Device
  {
  public:
    Device();
    ~Device();
    Device(const Device& rhs);
    Device(const Rule& device_rule);
    const Device& operator=(const Device& rhs);
    
    std::mutex& refDeviceMutex();
    Pointer<Rule> getDeviceRule(bool include_port = true);
    String getHash(bool include_port = false) const;

    void setID(uint32_t id);
    uint32_t getID() const;

    void setTarget(Rule::Target target);
    Rule::Target getTarget() const;

    void setName(const String& name);
    const String& getName() const;

    void setDeviceID(const USBDeviceID& device_id);
    const USBDeviceID& getDeviceID() const;

    void setPort(const String& port);
    const String& getPort() const;

    void setSerial(const String& serial_number);
    const String& getSerial() const;

    std::vector<USBInterfaceType>& refMutableInterfaceTypes();
    const std::vector<USBInterfaceType>& getInterfaceTypes() const;

    virtual bool isController() const = 0;

    void loadDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
    void loadConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
    void loadInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
    void loadEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);

  private:
    DevicePrivate *d_pointer;
  };
} /* namespace usbguard */
