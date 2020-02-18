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
#pragma once

#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include <iostream>

#include "usbguard/Typedefs.hpp"
#include "usbguard/RuleSet.hpp"
#include "usbguard/MemoryRuleSet.hpp"

#include "FileRuleSet.hpp"

#ifdef HAVE_LDAP
  #include "LDAPRuleSet.hpp"
#endif

#include "NSHandler.hpp"
#include "usbguard/Interface.hpp"

namespace usbguard
{
  class DLL_PUBLIC RuleSetFactory
  {
  public:

    static void setInterface(Interface* ptr);

    static std::vector<std::shared_ptr<RuleSet>> generateDefaultRuleSet();
    static std::vector<std::shared_ptr<RuleSet>> generateRuleSetBySource(NSHandler::SourceType type);

  private:
    static Interface* interface_ptr;

  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
