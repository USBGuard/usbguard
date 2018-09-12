//
// Copyright (C) 2018 Red Hat, Inc.
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
// Authors: Allen Webb <allenwebb@google.com>
//
#include "usbguard/Rule.hpp"
#include "usbguard/Rule.hpp"

#include <catch.hpp>

using namespace usbguard;

TEST_CASE("Regression: GitHub issue #247", "[regression]")
{
  Rule target_rule;
  Rule source_rule;
  const std::string source_rule_spec =
    "allow with-interface equals-ordered { 03:00:00 03:01:01 }";
  const std::string target_rule_spec =
    "allow with-interface equals-ordered { 03:00:00 03:01:01 }";
  CHECK_NOTHROW(target_rule = Rule::fromString(target_rule_spec));
  CHECK_NOTHROW(source_rule = Rule::fromString(source_rule_spec));
  REQUIRE(source_rule.appliesTo(target_rule));
}

/* vim: set ts=2 sw=2 et */
