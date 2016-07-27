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

#include "Typedefs.hpp"
#include "USB.hpp"
#include "Predicates.hpp"
#include "Utility.hpp"
#include "RuleCondition.hpp"
#include <cstdint>
#include <chrono>

namespace usbguard {
  template<typename T>
  String toRuleString(T* const value)
  {
    return value->toRuleString();
  }

  template<typename T>
  String toRuleString(const T& value)
  {
    return value.toRuleString();
  }

  template<>
  String DLL_PUBLIC toRuleString(const String& value);

  class RulePrivate;
  class DLL_PUBLIC Rule
  {
  public:
    /**
     * Rule target enumeration.
     */
    enum class Target {
      Allow, /**< Devices matching this rule will be authorized */
      Block, /**< Devices matching this rule will not be authorized */
      Reject, /**< Devices matching this rule will not be authorized and will be detached */
      Match, /**< Special target which can be used to trigger actions. The rule wont affect the final decision. */
      Unknown, /**< Unknown target. Used for default constructed rules. */
      Device, /**< Special target which can only be used for a rule that represents a single device */
      Invalid
    };

    static const String targetToString(const Target& target);
    static Target targetFromString(const String& target_string);

    enum class SetOperator
    {
      AllOf,
      OneOf,
      NoneOf,
      Equals,
      EqualsOrdered,
      Match /* Special operator: matches anything, cannot be used directly in a rule */
    };

    static const String setOperatorToString(const Rule::SetOperator& op);
    static SetOperator setOperatorFromString(const String& set_operator_string);

    /**< Sequence number of the (fake) root rule */
    static const uint32_t RootID;
    /**< Sequence number assigned to default constructed rules. Cannot be used for searching. */
    static const uint32_t DefaultID;
    /**< Sequence number for specifying that the last rule in the ruleset should be used in context of the operation */
    static const uint32_t LastID;

    template<class ValueType>
    class Attribute
    {
      public:
        Attribute(const char * name)
        {
          _name = name;
          _set_operator = SetOperator::Equals;
        }

        Attribute(const Attribute<ValueType>& rhs)
        {
          _name = rhs._name;
          _set_operator = rhs._set_operator;
          _values = rhs._values;
        }

        void setSetOperator(SetOperator op)
        {
          _set_operator = op;
        }

        SetOperator setOperator() const
        {
          return _set_operator;
        }

        void append(const ValueType& value)
        {
          _values.push_back(value);
        }

        size_t count() const
        {
          return _values.size();
        }

        bool empty() const
        {
          return count() == 0;
        }

        void clear()
        {
          _values.clear();
          _set_operator = SetOperator::Equals;
        }

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

        const ValueType& get(size_t index) const
        {
          return _values.at(index);
        }

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

        void set(const std::vector<ValueType>& values, SetOperator op)
        {
          _values = values;
          _set_operator = op;
        }

        bool appliesTo(const Attribute<ValueType>& target) const
        {
          /* Nothing applies to anything */
          if (empty()) {
            return true;
          }

          switch(setOperator()) {
            case SetOperator::Match:
              return true;
            case SetOperator::AllOf:
              if (!setSolveAllOf(_values, target._values)) {
                return false;
              }
              break;
            case SetOperator::OneOf:
              if (!setSolveOneOf(_values, target._values)) {
                return false;
              }
              break;
            case SetOperator::NoneOf:
              if (!setSolveNoneOf(_values, target._values)) {
                return false;
              }
              break;
            case SetOperator::Equals:
              if (!setSolveEquals(_values, target._values)) {
                return false;
              }
              break;
            case SetOperator::EqualsOrdered:
              if (!setSolveEqualsOrdered(_values, target._values)) {
                return false;
              }
              break;
          }

          return true;
        }

