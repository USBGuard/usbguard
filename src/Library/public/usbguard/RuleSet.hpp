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
#pragma once

#include "Rule.hpp"
#include "Typedefs.hpp"

#include <istream>
#include <ostream>
#include <string>
#include <memory>

#include <cstdint>

namespace usbguard
{
  class RuleSetPrivate;
  class Interface;
  class DLL_PUBLIC RuleSet
  {
  public:
    /**
     * Construct an empty ruleset.
     */
    RuleSet(Interface* const interface_ptr);
    RuleSet(const RuleSet& rhs);
    const RuleSet& operator=(const RuleSet& rhs);
    ~RuleSet();

    /**
     * Load a ruleset from a file at `path'.
     * Internally, this opens an input file stream and calls load(std::istream& stream).
     */
    void load(const std::string& path);

    /**
     * Load a ruleset from an input stream.
     * The stream is read using std::getline() and each line is parsed as a separate rule.
     * Empty lines are skipped.
     */
    void load(std::istream& stream);

    /**
     * Save the ruleset to a file at `path'.
     * If a file exists at `path', it will be overwritten. Internally, this opens an output
     * stream and calls save(std::ostream& stream).
     */
    void save(const std::string& path) const;

    /**
     * Write the ruleset to an output stream.
     * Each rule is serialized to it's string representation and written line by line to
     * the output stream.
     */
    void save(std::ostream& stream) const;

    /**
     * Set an implicit default target which will be used if there's no match for a device
     * rule.
     */
    void setDefaultTarget(Rule::Target target);

    /**
     * Get the implicit target value.
     */
    Rule::Target getDefaultTarget() const;

    /**
     * Set an implicit default action which will be used if there's no match for a device
     * rule.
     */
    void setDefaultAction(const std::string& action);

    /**
     * Assign a sequence number to a rule and append it to the rule set.
     * If `parent_id' is not specified, the rule will be appended at the end od the ruleset.
     * The method returns the sequence number assigned to the rule.
     */
    uint32_t appendRule(const Rule& rule, uint32_t parent_id = Rule::LastID);

    /**
     * Search for a rule that matches `match_rule' rule and update it with a rule specified
     * by `new_rule'. Fail if multiple rules match. If there are no matching rules, append
     * the `new_rule' rule to the rule set.
     *
     * Returns the id of the updated or new rule.
     */
    uint32_t upsertRule(const Rule& match_rule, const Rule& new_rule, bool parent_insensitive = false);

    /**
     * Get a rule pointer to a rule with the specified sequence number.
     * Returns nullptr if no such rule exists.
     */
    std::shared_ptr<Rule> getRule(uint32_t id);

    /**
     * Remove a rule from the ruleset.
     * The method returns true if a rule was removed and false otherwise.
     */
    bool removeRule(uint32_t id);

    /**
     * Find first rule in the ruleset which matched the specified device rule.
     * If `from_id' isn't specified, the method searches from the beginning of the ruleset.
     * If no matching rule is found, nullptr is returned.
     */
    std::shared_ptr<Rule> getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id = 1) const;

    /**
     * Get all rules from the set.
     */
    std::vector<std::shared_ptr<const Rule>> getRules();

    /**
     * Get the oldest rule that timed out and should be removed from the ruleset.
     * Returns nullptr if there are not timed out rules.
     */
    std::shared_ptr<Rule> getTimedOutRule();

    /**
     * Assign a unique sequence number to a rule.
     * Return the assigned sequence number.
     */
    uint32_t assignID(std::shared_ptr<Rule> rule);

    /**
     * Generate a unique sequence number.
     */
    uint32_t assignID();

  private:
    RuleSetPrivate* d_pointer;
  };
}

/* vim: set ts=2 sw=2 et */
