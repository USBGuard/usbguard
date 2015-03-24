#include "RulePrivate.hpp"
#include "RuleParser.hpp"

namespace usbguard {
  RulePrivate::RulePrivate(Rule& p_instance)
    : _p_instance(p_instance)
  {
    _seqn = Rule::SeqnDefault;
    _target = Rule::Target::Invalid;
    _timeout_seconds = 0;
    return;
  }
  
  uint32_t RulePrivate::getSeqn() const
  {
    return _seqn;
  }
  
  const String& RulePrivate::getVendorID() const
  {
    return _vendor_id;
  }
  
  const String& RulePrivate::getProductID() const
  {
    return _product_id;
  }
  
  const String& RulePrivate::getDeviceName() const
  {
    return _device_name;
  }
  
  const String& RulePrivate::getDeviceHash() const
  {
    return _device_hash;
  }
  
  const StringVector& RulePrivate::getDevicePorts() const
  {
    return _device_ports;
  }
  
  const StringVector& RulePrivate::getInterfaceTypes() const
  {
    return _interface_types;
  }
  
  Rule::Target RulePrivate::getTarget() const
  {
    return _target;
  }
  
  const String& RulePrivate::getAction() const
  {
    return _action;
  }
  
  const std::chrono::steady_clock::time_point RulePrivate::getTimePointAdded() const
  {
    return _tp_added;
  }
  
  uint32_t RulePrivate::getTimeoutSeconds() const
  {
    return _timeout_seconds;
  }

  bool RulePrivate::appliesTo(Pointer<const Rule> rhs) const
  {
    return appliesTo(*rhs);
  }
  
  bool RulePrivate::appliesTo(const Rule& rhs) const
  {
    /*
     * If a this set of rules contains the rhs rule, return true. Otherwise false.
     * Ignored fields: rule_seqn, target, action, ts_added, timeout_sec, ref_syspath
     */
    if (!_vendor_id.empty()) {
      if (_vendor_id != rhs.getVendorID()) {
	return false;
      }
    }
    if (!_product_id.empty()) {
      if (_product_id != rhs.getProductID()) {
	return false;
      }
    }
    if (!_device_name.empty()) {
      if (_device_name != rhs.getDeviceName()) {
	return false;
      }
    }
    if (!_device_hash.empty()) {
      if (_device_hash != rhs.getDeviceHash()) {
	return false;
      }
    }
    if (_device_ports.size() > 0) {
      bool match = false;
      for (auto const& port : _device_ports) {
	for (auto const& rhs_port : rhs.getDevicePorts()) {
	  if (port == rhs_port) {
	    match = true;
	    break;
	  }
	}
	if (match) {
	  break;
	}
      }
      if (!match) {
	return false;
      }
    }
    return true;
  }
  
  void RulePrivate::setSeqn(uint32_t seqn)
  {
    _seqn = seqn;
    return;
  }

  void RulePrivate::setVendorID(const String& vendor_id)
  {
    _vendor_id = vendor_id;
    return;
  }
  
  void RulePrivate::setProductID(const String& product_id)
  {
    _product_id = product_id;
    return;
  }
  
  void RulePrivate::setDeviceName(const String& device_name)
  {
    _device_name = device_name;
    return;
  }
  
  void RulePrivate::setDeviceHash(const String& device_hash)
  {
    _device_hash = device_hash;
    return;
  }
  
  void RulePrivate::setDevicePorts(const StringVector& device_ports)
  {
    _device_ports = device_ports;
    return;
  }
  
  void RulePrivate::setInterfaceTypes(const StringVector& interface_types)
  {
    _interface_types = interface_types;
    return;
  }

  StringVector& RulePrivate::refDevicePorts()
  {
    return _device_ports;
  }

  StringVector& RulePrivate::refInterfaceTypes()
  {
    return _interface_types;
  }
  
  void RulePrivate::setTarget(Rule::Target target)
  {
    _target = target;
    return;
  }
  
  void RulePrivate::setAction(const String& action)
  {
    _action = action;
    return;
  }
  
  void RulePrivate::setTimePointAdded(const std::chrono::steady_clock::time_point tp_added)
  {
    _tp_added = tp_added;
    return;
  }
  
  void RulePrivate::setTimeoutSeconds(uint32_t timeout_seconds)
  {
    _timeout_seconds = timeout_seconds;
    return;
  }
  
  String RulePrivate::toString() const
  {
    return String();
  }

  Rule RulePrivate::fromString(const String& rule_string)
  {
    return parseRuleSpecification(rule_string);
  }
} /* namespace usbguard */
