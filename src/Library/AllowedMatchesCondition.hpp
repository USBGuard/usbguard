//
// Copyright (C) 2016 Red Hat, Inc.
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
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/RuleCondition.hpp"

#include "usbguard/Rule.hpp"
#include "usbguard/Typedefs.hpp"

#include <string>

namespace usbguard
{
  class AllowedMatchesCondition : public RuleConditionBase
  {
  public:
    AllowedMatchesCondition(const std::string& device_spec, bool negated = false);
    AllowedMatchesCondition(const AllowedMatchesCondition& rhs);
    void init(Interface* const interface_ptr);
    bool update(const Rule& rule);
    RuleConditionBase* clone() const;
  private:
    Rule _device_match_rule;
    Interface* _interface_ptr;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
