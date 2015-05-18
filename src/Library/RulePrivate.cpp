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
#include "RulePrivate.hpp"
#include "RuleParser.hpp"
#include "Common/Utility.hpp"

namespace usbguard {
  RulePrivate::RulePrivate(Rule& p_instance)
    : _p_instance(p_instance)
  {
    _seqn = Rule::SeqnDefault;
    _target = Rule::Target::Invalid;
    _timeout_seconds = 0;
    _device_ports_op = Rule::SetOperator::Match;
    _interface_types_op = Rule::SetOperator::Match;
    return;
  }

  RulePrivate::RulePrivate(Rule& p_instance, const RulePrivate& rhs)
    : _p_instance(p_instance)
  {
    *this = rhs;
    return;
  }
  
  const RulePrivate& RulePrivate::operator=(const RulePrivate& rhs)
  {
    _seqn = rhs._seqn;
    _vendor_id = rhs._vendor_id;
    _product_id = rhs._product_id;
    _serial_number = rhs._serial_number;
    _device_name = rhs._device_name;
    _device_hash = rhs._device_hash;
    _device_ports = rhs._device_ports;
    _device_ports_op = rhs._device_ports_op;
    _interface_types = rhs._interface_types;
    _interface_types_op = rhs._interface_types_op;
    _target = rhs._target;
    _action = rhs._action;
    _tp_added = rhs._tp_added;
    _timeout_seconds = rhs._timeout_seconds;
    return *this;
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

  const String& RulePrivate::getSerialNumber() const
  {
    return _serial_number;
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
  
  const std::vector<USBInterfaceType>& RulePrivate::getInterfaceTypes() const
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
    if (!_serial_number.empty()) {
      if (_serial_number != rhs.getSerialNumber()) {
	return false;
      }
    }

    /*
     * Solve device ports set match
     */
    switch (_device_ports_op) {
    case Rule::SetOperator::Match:
      /* Skip device ports matching */
      break;
    case Rule::SetOperator::AllOf:
      if (!setSolveAllOf(_device_ports, rhs.getDevicePorts())) {
	return false;
      }
      break;
    case Rule::SetOperator::OneOf:
      if (!setSolveOneOf(_device_ports, rhs.getDevicePorts())) {
	return false;
      }
      break;
    case Rule::SetOperator::NoneOf:
      if (!setSolveNoneOf(_device_ports, rhs.getDevicePorts())) {
	return false;
      }
      break;
    case Rule::SetOperator::Equals:
      if (!setSolveEquals(_device_ports, rhs.getDevicePorts())) {
	return false;
      }
      break;
    case Rule::SetOperator::EqualsOrdered:
      if (!setSolveEqualsOrdered(_device_ports, rhs.getDevicePorts())) {
	return false;
      }
      break;
    }

    /*
     * Solve interface types set match
     */
    switch (_interface_types_op) {
    case Rule::SetOperator::Match:
      /* Skip interface type matching */
      break;
    case Rule::SetOperator::AllOf:
      if (!setSolveAllOf(_interface_types, rhs.getInterfaceTypes())) {
	return false;
      }
      break;
    case Rule::SetOperator::OneOf:
      if (!setSolveOneOf(_interface_types, rhs.getInterfaceTypes())) {
	return false;
      }
      break;
    case Rule::SetOperator::NoneOf:
      if (!setSolveNoneOf(_interface_types, rhs.getInterfaceTypes())) {
	return false;
      }
      break;
    case Rule::SetOperator::Equals:
      if (!setSolveEquals(_interface_types, rhs.getInterfaceTypes())) {
	return false;
      }
      break;
    case Rule::SetOperator::EqualsOrdered:
      if (!setSolveEqualsOrdered(_interface_types, rhs.getInterfaceTypes())) {
	return false;
      }
      break;
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

  void RulePrivate::setSerialNumber(const String& serial_number)
  {
    _serial_number = serial_number;
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
  
  void RulePrivate::setInterfaceTypes(const std::vector<USBInterfaceType>& interface_types)
  {
    _interface_types = interface_types;
    return;
  }

  StringVector& RulePrivate::refDevicePorts()
  {
    return _device_ports;
  }

  std::vector<USBInterfaceType>& RulePrivate::refInterfaceTypes()
  {
    return _interface_types;
  }
  
  void RulePrivate::setDevicePortsSetOperator(Rule::SetOperator op)
  {
    _device_ports_op = op;
    return;
  }

  void RulePrivate::setInterfaceTypesSetOperator(Rule::SetOperator op)
  {
    _interface_types_op = op;
    return;
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
  
  String RulePrivate::toString(bool invalid) const
  {
    String rule_string;

    switch(_target) {
    case Rule::Target::Allow:
      rule_string = "allow";
      break;
    case Rule::Target::Block:
      rule_string = "block";
      break;
    case Rule::Target::Reject:
      rule_string = "reject";
      break;
    default:
      if (!invalid) {
	throw std::runtime_error("Cannot convert Rule to string representation; Invalid target");
      }
      else {
	rule_string = "<INVALID>";
      }
    }

    if (!_vendor_id.empty() && !_product_id.empty()) {
      rule_string.append(" ");
      rule_string.append(_vendor_id);
      rule_string.append(":");
      rule_string.append(_product_id);
    }
    else if (!_vendor_id.empty() && _product_id.empty()) {
      rule_string.append(" ");
      rule_string.append(_vendor_id);
      rule_string.append(":*");
    }
    else if (_vendor_id.empty() && !_product_id.empty()) {
      if (!invalid) {
	throw std::runtime_error("Cannot convert Rule to string representation; Vendor ID field missing");
      }
      else {
	rule_string.append("<INVALID>:<INVALID>");
      }
    }
    else {
      /* DeviceID not specified is the same as "*:*" */
    }
    /* Serial Number */
    toString_addNonEmptyField(rule_string, "serial", _serial_number);
    /* Device Name */
    toString_addNonEmptyField(rule_string, "name", _device_name);

    /* Device Ports */
    if (_device_ports.size() == 1
	&& _device_ports_op == Rule::SetOperator::OneOf) {
      toString_addNonEmptyField(rule_string, "via-port", _device_ports[0]);
    }
    else if (_device_ports.size() > 0) {
      rule_string.append(" via-port ");
      rule_string.append(Rule::setOperatorToString(_device_ports_op));
      rule_string.append(" { ");
      for (auto const& port : _device_ports) {
	rule_string.append(quoteEscapeString(port));
	rule_string.append(" ");
      }
      rule_string.append("}");
    }

    /* Interface types */
    if (_interface_types.size() == 1
	&& _interface_types_op == Rule::SetOperator::OneOf) {
      toString_addNonEmptyField(rule_string, "with-interface", _interface_types[0].typeString());
    }
    else if (_interface_types.size() > 0) {
      rule_string.append(" with-interface ");
      rule_string.append(Rule::setOperatorToString(_interface_types_op));
      rule_string.append(" { ");
      for (auto const& type : _interface_types) {
	rule_string.append(type.typeString());
	rule_string.append(" ");
      }
      rule_string.append("}");
    }

    /* Device Hash */
    toString_addNonEmptyField(rule_string, "hash", _device_hash);
    
    /* Action */
    toString_addNonEmptyField(rule_string, "action", _action);

    return rule_string;
  }

  Rule RulePrivate::fromString(const String& rule_string)
  {
    return parseRuleSpecification(rule_string);
  }

  void RulePrivate::toString_addNonEmptyField(String& rule, const String& name, const String& value)
  {
    if (value.empty()) {
      return;
    }

    rule.append(" ");
    rule.append(name);
    rule.append(" ");
    rule.append(quoteEscapeString(value));

    return;
  }

  String RulePrivate::quoteEscapeString(const String& value)
  {
    String result;
    result.push_back('"');
    result.append(Rule::escapeString(value));
    result.push_back('"');
    return result;
  }

} /* namespace usbguard */