        String toRuleString() const
        {
          String result;

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

          for(const auto& value : _values) {
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

        const std::vector<ValueType>& values() const
        {
          return _values;
        }

        std::vector<ValueType>& values()
        {
          return _values;
        }

      private:
        /*
         * All of the items in source set must match an item in the target set
         */
        bool setSolveAllOf(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
        {
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

        /*
         * At least one of the items in the source set must match an item in the target set
         */
        bool setSolveOneOf(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
        {
          for (auto const& source_item : source_set) {
            for (auto const& target_item : target_set) {
              if (Predicates::isSubsetOf(source_item, target_item)) {
                return true;
              }
            }
          }
          return false;
        }

        /*
         * None of the the items in the rule set must match any item in the
         * applies_to set
         */
        bool setSolveNoneOf(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
        {
          for (auto const& source_item : source_set) {
            for (auto const& target_item : target_set) {
              if (Predicates::isSubsetOf(source_item, target_item)) {
                return false;
              }
            }
          }
          return true;
        }

        /*
         * Every item in the rule set must match one item in the
         * applies_to set and the sets have to have the same number
         * of items
         */
        bool setSolveEquals(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
        {
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

        /*
         * The sets are treated as arrays and they have to me equal
         * (same number of items at the same positions)
         */
        bool setSolveEqualsOrdered(const std::vector<ValueType>& source_set, const std::vector<ValueType>& target_set) const
        {
          if (source_set.size() != target_set.size()) {
            return false;
          }
          for (size_t i = 0; i < source_set.size(); ++i) {
            if (!Predicates::isSubsetOf(source_set[i], target_set[i])) {
              return false;
            }
          }
          return false;
        }

        String _name;
        SetOperator _set_operator;
        std::vector<ValueType> _values;
    };

    /**
     * Construct a default rule.
     * This rule matches only a default rule and cannot be converted to a string
     * representation.
     */
    Rule();
    ~Rule();
    Rule(const Rule& rhs);
    const Rule& operator=(const Rule& rhs);

    void setRuleID(uint32_t rule_id);
    uint32_t getRuleID() const;

    void setTarget(Rule::Target target);
    Target getTarget() const;

    void setDeviceID(const USBDeviceID& value);
    const USBDeviceID& getDeviceID() const;
    const Attribute<USBDeviceID>& attributeDeviceID() const;
    Attribute<USBDeviceID>& attributeDeviceID();

    void setSerial(const String& value);
    const String& getSerial() const;
    const Attribute<String>& attributeSerial() const;
    Attribute<String>& attributeSerial();

    void setName(const String& value);
    const String& getName() const;
    const Attribute<String>& attributeName() const;
    Attribute<String>& attributeName();

    void setHash(const String& value);
    const String& getHash() const;
    const Attribute<String>& attributeHash() const;
    Attribute<String>& attributeHash();

    void setParentHash(const String& value);
    const String& getParentHash() const;
    const Rule::Attribute<String>& attributeParentHash() const;
    Rule::Attribute<String>& attributeParentHash();

    void setViaPort(const String& value);
    const String& getViaPort() const;
    const Attribute<String>& attributeViaPort() const;
    Attribute<String>& attributeViaPort();

    /*
     * Set/get for a single value isn't useful for the
     * with-interface attribute as it usualy contains
     * multiple values. Therefore, we provide only the
     * attribute accessors in this case.
     */
    const Attribute<USBInterfaceType>& attributeWithInterface() const;
    Attribute<USBInterfaceType>& attributeWithInterface();

    const Attribute<RuleCondition*>& attributeConditions() const;
    Attribute<RuleCondition*>& attributeConditions();

    void setTimeoutSeconds(uint32_t timeout_seconds);
    uint32_t getTimeoutSeconds() const;

    bool appliesTo(Pointer<const Rule> rhs) const;
    bool appliesTo(const Rule& rhs) const;
    bool appliesTo(const Rule& rhs);
    bool isImplicit() const;


    operator bool() const;
    String toString(bool invalid = false) const;

    void updateMetaDataCounters(bool applied = true, bool evaluated = false);

    RulePrivate* internal();
    const RulePrivate* internal() const;
    
    /*** Static methods ***/
    static Rule fromString(const String& rule_string);

  private:
    RulePrivate* d_pointer;
  };
} /* namespace usbguard */
