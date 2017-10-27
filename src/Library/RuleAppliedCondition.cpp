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

#include "RuleAppliedCondition.hpp"
#include "RulePrivate.hpp"

#include "usbguard/RuleParser.hpp"

#include <string>
#ifndef _XOPEN_SOURCE
  #define _XOPEN_SOURCE
  #include <ctime>
#endif

namespace usbguard
{
  RuleAppliedCondition::RuleAppliedCondition(const std::string& elapsed_time, bool negated)
    : RuleConditionBase("rule-applied", elapsed_time, negated)
  {
    _elapsed_time = std::chrono::steady_clock::duration(stringToSeconds(elapsed_time));
  }

  RuleAppliedCondition::RuleAppliedCondition(const RuleAppliedCondition& rhs)
    : RuleConditionBase(rhs),
      _elapsed_time(rhs._elapsed_time)
  {
  }

  bool RuleAppliedCondition::update(const Rule& rule)
  {
    if (rule.internal()->metadata().counter_applied > 0) {
      if (_elapsed_time == std::chrono::steady_clock::duration::zero()) {
        return true;
      }
      else {
        const auto last_applied_duration = std::chrono::steady_clock::now() \
          - rule.internal()->metadata().tp_last_applied;

        if (last_applied_duration <= _elapsed_time) {
          return true;
        }
      }
    }

    return false;
  }

  RuleConditionBase* RuleAppliedCondition::clone() const
  {
    return new RuleAppliedCondition(*this);
  }

  uint64_t RuleAppliedCondition::stringToSeconds(const std::string& string)
  {
    struct ::tm tm = { };

    if (string.empty() || string == "") {
      return 0;
    }

    if (::strptime(string.c_str(), "%H:%M:%s", &tm) == nullptr) {
      if (::strptime(string.c_str(), "%H:%M", &tm) == nullptr) {
        if (::strptime(string.c_str(), "%s", &tm) == nullptr) {
          throw std::runtime_error("Invalid time string. Expecing either HH:MM or HH:MM:SS format.");
        }
      }
    }

    return tm.tm_sec + 60*tm.tm_min + 60*60*tm.tm_hour;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
