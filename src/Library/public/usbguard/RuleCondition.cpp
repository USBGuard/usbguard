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

#include "usbguard/RuleCondition.hpp"

#include "usbguard/Rule.hpp"
#include "usbguard/Logger.hpp"

#include <unordered_map>
#include <functional>

namespace usbguard
{
  RuleConditionBase::RuleConditionBase(const std::string& identifier, const std::string& parameter, bool negated)
    : _identifier(identifier),
      _parameter(parameter),
      _negated(negated)
  {
  }

  RuleConditionBase::RuleConditionBase(const std::string& identifier, bool negated)
    : _identifier(identifier),
      _negated(negated)
  {
  }

  RuleConditionBase::RuleConditionBase(const RuleConditionBase& rhs)
    : _identifier(rhs._identifier),
      _parameter(rhs._parameter),
      _negated(rhs._negated)
  {
  }

  RuleConditionBase::~RuleConditionBase()
  {
    fini();
  }

  void RuleConditionBase::init(Interface* const interface_ptr)
  {
    (void)interface_ptr;
  }

  void RuleConditionBase::fini()
  {
  }

  bool RuleConditionBase::evaluate(const Rule& rule)
  {
    return isNegated() ? !update(rule) : update(rule);
  }

  const std::string& RuleConditionBase::identifier() const
  {
    return _identifier;
  }

  const std::string& RuleConditionBase::parameter() const
  {
    return _parameter;
  }

  bool RuleConditionBase::hasParameter() const
  {
    return !_parameter.empty();
  }

  bool RuleConditionBase::isNegated() const
  {
    return _negated;
  }

  const std::string RuleConditionBase::toString() const
  {
    std::string condition_string;

    if (isNegated()) {
      condition_string.append("!");
    }

    condition_string.append(identifier());

    if (hasParameter()) {
      condition_string.append("(");
      condition_string.append(parameter()); /* TODO: Escape parameter string */
      condition_string.append(")");
    }

    return condition_string;
  }

  const std::string RuleConditionBase::toRuleString() const
  {
    return toString();
  }
} /* namespace usbguard */

#include "AllowedMatchesCondition.hpp"
#include "LocaltimeCondition.hpp"
#include "FixedStateCondition.hpp"
#include "RandomStateCondition.hpp"
#include "RuleAppliedCondition.hpp"
#include "RuleEvaluatedCondition.hpp"

#include <iostream>
#include <memory>

namespace usbguard
{
  RuleConditionBase* RuleConditionBase::getImplementation(const std::string& condition_string)
  {
    if (condition_string.empty()) {
      throw std::runtime_error("Empty condition");
    }

    const bool negated = condition_string[0] == '!';
    const size_t identifier_start = negated ? 1 : 0;
    const size_t p_pos = condition_string.find_first_of('(');
    std::string identifier;
    std::string parameter;

    if (p_pos == std::string::npos) {
      /*
       * The rest of the condition_string should be
       * a condition identifier -- without a parameter.
       */
      identifier = condition_string.substr(identifier_start);

      if (identifier.size() < 1) {
        throw std::runtime_error("Invalid condition string. Missing identifier.");
      }
    }
    else {
      const size_t parameter_size = condition_string.size() - p_pos;

      if (parameter_size < 3 /* two parentheses + at least one character */) {
        throw std::runtime_error("Invalid condition string. Invalid parameter.");
      }

      const size_t identifier_size = p_pos - identifier_start;
      identifier = condition_string.substr(identifier_start, identifier_size);

      if (condition_string[condition_string.size() - 1] != ')') {
        throw std::runtime_error("Invalid condition string. Malformed parameter.");
      }

      parameter = condition_string.substr(p_pos + 1, parameter_size - 2);
    }

    return getImplementation(identifier, parameter, negated);
  }

  RuleConditionBase* RuleConditionBase::getImplementation(const std::string& identifier, const std::string& parameter,
    bool negated)
  {
    if (identifier == "allowed-matches") {
      return new AllowedMatchesCondition(parameter, negated);
    }

    if (identifier == "localtime") {
      return new LocaltimeCondition(parameter, negated);
    }

    if (identifier == "true") {
      return new FixedStateCondition(true, negated);
    }

    if (identifier == "false") {
      return new FixedStateCondition(false, negated);
    }

    if (identifier == "random") {
      return new RandomStateCondition(parameter, negated);
    }

    if (identifier == "rule-applied") {
      return new RuleAppliedCondition(parameter, negated);
    }

    if (identifier == "rule-evaluated") {
      return new RuleEvaluatedCondition(parameter, negated);
    }

    throw std::runtime_error("Unknown rule condition");
  }

  RuleCondition::RuleCondition()
  {
  }

  RuleCondition::RuleCondition(const std::string& condition_string)
    : _condition(RuleConditionBase::getImplementation(condition_string))
  {
  }

  RuleCondition::RuleCondition(const RuleCondition& rhs)
    : _condition(rhs._condition->clone())
  {
  }

  RuleCondition::RuleCondition(RuleCondition&& rhs)
    : _condition(std::move(rhs._condition))
  {
  }

  RuleCondition& RuleCondition::operator=(const RuleCondition& rhs)
  {
    _condition.reset(rhs._condition->clone());
    return *this;
  }

  RuleCondition& RuleCondition::operator=(RuleCondition&& rhs)
  {
    _condition = std::move(rhs._condition);
    return *this;
  }

  RuleConditionBase* RuleCondition::operator->()
  {
    return _condition.get();
  }

  RuleConditionBase& RuleCondition::operator*()
  {
    return *_condition.get();
  }

  std::string RuleCondition::toRuleString() const
  {
    return _condition->toRuleString();
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
