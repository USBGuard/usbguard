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
//          Radovan Sroka <rsroka@redhat.com>
//          Marek Tamaskovic <mtamasko@redhat.com>
//
#pragma once

#include "RuleCondition.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
#include "Predicates.hpp"
#include "Typedefs.hpp"
#include "USB.hpp"

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <memory>

namespace usbguard
{

  /**
   * @brief Converts given value to rule string.
   *
   * @param value Value to convert.
   * @return Rule string.
   */
  template<typename T>
  std::string toRuleString(T* const value)
  {
    return value->toRuleString();
  }

  /**
   * @brief Converts given value to rule string.
   *
   * @param value Value to convert.
   * @return Rule string.
   */
  template<typename T>
  std::string toRuleString(const T& value)
  {
    return value.toRuleString();
  }

  /**
   * @brief Converts given string to rule string.
   *
   * @param value String to convert.
   * @return Rule string.
   */
  template<>
  std::string DLL_PUBLIC toRuleString(const std::string& value);

  class RulePrivate;

  /**
   * @brief Determines whether USB device mathing specified criteria should be
   * authorized, deauthorized or removed.
   */
  class DLL_PUBLIC Rule
  {
  public:
    /**
     * @brief Enumeration of possible rule targets.
     *
     * The target of a rule specifies whether the device will be authorized for
     * use or not.
     */
    enum class Target {
      Allow = 0, /**< Devices matching this rule will be authorized. In other
                   words, the device and its interface will be allowed to
                   communicate with the system. */
      Block = 1, /**< Devices matching this rule will not be authorized. */
      Reject = 2, /**< Devices matching this rule will not be authorized and
                    will be detached. */
      Match = 3, /**< Special target which can be used to trigger actions.
                   The rule wont affect the final decision. */
      Unknown = 4, /**< Unknown target. Used for default constructed rules. */
      Device = 5, /**< Special target which can only be used for a rule that
                    represents a single device. */
      Empty = 6, /**< Special target to represent the case the parser reaches
                   a comment only line. */
      Invalid = 7 /**< Represents invalid target. */
    };

    /**
     * @brief Converts given rule target to its string representation.
     *
     * @param target Rule target to convert.
     * @return String representation of given target.
     * @throw runtime_error If no string representation for given rule target
     * has been found.
     */
    static const std::string targetToString(Target target);

    /**
     * @brief Converts given string into rule target.
     *
     * @param target_string String to convert.
     * @return Rule target.
     * @throw runtime_error If string does not contain valid rule target.
     */
    static Target targetFromString(const std::string& target_string);

    /**
     * @brief Converts rule target to its integer representation.
     *
     * @param target Rule target to convert.
     * @return Integer representation of given rule target.
     */
    static uint32_t targetToInteger(Target target);

    /**
     * @brief Converts integer into rule target.
     *
     * @param target_integer Integer to convert.
     * @return Rule target.
     * @throw runtime_error If given integer can not be converted
     * into rule target.
     */
    static Target targetFromInteger(uint32_t target_integer);

    /**
     * @brief Defines set operators that can be used in the rules.
     */
    enum class SetOperator {
      AllOf, /**< The device attribute set must contain all of the specified
               values for the rule to match. */
      OneOf, /**< The device attribute set must contain at least one of the
               specified values for the rule to match. */
      NoneOf, /**< The device attribute set must not contain any of the
                specified values for the rule to match. */
      Equals, /**< The device attribute set must contain exactly the same set
                of values for the rule to match. */
      EqualsOrdered, /**< The device attribute set must contain exactly
                       the same set of values in the same order for the rule
                       to match. */
      Match, /**< Special operator: matches anything, cannot be used directly
              in a rule. */
      MatchAll /**< The device attribute set must be a subset of the specified
                 values for the rule to match. */
    };

    /**
     * @brief Converts given set operator to its string representation.
     *
     * @param op Set operator to convert.
     * @return String representation of given set operator.
     * @throw runtime_error If string representation for given set operator
     * does not exist.
     */
    static const std::string setOperatorToString(const Rule::SetOperator& op);

    /**
     * @brief Converts string into set operator.
     *
     * @param set_operator_string String to convert.
     * @return Set operator.
     * @throw runtime_error If given string does not contain valid set
     * operator.
     */
    static SetOperator setOperatorFromString(const std::string& set_operator_string);

    /**
     * @brief Sequence number of the (fake) root rule.
     */
    static const uint32_t RootID;

