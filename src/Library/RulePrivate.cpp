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
//          Marek Tamaskovic <mtamasko@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "RulePrivate.hpp"
#include "Common/Utility.hpp"

#include "usbguard/RuleParser.hpp"
#include "usbguard/Logger.hpp"

namespace usbguard
{
  RulePrivate::RulePrivate()
    : _device_id("id"),
      _serial("serial"),
      _with_connect_type("with-connect-type"),
      _name("name"),
      _hash("hash"),
      _parent_hash("parent-hash"),
      _via_port("via-port"),
      _with_interface("with-interface"),
      _conditions("if"),
      _label("label")
  {
    _rule_id = Rule::DefaultID;
    _target = Rule::Target::Invalid;
    _conditions_state = 0;
  }

  RulePrivate::RulePrivate(const RulePrivate& rhs)
    : _device_id("id"),
      _serial("serial"),
      _with_connect_type("with-connect-type"),
      _name("name"),
      _hash("hash"),
      _parent_hash("parent-hash"),
      _via_port("via-port"),
      _with_interface("with-interface"),
      _conditions("if"),
      _label("label")
  {
    *this = rhs;
  }

  bool RulePrivate::appliesTo(std::shared_ptr<const Rule> rhs, bool parent_insensitive) const
  {
    return appliesTo(*rhs, parent_insensitive);
  }

  bool RulePrivate::appliesTo(const Rule& rhs, bool parent_insensitive) const
  {
    /*
     * This method checks whether the rule referenced by rhs belongs to
     * a set defined by this rule.
     */
    USBGUARD_LOG(Trace) << "entry:"
      << " rhs=" << rhs.toString()
      << " parent_insensitive=" << parent_insensitive;
    bool applies = false;

    if (!_device_id.appliesTo(rhs.internal()->_device_id) ||
      !_serial.appliesTo(rhs.internal()->_serial) ||
      !_with_connect_type.appliesTo(rhs.internal()->_with_connect_type) ||
      !_name.appliesTo(rhs.internal()->_name) ||
      !_hash.appliesTo(rhs.internal()->_hash) ||
      !(parent_insensitive || _parent_hash.appliesTo(rhs.internal()->_parent_hash)) ||
      !(parent_insensitive || _via_port.appliesTo(rhs.internal()->_via_port)) ||
      !_with_interface.appliesTo(rhs.internal()->_with_interface)) {
      applies = false;
    }
    else {
      applies = true;
    }

    USBGUARD_LOG(Trace) << "return:"
      << " applies=" << applies;
    return applies;
  }

  bool RulePrivate::appliesToWithConditions(const Rule& rhs, bool with_update)
  {
    USBGUARD_LOG(Trace) << "entry:"
      << " rhs=" << rhs.toString()
      << " with_updates=" << with_update;
    bool applies = false;

    if (appliesTo(rhs)) {
      applies = meetsConditions(rhs, with_update);
    }
    else {
      applies = false;
    }

    USBGUARD_LOG(Trace) << "return:"
      << " applies=" << applies;
    return applies;
  }

  bool RulePrivate::meetsConditions(const Rule& rhs, bool with_update)
  {
    USBGUARD_LOG(Trace) << "entry:"
      << " conditions=" << _conditions.count()
      << " rhs=" << rhs.toString()
      << " with_update=" << with_update;

    if (with_update) {
      (void)updateConditionsState(rhs);
    }

    USBGUARD_LOG(Debug) << "set_operator=" << Rule::setOperatorToString(_conditions.setOperator());
    bool meets_conditions = false;

    switch (_conditions.setOperator()) {
    case Rule::SetOperator::OneOf:
      meets_conditions = conditionsState() > 0;
      break;

    case Rule::SetOperator::NoneOf:
      meets_conditions = conditionsState() == 0;
      break;

    case Rule::SetOperator::AllOf:
    case Rule::SetOperator::Equals:
    case Rule::SetOperator::EqualsOrdered:
    case Rule::SetOperator::MatchAll:
      meets_conditions = \
        (conditionsState() == ((((uint64_t)1) << _conditions.count()) - 1));
      break;

    case Rule::SetOperator::Match:
    default:
      throw std::runtime_error("BUG: meetsConditions: invalid conditions set operator");
    }

    USBGUARD_LOG(Trace) << "return:"
      << " meets_conditions=" << meets_conditions;
    return meets_conditions;
  }

  void RulePrivate::initConditions(Interface* const interface)
  {
    for (auto& condition : _conditions.values()) {
      condition->init(interface);
    }

    /* FIXME: prevent leaks when init() throws an exception */
  }

  void RulePrivate::finiConditions()
  {
    for (auto& condition : _conditions.values()) {
      condition->fini();
    }
  }

  bool RulePrivate::updateConditionsState(const Rule& rhs)
  {
    uint64_t updated_state = 0;
    unsigned int i = 0;

    for (auto& condition : _conditions.values()) {
      if (i >= (sizeof updated_state * 8)) {
        throw std::runtime_error("BUG: updateConditionsState: too many conditions");
      }

      updated_state |= uint64_t(condition->evaluate(rhs)) << i;
      ++i;
    }

    USBGUARD_LOG(Debug) << "current=" << conditionsState()
      << " updated=" << updated_state;
    bool retval = false;

    if (updated_state != conditionsState()) {
      setConditionsState(updated_state);
      retval = true;
    }
    else {
      retval = false;
    }

    USBGUARD_LOG(Trace) << "return:"
      << " retval=" << retval;
    return retval;
  }

  uint64_t RulePrivate::conditionsState() const
  {
    return _conditions_state;
  }

