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

#include "RuleSet.hpp"
#include "MemoryRuleSet.hpp"

namespace usbguard
{
  MemoryRuleSet::MemoryRuleSet(Interface* const interface_ptr)
    : RuleSet(interface_ptr)
  {
    setWritable();
    USBGUARD_LOG(Info) << "Creating MemoryRuleSet";
  }

  MemoryRuleSet::MemoryRuleSet(const MemoryRuleSet& rhs)
    : RuleSet(rhs._interface_ptr)
  {
    *this = rhs;
  }

  const MemoryRuleSet& MemoryRuleSet::operator=(const MemoryRuleSet& rhs)
  {
    RuleSet::operator = (rhs);
    return *this;
  }

  void MemoryRuleSet::load()
  {
  }

  void MemoryRuleSet::save()
  {
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
