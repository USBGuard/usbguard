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
#include "Policy.hpp"
#include "Exception.hpp"

namespace usbguard
{
  Policy::Policy(Interface * const interface_ptr)
    : _ruleset(interface_ptr)
  {
    _source = Policy::SourceType::Local;
  }

  const RuleSet Policy::getRuleSet()
  {
    return _ruleset;
  }

  void Policy::setPolicySource(Policy::SourceType source)
  {
    _source = source;
  }

  void Policy::setLocalSourcePath(const String& path)
  {
    _localPath = path;
  }

  void Policy::load()
  {
    if (this->_source == SourceType::Local && _localPath != "") {
      load(_localPath);
    } else {
      throw Exception("Policy load", "localPath", "Local path for local rules was not set");
    }

  }

  void Policy::load(const String& path)
  {
    _ruleset.load(path);
  }

  void Policy::load(std::istream& stream)
  {
    _ruleset.load(stream);
  }

  void Policy::save(const String& path) const
  {
    _ruleset.save(path);
  }

  void Policy::save(std::ostream& stream) const
  {
    _ruleset.save(stream);
  }

  void Policy::setDefaultTarget(Rule::Target target)
  {
    _ruleset.setDefaultTarget(target);
  }

  Rule::Target Policy::getDefaultTarget() const
  {
    return _ruleset.getDefaultTarget();
  }

  void Policy::setDefaultAction(const String& action)
  {
    _ruleset.setDefaultAction(action);
  }

  uint32_t Policy::appendRule(const Rule& rule, uint32_t parent_id)
  {
    return _ruleset.appendRule(rule, parent_id);
  }

  uint32_t Policy::upsertRule(const Rule& match_rule, const Rule& new_rule, const bool parent_insensitive)
  {
    return _ruleset.upsertRule(match_rule, new_rule, parent_insensitive);
  }

  Pointer<Rule> Policy::getRule(uint32_t id)
  {
    return _ruleset.getRule(id);
  }

  bool Policy::removeRule(uint32_t id)
  {
    return _ruleset.removeRule(id);
  }

  Pointer<Rule> Policy::getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_id) const
  {
    return _ruleset.getFirstMatchingRule(device_rule, from_id);
  }

  PointerVector<const Rule> Policy::getRules()
  {
    return _ruleset.getRules();
  }

  Pointer<Rule> Policy::getTimedOutRule()
  {
    return _ruleset.getTimedOutRule();
  }

  uint32_t Policy::assignID(Pointer<Rule> rule)
  {
    return _ruleset.assignID(rule);
  }

  uint32_t Policy::assignID()
  {
    return _ruleset.assignID();
  }

  std::string Policy::eventTypeToString(Policy::EventType event)
  {
    switch(event) {
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

  std::string Policy::sourceTypeToString(Policy::SourceType source)
  {
    switch(source) {
      case Policy::SourceType::Local:
        return "Local";
      case Policy::SourceType::Ldap:
        return "Ldap";
      case Policy::SourceType::SSSD:
        return "SSSD";
      default:
        throw USBGUARD_BUG("unknown Policy::SourceType value");
    }
  }
} /* namespace usbguard */
