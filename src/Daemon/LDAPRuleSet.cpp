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

#ifdef HAVE_LDAP

#include <algorithm>

#include "LDAPRuleSet.hpp"
#include "LDAPHandler.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/RuleParser.hpp"
#include "usbguard/Exception.hpp"

#include "Common/LDAPUtil.hpp"

#include <future>
#include <unistd.h>


namespace usbguard
{
  LDAPRuleSet::LDAPRuleSet(Interface* const interface_ptr, std::shared_ptr<LDAPHandler> ldap)
    : RuleSet(interface_ptr),
      _LDAP(ldap),
      _last_update(0)

  {
    clearWritable();
    USBGUARD_LOG(Info) << "Creating LDAPRuleSet";
  }

  LDAPRuleSet::LDAPRuleSet(const LDAPRuleSet& rhs)
    : RuleSet(rhs._interface_ptr),
      _LDAP(rhs._LDAP),
      _last_update(rhs._last_update)
  {
    *this = rhs;
  }

  const LDAPRuleSet& LDAPRuleSet::operator=(const LDAPRuleSet& rhs)
  {
    RuleSet::operator = (rhs);
    return *this;
  }

  LDAPRuleSet::~LDAPRuleSet()
  {
  }

  void LDAPRuleSet::load()
  {
    std::shared_ptr<LDAPMessage> message = _LDAP->query(_LDAP->getRuleQuery());
    std::vector<std::pair<long, std::string>> v = _LDAP->ldapToRules(message);
    std::sort(v.begin(), v.end(), [](std::pair<long, std::string> a, std::pair<long, std::string> b) {
      return a.first < b.first;
    });
    size_t rule_number = 1;

    for (auto _rule: v) {
      USBGUARD_LOG(Info) << "Parsing rule: " << rule_number << "  RuleOrder: "<< _rule.first;
      USBGUARD_LOG(Info) << _rule.second;
      auto rule = parseRuleFromString(_rule.second, "", rule_number);
      appendRule(rule, Rule::LastID, /*lock=*/false);
      rule_number++;
      USBGUARD_LOG(Info);
    }

    _last_update = std::time(nullptr);
  }
  void LDAPRuleSet::save() {}

  void LDAPRuleSet::update()
  {
    USBGUARD_LOG(Trace);

    if (std::time(nullptr) - _last_update < _LDAP->getUpdateInterval()) {
      USBGUARD_LOG(Trace) << "UPDATE is not needed!";
      return;
    }

    /*TODO:
    *
    * we should handle that LDAP server can be unreachable
    *
    */
    std::unique_lock<std::mutex> op_lock(_op_mutex);
    USBGUARD_LOG(Trace) << "Processing UPDATE!";
    _rules.clear();
    _id_next = Rule::RootID + 1;
    load();
  }

  std::shared_ptr<Rule> LDAPRuleSet::getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id) const
  {
    USBGUARD_LOG(Trace);
    std::future<void> ft = std::async(std::launch::deferred, std::bind(&LDAPRuleSet::update, *this));
    ft.get();
    return RuleSet::getFirstMatchingRule(device_rule, from_id);
  }
} /* namespace usbguard */

#endif

/* vim: set ts=2 sw=2 et */
