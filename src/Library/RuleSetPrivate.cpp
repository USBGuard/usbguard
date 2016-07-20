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
#include "Typedefs.hpp"
#include "RuleSetPrivate.hpp"
#include "RulePrivate.hpp"
#include <stdexcept>
#include <fstream>

namespace usbguard {
  
  RuleSetPrivate::RuleSetPrivate(RuleSet& p_instance, Interface * const interface_ptr)
    : _p_instance(p_instance),
      _interface_ptr(interface_ptr)
  {
    (void)_p_instance;
    _default_target = Rule::Target::Block;
    _default_action = String();
    _id_next = Rule::RootID + 1;
    return;
  }

  RuleSetPrivate::RuleSetPrivate(RuleSet& p_instance, const RuleSetPrivate& rhs)
    : _p_instance(p_instance),
      _interface_ptr(rhs._interface_ptr)
  {
    *this = rhs;
    return;
  }

  const RuleSetPrivate& RuleSetPrivate::operator=(const RuleSetPrivate& rhs)
  {
    _default_target = rhs._default_target;
    _default_action = rhs._default_action;
    _id_next = rhs._id_next.load();
    _rules = rhs._rules;
    _rules_timed = rhs._rules_timed;
    return *this;
  }

  RuleSetPrivate::~RuleSetPrivate()
  {
  }

  void RuleSetPrivate::load(const String& path)
  {
    std::ifstream stream(path);
    if (!stream.is_open()) {
      throw std::runtime_error("Cannot load ruleset file");
    }
    load(stream);
    return;
  }
  
  void RuleSetPrivate::load(std::istream& stream)
  {
    std::unique_lock<std::mutex> lock(_io_mutex);
    std::string line_string;
    size_t line_number = 0;

    do {
      ++line_number;
      std::getline(stream, line_string);
      const Rule rule = Rule::fromString(line_string);
      if (rule) {
	appendRule(rule);
      }
    } while(stream.good());

    return;
  }
  
  void RuleSetPrivate::save(const String& path) const
  {
    std::ofstream stream(path, std::fstream::trunc);
    if (!stream.is_open()) {
      throw std::runtime_error("Cannot store ruleset to file");
    }
    save(stream);
    return;
  }
  
  void RuleSetPrivate::save(std::ostream& stream) const
  {
    std::unique_lock<std::mutex> io_lock(_io_mutex);
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto const& rule : _rules) {
      const std::string rule_string = rule->toString();
      stream << rule_string << std::endl;
    }
    return;
  }
  
  void RuleSetPrivate::setDefaultTarget(Rule::Target target)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    _default_target = target;
    return;
  }

  void RuleSetPrivate::setDefaultAction(const String& action)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    _default_action = action;
    return;
  }

  uint32_t RuleSetPrivate::appendRule(const Rule& rule, uint32_t parent_id)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    auto rule_ptr = makePointer<Rule>(rule);

    /*
     * If the rule doesn't already have a sequence number
     * assigned, do it now. Otherwise update the sequence
     * number counter so that we don't generate a duplicit
     * one if assignID() gets called in the future.
     */
    if (rule_ptr->getRuleID() == Rule::DefaultID) {
      assignID(rule_ptr);
    }
    else {
      _id_next = std::max(_id_next.load(), rule_ptr->getRuleID() + 1);
    }

    /* Initialize conditions */
    rule_ptr->internal()->initConditions(_interface_ptr);

    /* Append the rule to the main rule table */
    if (parent_id == Rule::LastID) {
      _rules.push_back(rule_ptr);
    }
    else if (parent_id == 0) {
      _rules.insert(_rules.begin(), rule_ptr);
    }
    else {
      bool parent_found = false;
      for (auto it = _rules.begin(); it != _rules.end(); ++it) {
	const Rule& rule = **it;
	if (rule.getRuleID() == parent_id) {
	  _rules.insert(it+1, rule_ptr);
	  parent_found = true;
	  break;
	}
      }
      if (!parent_found) {
	throw std::runtime_error("Invalid parent_id");
      }
    }

    /* If the rule is timed, put it into the priority queue */
    if (rule_ptr->getTimeoutSeconds() > 0) {
      _rules_timed.push(rule_ptr);
    }

    return rule_ptr->getRuleID();
  }

  Pointer<const Rule> RuleSetPrivate::getRule(uint32_t id)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    for (auto const& rule : _rules) {
      if (rule->getRuleID() == id) {
	return rule;
      }
    }
    throw std::out_of_range("Rule not found");
  }

  bool RuleSetPrivate::removeRule(uint32_t id)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    for (auto it = _rules.begin(); it != _rules.end(); ++it) {
      auto const& rule_ptr = *it;
      if (rule_ptr->getRuleID() == id) {
        _rules.erase(it);
        return true;
      }
    }
    /* FIXME: Remove the rule from the priority queue too */
    throw std::out_of_range("Rule not found");
  }

  Pointer<Rule> RuleSetPrivate::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_id) const
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto& rule_ptr : _rules) {
      if (rule_ptr->internal()->appliesToWithConditions(*device_rule, /*with_update*/true)) {
	return rule_ptr;
      }
    }

    Pointer<Rule> default_rule = makePointer<Rule>();

    default_rule->setRuleID(Rule::DefaultID);
    default_rule->setTarget(_default_target);

    return default_rule;
  }

  PointerVector<const Rule> RuleSetPrivate::getRules()
  {
    PointerVector<const Rule> rules;

    for (auto const& rule : _rules) {
      rules.push_back(rule);
    }

    return rules;
  }

  Pointer<Rule> RuleSetPrivate::getTimedOutRule()
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    if (_rules_timed.size() < 1) {
      return nullptr;
    }

    Pointer<Rule> oldest_rule = _rules_timed.top();
    std::chrono::steady_clock::time_point tp_current =	\
      std::chrono::steady_clock::now();

    if ((tp_current - oldest_rule->internal()->metadata().tp_created) \
	< std::chrono::seconds(oldest_rule->getTimeoutSeconds())) {
      return nullptr;
    } else {
      _rules_timed.pop();
    }

    return oldest_rule;
  }

  uint32_t RuleSetPrivate::assignID(Pointer<Rule> rule)
  {
    rule->setRuleID(assignID());
    return rule->getRuleID();
  }

  uint32_t RuleSetPrivate::assignID()
  {
    return _id_next++;
  }

} /* namespace usbguard */
