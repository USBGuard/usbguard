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
#include <Device.hpp>
#include "DevicePrivate.hpp"

namespace usbguard {
  Device::Device()
  {
    d_pointer = new DevicePrivate(*this);
    return;
  }

  Device::~Device()
  {
    delete d_pointer;
    d_pointer = nullptr;
    return;
  }

  Device::Device(const Device& rhs)
  {
    d_pointer = new DevicePrivate(*this, *rhs.d_pointer);
    return;
  }

  Device::Device(const Rule& device_rule)
  {
    d_pointer = new DevicePrivate(*this, device_rule);
    return;
  }

  const Device& Device::operator=(const Device &rhs)
  {
    DevicePrivate* n_pointer = new DevicePrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
  }
 
  std::mutex& Device::refDeviceMutex()
  {
    return d_pointer->refDeviceMutex();
  }

  Pointer<Rule> Device::getDeviceRule(const bool include_port)
  {
    return d_pointer->getDeviceRule(include_port);
  }

  void Device::updateHash(std::istream& descriptor_stream, const size_t expected_size)
  {
    d_pointer->updateHash(descriptor_stream, expected_size);
  }

  const String& Device::getHash() const
  {
    return d_pointer->getHash();
  }

  void Device::setID(uint32_t id)
  {
    d_pointer->setID(id);
  }

  uint32_t Device::getID() const
  {
    return d_pointer->getID();
  }

  void Device::setTarget(Rule::Target target)
  {
    d_pointer->setTarget(target);
  }

  Rule::Target Device::getTarget() const
  {
    return d_pointer->getTarget();
  }

  void Device::setName(const String& name)
  {
    d_pointer->setName(name);
  }

  const String& Device::getName() const
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

  void Device::setPort(const String& port)
  {
    d_pointer->setPort(port);
  }

  const String& Device::getPort() const
  {
    return d_pointer->getPort();
  }

  void Device::setSerial(const String& serial_number)
  {
    d_pointer->setSerial(serial_number);
  }

  const String& Device::getSerial() const
  {
    return d_pointer->getSerial();
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
    return;
  }

  void Device::loadConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadConfigurationDescriptor(parser, descriptor);
    return;
  }

  void Device::loadInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadInterfaceDescriptor(parser, descriptor);
    return;
  }

  void Device::loadEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    d_pointer->loadEndpointDescriptor(parser, descriptor);
    return;
  }
} /* namespace usbguard */
