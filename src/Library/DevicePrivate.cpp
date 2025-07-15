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
#include "Hash.hpp"
#include "Common/Utility.hpp"

#include "usbguard/DeviceManager.hpp"
#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"

#include <mutex>

namespace usbguard
{
  DevicePrivate::DevicePrivate(Device& p_instance, DeviceManager& manager)
    : //_p_instance(p_instance),
      _manager(manager)
  {
    (void)p_instance;
    _id = Rule::DefaultID;
    _parent_id = Rule::RootID;
    _target = Rule::Target::Unknown;
  }

  DevicePrivate::DevicePrivate(Device& p_instance, const DevicePrivate& rhs)
    : //_p_instance(p_instance),
      _manager(rhs._manager)
  {
    (void)p_instance;
    *this = rhs;
  }

  const DevicePrivate& DevicePrivate::operator=(const DevicePrivate& rhs)
  {
    _id = rhs._id;
    _parent_id = rhs._parent_id;
    _target = rhs._target;
    _name = rhs._name;
    _device_id = rhs._device_id;
    _serial_number = rhs._serial_number;
    _port = rhs._port;
    _interface_types = rhs._interface_types;
    _hash_base64 = rhs._hash_base64;
    return *this;
  }

  DeviceManager& DevicePrivate::manager() const
  {
    return _manager;
  }

  std::mutex& DevicePrivate::refDeviceMutex()
  {
    return _mutex;
  }

  std::shared_ptr<Rule> DevicePrivate::getDeviceRule(const bool with_port, const bool with_parent_hash, const bool match_rule)
  {
    USBGUARD_LOG(Trace) << "entry: "
      << " with_port=" << with_port
      << " with_parent_hash=" << with_parent_hash
      << " match_rule=" << match_rule;
    std::shared_ptr<Rule> device_rule = std::make_shared<Rule>();
    std::unique_lock<std::mutex> device_lock(refDeviceMutex());
    device_rule->setRuleID(_id);

    if (match_rule) {
      device_rule->setTarget(Rule::Target::Match);
    }
    else {
      device_rule->setTarget(_target);
    }

    device_rule->setDeviceID(_device_id);
    device_rule->setSerial(_serial_number);
    device_rule->setWithConnectType(_connect_type);

    if (with_port) {
      device_rule->setViaPort(_port);
    }

    device_rule->attributeWithInterface().set(getInterfaceTypes(), Rule::SetOperator::Equals);
    device_rule->setName(_name);
    device_rule->setHash(getHash());

    if (with_parent_hash) {
      if (!_parent_hash.empty()) {
        device_rule->setParentHash(_parent_hash);
      }
      else {
        if (_parent_id != Rule::RootID) {
          auto parent_device = manager().getDevice(_parent_id);
          device_rule->setParentHash(parent_device->getHash());
        }
        else {
          throw std::runtime_error("Cannot generate device rule: parent hash value not available");
        }
      }
    }

    USBGUARD_LOG(Trace) << "return:"
      << " device_rule=" << device_rule->toString();
    return device_rule;
  }

  std::string DevicePrivate::hashString(const std::string& value) const
  {
    Hash hash;
    hash.update(value);
    return hash.getBase64();
  }

  void DevicePrivate::initializeHash()
  {
    Hash hash;
    const std::string vendor_id = _device_id.getVendorID();
    const std::string product_id = _device_id.getProductID();

    if (vendor_id.empty() || product_id.empty()) {
      throw Exception("Device hash initialization", numberToString(getID()), "vendor and/or product id values not available");
    }

    /*
     * Hash name, device id and serial number fields.
     */
    for (const std::string& field : {
        _name, vendor_id, product_id, _serial_number
      }) {
      hash.update(field);
    }
    _hash = std::move(hash);
  }

  void DevicePrivate::updateHash(std::istream& descriptor_stream, const size_t expected_size)
  {
    Hash hash(_hash);

    if (hash.update(descriptor_stream) != expected_size) {
      throw Exception("Device hash update", numberToString(getID()), "descriptor stream returned less data than expected");
    }

    _hash = std::move(hash);
  }

  void DevicePrivate::updateHash(const void* const ptr, size_t size)
  {
    Hash hash(_hash);

    if (hash.update(ptr, size) != size) {
      throw Exception("Device hash update", numberToString(getID()), "hashed less data than expected");
    }

    _hash = std::move(hash);
  }

  std::string DevicePrivate::finalizeHash()
  {
    _hash_base64 = _hash.getBase64();
    return _hash_base64;
  }

