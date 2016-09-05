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
#include "PolicyGenerator.hpp"

namespace usbguard
{

  PolicyGenerator::PolicyGenerator()
   : _ruleset(nullptr)
  {
    _with_hash = true;
    _hash_only = false;
    _port_specific = false;
    _port_specific_noserial = true;
    _with_catchall = false;
    _catchall_target = Rule::Target::Block;
    _dm = DeviceManager::create(*this);
    return;
  }

  void PolicyGenerator::setWithHashAttribute(bool state)
  {
    _with_hash = state;
    return;
  }

  void PolicyGenerator::setHashOnly(bool state)
  {
    _hash_only = state;
    return;
  }

  void PolicyGenerator::setPortSpecificRules(bool state)
  {
    _port_specific = state;
    return;
  }

  void PolicyGenerator::setPortSpecificNoSerialRules(bool state)
  {
    _port_specific_noserial = state;
    return;
  }

  void PolicyGenerator::generate()
  {
    _dm->scan();

    if (_with_catchall) {
      Rule catchall_rule;
      catchall_rule.setTarget(_catchall_target);
      _ruleset.appendRule(catchall_rule);
    }

    return;
  }

  const RuleSet& PolicyGenerator::refRuleSet() const
  {
    return _ruleset;
  }

  void PolicyGenerator::setExplicitCatchAllRule(bool state, Rule::Target target)
  {
    _with_catchall = state;
    _catchall_target = target;
    return;
  }

  void PolicyGenerator::dmHookDevicePresent(Pointer<Device> device)
  {
    bool port_specific = _port_specific;
    /*
     * If the the global "port specific" flag isn't
     * set, check the "no iSerial port specific" flag
     * applicability.
     */
    if (!port_specific && _port_specific_noserial) {
      port_specific = device->getSerial().empty();
    }

    Pointer<Rule> rule = device->getDeviceRule(/*include_port=*/port_specific);

    /* Remove everything but the hash value for hash-only rules */
    if (_hash_only) {
      Pointer<Rule> rule_hashonly(new Rule());
      rule_hashonly->setRuleID(rule->getRuleID());
      rule_hashonly->setHash(rule->getHash());
      rule_hashonly->setParentHash(rule->getParentHash());

      if (port_specific) {
        rule_hashonly->attributeViaPort().set(rule->attributeViaPort().values(), Rule::SetOperator::Equals);
      }

      rule = rule_hashonly;
    }
    /* Remove the hash attributes if set to do so */
    else if (!_with_hash) {
      rule->attributeHash().clear();
      rule->attributeParentHash().clear();
    }

    rule->setTarget(Rule::Target::Allow);
    _ruleset.appendRule(*rule);
    return;
  }

  void PolicyGenerator::dmHookDeviceInserted(Pointer<Device> device)
  {
    throw std::runtime_error("BUG: DeviceInserted hook should not be called");
  }

  void PolicyGenerator::dmHookDeviceRemoved(Pointer<Device> device)
  {
    throw std::runtime_error("BUG: DeviceRemoved hook should not be called");
  }

  void PolicyGenerator::dmHookDeviceAllowed(Pointer<Device> device)
  {
    throw std::runtime_error("BUG: DeviceAllowed hook should not be called");
  }

  void PolicyGenerator::dmHookDeviceBlocked(Pointer<Device> device)
  {
    throw std::runtime_error("BUG: DeviceBlocked hook should not be called");
  }

  void PolicyGenerator::dmHookDeviceRejected(Pointer<Device> device)
  {
    throw std::runtime_error("BUG: DeviceRejected hook should not be called");
  }

  uint32_t PolicyGenerator::dmHookAssignID()
  {
    return _ruleset.assignID();
  }
} /* namespace usbguard */
