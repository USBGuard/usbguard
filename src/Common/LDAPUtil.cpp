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
// Authors: Radovan Sroka <rsroka@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "LDAPUtil.hpp"

#include "usbguard/Rule.hpp"
#include "usbguard/RuleSet.hpp"

namespace usbguard
{
  std::vector<std::string> LDAPUtil::_ldap_keys = {
    "USBGuardRuleTarget",
    "USBGuardHost",
    "USBGuardRuleOrder",
    "USBID",
    "USBSerial",
    "USBWithConnectType",
    "USBName",
    "USBHash",
    "USBParentHash",
    "USBViaPort",
    "USBWithInterface",
    "USBGuardRuleCondition"
  };

  std::vector<std::string> LDAPUtil::_rule_keys = {
    "RuleTarget", /* just for indexing */
    "USBGuardHost", /* just for indexing */
    "USBGuardOrder", /* just for indexing */
    "id",
    "serial",
    "with-connect-type",
    "name",
    "hash",
    "parent-hash",
    "via-port",
    "with-interface",
    "if"
  };

  template<class ValueType>
  static void toLDIF_appendNonEmptyAttribute(std::string& rule_string, const Rule::Attribute<ValueType>& attribute)
  {
    if (attribute.empty()) {
      return;
    }

    std::string name = attribute.getName();
    std::string ldif_name = "unknown";

    for (size_t i = 0; i < LDAPUtil::_rule_keys.size(); i++) {
      if (LDAPUtil::_rule_keys[i] == name) {
        ldif_name = LDAPUtil::_ldap_keys[i];
        break;
      }
    }

    rule_string.append("\n");
    rule_string.append(ldif_name + ": ");
    std::string tmp = attribute.toRuleString();
    size_t index = tmp.find_first_of(' ');

    if (index == std::string::npos) {
      rule_string.append(tmp);
    }
    else {
      rule_string.append(tmp.substr(index));
    }

    return;
  }

  std::string LDAPUtil::toLDIF(const std::shared_ptr<const Rule> rule, std::map<std::string, std::string>& values,
    bool invalid)
  {
    std::string rule_string;
    std::string name = values["NAME_PREFIX"] + std::to_string(rule->getRuleID());
    rule_string  = "dn: cn=" + name + "," + values["USBGUARD_BASE"] + "\n";
    rule_string += "objectClass: " + values["OBJCLASS"] + "\n";
    rule_string += "objectClass: top\n";
    rule_string += "cn: " + name + "\n";
    rule_string += LDAPUtil::_ldap_keys[static_cast<unsigned>(LDAPUtil::LDAP_KEY_INDEX::USBGuardRuleTarget)] + ": ";

    try {
      rule_string.append(Rule::targetToString(rule->getTarget()));
    }
    catch (...) {
      if (invalid) {
        rule_string.append("<invalid>");
      }
      else {
        throw;
      }
    }

    rule_string += "\nUSBGuardHost: " + values["HOSTNAME"];
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeDeviceID());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeSerial());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeName());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeHash());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeParentHash());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeViaPort());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeWithInterface());
    toLDIF_appendNonEmptyAttribute(rule_string, rule->attributeConditions());
    rule_string.append("\n");
    return rule_string;
  }

  void LDAPUtil::serializeLDIF(const std::vector<std::shared_ptr<RuleSet>> rulesets, std::ostream& stream,
    std::map<std::string, std::string>& values)
  {
    for (auto ruleset : rulesets) {
      for (auto const& rule : ruleset->getRules()) {
        std::string rule_string = toLDIF(rule, values, false);
        stream << rule_string << std::endl;
      }
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
