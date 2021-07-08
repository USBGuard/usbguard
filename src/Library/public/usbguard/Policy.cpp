//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "Policy.hpp"
#include "usbguard/Exception.hpp"

namespace usbguard
{
  Policy::Policy()
  {
    _rulesets_ptr = std::vector<std::shared_ptr<RuleSet>>();
  }

  void Policy::setRuleSet(std::vector<std::shared_ptr<RuleSet>> ptr)
  {
    _rulesets_ptr = ptr;
  }

  std::vector<std::shared_ptr<RuleSet>> Policy::getRuleSet()
  {
    return _rulesets_ptr;
  }

  void Policy::setDefaultTarget(Rule::Target target)
  {
    _defaultTarget = target;

    for (auto ruleset : _rulesets_ptr) {
      ruleset->setDefaultTarget(target);
    }
  }

  Rule::Target Policy::getDefaultTarget() const
  {
    return _defaultTarget;
  }

  std::shared_ptr<RuleSet> Policy::findRuleSetByRuleId(uint32_t rule_id) const
  {
    if (rule_id == 0) {
      return _rulesets_ptr.front();
    }

    if (rule_id == Rule::LastID) {
      return _rulesets_ptr.back();
    }

    for (auto ruleset : _rulesets_ptr) {
      for (const auto& rule : ruleset->getRules()) {
        if (rule->getRuleID() == rule_id) {
          return ruleset;
        }
      }
    }

    return nullptr;
  }

  std::shared_ptr<RuleSet> Policy::findRuleSetByPrefix(const std::string& prefix) const
  {
    for (auto ruleset : _rulesets_ptr) {
      if (ruleset->getName().substr(0, prefix.length()) == prefix) {
        return ruleset;
      }
    }

    return nullptr;
  }

  uint32_t Policy::insertRule(const Rule& rule, uint32_t parent_id, const std::string& ruleset)
  {
    USBGUARD_LOG(Trace) << "parent_id=" << parent_id << " ruleset=" << ruleset;
    auto _rule = std::make_shared<Rule>(rule);
    auto _ruleset = ruleset.empty() ? findRuleSetByRuleId(parent_id) : findRuleSetByPrefix(ruleset);

    if (_ruleset == nullptr) {
      throw Exception("Policy insert", "rule", "Failed to find a ruleset");
    }

    if (parent_id != 0 && parent_id != Rule::LastID && !_ruleset->hasRule(parent_id)) {
      throw Exception("Policy insert", "rule", "Ruleset does not contain rule with given id");
    }

    return _ruleset->appendRule(*_rule, parent_id);
  }

  uint32_t Policy::appendRule(const Rule& _rule, uint32_t parent_id)
  {
    return insertRule(_rule, parent_id);
  }

  /*
   * the following function is called as a consequence of allowing device permanently
   * new-rule is exact Rule object constructed from device with all his attributes
   * and with proper target {allow|block|reject}
   * match-rule is Rule object with match target, it is also constructed from the same device as
   * the new-rule but it contains minimal subset of attributes
   * we are trying to match our match-rule against each rule in all rulesets
   * if our match-rule can be applied to some rule in a ruleset
   * we will replace the matched rule with our new-rule so we are basically
   * doing an update of existing rule in case when it differs in
   * ports, parent or perhaps some other attributes that are not specifying
   * device itself
   */
  uint32_t Policy::upsertRule(const Rule& match_rule, const Rule& new_rule, const bool parent_insensitive)
  {
    if (_rulesets_ptr.empty()) {
      throw Exception("Policy upsert", "rule", "There is no ruleset to upsert into");
    }

    for (auto ruleset : _rulesets_ptr) {
      uint32_t id = ruleset->upsertRule(match_rule, new_rule, parent_insensitive);

      if (id == Rule::DefaultID) {
        continue;
      }
      else {
        return id;
      }
    }

    return _rulesets_ptr.back()->appendRule(new_rule, Rule::LastID, /*lock*/true);
  }

  std::shared_ptr<Rule> Policy::getRule(uint32_t id)
  {
    for (auto ruleset_item : _rulesets_ptr) {
      try {
        return ruleset_item->getRule(id);
      }
      catch (const std::exception& e) {
        continue;
      }
    }

    throw Exception("Policy lookup", "rule id", "id doesn't exist");
  }

  bool Policy::removeRule(uint32_t id)
  {
    for (auto ruleset_item : _rulesets_ptr) {
      try {
        return ruleset_item->removeRule(id);
      }
      catch (const std::exception& e) {
        continue;
      }
    }

    throw Exception("Policy remove", "rule id", "id doesn't exist");
  }

  std::shared_ptr<Rule> Policy::getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id) const
  {
    // Try to find Rule with ID different then Rule:ImplicitID.
    for (auto ruleset : _rulesets_ptr) {
      // try to get matching rule
      auto matchingRule = ruleset->getFirstMatchingRule(device_rule, from_id);

      // if it is not the implicit rule return the rule
      if (matchingRule->getRuleID() != Rule::ImplicitID) {
        return matchingRule;
      }
    }

    // if we have not found the rule return the implicit one
    return _rulesets_ptr.front()->getFirstMatchingRule(device_rule, from_id);
  }

  std::vector<std::shared_ptr<const Rule>> Policy::getRules()
  {
    std::vector<std::shared_ptr<const Rule>> rules;

    // copy all rules from ruleset to rules vector
    for (auto ruleset : _rulesets_ptr) {
      // obtain vector of rules
      auto _rules = ruleset->getRules();
      // copy them to buffer which will be returned
      std::copy(_rules.begin(), _rules.end(), std::back_inserter(rules));
    }

    // return rules
    return rules;
  }

  uint32_t Policy::assignID(std::shared_ptr<Rule> rule)
  {
    return _rulesets_ptr.front()->assignID(rule);
  }

  uint32_t Policy::assignID()
  {
    return _rulesets_ptr.front()->assignID();
  }

  std::string Policy::eventTypeToString(Policy::EventType event)
  {
    switch (event) {
    case Policy::EventType::Insert:
      return "Insert";

    case Policy::EventType::Update:
      return "Update";

    case Policy::EventType::Remove:
      return "Remove";

    default:
      throw USBGUARD_BUG("unknown Policy::EventType value");
    }
  }

  void Policy::save(void)
  {
    for (auto ruleset : _rulesets_ptr) {
      ruleset->save();
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
