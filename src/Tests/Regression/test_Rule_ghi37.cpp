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
#include "usbguard/Rule.hpp"

#include <catch.hpp>

using namespace usbguard;

TEST_CASE("Regression: GitHub issue #37", "[regression]")
{
  Rule rule;
  const std::string valid_rule_string = \
    "allow 17ef:6016 name \"ThinkPad USB Laser Mouse\" with-interface 03:01:02 hash \"b2b49929ddce325f217adc89cd2b6c20\"";
  const std::string invalid_rule_string = \
    "allow 17ef:6016 name \"ThinkPad USB Laser Mouse\" with-interface \"03:01:02\" hash \"b2b49929ddce325f217adc89cd2b6c20\"";
  SECTION("from string") {
    REQUIRE_NOTHROW(rule = Rule::fromString(valid_rule_string));
    REQUIRE_THROWS(rule = Rule::fromString(invalid_rule_string));
  }
}

/* vim: set ts=2 sw=2 et */
