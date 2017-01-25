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
#include "LocaltimeCondition.hpp"
#include "RuleParser.hpp"
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#include <ctime>
#endif

namespace usbguard
{
  LocaltimeCondition::LocaltimeCondition(const String& time_range, bool negated)
    : RuleCondition("localtime", time_range, negated)
  {
    String time_begin;
    String time_end;
    const size_t dash_pos = time_range.find('-');

    if (dash_pos == std::string::npos) {
      time_begin = time_range;
    }
    else {
      time_begin = time_range.substr(0, dash_pos);
      time_end = time_range.substr(dash_pos + 1);
    }

    _tp_begin = stringToTimePoint(time_begin);

    if (!time_end.empty()) {
      _tp_end = stringToTimePoint(time_end);
    }
    else {
      _tp_end = _tp_begin;
    }
  }

  LocaltimeCondition::LocaltimeCondition(const LocaltimeCondition& rhs)
    : RuleCondition(rhs)
  {
    _tp_begin = rhs._tp_begin;
    _tp_end = rhs._tp_end;
  }

  bool LocaltimeCondition::update(const Rule& rule)
  {
    (void)rule;
    const auto tp_now = std::chrono::system_clock::now();
    return (tp_now >= _tp_begin && tp_now <= _tp_end);
  }

  RuleCondition * LocaltimeCondition::clone() const
  {
    return new LocaltimeCondition(*this);
  }

  std::chrono::system_clock::time_point LocaltimeCondition::stringToTimePoint(const String& string)
  {
    struct ::tm tm = { };

    if (::strptime(string.c_str(), "%H:%M:%s", &tm) == nullptr) {
      if (::strptime(string.c_str(), "%H:%M", &tm) == nullptr) {
        throw std::runtime_error("Invalid time string. Expecing either HH:MM or HH:MM:SS format.");
      }
    }

    return std::chrono::system_clock::from_time_t(::mktime(&tm)); 
  }
} /* namespace usbguard */