    /**
     * @brief Sequence number assigned to default constructed rules.
     * Cannot be used for searching.
     */
    static const uint32_t DefaultID;

    /**
     * @brief Sequence number for specifying that the last rule in the ruleset
     * should be used in context of the operation.
     */
    static const uint32_t LastID;

    /**
     * @brief Sequence number of the implicit target rule
     */
    static const uint32_t ImplicitID;

    /**
     * @brief Specifies which devices to match or what condition
     * have to be met for the rule to be aplicable.
     *
     * Attribute can be either single valued or multi-valued in which case set
     * operator may be used to allow finer definition of the rule.
     */
    template<class ValueType>
    class Attribute
    {
    public:
      /**
       * @brief Constructs new rule attribute with a given name and default
       * set operator.
       *
       * @note Default set operator is Equals.
       *
       * @param name Name of the attribute.
       */
      Attribute(const char* name)
      {
        _name = name;
        _set_operator = SetOperator::Equals;
      }

      /**
       * @brief Default copy constructor.
       *
       * @param rhs Attribute to copy.
       */
      Attribute(const Attribute<ValueType>& rhs) = default;

      /**
       * @brief Default copy assignment.
       *
       * @param rhs Attribute to copy assign.
       */
      Attribute& operator=(const Attribute<ValueType>& rhs) = default;

      /**
       * @brief Sets set operator.
       *
       * @param op Set operator to set.
       */
      void setSetOperator(SetOperator op)
      {
        _set_operator = op;
      }

      /**
       * @brief Returns set operator of this attribute.
       *
       * @return Set operator of this attribute.
       */
      SetOperator setOperator() const
      {
        return _set_operator;
      }

      /**
       * @brief Appends new value into attribute values using move semantics.
       *
       * @param value Value to append into attribute values.
       */
      void append(ValueType&& value)
      {
        _values.emplace_back(std::move(value));
      }

      /**
       * @brief Appends new value into attribute values.
       *
       * @param value Value to append into attribute values.
       */
      void append(const ValueType& value)
      {
        _values.push_back(value);
      }

      /**
       * @brief Returns number of values of this attribute.
       *
       * @return Number of values of this attribute.
       */
      size_t count() const
      {
        return _values.size();
      }

      /**
       * @brief Checks whether attribute does not contain any values.
       *
       * @return True if this attribute does not contain any value,
       * false otherwise.
       */
      bool empty() const
      {
        return count() == 0;
      }

      /**
       * @brief Clears all values of this attribute and sets set operator
       * to default.
       *
       * @note Default set operator is Equals.
       */
      void clear()
      {
        _values.clear();
        _set_operator = SetOperator::Equals;
      }

      /**
       * @brief Returns name of the attribute.
       *
       * @return Name of the attribute.
       */
      std::string getName() const
      {
        return _name;
      }

      /**
       * @brief Returns value of single-value attribute.
       *
       * @return Value of single-value attribute.
       * @throw runtime_error If attribute is not single-value or is empty.
       */
      const ValueType& get() const
      {
        if (count() == 1) {
          return _values[0];
        }
        else if (count() == 0) {
          throw std::runtime_error("BUG: Accessing an empty attribute");
        }
        else {
          throw std::runtime_error("BUG: Accessing a multivalued attribute using get()");
        }
      }

      /**
       * @brief Returns value at given index in values vector.
       *
       * @param index Index in values vector.
       * @return Value at given index.
       * @throw std::out_of_range If index >= number of values.
       */
      const ValueType& get(size_t index) const
      {
        return _values.at(index);
      }

      /**
       * @brief Sets value in single-value attribute using move semantics.
       *
       * @param Value to set.
       * @throw runtime_error If attribute is multi-value.
       */
      void set(ValueType&& value)
      {
        if (count() > 1) {
          throw std::runtime_error("BUG: Setting single value for a multivalued attribute");
        }

        if (count() == 0) {
          append(value);
        }
        else {
          _values[0] = std::move(value);
        }
      }

      /**
       * @brief Sets value in single-value attribute.
       *
       * @param Value to set.
       * @throw runtime_error If attribute is multi-value.
       */
      void set(const ValueType& value)
      {
        if (count() > 1) {
          throw std::runtime_error("BUG: Setting single value for a multivalued attribute");
        }

        if (count() == 0) {
          append(value);
        }
        else {
          _values[0] = value;
        }
      }

