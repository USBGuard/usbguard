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

#include "LocaltimeCondition.hpp"
#include "Common/Utility.hpp"

#include "usbguard/RuleParser.hpp"

#ifndef _XOPEN_SOURCE
  #define _XOPEN_SOURCE
  #include <ctime>
#endif

namespace usbguard
{
  LocaltimeCondition::LocaltimeCondition(const std::string& time_range, bool negated)
    : RuleConditionBase("localtime", time_range, negated)
  {
    std::string time_begin;
    std::string time_end;
    const size_t dash_pos = time_range.find('-');

    if (dash_pos == std::string::npos) {
      time_begin = time_range;
    }
    else {
      time_begin = time_range.substr(0, dash_pos);
      time_end = time_range.substr(dash_pos + 1);
    }

    _daytime_begin = stringToDaytime(time_begin);

    if (!time_end.empty()) {
      _daytime_end = stringToDaytime(time_end);
    }
    else {
      _daytime_end = _daytime_begin;
    }

    if (_daytime_begin > _daytime_end) {
      throw Exception("LocaltimeCondition", "Invalid time range (begin > end)", time_range);
    }
  }

  LocaltimeCondition::LocaltimeCondition(const LocaltimeCondition& rhs)
    : RuleConditionBase(rhs)
  {
    _daytime_begin = rhs._daytime_begin;
    _daytime_end = rhs._daytime_end;
  }

  bool LocaltimeCondition::update(const Rule& rule)
  {
    USBGUARD_LOG(Trace);
    (void)rule;
    const auto tp_now = std::chrono::system_clock::now();
    const auto daytime = std::chrono::system_clock::to_time_t(tp_now) % 86400;
    USBGUARD_LOG(Trace) << "daytime=" << daytime
      << " daytime_begin=" << _daytime_begin
      << " daytime_end=" << _daytime_end;
    return (daytime >= _daytime_begin && daytime <= _daytime_end);
  }

  RuleConditionBase* LocaltimeCondition::clone() const
  {
    return new LocaltimeCondition(*this);
  }

  std::string LocaltimeCondition::tmToString(const struct tm* const tm)
  {
    std::string tm_string;
    tm_string.append("{ tm.tm_sec=");
    tm_string.append(numberToString(tm->tm_sec));
    tm_string.append(" tm.tm_min=");
    tm_string.append(numberToString(tm->tm_min));
    tm_string.append(" tm.tm_hour=");
    tm_string.append(numberToString(tm->tm_hour));
    tm_string.append(" tm.tm_mday=");
    tm_string.append(numberToString(tm->tm_mday));
    tm_string.append(" tm.tm_mon=");
    tm_string.append(numberToString(tm->tm_mon));
    tm_string.append(" tm.tm_year=");
    tm_string.append(numberToString(tm->tm_year));
    tm_string.append(" tm.tm_wday=");
    tm_string.append(numberToString(tm->tm_wday));
    tm_string.append(" tm.tm_yday=");
    tm_string.append(numberToString(tm->tm_yday));
    tm_string.append(" tm.tm_isdst=");
    tm_string.append(numberToString(tm->tm_isdst));
    tm_string.append(" }");
    return tm_string;
  }

  std::time_t LocaltimeCondition::stringToDaytime(const std::string& string)
  {
    USBGUARD_LOG(Trace) << "string=" << string;
    struct ::tm tm = { };

    if (::strptime(string.c_str(), "%H:%M:%s", &tm) == nullptr) {
      if (::strptime(string.c_str(), "%H:%M", &tm) == nullptr) {
        throw Exception("LocaltimeCondition", "Invalid time or range format", string);
      }
    }

    USBGUARD_LOG(Trace) << "tm=" << tmToString(&tm);
    return tm.tm_sec + 60*tm.tm_min + 60*60*tm.tm_hour;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
