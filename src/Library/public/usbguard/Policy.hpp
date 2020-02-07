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
#pragma once

#include <string>
#include <vector>

#include "RuleSet.hpp"
#include "Typedefs.hpp"

namespace usbguard
{
  class DLL_PUBLIC Policy
  {
  public:
    enum class EventType {
      Insert = 1,
      Update = 2,
      Remove = 3
    };

    Policy();

    void setRuleSet(std::vector<std::shared_ptr<RuleSet>> ptr);
    std::vector<std::shared_ptr<RuleSet>> getRuleSet();

    void setDefaultTarget(Rule::Target target);
    Rule::Target getDefaultTarget() const;
    void setDefaultAction(const std::string& action);
    uint32_t appendRule(const Rule& rule, uint32_t parent_id = Rule::LastID);
    uint32_t upsertRule(const Rule& match_rule, const Rule& new_rule, bool parent_insensitive = false);
    std::shared_ptr<Rule> getRule(uint32_t id);
    bool removeRule(uint32_t id);
    std::shared_ptr<Rule> getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id = 1) const;
    std::vector<std::shared_ptr<const Rule>> getRules();
    uint32_t assignID(std::shared_ptr<Rule> rule);
    uint32_t assignID();
    void save(void);


    /**
     * @brief Converts EventType to its string representation.
     *
     * @param event EventType to convert.
     * @return String representation of given EventType.
     * @throw USBGUARD_BUG If given EventType is unknown.
     */
    static std::string eventTypeToString(EventType event);
  private:

    std::vector<std::shared_ptr<RuleSet>> _rulesets_ptr;
    Rule::Target _defaultTarget;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