      /**
       * @brief Sets given values and set operator.
       *
       * @param values Values to set.
       * @param op Set operator to set.
       */
      void set(const std::vector<ValueType>& values, SetOperator op)
      {
        _values = values;
        _set_operator = op;
      }

      /**
       * @brief Checks whether attribute applies to target attribute.
       *
       * In other words, checks whether values of this attribute satisfy
       * set operator against target attribute values.\n
       * For example if \p this\.values = { \p v1 } and \p target\.values =
       * { \p v1, \p v2 }, then { \p v1 } \p OneOf { \p v1, \p v2 } = \p True.
       *
       * @param target Target attribute to apply to.
       * @throw USBGUARD_BUG If attributes set operator is invalid.
       */
      bool appliesTo(const Attribute<ValueType>& target) const
      {
        USBGUARD_LOG(Trace) << "entry:"
          << " source=" << this->toRuleString()
          << " target=" << target.toRuleString();
        bool applies = false;

        /* Nothing applies to anything */
        if (empty()) {
          USBGUARD_LOG(Debug) << "empty source value, setting applies=true";
          applies = true;
        }
        else {
          USBGUARD_LOG(Debug) << "set_operator=" << setOperatorToString(setOperator());

          switch (setOperator()) {
          case SetOperator::Match:
            applies = true;
            break;

          case SetOperator::AllOf:
            applies = setSolveAllOf(_values, target._values);
            break;

          case SetOperator::OneOf:
            applies = setSolveOneOf(_values, target._values);
            break;

          case SetOperator::NoneOf:
            applies = setSolveNoneOf(_values, target._values);
            break;

          case SetOperator::Equals:
            applies = setSolveEquals(_values, target._values);
            break;

          case SetOperator::EqualsOrdered:
            applies = setSolveEqualsOrdered(_values, target._values);
            break;

          case SetOperator::MatchAll:
            applies = setSolveMatchAll(_values, target._values);
            break;

          default:
            throw USBGUARD_BUG("Invalid set operator value");
          }
        }

        USBGUARD_LOG(Trace) << "return:"
          << " applies=" << applies;
        return applies;
      }

      /**
       * @brief Returns string representation of this attribute.
       *
       * Format:
       *  - single-valued:
       *    \<name\> \<value\>
       *  - multi-valued:
       *    \<name\> \<non-default set operator\> { \<value[0]\> \<value[1]\> ... }
       * Default set operator is not included in the string.
       *
       * @return String representation of this attribute.
       */
      std::string toRuleString() const
      {
        std::string result;
        result.append(_name);
        result.append(" ");
        const bool nondefault_op = setOperator() != SetOperator::Equals;
        const bool multiset_form = count() > 1 || nondefault_op;

        if (multiset_form) {
          if (nondefault_op) {
            result.append(setOperatorToString(setOperator()));
            result.append(" ");
          }

          result.append("{ ");
        }

        for (const auto& value : _values) {
          result.append(usbguard::toRuleString(value));
          result.append(" ");
        }

        if (multiset_form) {
          result.append("}");
        }
        else {
          /*
           * Remove the trailing space in case of a single
           * valued attribute.
           */
          result.erase(result.end() - 1);
        }

        return result;
      }

      /**
       * @brief Returns imutable vector of attribute values.
       *
       * @return Imutable vector of attribute values.
       */
      const std::vector<ValueType>& values() const
      {
        return _values;
      }

      /**
       * @brief Returns mutable vector of attribute values.
       *
       * @return Mutable vector of attribute values.
       */
      std::vector<ValueType>& values()
      {
        return _values;
      }

