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

#include <vector>
#include <map>

#include "usbguard/Rule.hpp"
#include "usbguard/RuleSet.hpp"

namespace usbguard
{
  class DLL_PUBLIC LDAPUtil
  {
  public:
    enum class LDAP_KEY_INDEX {
      USBGuardRuleTarget = 0,
      USBGuardHost,
      USBGuardRuleOrder,
      USBID,
      USBSerial,
      USBWithConnectType,
      USBName,
      USBHash,
      USBParentHash,
      USBViaPort,
      USBWithInterface,
      USBGuardRuleCondition
    };
    static std::vector<std::string> _ldap_keys;
    static std::vector<std::string> _rule_keys;

    static std::string toLDIF(const std::shared_ptr<const Rule> rule, std::map<std::string, std::string>& values, bool invalid);
    static void serializeLDIF(const std::vector<std::shared_ptr<RuleSet>> rulesets, std::ostream& stream,
      std::map<std::string, std::string>& values);
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
