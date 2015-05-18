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
#include "LoggerPrivate.hpp"
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

  Pointer<Rule> DevicePrivate::getDeviceRule(const bool include_port)
  {
    Pointer<Rule> device_rule = makePointer<Rule>();
    std::unique_lock<std::mutex> device_lock(refDeviceMutex());

    logger->trace("Generating rule for device {}:{}@{} (name={}); include_port={}",
		  _vendor_id, _product_id, _port, _name, include_port);

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

  String DevicePrivate::getDeviceHash(const bool include_port) const
  {
    unsigned char hash[crypto_generichash_BYTES_MIN];
    crypto_generichash_state state;

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

    const std::string hash_string(hexval, hexlen - 1);
    return std::move(hash_string);
  }

  const String DevicePrivate::getPort() const
  {
    return _port;
  }

  const String& DevicePrivate::getSerialNumber() const
  {
    return _serial_number;
  }

  const std::vector<USBInterfaceType>& DevicePrivate::getInterfaceTypes() const
  {
    return _interface_types;
  }

  void DevicePrivate::setSeqn(const uint32_t seqn)
  {
    _seqn = seqn;
    return;
  }

  void DevicePrivate::setTarget(const Rule::Target target)
  {
    _target = target;
    return;
  }

  void DevicePrivate::setDeviceName(const String& name)
  {
    if (name.size() > USB_GENERIC_STRING_MAX_LENGTH) {
      throw std::runtime_error("setDeviceName: value size out-of-range");
    }
    _name = name;
    return;
  }

  void DevicePrivate::setVendorID(const String& vendor_id)
  {
    if (vendor_id.size() > USB_VID_STRING_MAX_LENGTH) {
      throw std::runtime_error("setVendorID: value size out-of-range");
    }
    _vendor_id = vendor_id;
    return;
  }

  void DevicePrivate::setProductID(const String& product_id)
  {
    if (product_id.size() > USB_PID_STRING_MAX_LENGTH) {
      throw std::runtime_error("setProductID: value size out-of-range");
    }
    _product_id = product_id;
    return;
  }

  void DevicePrivate::setDevicePort(const String& port)
  {
    if (port.size() > USB_PORT_STRING_MAX_LENGTH) {
      throw std::runtime_error("setDevicePort: value size out-of-range");
    }
    _port = port;
    return;
  }

  void DevicePrivate::setSerialNumber(const String& serial_number)
  {
    if (serial_number.size() > USB_GENERIC_STRING_MAX_LENGTH) {
      throw std::runtime_error("setSerialNumber: value size out-of-range");
    }
    _serial_number = serial_number;
    return;
  }

  std::vector<USBInterfaceType>& DevicePrivate::refInterfaceTypes()
  {
    return _interface_types;
  }

  void DevicePrivate::loadDeviceDescriptor(const USBDeviceDescriptor* const descriptor)
  {
    logger->trace("Loading device descriptor for device {}:{}@{} (name={}); descriptor={:p}",
		  _vendor_id, _product_id, _port, _name, (void *)descriptor);

    if (descriptor == nullptr) {
      throw std::runtime_error("loadDeviceDescriptor: NULL descriptor");
    }
    _num_configurations = descriptor->bNumConfigurations;
    return;
  }

  void DevicePrivate::loadConfigurationDescriptor(const int c_num, const USBConfigurationDescriptor* const descriptor)
  {
    logger->trace("Loading configuration descriptor {} for device {}:{}@{} (name={}); descriptor={:p}",
		  c_num, _vendor_id, _product_id, _port, _name, (void *)descriptor);

    if (c_num < 0 || c_num >= _num_configurations) {
      throw std::runtime_error("loadConfigurationDescriptor: configuration index out-of-range");
    }
    if (descriptor == nullptr) {
      throw std::runtime_error("loadConfigurationDescriptor: NULL descriptor");
    }

    logger->debug("Increasing interface count");
    logger->debug(" from: {}", _num_interfaces);
    //
    _num_interfaces += descriptor->bNumInterfaces;
    //
    logger->debug(" to: {} (+{:d})", _num_interfaces, descriptor->bNumInterfaces);

    return;
  }

  void DevicePrivate::loadInterfaceDescriptor(const int c_num, const int i_num, const USBInterfaceDescriptor* const descriptor)
  {
    logger->trace("Loading interface descriptor {}-{} for device {}:{}@{} (name={}); descriptor={:p}",
		  c_num, i_num, _vendor_id, _product_id, _port, _name, (void *)descriptor);

    if (c_num < 0 || c_num >= _num_configurations) {
      throw std::runtime_error("loadInterfaceDescriptor: configuration index out-of-range");
    }
    if (i_num < 0 || i_num >= _num_interfaces) {
      throw std::runtime_error("loadInterfaceDescriptor: interface index out-of-range");
    }
    if (descriptor == nullptr) {
      throw std::runtime_error("loadInterfaceDescriptor: NULL descriptor");
    }

    const USBInterfaceType interface_type(*descriptor);
    _interface_types.push_back(interface_type);

    return;
  }

} /* namespace usbguard */
