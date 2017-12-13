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

#include "LDAPRuleSet.hpp"
#include "LDAPHandler.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/RuleParser.hpp"
#include "usbguard/Exception.hpp"

#include "Common/LDAPUtil.hpp"


namespace usbguard
{
  LDAPRuleSet::LDAPRuleSet(Interface* const interface_ptr, std::shared_ptr<LDAPHandler> ldap)
    : RuleSetAbstract(interface_ptr),
      LDAP(ldap)
  {
    clearWritable();
    USBGUARD_LOG(Info) << "Creating LDAPRuleSet";
  }

  LDAPRuleSet::LDAPRuleSet(const LDAPRuleSet& rhs)
    : RuleSetAbstract(rhs._interface_ptr),
      LDAP(rhs.LDAP)
  {
    *this = rhs;
  }

  const LDAPRuleSet& LDAPRuleSet::operator=(const LDAPRuleSet& rhs)
  {
    RuleSet::operator = (rhs);
    return *this;
  }

  void LDAPRuleSet::load()
  {
    std::shared_ptr<LDAPMessage> message = LDAP->query(LDAP->getRuleQuery());
    std::vector<std::string> v = LDAP->ldapToRules(message);
    size_t rule_number = 1;

    for (std::string _rule: v) {
      USBGUARD_LOG(Info) << "Parsing rule: " << rule_number;
      USBGUARD_LOG(Info) << _rule;
      auto rule = parseRuleFromString(_rule, "", rule_number);
      appendRule(rule);
      rule_number++;
      USBGUARD_LOG(Info);
    }
  }
  void LDAPRuleSet::save() {}
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
