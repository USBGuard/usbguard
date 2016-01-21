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
  
  RuleSetPrivate::RuleSetPrivate(RuleSet& p_instance)
    : _p_instance(p_instance)
  {
    (void)_p_instance;
    _default_target = Rule::Target::Block;
    _default_action = String();
    _seqn_next = Rule::SeqnRoot + 1;
    return;
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

  uint32_t RuleSetPrivate::appendRule(const Rule& rule, uint32_t parent_seqn, Interface * const interface)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    auto rule_ptr = makePointer<Rule>(rule);

    /* Assign a unique sequence number to the rule */
    assignSeqn(rule_ptr);

    /* Set time */
    rule_ptr->setTimePointAdded(std::chrono::steady_clock::now());

    /* Initialize conditions */
    rule_ptr->internal()->initConditions(interface);

    /* Append the rule to the main rule table */
    if (parent_seqn == Rule::SeqnLast) {
      _rules.push_back(rule_ptr);
    }
    else if (parent_seqn == 0) {
      _rules.insert(_rules.begin(), rule_ptr);
    }
    else {
      bool parent_found = false;
      for (auto it = _rules.begin(); it != _rules.end(); ++it) {
	const Rule& rule = **it;
	if (rule.getSeqn() == parent_seqn) {
	  _rules.insert(it+1, rule_ptr);
	  parent_found = true;
	  break;
	}
      }
      if (!parent_found) {
	throw std::runtime_error("Invalid parent_seqn");
      }
    }

    /* If the rule is timed, put it into the priority queue */
    if (rule_ptr->getTimeoutSeconds() > 0) {
      _rules_timed.push(rule_ptr);
    }

    return rule_ptr->getSeqn();
  }

  Pointer<const Rule> RuleSetPrivate::getRule(uint32_t seqn)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    for (auto const& rule : _rules) {
      if (rule->getSeqn() == seqn) {
	return rule;
      }
    }
    throw std::out_of_range("Rule not found");
  }

  bool RuleSetPrivate::removeRule(uint32_t seqn)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    for (auto it = _rules.begin(); it != _rules.end(); ++it) {
      auto const& rule_ptr = *it;
      if (rule_ptr->getSeqn() == seqn) {
        _rules.erase(it);
        return true;
      }
    }
    /* FIXME: Remove the rule from the priority queue too */
    throw std::out_of_range("Rule not found");
  }

  Pointer<const Rule> RuleSetPrivate::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_seqn)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto const& rule_ptr : _rules) {
      if (rule_ptr->internal()->appliesToWithConditions(*device_rule, /*with_update*/true)) {
	return rule_ptr;
      }
    }

    Pointer<Rule> default_rule = makePointer<Rule>();

    default_rule->setSeqn(Rule::SeqnDefault);
    default_rule->setTarget(_default_target);
    default_rule->setAction(_default_action);

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

    if ((tp_current - oldest_rule->getTimePointAdded()) \
	< std::chrono::seconds(oldest_rule->getTimeoutSeconds())) {
      return nullptr;
    } else {
      _rules_timed.pop();
    }

    return oldest_rule;
  }

  uint32_t RuleSetPrivate::assignSeqn(Pointer<Rule> rule)
  {
    rule->setSeqn(assignSeqn());
    return rule->getSeqn();
  }

  uint32_t RuleSetPrivate::assignSeqn()
  {
    return _seqn_next++;
  }

} /* namespace usbguard */
