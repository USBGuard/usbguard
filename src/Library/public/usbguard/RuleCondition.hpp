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

  /**
   * @brief Base class for all specialized rule condition classes.
   */
  class DLL_PUBLIC RuleConditionBase
  {
  public:
    /**
     * @brief Constructs new rule condition.
     *
     * @param identifier Identifier/name of rule condition.
     * @param parameter Parameter of rule condition.
     * @param negated Defines whether rule condition should be negated or not.
     * Rule condition is defaultly not negated.
     */
    RuleConditionBase(const std::string& identifier, const std::string& parameter, bool negated = false);

    /**
     * @brief Constructs new rule condition without a parameter.
     *
     * @param identifier Identifier/name of rule condition.
     * @param negated Defines whether rule condition should be negated or not.
     * Rule condition is defaultly not negated.
     */
    RuleConditionBase(const std::string& identifier, bool negated = false);

    /**
     * @brief Constructs new rule condition with the same attributes as rhs.
     *
     * @param rhs Rule condition to copy.
     */
    RuleConditionBase(const RuleConditionBase& rhs);

    /**
     * @brief Calls \link fini() fini()\endlink and destroys this object.
     *
     * @see \link fini() fini()\endlink
     */
    virtual ~RuleConditionBase();

    /**
     * @brief Initialization algorithm for this object.
     *
     * @param interface_ptr Pointer to the interface.
     */
    virtual void init(Interface* const interface_ptr);

    /**
     * @brief Implement what should be done before destroying this object.
     */
    virtual void fini();

    /**
     * @brief Implement update algorithm for this object.
     *
     * @param rule You might want to update based on some rule.
     * @return True if update is successful, false otherwise.
     */
    virtual bool update(const Rule& rule) = 0;

    /**
     * @brief Implement cloning algorithm for this object.
     *
     * @return Clone of this object.
     */
    virtual RuleConditionBase* clone() const = 0;

    /**
     * @brief Evaluate based on given rule.
     *
     * Evaluation returns true if update(rule) succeeds or if this condition
     * is negated then returns true if update(rule) does not succeed.
     *
     * @param rule Rule to evaluate.
     * @return True if rule has been evaluated successfully, false otherwise.
     * @see \link update() update()\endlink
     */
    bool evaluate(const Rule& rule);

    /**
     * @brief Returns rule condition identifier/name.
     *
     * @return rule condition identifier/name.
     */
    const std::string& identifier() const;

    /**
     * @brief Returns parameter of rule condition.
     *
     * @return Parameter of rule condition.
     */
    const std::string& parameter() const;

    /**
     * @brief Checks whether rule condition has parameter.
     *
     * In other words, checks whether the parameter is not empty.
     *
     * @return True if rule condition has parameter, false otherwise.
     */
    bool hasParameter() const;

    /**
     * @brief Checks whether this rule condition is negated or not.
     *
     * @return True if this rule condition is negated, false otherwise.
     */
    bool isNegated() const;

    /**
     * @brief Returns string representation of this rule condition.
     *
     * - Format: [!]\<identifier\>[(\<parameter\>)]
     * - Example 1: !rule-evaluated
     * - Example 2: random(0.1666)
     *
     * @return String representation of this rule condition.
     */
    const std::string toString() const;

    /**
     * @copydoc toString()
     */
    const std::string toRuleString() const;

    /**
     * @brief Parses given rule string and constructs new specialized
     * rule condition object.
     *
     * @param condition_string String should be a valid rule string.
     * @return Pointer to newly constructed specialized rule condition object.
     * @throw runtime_error If given string is empty or does not contain
     * a valid rule.
     */
    static RuleConditionBase* getImplementation(const std::string& condition_string);

    /**
     * @brief Constructs new specialized rule condition object
     * based on given arguments.
     *
     * @param identifier Identifies what rule condition object should be
     * constructed.
     * @param parameter Parameter of new rule condition.
     * @param negated Defines whether new rule condition is negated or not.
     * @return Pointer to newly constructed specialized rule condition object.
     * @throw runtime_error If identifier is not valid.
     */
    static RuleConditionBase* getImplementation(const std::string& identifier, const std::string& parameter, bool negated);

  private:
    const std::string _identifier; /**< Identifier/name of this
                                     rule condition. */
    const std::string _parameter; /**< Parameter of rule condition.
                                    Might be empty if this rule condition
                                    does not have any parameter. */
    const bool _negated; /**< Defines whether this rule condition
                           is negated or not. */
  };

  /**
   * @brief Wraps any type of rule condition.
   */
  class DLL_PUBLIC RuleCondition
  {
  public:
    /**
     * @brief Constructs empty rule condition object.
     *
     * In other words, the unique pointer that points to the underlying
     * rule condition object is empty.
     */
    RuleCondition();

    /**
     * @brief Constructs rule condition object based on given rule string.
     *
     * @param condition_string String should be a valid rule string.
     * @throw runtime_error If given string is empty or does not contain
     * a valid rule.
     * @see \link RuleConditionBase::getImplementation()
     * getImplementation()\endlink
     */
    RuleCondition(const std::string& condition_string);

    /**
     * @brief Constructs rule condition by copying rhs.
     *
     * @param rhs Rule condition to copy.
     */
    RuleCondition(const RuleCondition& rhs);

    /**
     * @brief Constructs rule condition from rhs using move semantics.
     *
     * @param rhs Rule condition to move.
     */
    RuleCondition(RuleCondition&& rhs);

    /**
     * @brief Copy assignes rhs rule condition to this.
     *
     * @param rhs Rule condition to copy assign to this.
     * @return This object.
     */
    RuleCondition& operator=(const RuleCondition& rhs);

    /**
     * @brief Assignes rhs rule condition to this using move semantics.
     *
     * @param rhs Rule condition to move assign to this.
     * @return This object.
     */
    RuleCondition& operator=(RuleCondition&& rhs);

    /**
     * @brief Returns pointer to the underlying rule condition object.
     *
     * @return Pointer to the underlying rule condition object.
     */
    RuleConditionBase* operator->();

    /**
     * @brief Returns reference to the underlying rule condition object.
     *
     * @return Reference to the underlying rule condition object.
     */
    RuleConditionBase& operator*();

    /**
     * @brief Returns string representation of underlying rule condition.
     *
     * @return String representation of underlying rule condition.
     */
    std::string toRuleString() const;
  private:
    std::unique_ptr<RuleConditionBase> _condition; /**< Specific rule condition
                                                     object. */
  };
} /*namespace usbguard */

/* vim: set ts=2 sw=2 et */
