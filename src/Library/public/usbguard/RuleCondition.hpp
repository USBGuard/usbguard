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
#pragma once

#include "Typedefs.hpp"

#include <string>
#include <memory>

namespace usbguard
{
  class Interface;
  class Rule;
  class DLL_PUBLIC RuleConditionBase
  {
  public:
    RuleConditionBase(const std::string& identifier, const std::string& parameter, bool negated = false);
    RuleConditionBase(const std::string& identifier, bool negated = false);
    RuleConditionBase(const RuleConditionBase& rhs);
    virtual ~RuleConditionBase();

    virtual void init(Interface* const interface_ptr);
    virtual void fini();
    virtual bool update(const Rule& rule) = 0;
    virtual RuleConditionBase* clone() const = 0;

    bool evaluate(const Rule& rule);
    const std::string& identifier() const;
    const std::string& parameter() const;
    bool hasParameter() const;
    bool isNegated() const;
    const std::string toString() const;
    const std::string toRuleString() const;

    static RuleConditionBase* getImplementation(const std::string& condition_string);
    static RuleConditionBase* getImplementation(const std::string& identifier, const std::string& parameter, bool negated);

  private:
    const std::string _identifier;
    const std::string _parameter;
    const bool _negated;
  };

  class DLL_PUBLIC RuleCondition
  {
  public:
    RuleCondition();
    RuleCondition(const std::string& condition_string);
    RuleCondition(const RuleCondition& rhs);
    RuleCondition(RuleCondition&& rhs);

    RuleCondition& operator=(const RuleCondition& rhs);
    RuleCondition& operator=(RuleCondition&& rhs);

    RuleConditionBase* operator->();
    RuleConditionBase& operator*();

    std::string toRuleString() const;
  private:
    std::unique_ptr<RuleConditionBase> _condition;
  };
} /*namespace usbguard */

/* vim: set ts=2 sw=2 et */
