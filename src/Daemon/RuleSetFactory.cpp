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
// Authors: Radovan Sroka <rsroka@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/RuleSet.hpp"
#include "usbguard/MEMRuleSet.hpp"

#include "FileRuleSet.hpp"

#ifdef HAVE_LDAP
  #include "LDAPRuleSet.hpp"
#endif

#include "NSHandler.hpp"

#include "RuleSetFactory.hpp"

namespace usbguard
{

  Interface* RuleSetFactory::interface_ptr = nullptr;

  void RuleSetFactory::setInterface(Interface* ptr)
  {
    RuleSetFactory::interface_ptr = ptr;
  }

  std::shared_ptr<RuleSet> RuleSetFactory::generateDefaultRuleSet()
  {
    auto rule_set = std::make_shared<MEMRuleSet>(RuleSetFactory::interface_ptr);
    return  std::dynamic_pointer_cast<RuleSet>(rule_set);
  }

  std::shared_ptr<RuleSet> RuleSetFactory::generateRuleSetBySource(NSHandler::SourceType type)
  {
    NSHandler& ns = NSHandler::getRef();
    std::shared_ptr<RuleSet> ruleSet(nullptr);

    switch (type) {
    case NSHandler::SourceType::LOCAL:
      if (ns.getRulesPath() != "") {
        ruleSet.reset(new FileRuleSet(interface_ptr, ns.getRulesPath()));
      }
      else {
        USBGUARD_LOG(Warning) << "RuleFile not set; Modification of the permanent policy won't be possible.";
        ruleSet = generateDefaultRuleSet();
      }

      break;
#ifdef HAVE_LDAP

    case NSHandler::SourceType::LDAP:
      ruleSet.reset(new LDAPRuleSet(interface_ptr, ns.getLDAPHandler()));
      break;
#endif

    default:
      ruleSet = generateDefaultRuleSet();
      break;
    }

    return ruleSet;
  }

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