  const std::string& DevicePrivate::getHash() const
  {
    if (_hash_base64.empty()) {
      throw USBGUARD_BUG("Accessing unfinalized device hash value");
    }

    return _hash_base64;
  }

  void DevicePrivate::setParentHash(const std::string& hash)
  {
    _parent_hash = hash;
  }

  void DevicePrivate::setID(uint32_t id)
  {
    _id = id;
  }

  uint32_t DevicePrivate::getID() const
  {
    return _id;
  }

  void DevicePrivate::setParentID(uint32_t id)
  {
    _parent_id = id;
  }

  uint32_t DevicePrivate::getParentID() const
  {
    return _parent_id;
  }

  void DevicePrivate::setTarget(Rule::Target target)
  {
    _target = target;
  }

  Rule::Target DevicePrivate::getTarget() const
  {
    return _target;
  }

  void DevicePrivate::setName(const std::string& name)
  {
    if (name.size() > USB_GENERIC_STRING_MAX_LENGTH) {
      throw Exception("DevicePrivate::setName", numberToString(getID()), "name string size out-of-range");
    }

    _name = name;
  }

  const std::string& DevicePrivate::getName() const
  {
    return _name;
  }

  void DevicePrivate::setDeviceID(const USBDeviceID& device_id)
  {
    _device_id = device_id;
  }

  const USBDeviceID& DevicePrivate::getDeviceID() const
  {
    return _device_id;
  }

  void DevicePrivate::setPort(const std::string& port)
  {
    if (port.size() > USB_PORT_STRING_MAX_LENGTH) {
      throw std::runtime_error("device port string size out of range");
    }

    _port = port;
  }

  const std::string& DevicePrivate::getPort() const
  {
    return _port;
  }

  void DevicePrivate::setSerial(const std::string& serial_number)
  {
    if (serial_number.size() > USB_GENERIC_STRING_MAX_LENGTH) {
      throw std::runtime_error("device serial number string size out of range");
    }

    _serial_number = serial_number;
  }

  const std::string& DevicePrivate::getSerial() const
  {
    return _serial_number;
  }

  void DevicePrivate::setConnectType(const std::string& connect_type)
  {
    if (connect_type.size() > USB_GENERIC_STRING_MAX_LENGTH) {
      throw std::runtime_error("device connect-type string size out of range");
    }

    _connect_type = connect_type;
  }

  const std::string& DevicePrivate::getConnectType() const
  {
    return _connect_type;
  }

  std::vector<USBInterfaceType>& DevicePrivate::refMutableInterfaceTypes()
  {
    return _interface_types;
  }

  const std::vector<USBInterfaceType>& DevicePrivate::getInterfaceTypes() const
  {
    return _interface_types;
  }

  void DevicePrivate::loadDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    (void)descriptor;

    if (parser->haveDescriptor(USB_DESCRIPTOR_TYPE_DEVICE)) {
      throw std::runtime_error("Invalid descriptor data: multiple device descriptors for one device");
    }

    _interface_types.clear();
    return;
  }

  void DevicePrivate::loadConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    (void)descriptor;

    if (!parser->haveDescriptor(USB_DESCRIPTOR_TYPE_DEVICE)) {
      throw std::runtime_error("Invalid descriptor data: missing parent device descriptor while loading configuration");
    }

    /*
     * Clean the descriptor state. There shouldn't be any Interface or Endpoint
     * descriptors while loading.
     */
    parser->delDescriptor(USB_DESCRIPTOR_TYPE_INTERFACE);
    parser->delDescriptor(USB_DESCRIPTOR_TYPE_ENDPOINT);
    return;
  }

  void DevicePrivate::loadInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    if (!parser->haveDescriptor(USB_DESCRIPTOR_TYPE_CONFIGURATION)) {
      throw std::runtime_error("Invalid descriptor data: missing parent configuration descriptor while loading interface");
    }

    const USBInterfaceType interface_type(*reinterpret_cast<const USBInterfaceDescriptor*>(descriptor));
    _interface_types.push_back(interface_type);
    return;
  }

  void DevicePrivate::loadEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* const descriptor)
  {
    (void)descriptor;

    /*
     * WARNING: This method can receive USB descriptors of two sizes! (endpoint, audio endpoint)
     */
    if (!parser->haveDescriptor(USB_DESCRIPTOR_TYPE_INTERFACE)) {
      throw std::runtime_error("Invalid descriptor data: missing parent interface descriptor while loading endpoint");
    }

    return;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
