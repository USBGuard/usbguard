#include "Device.hpp"
#include <mutex>

namespace usbguard {
  Device::Device()
  {
    _seqn = Rule::SeqnDefault;
    _target = Rule::Target::Unknown;
    return;
  }

  std::mutex& Device::refDeviceMutex()
  {
    return _mutex;
  }

  Pointer<Rule> Device::getDeviceRule()
  {
    Pointer<Rule> device_rule = makePointer<Rule>();
    std::unique_lock<std::mutex> device_lock(refDeviceMutex());

    device_rule->setSeqn(_seqn);
    device_rule->setTarget(_target);
    device_rule->setVendorID(_vendor_id);
    device_rule->setProductID(_product_id);
    device_rule->setSerialNumber(_serial_number);
    device_rule->setDeviceName(_name);
    device_rule->setDeviceHash(_hash);
    device_rule->setInterfaceTypes(_interface_types);
    
    return std::move(device_rule);
  }

  uint32_t Device::getSeqn() const
  {
    return _seqn;
  }

  void Device::setSeqn(uint32_t seqn)
  {
    _seqn = seqn;
    return;
  }

  void Device::setTarget(Rule::Target target)
  {
    _target = target;
    return;
  }

  void Device::setDeviceName(const String& name)
  {
    _name = name;
    return;
  }

  void Device::setDeviceHash(const String& hash)
  {
    _hash = hash;
    return;
  }

  void Device::setVendorID(const String& vendor_id)
  {
    _vendor_id = vendor_id;
    return;
  }

  void Device::setProductID(const String& product_id)
  {
    _product_id = product_id;
    return;
  }

  void Device::setSerialNumber(const String& serial_number)
  {
    _serial_number = serial_number;
    return;
  }

  void Device::setInterfaceTypes(const StringVector& types)
  {
    _interface_types = types;
    return;
  }

  StringVector& Device::refInterfaceTypes()
  {
    return _interface_types;
  }

} /* namespace usbguard */
