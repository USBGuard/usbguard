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
#include "DevicePrivate.hpp"
#include "Common/Logging.hpp"
#include <mutex>
#include <sodium.h>

namespace usbguard {
  DevicePrivate::DevicePrivate(Device& p_instance)
    : _p_instance(p_instance)
  {
    _seqn = Rule::SeqnDefault;
    _target = Rule::Target::Unknown;
    _num_configurations = -1;
    _num_interfaces = -1;
    return;
  }

  DevicePrivate::DevicePrivate(Device& p_instance, const DevicePrivate& rhs)
    : _p_instance(p_instance)
  {
    *this = rhs;
    return;
  }
  
  const DevicePrivate& DevicePrivate::operator=(const DevicePrivate& rhs)
  {
    _seqn = rhs._seqn;
    _target = rhs._target;
    _name = rhs._name;
    _vendor_id = rhs._vendor_id;
    _product_id = rhs._product_id;
    _serial_number = rhs._serial_number;
    _port = rhs._port;
    _interface_types = rhs._interface_types;
    _num_configurations = rhs._num_configurations;
    _num_interfaces = rhs._num_interfaces;
    return *this;
  }
  
  std::mutex& DevicePrivate::refDeviceMutex()
  {
    return _mutex;
  }

  Pointer<Rule> DevicePrivate::getDeviceRule(bool include_port)
  {
    Pointer<Rule> device_rule = makePointer<Rule>();
    std::unique_lock<std::mutex> device_lock(refDeviceMutex());

    device_rule->setSeqn(_seqn);
    device_rule->setTarget(_target);
    device_rule->setVendorID(_vendor_id);
    device_rule->setProductID(_product_id);
    device_rule->setSerialNumber(_serial_number);

    if (include_port) {
      device_rule->refDevicePorts().push_back(_port);
      device_rule->setDevicePortsSetOperator(Rule::SetOperator::Equals);
    }

    device_rule->setInterfaceTypes(refInterfaceTypes());
    device_rule->setInterfaceTypesSetOperator(Rule::SetOperator::Equals);
    device_rule->setDeviceName(_name);
    device_rule->setDeviceHash(getDeviceHash(/*include_port=*/false));
    
    return std::move(device_rule);
  }

  uint32_t DevicePrivate::getSeqn() const
  {
    return _seqn;
  }

  String DevicePrivate::getDeviceHash(bool include_port) const
  {
    unsigned char hash[crypto_generichash_BYTES_MIN];
    crypto_generichash_state state;
    std::string hash_string;

    if (_vendor_id.empty() || _product_id.empty()) {
      throw std::runtime_error("Cannot compute device hash value. Vendor ID and/or Product ID empty.");
    }

    crypto_generichash_init(&state, NULL, 0, sizeof hash);

    for (auto field : {
	&_name, &_vendor_id, &_product_id, &_serial_number }) {
      /* Update the hash value */
      crypto_generichash_update(&state, (const uint8_t *)field->c_str(), field->size());
    }

    /* Finalize the hash value */
    crypto_generichash_final(&state, hash, sizeof hash);

    /* Binary => Hex string conversion */
    const size_t hexlen = crypto_generichash_BYTES_MIN * 2 + 1;
    char hexval[hexlen];
    sodium_bin2hex(hexval, hexlen, hash, sizeof hash);

    return String(hexval, hexlen - 1);
  }

  const String DevicePrivate::getPort() const
  {
    return _port;
  }

  const std::vector<USBInterfaceType>& DevicePrivate::getInterfaceTypes() const
  {
    return _interface_types;
  }

  void DevicePrivate::setSeqn(uint32_t seqn)
  {
    _seqn = seqn;
    return;
  }

  void DevicePrivate::setTarget(Rule::Target target)
  {
    _target = target;
    return;
  }

  void DevicePrivate::setDeviceName(const String& name)
  {
    _name = name;
    return;
  }

  void DevicePrivate::setVendorID(const String& vendor_id)
  {
    _vendor_id = vendor_id;
    return;
  }

  void DevicePrivate::setProductID(const String& product_id)
  {
    _product_id = product_id;
    return;
  }

  void DevicePrivate::setDevicePort(const String& port)
  {
    _port = port;
    return;
  }

  void DevicePrivate::setSerialNumber(const String& serial_number)
  {
    _serial_number = serial_number;
    return;
  }

  std::vector<USBInterfaceType>& DevicePrivate::refInterfaceTypes()
  {
    return _interface_types;
  }

  void DevicePrivate::loadDeviceDescriptor(const USBDeviceDescriptor* descriptor)
  {
    _num_configurations = descriptor->bNumConfigurations;
    return;
  }

  void DevicePrivate::loadConfigurationDescriptor(int c_num, const USBConfigurationDescriptor* descriptor)
  {
    _num_interfaces += descriptor->bNumInterfaces;
    return;
  }

  void DevicePrivate::loadInterfaceDescriptor(int c_num, int i_num, const USBInterfaceDescriptor* descriptor)
  {
    USBInterfaceType interface_type(*descriptor);
    _interface_types.push_back(interface_type);
    //log->debug("Added interface type: {}", interface_type.typeString());
    return;
  }

} /* namespace usbguard */
