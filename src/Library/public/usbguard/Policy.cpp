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
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Policy.hpp"
#include "usbguard/Exception.hpp"

namespace usbguard
{
  Policy::Policy()
  {
    _ruleset_ptr = nullptr;
  }

  void Policy::setRuleSet(std::shared_ptr<RuleSet> ptr)
  {
    _ruleset_ptr = ptr;
  }

  const RuleSet Policy::getRuleSet()
  {
    return *_ruleset_ptr;
  }

  void Policy::load(const String& path)
  {
    _ruleset_ptr->load(path);
  }

  void Policy::load(std::istream& stream)
  {
    _ruleset_ptr->load(stream);
  }

  void Policy::save(const String& path) const
  {
    _ruleset_ptr->save(path);
  }

  void Policy::save(std::ostream& stream) const
  {
    _ruleset_ptr->save(stream);
  }

  void Policy::setDefaultTarget(Rule::Target target)
  {
    _ruleset_ptr->setDefaultTarget(target);
  }

  Rule::Target Policy::getDefaultTarget() const
  {
    return _ruleset_ptr->getDefaultTarget();
  }

  void Policy::setDefaultAction(const String& action)
  {
    _ruleset_ptr->setDefaultAction(action);
  }

  uint32_t Policy::appendRule(const Rule& rule, uint32_t parent_id)
  {
    return _ruleset_ptr->appendRule(rule, parent_id);
  }

  uint32_t Policy::upsertRule(const Rule& match_rule, const Rule& new_rule, const bool parent_insensitive)
  {
    return _ruleset_ptr->upsertRule(match_rule, new_rule, parent_insensitive);
  }

  Pointer<Rule> Policy::getRule(uint32_t id)
  {
    return _ruleset_ptr->getRule(id);
  }

  bool Policy::removeRule(uint32_t id)
  {
    return _ruleset_ptr->removeRule(id);
  }

  Pointer<Rule> Policy::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_id) const
  {
    return _ruleset_ptr->getFirstMatchingRule(device_rule, from_id);
  }

  PointerVector<const Rule> Policy::getRules()
  {
    return _ruleset_ptr->getRules();
  }

  Pointer<Rule> Policy::getTimedOutRule()
  {
    return _ruleset_ptr->getTimedOutRule();
  }

  uint32_t Policy::assignID(Pointer<Rule> rule)
  {
    return _ruleset_ptr->assignID(rule);
  }

  uint32_t Policy::assignID()
  {
    return _ruleset_ptr->assignID();
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
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