    private:
      /**
       * @brief All of the items in source set must match an item
       * in the target set.
       *
       * @param source_set Source set.
       * @param target_set Target set.
       * @return True if every item in the \p source_set matches an item in the
       * \p target_set, false otherwise.
       */
      bool setSolveAllOf(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
      {
        USBGUARD_LOG(Trace);

        for (auto const& source_item : source_set) {
          bool match = false;

          for (auto const& target_item : target_set) {
            if (Predicates::isSubsetOf(source_item, target_item)) {
              match = true;
              break;
            }
          }

          if (!match) {
            return false;
          }
        }

        return true;
      }

      /**
       * @brief At least one of the items in the source set must match an item
       * in the target set.
       *
       * @param source_set Source set.
       * @param target_set Target set.
       * @return True if at least one of the items from the \p source_set
       * matches an item in the \p target_set, false otherwise.
       */
      bool setSolveOneOf(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
      {
        USBGUARD_LOG(Trace);

        for (auto const& source_item : source_set) {
          for (auto const& target_item : target_set) {
            if (Predicates::isSubsetOf(source_item, target_item)) {
              return true;
            }
          }
        }

        return false;
      }

      /**
       * @brief None of the items in the source set must match any item in the
       * target set.
       *
       * @param source_set Source set.
       * @param target_set Target set.
       * @return True if no item in the \p source_set matches any item in the
       * \p target_set.
       */
      bool setSolveNoneOf(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
      {
        USBGUARD_LOG(Trace);

        for (auto const& source_item : source_set) {
          for (auto const& target_item : target_set) {
            if (Predicates::isSubsetOf(source_item, target_item)) {
              return false;
            }
          }
        }

        return true;
      }

      /**
       * @brief Every item in the source set must match one item in the
       * target set and the sets have to have the same number of items.
       *
       * @param source_set Source set.
       * @param target_set Target set.
       * @return True if every item in the \p source_set matches one item in
       * the \p target_set and sets have the same number of items,
       * false otherwise.
       */
      bool setSolveEquals(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
      {
        USBGUARD_LOG(Trace);

        if (source_set.size() != target_set.size()) {
          return false;
        }
        else {
          for (auto const& source_item : source_set) {
            bool match = false;

            for (auto const& target_item : target_set) {
              if (Predicates::isSubsetOf(source_item, target_item)) {
                match = true;
                break;
              }
            }

            if (!match) {
              return false;
            }
          }

          return true;
        }
      }

      /**
       * @brief The sets are treated as arrays and they have to be equal
       * (same number of items at the same positions).
       *
       * @param source_set Source set.
       * @param target_set Target set.
       * @return True if sets are equal and ordered, false otherwise.
       */
      bool setSolveEqualsOrdered(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
      {
        USBGUARD_LOG(Trace);

        if (source_set.size() != target_set.size()) {
          return false;
        }

        for (size_t i = 0; i < source_set.size(); ++i) {
          if (!Predicates::isSubsetOf(source_set[i], target_set[i])) {
            return false;
          }
        }

        return true;
      }

      /**
       * @brief All of the items in \p target_set must match an item in
       * the \p source_set.
       *
       * @param source_set Source set.
       * @param Target_set Target set.
       * @return True if all items in \p target_set match an item in
       * the \p source_set.
       */
      bool setSolveMatchAll(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
      {
        USBGUARD_LOG(Trace);
        size_t match = 0;

        for (auto const& target_item : target_set) {
          for (auto const& source_item : source_set) {
            if (Predicates::isSupersetOf(source_item, target_item)) {
              match++;
              break;
            }
          }
        }

        return match == target_set.size();
      }

      std::string _name; /**< Name of the rule attribute. */
      SetOperator _set_operator; /**< Set operator can be used for
                                   a multi-valued attribute for finer
                                   rule definition. */
      std::vector<ValueType> _values; /**< Values of the attribute. */
    };

    /**
     * @brief Construct a default rule.
     *
     * This rule matches only a default rule and cannot be converted to
     * a string representation.
     */
    Rule();

    /**
     * @brief Default destructor.
     */
    ~Rule();

    /**
     * @brief Constructs rule based on rhs.
     *
     * @param rhs Rule to copy.
     */
    Rule(const Rule& rhs);

    /**
     * @brief Assignes rhs rule to this.
     *
     * @param rhs Rule to assign.
     * @return This object.
     */
    const Rule& operator=(const Rule& rhs);

    /**
     * @brief Sets rule ID.
     *
     * @param rule_id Rule ID to set.
     */
    void setRuleID(uint32_t rule_id);

    /**
     * @brief Returns rule ID.
     *
     * @return Rule ID.
     */
    uint32_t getRuleID() const;

    /**
     * @brief Sets rule target.
     *
     * In other words, sets whether USB device will be authorized
     * for use or not.
     *
     * @param target Rule target to set.
     */
    void setTarget(Rule::Target target);

    /**
     * @brief Returns rule target.
     *
     * @return Rule target.
     */
    Target getTarget() const;

    /**
     * @brief Sets USB Device ID attribute.
     *
     * @param value USB device ID to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setDeviceID(const USBDeviceID& value);

    /**
     * @brief Returns USB device ID.
     *
     * @return USB device ID.
     * @see \link Attribute::get() get()\endlink
     */
    const USBDeviceID& getDeviceID() const;

    /**
     * @brief Returns imutable USB device ID attribute.
     *
     * @return Imutable USB device ID attribute.
     */
    const Attribute<USBDeviceID>& attributeDeviceID() const;

    /**
     * @brief Returns mutable USB device ID attribute.
     *
     * @return Mutable USB device ID attribute.
     */
    Attribute<USBDeviceID>& attributeDeviceID();

    /**
     * @brief Sets serial number attribute.
     *
     * @param value Serial number to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setSerial(const std::string& value);

    /**
     * @brief Returns serial number.
     *
     * @return Serial number.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getSerial() const;

    /**
     * @brief Returns imutable serial number attribute.
     *
     * @return Imutable serial number attribute.
     */
    const Attribute<std::string>& attributeSerial() const;

    /**
     * @brief Returns mutable serial number attribute.
     *
     * @return Mutable serial number attribute.
     */
    Attribute<std::string>& attributeSerial();

    /**
     * @brief Sets rule label attribute.
     *
     * Label assosiates an arbitrary string with a rule.
     * This string can, for instance, be used to store the origin
     * of the rule or some contextual information about it.
     * It is not used when testing if a rule applies to a device.
     *
     * @param value Label to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setLabel(const std::string& value);

    /**
     * @brief Returns rule label.
     *
     * @return Rule label.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getLabel() const;

    /**
     * @brief Returns imutable rule label attribute.
     *
     * @return Imutable rule label attribute.
     */
    const Attribute<std::string>& attributeLabel() const;

    /**
     * @brief Returns mutable rule label attribute.
     *
     * @return Mutable rule label attribute.
     */
    Attribute<std::string>& attributeLabel();

    /**
     * @brief Sets with-connect-type attribute.
     *
     * Describes what type of connection USB device uses.
     * Helps to distinguish between internal (hardwired)
     * and external (hotplug) ports.
     *
     * @param value With-connect-type to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setWithConnectType(const std::string& value);

    /**
     * @brief Returns type of conncetion USB device uses.
     *
     * @return Type of connection USB device uses.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getWithConnectType() const;

    /**
     * @brief Returns imutable with-connect-type attribute.
     *
     * @return Imutable with-connect-type attribute.
     */
    const Attribute<std::string>& attributeWithConnectType() const;

    /**
     * @brief Returns mutable with-connect-type attribute.
     *
     * @return Mutable with-connect-type attribute.
     */
    Attribute<std::string>& attributeWithConnectType();

    /**
     * @brief Sets device name attribute.
     *
     * @param value Device name to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setName(const std::string& value);

    /**
     * @brief Returns device name.
     *
     * @return Device name.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getName() const;

    /**
     * @brief Returns imutable device name attribute.
     *
     * @return Imutable device name attribute.
     */
    const Attribute<std::string>& attributeName() const;

    /**
     * @brief Returns mutable device name attribute.
     *
     * @return Mutable device name attribute.
     */
    Attribute<std::string>& attributeName();

    /**
     * @brief Sets device hash attribute.
     *
     * @param value Device hash to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setHash(const std::string& value);

    /**
     * @brief Returns device hash.
     *
     * @return Device hash.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getHash() const;

    /**
     * @brief Returns imutable device hash attribute.
     *
     * @return Imutable device hash attribute.
     */
    const Attribute<std::string>& attributeHash() const;

    /**
     * @brief Returns mutable device hash attribute.
     *
     * @return Mutable device hash attribute.
     */
    Attribute<std::string>& attributeHash();

    /**
     * @brief Sets parent hash attribute.
     *
     * @param value Parent hash to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setParentHash(const std::string& value);

    /**
     * @brief Returns parent hash.
     *
     * @return Parent hash.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getParentHash() const;

    /**
     * @brief Returns imutable parent hash attribute.
     *
     * @return Imutable parent hash attribute.
     */
    const Rule::Attribute<std::string>& attributeParentHash() const;

    /**
     * @brief Returns mutable parent hash attribute.
     *
     * @return Mutable parent hash attribute.
     */
    Rule::Attribute<std::string>& attributeParentHash();

    /**
     * @brief Sets via-port attribute.
     *
     * Describes through which USB port the device is connected.
     *
     * @param value Port to set.
     * @see \link Attribute::set() set()\endlink
     */
    void setViaPort(const std::string& value);

    /**
     * @brief Returns USB port through which the device is connected.
     *
     * @return USB port through which the device is connected.
     * @see \link Attribute::get() get()\endlink
     */
    const std::string& getViaPort() const;

    /**
     * @brief Returns imutable via-port attribute.
     *
     * @return Imutable via-port attribute.
     */
    const Attribute<std::string>& attributeViaPort() const;

    /**
     * @brief Returns mutable via-port attribute.
     *
     * @return Mutable via-port attribute.
     */
    Attribute<std::string>& attributeViaPort();

    /**
     * @brief Returns imutable with-interface attribute.
     *
     * @note Set/get methods for a single value isn't useful for the
     * with-interface attribute as it usually contains
     * multiple values. Therefore, we provide only the
     * attribute accessors in this case.
     *
     * @return Imutable with-interface attribute.
     */
    const Attribute<USBInterfaceType>& attributeWithInterface() const;

    /**
     * @brief Returns mutable with-interface attribute.
     *
     * @note Set/get methods for a single value isn't useful for the
     * with-interface attribute as it usually contains
     * multiple values. Therefore, we provide only the
     * attribute accessors in this case.
     *
     * @return Mutable with-interface attribute.
     */
    Attribute<USBInterfaceType>& attributeWithInterface();

    /**
     * @brief Returns imutable rule conditions attribute.
     *
     * @return Imutable rule conditions attribute.
     */
    const Attribute<RuleCondition>& attributeConditions() const;

    /**
     * @brief Returns mutable rule conditions attribute.
     *
     * @return Mutable rule conditions attribute.
     */
    Attribute<RuleCondition>& attributeConditions();

    /**
     * @brief Checks whether this rule applies to rhs rule.
     *
     * @param rhs Rule to apply to.
     * @return True if this rule applies to rhs rule, false otherwise.
     */
    bool appliesTo(std::shared_ptr<const Rule> rhs) const;

    /**
     * @brief Checks whether this rule applies to rhs rule.
     *
     * @param rhs Rule to apply to.
     * @return True if this rule applies to rhs rule, false otherwise.
     */
    bool appliesTo(const Rule& rhs) const;

    /**
     * @brief Checks whether this rule applies to rhs rule
     * and updates last evaluated meta-data counter.
     *
     * @param rhs Rule to apply to.
     * @return True if this rule applies to rhs rule, false otherwise.
     */
    bool appliesTo(const Rule& rhs);

    /**
     * @brief Checks whether the rule is implicit (rule has default rule ID).
     *
     * @return True if rule has default rule ID, false otherwise.
     */
    bool isImplicit() const;

    /**
     * @brief Returns true if rule target is neither unknown nor
     * invalid nor empty.
     *
     * @return True if rule target is neither unknown nor invalid nor empty,
     * false otherwise.
     */
    operator bool() const;

    /**
     * @brief Returns string representation of this rule.
     *
     * @param invalid If rule target is invalid and this option is set to true,
     * then no exception will be thrown but string \"\<invalid\>\" will be put
     * instead of rule target into the string.
     * @param hide_serial If true, serial number and any PII (Personally
     * identifiable information) will not be included in the string.
     * @return String representation of this rule.
     */
    std::string toString(bool invalid = false, bool hide_serial = false) const;

    /**
     * @brief Updates meta-data last applied and last evaluated counters.
     *
     * RulePrivate object contains three meta-data counters:
     *  - when was Rule object created.
     *  - when was rule last applied.
     *  - when was rule last evaluated.
     *
     * @param applied If true, last applied counter is updated to current time.
     * @param evaluated If true, last evaluated counter is updated to current
     * time.
     */
    void updateMetaDataCounters(bool applied = true, bool evaluated = false);

    /**
     * @brief Returns reference to mutable RulePrivate object of this rule.
     *
     * @return Mutable RulePrivate.
     */
    std::unique_ptr<RulePrivate>& internal();

    /**
     * @brief Returns reference to imutable RulePrivate object of this rule.
     *
     * @return Imutable RulePrivate.
     */
    const std::unique_ptr<RulePrivate>& internal() const;

    /*************************************************************************
     ***************************** STATIC METHODS ****************************
     *************************************************************************/

    /**
     * @brief Parses given rule_string and returns Rule object.
     *
     * @param rule_string String to parse.
     * @return Rule object from given rule_string.
     * @see \link RuleParser::parseRuleFromString()
     * parseRuleFromString()\endlink
     */
    static Rule fromString(const std::string& rule_string);

  private:
    std::unique_ptr<RulePrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
