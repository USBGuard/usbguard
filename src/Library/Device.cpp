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

  uint32_t Device::getSeqn() const
  {
    return d_pointer->getSeqn();
  }

  String Device::getDeviceHash(const bool include_port) const
  {
    return d_pointer->getDeviceHash(include_port);
  }

  const String Device::getPort() const
  {
    return d_pointer->getPort();
  }

  const String& Device::getSerialNumber() const
  {
    return d_pointer->getSerialNumber();
  }

  const std::vector<USBInterfaceType>& Device::getInterfaceTypes() const
  {
    return d_pointer->getInterfaceTypes();
  }

  void Device::setSeqn(const uint32_t seqn)
  {
    d_pointer->setSeqn(seqn);
    return;
  }

  void Device::setTarget(const Rule::Target target)
  {
    d_pointer->setTarget(target);
    return;
  }

  void Device::setDeviceName(const String& name)
  {
    d_pointer->setDeviceName(name);
    return;
  }

  void Device::setVendorID(const String& vendor_id)
  {
    d_pointer->setVendorID(vendor_id);
    return;
  }

  void Device::setProductID(const String& product_id)
  {
    d_pointer->setProductID(product_id);
    return;
  }

  void Device::setDevicePort(const String& port)
  {
    d_pointer->setDevicePort(port);
    return;
  }

  void Device::setSerialNumber(const String& serial_number)
  {
    d_pointer->setSerialNumber(serial_number);
    return;
  }

  std::vector<USBInterfaceType>& Device::refInterfaceTypes()
  {
    return d_pointer->refInterfaceTypes();
  }

  void Device::loadDeviceDescriptor(const USBDeviceDescriptor* const descriptor)
  {
    d_pointer->loadDeviceDescriptor(descriptor);
    return;
  }

  void Device::loadConfigurationDescriptor(const int c_num, const USBConfigurationDescriptor* const descriptor)
  {
    d_pointer->loadConfigurationDescriptor(c_num, descriptor);
    return;
  }

  void Device::loadInterfaceDescriptor(const int c_num, const int i_num, const USBInterfaceDescriptor* const descriptor)
  {
    d_pointer->loadInterfaceDescriptor(c_num, i_num, descriptor);
    return;
  }

} /* namespace usbguard */
