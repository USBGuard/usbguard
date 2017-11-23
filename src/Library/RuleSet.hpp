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
// Authors: Radovan Sroka <rsroka@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Typedefs.hpp"
#include "usbguard/Rule.hpp"

#include <istream>
#include <ostream>
#include <mutex>

namespace usbguard
{
  class Interface;
  class DLL_PUBLIC RuleSet
  {
  public:

    RuleSet(Interface* const interface_ptr);
    RuleSet(const RuleSet& rhs);
    const RuleSet& operator=(const RuleSet& rhs);

    virtual void load();
    virtual void save();

    void serialize(std::ostream& stream) const;

    void setDefaultTarget(Rule::Target target);
    Rule::Target getDefaultTarget() const;
    void setDefaultAction(const std::string& action);
    uint32_t appendRule(const Rule& rule, uint32_t parent_id = Rule::LastID, bool lock = true);
    uint32_t upsertRule(const Rule& match_rule, const Rule& new_rule, bool parent_insensitive = false);
    std::shared_ptr<Rule> getRule(uint32_t id);
    bool removeRule(uint32_t id);
    std::shared_ptr<Rule> getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id = 1) const;
    std::vector<std::shared_ptr<const Rule>> getRules();
    uint32_t assignID(std::shared_ptr<Rule> rule);
    uint32_t assignID();

    void setWritable();
    void clearWritable();
    bool isWritable();

  protected:
    mutable std::mutex _io_mutex; /* mutex for load/save */
    mutable std::mutex _op_mutex; /* mutex for operations on the rule set */

    bool _writable;

    Interface* const _interface_ptr;
    Rule::Target _default_target;
    std::string _default_action;
    Atomic<uint32_t> _id_next;
    std::vector<std::shared_ptr<Rule>> _rules;
  };

  class RuleSetAbstract : public RuleSet
  {
  public:
    RuleSetAbstract(Interface* const interface_ptr)
      : RuleSet(interface_ptr) {};
    RuleSetAbstract(const RuleSetAbstract& rhs)
      : RuleSet(rhs._interface_ptr) {};

    virtual void load() = 0;
    virtual void save() = 0;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
