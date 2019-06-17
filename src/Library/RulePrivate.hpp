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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/RuleCondition.hpp"

#include "usbguard/Rule.hpp"

#include <chrono>

namespace usbguard
{
  class Interface;
  class RulePrivate
  {
  public:
    struct MetaData {
      MetaData()
        : tp_created(std::chrono::steady_clock::now())
      {
      }

      MetaData(const MetaData& rhs) = default;
      MetaData& operator=(const MetaData& rhs) = default;

      uint64_t counter_evaluated{0};
      uint64_t counter_applied{0};
      std::chrono::steady_clock::time_point tp_created;
      std::chrono::steady_clock::time_point tp_last_evaluated;
      std::chrono::steady_clock::time_point tp_last_applied;
    };

    RulePrivate();
    RulePrivate(const RulePrivate& rhs);
    RulePrivate& operator=(const RulePrivate& rhs) = default;
    ~RulePrivate() = default;

    bool appliesTo(std::shared_ptr<const Rule> rhs, bool parent_insensitive = false) const;
    bool appliesTo(const Rule& rhs, bool parent_insensitive = false) const;
    bool appliesToWithConditions(const Rule& rhs, bool with_update = false);

    bool meetsConditions(const Rule& rhs, bool with_update = false);
    void initConditions(Interface* const interface);
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

    void setSerial(const std::string& value);
    const std::string& getSerial() const;
    const Rule::Attribute<std::string>& attributeSerial() const;
    Rule::Attribute<std::string>& attributeSerial();

    void setLabel(const std::string& value);
    const std::string& getLabel() const;
    const Rule::Attribute<std::string>& attributeLabel() const;
    Rule::Attribute<std::string>& attributeLabel();

    void setWithConnectType(const std::string& value);
    const std::string& getWithConnectType() const;
    const Rule::Attribute<std::string>& attributeWithConnectType() const;
    Rule::Attribute<std::string>& attributeWithConnectType();

    void setName(const std::string& value);
    const std::string& getName() const;
    const Rule::Attribute<std::string>& attributeName() const;
    Rule::Attribute<std::string>& attributeName();

    void setHash(const std::string& value);
    const std::string& getHash() const;
    const Rule::Attribute<std::string>& attributeHash() const;
    Rule::Attribute<std::string>& attributeHash();

    void setParentHash(const std::string& value);
    const std::string& getParentHash() const;
    const Rule::Attribute<std::string>& attributeParentHash() const;
    Rule::Attribute<std::string>& attributeParentHash();

    void setViaPort(const std::string& value);
    const std::string& getViaPort() const;
    const Rule::Attribute<std::string>& attributeViaPort() const;
    Rule::Attribute<std::string>& attributeViaPort();

    /*
     * Set/get for a single value isn't useful for the
     * with-interface attribute as it usualy contains
     * multiple values. Therefore, we provide only the
     * attribute accessors in this case.
     */
    const Rule::Attribute<USBInterfaceType>& attributeWithInterface() const;
    Rule::Attribute<USBInterfaceType>& attributeWithInterface();

    const Rule::Attribute<RuleCondition>& attributeConditions() const;
    Rule::Attribute<RuleCondition>& attributeConditions();

    std::string toString(bool invalid = false, bool hide_pii = false) const;

    MetaData& metadata();
    const MetaData& metadata() const;
    void updateMetaDataCounters(bool applied = true, bool evaluated = false);

    /*** Static methods ***/
    static Rule fromString(const std::string& rule_string);

  private:
    MetaData _meta;
    uint32_t _rule_id;
    Rule::Target _target;
    Rule::Attribute<USBDeviceID> _device_id;
    Rule::Attribute<std::string> _serial;
    Rule::Attribute<std::string> _with_connect_type;
    Rule::Attribute<std::string> _name;
    Rule::Attribute<std::string> _hash;
    Rule::Attribute<std::string> _parent_hash;
    Rule::Attribute<std::string> _via_port;
    Rule::Attribute<USBInterfaceType> _with_interface;
    Rule::Attribute<RuleCondition> _conditions;
    Rule::Attribute<std::string> _label;
    uint64_t _conditions_state;
  };
}

/* vim: set ts=2 sw=2 et */
