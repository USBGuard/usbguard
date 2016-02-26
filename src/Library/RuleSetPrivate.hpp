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
#include <build-config.h>
#include "Typedefs.hpp"
#include "RuleSet.hpp"
#include <istream>
#include <ostream>
#include <mutex>

namespace usbguard {
  class RuleSetPrivate
  {
  public:
    RuleSetPrivate(RuleSet& p_instance, Interface * const interface_ptr);
    RuleSetPrivate(RuleSet& p_instance, const RuleSetPrivate& rhs);
    const RuleSetPrivate& operator=(const RuleSetPrivate& rhs);
    ~RuleSetPrivate();

    void load(const String& path);
    void load(std::istream& stream);
    void save(const String& path) const;
    void save(std::ostream& stream) const;
    void setDefaultTarget(Rule::Target target);
    void setDefaultAction(const String& action);
    uint32_t appendRule(const Rule& rule, uint32_t parent_id = Rule::LastID);
    Pointer<const Rule> getRule(uint32_t id);
    bool removeRule(uint32_t id);
    Pointer<Rule> getFirstMatchingRule(Pointer<const Rule> device_rule, uint32_t from_id = 1) const;
    PointerVector<const Rule> getRules();
    Pointer<Rule> getTimedOutRule();
    uint32_t assignID(Pointer<Rule> rule);
    uint32_t assignID();

  private:
    mutable std::mutex _io_mutex; /* mutex for load/save */
    mutable std::mutex _op_mutex; /* mutex for operations on the rule set */
    RuleSet& _p_instance;
    Interface * const _interface_ptr;
    Rule::Target _default_target;
    String _default_action;
    Atomic<uint32_t> _id_next;
    PointerVector<Rule> _rules;
    PointerPQueue<Rule> _rules_timed;
  };
}
