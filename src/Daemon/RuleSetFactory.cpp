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

#include <algorithm>

#include "usbguard/RuleSet.hpp"
#include "usbguard/MemoryRuleSet.hpp"

#include "Common/Utility.hpp"

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

  std::vector<std::shared_ptr<RuleSet>> RuleSetFactory::generateDefaultRuleSet()
  {
    std::shared_ptr<MemoryRuleSet> rule_set = std::make_shared<MemoryRuleSet>(RuleSetFactory::interface_ptr);
    std::shared_ptr<RuleSet> _rule_set = std::dynamic_pointer_cast<RuleSet>(rule_set);
    std::vector<std::shared_ptr<RuleSet>> vec;
    vec.push_back(_rule_set);
    return vec;
  }

  std::vector<std::shared_ptr<RuleSet>> RuleSetFactory::generateRuleSetBySource(NSHandler::SourceType type)
  {
    NSHandler& ns = NSHandler::getRef();
    std::vector<std::shared_ptr<RuleSet>> ruleSet;

    switch (type) {
    case NSHandler::SourceType::LOCAL:
      if (!ns.getRulesPath().empty()) {
        ruleSet.emplace_back(new FileRuleSet(interface_ptr, ns.getRulesPath()));
      }

      if (!ns.getRulesDirPath().empty()) {
        for (auto path : getConfigsFromDir(ns.getRulesDirPath())) {
          auto frs = std::make_shared<FileRuleSet>(interface_ptr, path);
          auto rs = std::dynamic_pointer_cast<RuleSet>(frs);
          ruleSet.push_back(rs);
        }
      }

      /*
       * This means one of the following:
       *  - Neither RuleFile nor RuleFolder are specified
       *  - RuleFile not specified, RuleFolder is but it does not contain any files,
       *    where we could save permanent rules
       */
      if (ruleSet.empty()) {
        std::string msg;

        if (ns.getRulesPath().empty() && ns.getRulesDirPath().empty()) {
          msg = "Neither RuleFile nor RuleFolder are set.";
        }
        else {
          msg = "RuleFile is not set, RuleFolder is but it does not contain any rule files.";
        }

        USBGUARD_LOG(Warning) << "Modification of the permanent policy won't be possible."
          << " Reason: " << msg;
        ruleSet = generateDefaultRuleSet();
      }

      break;
#ifdef HAVE_LDAP

    case NSHandler::SourceType::LDAP:
      ruleSet.emplace_back(new LDAPRuleSet(interface_ptr, ns.getLDAPHandler()));
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
