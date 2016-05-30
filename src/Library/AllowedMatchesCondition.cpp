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
#include "AllowedMatchesCondition.hpp"
#include "RuleParser.hpp"
#include "LoggerPrivate.hpp"
#include <Interface.hpp>

namespace usbguard
{
  AllowedMatchesCondition::AllowedMatchesCondition(const String& device_spec, bool negated)
    : RuleCondition("allowed-matches", device_spec, negated)
  {
    _device_match_rule = parseRuleSpecification(std::string("allow ") + device_spec);
    _interface_ptr = nullptr;
  }

  AllowedMatchesCondition::AllowedMatchesCondition(const AllowedMatchesCondition& rhs)
    : RuleCondition(rhs)
  {
    _device_match_rule = rhs._device_match_rule;
    _interface_ptr = rhs._interface_ptr;
  }

  void AllowedMatchesCondition::init(Interface * const interface_ptr)
  {
    logger->debug("AllowedMatchesCondition::init setting interface ptr to {}", (void *)interface_ptr);
    _interface_ptr = interface_ptr;
  }

  bool AllowedMatchesCondition::update(const Rule& rule)
  {
    (void)rule;
    if (_interface_ptr == nullptr) {
      logger->debug("AllowedMatchesCondition::update interface ptr not set!");
      return false;
    }
    auto devices = _interface_ptr->listDevices(_device_match_rule.toString());
    logger->debug("AllowedMatches: {} devices matches query {}", devices.size(), _device_match_rule.toString());
    return !devices.empty();
  }

  RuleCondition * AllowedMatchesCondition::clone() const
  {
    return new AllowedMatchesCondition(*this);
  }
} /* namespace usbguard */

