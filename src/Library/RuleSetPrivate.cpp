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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "RuleSetPrivate.hpp"
#include "RulePrivate.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/RuleParser.hpp"
#include "usbguard/Exception.hpp"

#include <fstream>

namespace usbguard
{

  RuleSetPrivate::RuleSetPrivate(RuleSet& p_instance, Interface* const interface_ptr)
    : _p_instance(p_instance),
      _interface_ptr(interface_ptr)
  {
    (void)_p_instance;
    _default_target = Rule::Target::Block;
    _default_action = std::string();
    _id_next = Rule::RootID + 1;
  }

  RuleSetPrivate::RuleSetPrivate(RuleSet& p_instance, const RuleSetPrivate& rhs)
    : _p_instance(p_instance),
      _interface_ptr(rhs._interface_ptr)
  {
    *this = rhs;
  }

  const RuleSetPrivate& RuleSetPrivate::operator=(const RuleSetPrivate& rhs)
  {
    _default_target = rhs._default_target;
    _default_action = rhs._default_action;
    _id_next = rhs._id_next.load();
    _rules = rhs._rules;
    return *this;
  }

  RuleSetPrivate::~RuleSetPrivate()
  {
  }

  void RuleSetPrivate::load(const std::string& path)
  {
    std::ifstream stream(path);

    if (!stream.is_open()) {
      throw ErrnoException("RuleSet loading", path, errno);
    }

    load(stream);
  }

  void RuleSetPrivate::load(std::istream& stream)
  {
    std::unique_lock<std::mutex> lock(_io_mutex);
    std::string line_string;
    size_t line_number = 0;

    do {
      ++line_number;
      std::getline(stream, line_string);
      const Rule rule = parseRuleFromString(line_string, "", line_number);

      if (rule) {
        appendRule(rule);
      }
    }
    while (stream.good());
  }

  void RuleSetPrivate::save(const std::string& path) const
  {
    std::ofstream stream(path, std::fstream::trunc);

    if (!stream.is_open()) {
      throw ErrnoException("RuleSet saving", path, errno);
    }

    save(stream);
  }

  void RuleSetPrivate::save(std::ostream& stream) const
  {
    std::unique_lock<std::mutex> io_lock(_io_mutex);
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto const& rule : _rules) {
      const std::string rule_string = rule->toString();
      stream << rule_string << std::endl;
    }
  }

  void RuleSetPrivate::setDefaultTarget(Rule::Target target)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    _default_target = target;
  }

  Rule::Target RuleSetPrivate::getDefaultTarget() const
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    return _default_target;
  }

  void RuleSetPrivate::setDefaultAction(const std::string& action)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    _default_action = action;
  }

  uint32_t RuleSetPrivate::appendRule(const Rule& rule, uint32_t parent_id, bool lock)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex, std::defer_lock);

    if (lock) {
      op_lock.lock();
    }

    auto rule_ptr = std::make_shared<Rule>(rule);

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
        throw Exception("Rule set append", "rule", "Invalid parent ID");
      }
    }

    return rule_ptr->getRuleID();
  }

  uint32_t RuleSetPrivate::upsertRule(const Rule& match_rule, const Rule& new_rule, const bool parent_insensitive)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    std::shared_ptr<Rule> matching_rule;

    for (auto& rule_ptr : _rules) {
      if (rule_ptr->internal()->appliesTo(match_rule, parent_insensitive)) {
        if (!matching_rule) {
          matching_rule = rule_ptr;
        }
        else {
          throw Exception("Rule set upsert", "rule", "Cannot upsert; multiple matching rules");
        }
      }
    }

    if (matching_rule) {
      const uint32_t id = matching_rule->getRuleID();
      *matching_rule = new_rule;
      matching_rule->setRuleID(id);
      return id;
    }
    else {
      return appendRule(new_rule, Rule::LastID, /*lock=*/false);
    }
  }

  std::shared_ptr<Rule> RuleSetPrivate::getRule(uint32_t id)
  {
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto const& rule : _rules) {
      if (rule->getRuleID() == id) {
        return rule;
      }
    }

    throw Exception("Rule set lookup", "rule id", "id doesn't exist");
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
    throw Exception("Rule set remove", "rule id", "id doesn't exist");
  }

  std::shared_ptr<Rule> RuleSetPrivate::getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id) const
  {
    (void)from_id; /* TODO */
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto& rule_ptr : _rules) {
      if (rule_ptr->internal()->appliesToWithConditions(*device_rule, /*with_update*/true)) {
        return rule_ptr;
      }
    }

    std::shared_ptr<Rule> default_rule = std::make_shared<Rule>();
    default_rule->setRuleID(Rule::ImplicitID);
    default_rule->setTarget(_default_target);
    return default_rule;
  }

  std::vector<std::shared_ptr<const Rule>> RuleSetPrivate::getRules()
  {
    std::vector<std::shared_ptr<const Rule>> rules;

    for (auto const& rule : _rules) {
      rules.push_back(rule);
    }

    return rules;
  }

  uint32_t RuleSetPrivate::assignID(std::shared_ptr<Rule> rule)
  {
    rule->setRuleID(assignID());
    return rule->getRuleID();
  }

  uint32_t RuleSetPrivate::assignID()
  {
    return _id_next++;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
