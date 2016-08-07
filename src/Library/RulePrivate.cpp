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
#include "LoggerPrivate.hpp"
#include "Common/Utility.hpp"

namespace usbguard {
  RulePrivate::RulePrivate(Rule& p_instance)
    : _p_instance(p_instance),
      _device_id("id"),
      _serial("serial"),
      _name("name"),
      _hash("hash"),
      _parent_hash("parent-hash"),
      _via_port("via-port"),
      _with_interface("with-interface"),
      _conditions("if")
  {
    _rule_id = Rule::DefaultID;
    _target = Rule::Target::Invalid;
    _conditions_state = 0;
    _timeout_seconds = 0;
  }

  RulePrivate::RulePrivate(Rule& p_instance, const RulePrivate& rhs)
    : _p_instance(p_instance),
      _device_id("id"),
      _serial("serial"),
      _name("name"),
      _hash("hash"),
      _parent_hash("parent-hash"),
      _via_port("via-port"),
      _with_interface("with-interface"),
      _conditions("if")
  {
    *this = rhs;
  }
  
  const RulePrivate& RulePrivate::operator=(const RulePrivate& rhs)
  {
    _meta = rhs._meta;
    _rule_id = rhs._rule_id;
    _target = rhs._target;

    _device_id = rhs._device_id;
    _serial = rhs._serial;
    _name = rhs._name;
    _hash = rhs._hash;
    _parent_hash = rhs._parent_hash;
    _via_port = rhs._via_port;
    _with_interface = rhs._with_interface;
    _conditions = rhs._conditions;

    _conditions_state = rhs._conditions_state;
    _timeout_seconds = rhs._timeout_seconds;

    return *this;
#if 0
    try {
      for (auto const& condition : rhs._conditions) {
        _conditions.push_back(condition->clone());
      }
    }
    catch(...) {
      for (auto const& condition : _conditions) {
        delete condition;
      }
      throw;
    }
#endif
  }

  RulePrivate::~RulePrivate()
  {
  }

  bool RulePrivate::appliesTo(Pointer<const Rule> rhs, bool parent_insensitive) const
  {
    return appliesTo(*rhs, parent_insensitive);
  }

  bool RulePrivate::appliesTo(const Rule& rhs, bool parent_insensitive) const
  {
    /*
     * This method checks whether the rule referenced by rhs belongs to
     * a set defined by this rule.
     */
    logger->trace("Checking applicability of rule [{}] to rule [{}]",
        this->toString(/*invalid=*/true), rhs.toString(/*invalid=*/true));

    if (!_device_id.appliesTo(rhs.internal()->_device_id) ||
        !_serial.appliesTo(rhs.internal()->_serial) ||
        !_name.appliesTo(rhs.internal()->_name) ||
        !_hash.appliesTo(rhs.internal()->_hash) ||
        !(parent_insensitive || _parent_hash.appliesTo(rhs.internal()->_parent_hash)) ||
        !(parent_insensitive || _via_port.appliesTo(rhs.internal()->_via_port)) ||
        !_with_interface.appliesTo(rhs.internal()->_with_interface)) {
      return false;
    }

    logger->debug("Rule applies.");
    return true;
  }

  bool RulePrivate::appliesToWithConditions(const Rule& rhs, bool with_update)
  {
    if (!appliesTo(rhs)) {
      return false;
    }
    logger->debug("Evaluating whether rule {} meets conditions of rule {}", getRuleID(), rhs.getRuleID());
    if (!meetsConditions(rhs, with_update)) {
      logger->debug("Rule {} DOES NOT meet conditions of rule {}", rhs.getRuleID(), getRuleID());
      return false;
    }
    logger->debug("Rule {} meets conditions of rule {}", rhs.getRuleID(), getRuleID());
    return true;
  }

  bool RulePrivate::meetsConditions(const Rule& rhs, bool with_update)
  {
    if (with_update) {
      (void)updateConditionsState(rhs);
    }
    switch(_conditions.setOperator()) {
      case Rule::SetOperator::OneOf:
	logger->debug("meetsCondition: OneOf: {}", conditionsState() > 0 ? "true" : "false");
        return conditionsState() > 0;
      case Rule::SetOperator::NoneOf:
	logger->debug("meetsCondition: NoneOf: {}", conditionsState() == 0 ? "true" : "false");
        return conditionsState() == 0;
      case Rule::SetOperator::AllOf:
      case Rule::SetOperator::Equals:
      case Rule::SetOperator::EqualsOrdered:
	logger->debug("meetsCondition: AllOf, Equals, ...: {}",
                      conditionsState() == ((((uint64_t)1) << _conditions.count()) - 1) ? "true" : "false");
        return conditionsState() == ((((uint64_t)1) << _conditions.count()) - 1);
      case Rule::SetOperator::Match:
        throw std::runtime_error("BUG: meetsConditions: invalid conditions set operator");
    }
    return false;
  }

