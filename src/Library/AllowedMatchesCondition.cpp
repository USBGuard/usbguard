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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "AllowedMatchesCondition.hpp"

#include "usbguard/Interface.hpp"
#include "usbguard/Logger.hpp"
#include "usbguard/RuleParser.hpp"

namespace usbguard
{
  AllowedMatchesCondition::AllowedMatchesCondition(const std::string& device_spec, bool negated)
    : RuleConditionBase("allowed-matches", device_spec, negated)
  {
    _device_match_rule = parseRuleFromString(std::string("allow ") + device_spec);
    _interface_ptr = nullptr;
  }

  AllowedMatchesCondition::AllowedMatchesCondition(const AllowedMatchesCondition& rhs)
    : RuleConditionBase(rhs)
  {
    _device_match_rule = rhs._device_match_rule;
    _interface_ptr = rhs._interface_ptr;
  }

  void AllowedMatchesCondition::init(Interface* const interface_ptr)
  {
    _interface_ptr = interface_ptr;
  }

  bool AllowedMatchesCondition::update(const Rule& rule)
  {
    (void)rule;

    if (_interface_ptr == nullptr) {
      return false;
    }

    auto devices = _interface_ptr->listDevices(_device_match_rule.toString());
    return !devices.empty();
  }

  RuleConditionBase* AllowedMatchesCondition::clone() const
  {
    return new AllowedMatchesCondition(*this);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
