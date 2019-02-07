//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "Rule.hpp"
#include "Typedefs.hpp"
#include "USB.hpp"

#include <istream>
#include <mutex>
#include <string>
#include <memory>

#include <cstddef>
#include <cstdint>

namespace usbguard
{
  class DeviceManager;
  class DevicePrivate;
  class DLL_PUBLIC Device
  {
  public:
    Device(DeviceManager& manager);
    ~Device();
    Device(const Device& rhs);
    const Device& operator=(const Device& rhs);

    DeviceManager& manager() const;

    std::mutex& refDeviceMutex();
    std::shared_ptr<Rule> getDeviceRule(bool with_port = true, bool with_parent_hash = true, bool match_rule = false);
    std::string hashString(const std::string& value) const;
    void initializeHash();
    void updateHash(const void* ptr, size_t size);
    void updateHash(std::istream& descriptor_stream, size_t expected_size);
    std::string finalizeHash();
    const std::string& getHash() const;

    void setParentHash(const std::string& hash);

    void setID(uint32_t id);
    uint32_t getID() const;

    void setParentID(uint32_t id);
    uint32_t getParentID() const;

    void setTarget(Rule::Target target);
    Rule::Target getTarget() const;

    void setName(const std::string& name);
    const std::string& getName() const;

    void setDeviceID(const USBDeviceID& device_id);
    const USBDeviceID& getDeviceID() const;

    void setPort(const std::string& port);
    const std::string& getPort() const;

    void setSerial(const std::string& serial_number);
    const std::string& getSerial() const;

    void setConnectType(const std::string& connect_type);
    const std::string& getConnectType() const;

    std::vector<USBInterfaceType>& refMutableInterfaceTypes();
    const std::vector<USBInterfaceType>& getInterfaceTypes() const;

    virtual bool isController() const = 0;
    virtual std::string getSystemName() const = 0;

    void loadDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
    void loadConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
    void loadInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
    void loadEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);

  private:
    std::unique_ptr<DevicePrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
