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

#include "usbguard/Typedefs.hpp"
#include "usbguard/RuleSet.hpp"

#include <istream>
#include <ostream>
#include <mutex>

namespace usbguard
{
  class Interface;
  class DLL_PUBLIC MemoryRuleSet : public RuleSet
  {
  public:

    MemoryRuleSet(Interface* const interface_ptr);
    MemoryRuleSet(const MemoryRuleSet& rhs);
    const MemoryRuleSet& operator=(const MemoryRuleSet& rhs);

    void load() override;
    void save() override;

  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
