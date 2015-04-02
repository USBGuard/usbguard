#include "RulePrivate.hpp"
#include <utility>

namespace usbguard {
  Rule::Rule()
  {
    d_pointer = new RulePrivate(*this);
  }

  Rule::~Rule()
  {
    delete d_pointer;
  }

  Rule::Rule(const Rule& rhs)
  {
    d_pointer = new RulePrivate(*rhs.d_pointer);
    return;
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

  const String& Rule::getSerialNumber() const
  {
    return d_pointer->getSerialNumber();
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
  
  const std::vector<USBInterfaceType>& Rule::getInterfaceTypes() const
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

  bool Rule::isImplicit() const
  {
    return d_pointer->getSeqn() == Rule::SeqnDefault;
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
  
  void Rule::setSerialNumber(const String& serial_number)
  {
    d_pointer->setSerialNumber(serial_number);
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
  
  void Rule::setInterfaceTypes(const std::vector<USBInterfaceType>& interface_types)
  {
    d_pointer->setInterfaceTypes(interface_types);
    return;
  }

  StringVector& Rule::refDevicePorts()
  {
    return d_pointer->refDevicePorts();
  }

  std::vector<USBInterfaceType>& Rule::refInterfaceTypes()
  {
    return d_pointer->refInterfaceTypes();
  }

  void Rule::setDevicePortsSetOperator(Rule::SetOperator op)
  {
    d_pointer->setDevicePortsSetOperator(op);
    return;
  }

  void Rule::setInterfaceTypesSetOperator(Rule::SetOperator op)
  {
    d_pointer->setInterfaceTypesSetOperator(op);
    return;
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

  String Rule::toString(bool invalid) const
  {
    return d_pointer->toString(invalid);
  }

  Rule Rule::fromString(const String& rule_string)
  {
    return RulePrivate::fromString(rule_string);
  }

  Rule::SetOperator Rule::setOperatorFromString(const String& set_operator_string)
  {
    const std::vector<std::pair<String,Rule::SetOperator> > set_operator_ttable = {
      { "all-of", SetOperator::AllOf },
      { "one-of", SetOperator::OneOf },
      { "none-of", SetOperator::NoneOf },
      { "equals", SetOperator::Equals },
      { "equals-ordered", SetOperator::EqualsOrdered }
    };

    for (auto ttable_entry : set_operator_ttable) {
      if (ttable_entry.first == set_operator_string) {
	return ttable_entry.second;
      }
    }

    throw std::runtime_error("Invalid set operator string");
  }

  const String Rule::setOperatorToString(Rule::SetOperator op)
  {
    const std::vector<std::pair<String,Rule::SetOperator> > set_operator_ttable = {
      { "all-of", SetOperator::AllOf },
      { "one-of", SetOperator::OneOf },
      { "none-of", SetOperator::NoneOf },
      { "equals", SetOperator::Equals },
      { "equals-ordered", SetOperator::EqualsOrdered },
      { "<MATCH>", SetOperator::Match }
    };

    for (auto ttable_entry : set_operator_ttable) {
      if (ttable_entry.second == op) {
	return ttable_entry.first;
      }
    }

    throw std::runtime_error("Invalid set operator string");
  }
}