  void RulePrivate::setConditionsState(uint64_t state)
  {
    _conditions_state = state;
  }

  void RulePrivate::setRuleID(uint32_t rule_id)
  {
    _rule_id = rule_id;
  }

  uint32_t RulePrivate::getRuleID() const
  {
    return _rule_id;
  }

  void RulePrivate::setTarget(Rule::Target target)
  {
    _target = target;
  }

  Rule::Target RulePrivate::getTarget() const
  {
    return _target;
  }

  void RulePrivate::setDeviceID(const USBDeviceID& device_id)
  {
    _device_id.set(device_id);
  }

  const USBDeviceID& RulePrivate::getDeviceID() const
  {
    return _device_id.get();
  }

  const Rule::Attribute<USBDeviceID>& RulePrivate::attributeDeviceID() const
  {
    return _device_id;
  }

  Rule::Attribute<USBDeviceID>& RulePrivate::attributeDeviceID()
  {
    return _device_id;
  }

  void RulePrivate::setSerial(const std::string& value)
  {
    _serial.set(value);
  }

  const std::string& RulePrivate::getSerial() const
  {
    return _serial.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeSerial() const
  {
    return _serial;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeSerial()
  {
    return _serial;
  }

  void RulePrivate::setLabel(const std::string& value)
  {
    _label.set(value);
  }

  const std::string& RulePrivate::getLabel() const
  {
    return _label.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeLabel() const
  {
    return _label;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeLabel()
  {
    return _label;
  }

  void RulePrivate::setWithConnectType(const std::string& value)
  {
    _with_connect_type.set(value);
  }

  const std::string& RulePrivate::getWithConnectType() const
  {
    return _with_connect_type.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeWithConnectType() const
  {
    return _with_connect_type;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeWithConnectType()
  {
    return _with_connect_type;
  }

  void RulePrivate::setName(const std::string& value)
  {
    _name.set(value);
  }

  const std::string& RulePrivate::getName() const
  {
    return _name.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeName() const
  {
    return _name;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeName()
  {
    return _name;
  }

  void RulePrivate::setHash(const std::string& value)
  {
    _hash.set(value);
  }

  const std::string& RulePrivate::getHash() const
  {
    return _hash.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeHash() const
  {
    return _hash;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeHash()
  {
    return _hash;
  }

  void RulePrivate::setParentHash(const std::string& value)
  {
    _parent_hash.set(value);
  }

  const std::string& RulePrivate::getParentHash() const
  {
    return _parent_hash.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeParentHash() const
  {
    return _parent_hash;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeParentHash()
  {
    return _parent_hash;
  }

  void RulePrivate::setViaPort(const std::string& value)
  {
    _via_port.set(value);
  }

  const std::string& RulePrivate::getViaPort() const
  {
    return _via_port.get();
  }

  const Rule::Attribute<std::string>& RulePrivate::attributeViaPort() const
  {
    return _via_port;
  }

  Rule::Attribute<std::string>& RulePrivate::attributeViaPort()
  {
    return _via_port;
  }

  const Rule::Attribute<USBInterfaceType>& RulePrivate::attributeWithInterface() const
  {
    return _with_interface;
  }

  Rule::Attribute<USBInterfaceType>& RulePrivate::attributeWithInterface()
  {
    return _with_interface;
  }

  const Rule::Attribute<RuleCondition>& RulePrivate::attributeConditions() const
  {
    return _conditions;
  }

  Rule::Attribute<RuleCondition>& RulePrivate::attributeConditions()
  {
    return _conditions;
  }

  template<class ValueType>
  static void toString_appendNonEmptyAttribute(std::string& rule_string, const Rule::Attribute<ValueType>& attribute)
  {
    if (attribute.empty()) {
      return;
    }

    rule_string.append(" ");
    rule_string.append(attribute.toRuleString());
    return;
  }

  std::string RulePrivate::toString(bool invalid, bool hide_pii) const
  {
    std::string rule_string;

    try {
      rule_string.append(Rule::targetToString(_target));
    }
    catch (...) {
      if (invalid) {
        rule_string.append("<invalid>");
      }
      else {
        throw;
      }
    }

    toString_appendNonEmptyAttribute(rule_string, _device_id);

    if (!hide_pii) {
      toString_appendNonEmptyAttribute(rule_string, _serial);
    }

    toString_appendNonEmptyAttribute(rule_string, _name);

    if (!hide_pii) {
      toString_appendNonEmptyAttribute(rule_string, _hash);
      toString_appendNonEmptyAttribute(rule_string, _parent_hash);
    }

    toString_appendNonEmptyAttribute(rule_string, _via_port);
    toString_appendNonEmptyAttribute(rule_string, _with_interface);
    toString_appendNonEmptyAttribute(rule_string, _conditions);
    toString_appendNonEmptyAttribute(rule_string, _with_connect_type);
    toString_appendNonEmptyAttribute(rule_string, _label);
    return rule_string;
  }

  RulePrivate::MetaData& RulePrivate::metadata()
  {
    return _meta;
  }

  const RulePrivate::MetaData& RulePrivate::metadata() const
  {
    return _meta;
  }

  Rule RulePrivate::fromString(const std::string& rule_string)
  {
    return parseRuleFromString(rule_string);
  }

  void RulePrivate::updateMetaDataCounters(bool applied, bool evaluated)
  {
    if (evaluated) {
      ++_meta.counter_evaluated;
      _meta.tp_last_evaluated = std::chrono::steady_clock::now();
    }

    if (applied) {
      ++_meta.counter_applied;
      _meta.tp_last_applied = std::chrono::steady_clock::now();
    }

    return;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
