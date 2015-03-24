#include "RulePrivate.hpp"

namespace usbguard {
  Rule::Rule()
  {
    d_pointer = new RulePrivate(*this);
  }

  Rule::~Rule()
  {
    delete d_pointer;
  }

  uint32_t Rule::getSeqn() const
  {
    return d_pointer->getSeqn();
  }
  
  const String& Rule::getVendorID() const
  {
    return d_pointer->getVendorID();
  }
  
  const String& Rule::getProductID() const
  {
    return d_pointer->getProductID();
  }
  
  const String& Rule::getDeviceName() const
  {
    return d_pointer->getDeviceName();
  }
  
  const String& Rule::getDeviceHash() const
  {
    return d_pointer->getDeviceHash();
  }
  
  const StringVector& Rule::getDevicePorts() const
  {
    return d_pointer->getDevicePorts();
  }
  
  const StringVector& Rule::getInterfaceTypes() const
  {
    return d_pointer->getInterfaceTypes();
  }
  
  Rule::Target Rule::getTarget() const
  {
    return d_pointer->getTarget();
  }
  
  const String& Rule::getAction() const
  {
    return d_pointer->getAction();
  }
  
  const std::chrono::steady_clock::time_point Rule::getTimePointAdded() const
  {
    return d_pointer->getTimePointAdded();
  }
  
  uint32_t Rule::getTimeoutSeconds() const
  {
    return d_pointer->getTimeoutSeconds();
  }

  bool Rule::appliesTo(Pointer<const Rule> rhs) const
  {
    return appliesTo(*rhs);
  }
  
  bool Rule::appliesTo(const Rule& rhs) const
  {
    return d_pointer->appliesTo(rhs);
  }
  
  void Rule::setSeqn(uint32_t seqn)
  {
    d_pointer->setSeqn(seqn);
    return;
  }

  void Rule::setVendorID(const String& vendor_id)
  {
    d_pointer->setVendorID(vendor_id);
    return;
  }
  
  void Rule::setProductID(const String& product_id)
  {
    d_pointer->setProductID(product_id);
    return;
  }
  
  void Rule::setDeviceName(const String& device_name)
  {
    d_pointer->setDeviceName(device_name);
    return;
  }
  
  void Rule::setDeviceHash(const String& device_hash)
  {
    d_pointer->setDeviceHash(device_hash);
    return;
  }
  
  void Rule::setDevicePorts(const StringVector& device_ports)
  {
    d_pointer->setDevicePorts(device_ports);
    return;
  }
  
  void Rule::setInterfaceTypes(const StringVector& interface_types)
  {
    d_pointer->setInterfaceTypes(interface_types);
    return;
  }

  StringVector& Rule::refDevicePorts()
  {
    return d_pointer->refDevicePorts();
  }

  StringVector& Rule::refInterfaceTypes()
  {
    return d_pointer->refInterfaceTypes();
  }
  
  void Rule::setTarget(Rule::Target target)
  {
    d_pointer->setTarget(target);
    return;
  }
  
  void Rule::setAction(const String& action)
  {
    d_pointer->setAction(action);
    return;
  }
  
  void Rule::setTimePointAdded(const std::chrono::steady_clock::time_point tp_added)
  {
    d_pointer->setTimePointAdded(tp_added);
    return;
  }
  
  void Rule::setTimeoutSeconds(uint32_t timeout_seconds)
  {
    d_pointer->setTimeoutSeconds(timeout_seconds);
    return;
  }

  Rule::operator bool() const
  {
    return !(getTarget() == Target::Unknown ||
	     getTarget() == Target::Invalid);
  }

  String Rule::toString() const
  {
    return d_pointer->toString();
  }

  Rule Rule::fromString(const String& rule_string)
  {
    return RulePrivate::fromString(rule_string);
  }
}