  void RulePrivate::initConditions(Interface * const interface)
  {
    for (auto condition : _conditions.values()) {
      condition->init(interface);
    }
    /* FIXME: prevent leaks when init() throws an exception */
  }

  void RulePrivate::finiConditions()
  {
    for (auto condition : _conditions.values()) {
      condition->fini();
    }
  }

  bool RulePrivate::updateConditionsState(const Rule& rhs)
  {
    uint64_t updated_state = 0;
    unsigned int i = 0;

    for (auto condition : _conditions.values()) {
      if (i >= (sizeof updated_state * 8)) {
        throw std::runtime_error("BUG: updateConditionsState: too many conditions");
      }
      updated_state |= uint64_t(condition->evaluate(rhs)) << i;
      ++i;
    }

    logger->debug("Condition state of rule {}: current={} updated={}",
                  rhs.getRuleID(), conditionsState(), updated_state);

    if (updated_state != conditionsState()) {
      setConditionsState(updated_state);
      return true;
    }

    return false;
  }

  uint64_t RulePrivate::conditionsState() const
  {
    return _conditions_state;
  }

  void RulePrivate::setConditionsState(uint64_t state)
  {
    _conditions_state = state;
  }

  uint32_t RulePrivate::getTimeoutSeconds() const
  {
    return _timeout_seconds;
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

  void RulePrivate::setSerial(const String& value)
  {
    _serial.set(value);
  }

  const String& RulePrivate::getSerial() const
  {
    return _serial.get();
  }

  const Rule::Attribute<String>& RulePrivate::attributeSerial() const
  {
    return _serial;
  }

  Rule::Attribute<String>& RulePrivate::attributeSerial()
  {
    return _serial;
  }

  void RulePrivate::setName(const String& value)
  {
    _name.set(value);
  }

  const String& RulePrivate::getName() const
  {
    return _name.get();
  }

  const Rule::Attribute<String>& RulePrivate::attributeName() const
  {
    return _name;
  }

  Rule::Attribute<String>& RulePrivate::attributeName()
  {
    return _name;
  }

  void RulePrivate::setHash(const String& value)
  {
    _hash.set(value);
  }

  const String& RulePrivate::getHash() const
  {
    return _hash.get();
  }

  const Rule::Attribute<String>& RulePrivate::attributeHash() const
  {
    return _hash;
  }

  Rule::Attribute<String>& RulePrivate::attributeHash()
  {
    return _hash;
  }

  void RulePrivate::setParentHash(const String& value)
  {
    _parent_hash.set(value);
  }

  const String& RulePrivate::getParentHash() const
  {
    return _parent_hash.get();
  }

  const Rule::Attribute<String>& RulePrivate::attributeParentHash() const
  {
    return _parent_hash;
  }

  Rule::Attribute<String>& RulePrivate::attributeParentHash()
  {
    return _parent_hash;
  }

  void RulePrivate::setViaPort(const String& value)
  {
    _via_port.set(value);
  }

  const String& RulePrivate::getViaPort() const
  {
    return _via_port.get();
  }

  const Rule::Attribute<String>& RulePrivate::attributeViaPort() const
  {
    return _via_port;
  }

  Rule::Attribute<String>& RulePrivate::attributeViaPort()
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

  const Rule::Attribute<RuleCondition*>& RulePrivate::attributeConditions() const
  {
    return _conditions;
  }

  Rule::Attribute<RuleCondition*>& RulePrivate::attributeConditions()
  {
    return _conditions;
  }

  void RulePrivate::setTimeoutSeconds(uint32_t timeout_seconds)
  {
    _timeout_seconds = timeout_seconds;
  }

  template<class ValueType>
  static void toString_appendNonEmptyAttribute(String& rule_string, const Rule::Attribute<ValueType>& attribute)
  {
    if (attribute.empty()) {
      return;
    }

    rule_string.append(" ");
    rule_string.append(attribute.toRuleString());

    return;
  }

  String RulePrivate::toString(bool invalid) const
  {
    String rule_string;

    try {
      rule_string.append(Rule::targetToString(_target));
    } catch(...) {
      if (invalid) {
        rule_string.append("<invalid>");
      }
      else {
        throw;
      }
    }

    toString_appendNonEmptyAttribute(rule_string, _device_id);
    toString_appendNonEmptyAttribute(rule_string, _serial);
    toString_appendNonEmptyAttribute(rule_string, _name);
    toString_appendNonEmptyAttribute(rule_string, _hash);
    toString_appendNonEmptyAttribute(rule_string, _parent_hash);
    toString_appendNonEmptyAttribute(rule_string, _via_port);
    toString_appendNonEmptyAttribute(rule_string, _with_interface);
    toString_appendNonEmptyAttribute(rule_string, _conditions);

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

  Rule RulePrivate::fromString(const String& rule_string)
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
