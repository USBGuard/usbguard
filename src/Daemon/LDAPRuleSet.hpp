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
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#ifdef HAVE_LDAP

#include "usbguard/Typedefs.hpp"
#include "usbguard/Rule.hpp"
#include "usbguard/RuleSet.hpp"

#include "LDAPHandler.hpp"

#include "Common/Thread.hpp"

#include <ctime>
#include <future>

namespace usbguard
{
  class Interface;
  class DLL_PUBLIC LDAPRuleSet : public RuleSet
  {
  public:
    LDAPRuleSet(Interface* const interface_ptr, std::shared_ptr<LDAPHandler> ldap);
    LDAPRuleSet(const LDAPRuleSet& rhs);
    const LDAPRuleSet& operator=(const LDAPRuleSet& rhs);

    ~LDAPRuleSet();

    void update();
    void loop();

    void load() override;
    void save() override;

    std::shared_ptr<Rule> getFirstMatchingRule(std::shared_ptr<const Rule> device_rule, uint32_t from_id = 1) const override;

  private:
    std::shared_ptr<LDAPHandler> _LDAP;
    std::time_t _last_update;
  };
} /* namespace usbguard */

#endif

/* vim: set ts=2 sw=2 et */
