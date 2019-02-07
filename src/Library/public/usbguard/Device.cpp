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

#include "DevicePrivate.hpp"
#include "Common/Utility.hpp"

namespace usbguard
{
  Device::Device(DeviceManager& manager)
    : d_pointer(usbguard::make_unique<DevicePrivate>(*this, manager))
  {
  }

  Device::~Device() = default;

  Device::Device(const Device& rhs)
    : d_pointer(usbguard::make_unique<DevicePrivate>(*this, *rhs.d_pointer))
  {
  }

  const Device& Device::operator=(const Device& rhs)
  {
    d_pointer.reset(new DevicePrivate(*this, *rhs.d_pointer));
    return *this;
  }

  DeviceManager& Device::manager() const
  {
    return d_pointer->manager();
  }

  std::mutex& Device::refDeviceMutex()
  {
    return d_pointer->refDeviceMutex();
  }

  std::shared_ptr<Rule> Device::getDeviceRule(const bool with_port, const bool with_parent_hash, const bool match_rule)
  {
    return d_pointer->getDeviceRule(with_port, with_parent_hash, match_rule);
  }

  std::string Device::hashString(const std::string& value) const
  {
    return d_pointer->hashString(value);
  }

  void Device::initializeHash()
  {
    d_pointer->initializeHash();
  }

  void Device::updateHash(const void* const ptr, const size_t size)
  {
    d_pointer->updateHash(ptr, size);
  }

  void Device::updateHash(std::istream& descriptor_stream, const size_t expected_size)
  {
    d_pointer->updateHash(descriptor_stream, expected_size);
  }

  std::string Device::finalizeHash()
  {
    return d_pointer->finalizeHash();
  }

  const std::string& Device::getHash() const
  {
    return d_pointer->getHash();
  }

  void Device::setParentHash(const std::string& hash)
  {
    d_pointer->setParentHash(hash);
  }

  void Device::setID(uint32_t id)
  {
    d_pointer->setID(id);
  }

  uint32_t Device::getID() const
  {
    return d_pointer->getID();
  }

  void Device::setParentID(uint32_t id)
  {
    d_pointer->setParentID(id);
  }

  uint32_t Device::getParentID() const
  {
    return d_pointer->getParentID();
  }

  void Device::setTarget(Rule::Target target)
  {
    d_pointer->setTarget(target);
  }

  Rule::Target Device::getTarget() const
  {
    return d_pointer->getTarget();
  }

  void Device::setName(const std::string& name)
  {
    d_pointer->setName(name);
  }

  const std::string& Device::getName() const
  {
    return d_pointer->getName();
  }

  void Device::setDeviceID(const USBDeviceID& device_id)
  {
    d_pointer->setDeviceID(device_id);
  }

  const USBDeviceID& Device::getDeviceID() const
  {
    return d_pointer->getDeviceID();
  }

  void Device::setPort(const std::string& port)
  {
    d_pointer->setPort(port);
  }

  const std::string& Device::getPort() const
  {
    return d_pointer->getPort();
  }

  void Device::setSerial(const std::string& serial_number)
  {
    d_pointer->setSerial(serial_number);
  }

  const std::string& Device::getSerial() const
  {
    return d_pointer->getSerial();
  }

  void Device::setConnectType(const std::string& connect_type)
  {
    d_pointer->setConnectType(connect_type);
  }

  const std::string& Device::getConnectType() const
  {
    return d_pointer->getConnectType();
  }

  std::vector<USBInterfaceType>& Device::refMutableInterfaceTypes()
  {
    return d_pointer->refMutableInterfaceTypes();
  }

  const std::vector<USBInterfaceType>& Device::getInterfaceTypes() const
  {
    return d_pointer->getInterfaceTypes();
  }

  void Device::loadDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadDeviceDescriptor(parser, descriptor);
  }

  void Device::loadConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadConfigurationDescriptor(parser, descriptor);
  }

  void Device::loadInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadInterfaceDescriptor(parser, descriptor);
  }

  void Device::loadEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadEndpointDescriptor(parser, descriptor);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
