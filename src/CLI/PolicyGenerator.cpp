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
//          Radovan Sroka <rsroka@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "PolicyGenerator.hpp"

namespace usbguard
{

  PolicyGenerator::PolicyGenerator()
  {
    _ruleset.push_back(std::dynamic_pointer_cast<RuleSet>(std::make_shared<MemoryRuleSet>(nullptr)));
    _with_hash = true;
    _hash_only = false;
    _port_specific = false;
    _port_specific_noserial = true;
    _devpath = "";
    _with_catchall = false;
    _catchall_target = Rule::Target::Block;
    _dm = DeviceManager::create(*this, "uevent");
    _dm->setEnumerationOnlyMode(true);
  }

  void PolicyGenerator::setWithHashAttribute(bool state)
  {
    _with_hash = state;
  }

  void PolicyGenerator::setHashOnly(bool state)
  {
    _hash_only = state;
  }

  void PolicyGenerator::setPortSpecificRules(bool state)
  {
    _port_specific = state;
  }

  void PolicyGenerator::setPortSpecificNoSerialRules(bool state)
  {
    _port_specific_noserial = state;
  }

  void PolicyGenerator::generate()
  {
    if (_devpath.empty()) {
      _dm->start();
      _dm->scan();
    }
    else {
      _dm->scan(_devpath);
    }

    if (_with_catchall) {
      Rule catchall_rule;
      catchall_rule.setTarget(_catchall_target);
      _ruleset.front()->appendRule(catchall_rule);
    }
  }

  const std::vector<std::shared_ptr<RuleSet>> PolicyGenerator::refRuleSet() const
  {
    return _ruleset;
  }

  void PolicyGenerator::setDevpath(const std::string& devpath)
  {
    _devpath = devpath;
  }

  void PolicyGenerator::setExplicitCatchAllRule(bool state, Rule::Target target)
  {
    _with_catchall = state;
    _catchall_target = target;
  }

  void PolicyGenerator::dmHookDeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device)
  {
    if (event != DeviceManager::EventType::Present) {
      /*
       * Ignore run-time device events
       */
      return;
    }

    bool port_specific = _port_specific;

    /*
     * If the the global "port specific" flag isn't
     * set, check the "no iSerial port specific" flag
     * applicability.
     */
    if (!port_specific && _port_specific_noserial) {
      port_specific = device->getSerial().empty();
    }

    std::shared_ptr<Rule> rule = device->getDeviceRule(/*include_port=*/port_specific);

    /* Remove everything but the hash value for hash-only rules */
    if (_hash_only) {
      std::shared_ptr<Rule> rule_hashonly(new Rule());
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
    _ruleset.front()->appendRule(*rule);
  }


  uint32_t PolicyGenerator::dmHookAssignID()
  {
    return _ruleset.front()->assignID();
  }

  void PolicyGenerator::dmHookDeviceException(const std::string& message)
  {
    USBGUARD_LOG(Error) << message;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
