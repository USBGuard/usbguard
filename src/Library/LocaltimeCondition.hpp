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
#include "Typedefs.hpp"
#include "RuleCondition.hpp"
#include "Rule.hpp"
#include <chrono>

namespace usbguard
{
  class LocaltimeCondition : public RuleConditionBase
  {
  public:
    LocaltimeCondition(const std::string& time_range, bool negated = false);
    LocaltimeCondition(const LocaltimeCondition& rhs);
    bool update(const Rule& rule);
    RuleConditionBase * clone() const;

  protected:
    static std::time_t stringToDaytime(const std::string& string);
    static std::string tmToString(const struct ::tm * const tm);

  private:
    std::time_t _daytime_begin;
    std::time_t _daytime_end;
  };
} /* namespace usbguard */

