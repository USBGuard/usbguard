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
#include "Rule.hpp"
#include "RuleCondition.hpp"
#include <chrono>

namespace usbguard {
  class Interface;
  class RulePrivate
  {
  public:
    struct MetaData {
      MetaData()
        : tp_created(std::chrono::steady_clock::now())
      {
        counter_evaluated = 0;
        counter_applied = 0;
      }

      MetaData(const MetaData& rhs)
      {
        counter_evaluated = rhs.counter_evaluated;
        counter_applied = rhs.counter_applied;
        tp_created = rhs.tp_created;
        tp_last_evaluated = rhs.tp_last_evaluated;
        tp_last_applied = rhs.tp_last_applied;
      }

      uint64_t counter_evaluated;
      uint64_t counter_applied;
      std::chrono::steady_clock::time_point tp_created;
      std::chrono::steady_clock::time_point tp_last_evaluated;
      std::chrono::steady_clock::time_point tp_last_applied;
    };

    RulePrivate(Rule& p_instance);
    RulePrivate(Rule& p_instance, const RulePrivate& rhs);
    const RulePrivate& operator=(const RulePrivate& rhs);
    ~RulePrivate();

    bool appliesTo(Pointer<const Rule> rhs) const;
    bool appliesTo(const Rule& rhs) const;
    bool appliesToWithConditions(const Rule& rhs, bool with_update = false);
    
    bool meetsConditions(const Rule& rhs, bool with_update = false);
    void initConditions(Interface * const interface);
    void finiConditions();
    bool updateConditionsState(const Rule& rhs);
    uint64_t conditionsState() const;
    void setConditionsState(uint64_t state);

    void setRuleID(uint32_t rule_id);
    uint32_t getRuleID() const;

    void setTarget(Rule::Target target);
    Rule::Target getTarget() const;

    void setDeviceID(const USBDeviceID& value);
    const USBDeviceID& getDeviceID() const;
    const Rule::Attribute<USBDeviceID>& attributeDeviceID() const;
    Rule::Attribute<USBDeviceID>& attributeDeviceID();

    void setSerial(const String& value);
    const String& getSerial() const;
    const Rule::Attribute<String>& attributeSerial() const;
    Rule::Attribute<String>& attributeSerial();

    void setName(const String& value);
    const String& getName() const;
    const Rule::Attribute<String>& attributeName() const;
    Rule::Attribute<String>& attributeName();

    void setHash(const String& value);
    const String& getHash() const;
    const Rule::Attribute<String>& attributeHash() const;
    Rule::Attribute<String>& attributeHash();

    void setViaPort(const String& value);
    const String& getViaPort() const;
    const Rule::Attribute<String>& attributeViaPort() const;
    Rule::Attribute<String>& attributeViaPort();

    /*
     * Set/get for a single value isn't useful for the
     * with-interface attribute as it usualy contains
     * multiple values. Therefore, we provide only the
     * attribute accessors in this case.
     */
    const Rule::Attribute<USBInterfaceType>& attributeWithInterface() const;
    Rule::Attribute<USBInterfaceType>& attributeWithInterface();

    const Rule::Attribute<RuleCondition*>& attributeConditions() const;
    Rule::Attribute<RuleCondition*>& attributeConditions();

    void setTimeoutSeconds(uint32_t timeout_seconds);
    uint32_t getTimeoutSeconds() const;

    String toString(bool invalid = false) const;

    MetaData& metadata();
    const MetaData& metadata() const;
    void updateMetaDataCounters(bool applied = true, bool evaluated = false);

    /*** Static methods ***/
    static Rule fromString(const String& rule_string);

  private:
    Rule& _p_instance;
    MetaData _meta;
    uint32_t _rule_id;
    Rule::Target _target;
    Rule::Attribute<USBDeviceID> _device_id;
    Rule::Attribute<String> _serial;
    Rule::Attribute<String> _name;
    Rule::Attribute<String> _hash;
    Rule::Attribute<String> _via_port;
    Rule::Attribute<USBInterfaceType> _with_interface;
    Rule::Attribute<RuleCondition*> _conditions;
    uint64_t _conditions_state;
    uint32_t _timeout_seconds;
  };
}
