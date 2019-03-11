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

TEST_CASE("Non-printable characters in a rule string", "[RuleParser]")
{
  Rule rule, rule_from;
  std::string rule_string;
  const uint8_t non_printable_cstr[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xAA, 0xBB, 0xFF };
  const size_t non_printable_size = sizeof non_printable_cstr;
  const std::string non_printable_string((const char*)non_printable_cstr, non_printable_size);
  SECTION("to/from string: allow serial \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setSerial(non_printable_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow serial \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow serial \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow name \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setName(non_printable_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow name \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow name \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow hash \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setHash(non_printable_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow hash \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow hash \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow via-port \"<non printable>\"") {
    const std::vector<std::string> one_non_printable_string = { non_printable_string };
    rule.setTarget(Rule::Target::Allow);
    rule.attributeViaPort().set(one_non_printable_string, Rule::SetOperator::Equals);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow via-port { \"<non printable>\" \"<non printable>\" }") {
    const std::vector<std::string> two_non_printable_strings = \
    { non_printable_string, non_printable_string };
    rule.setTarget(Rule::Target::Allow);
    rule.attributeViaPort().set(two_non_printable_strings, Rule::SetOperator::OneOf);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string ==
      "allow via-port one-of { \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" }");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string ==
      "allow via-port one-of { \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" }");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow with-connect-type \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setWithConnectType(non_printable_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(
      rule_string ==
      "allow with-connect-type \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(
      rule_string ==
      "allow with-connect-type \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
}

TEST_CASE("Double quote and backslash characters in a rule string", "[RuleParser]")
{
  Rule rule, rule_from;
  std::string rule_string;
  const std::string dqb_string("\" \\ \"\" \\\\ \\\" \"\\");
  const std::string dqb_string_escaped("\\\" \\\\ \\\"\\\" \\\\\\\\ \\\\\\\" \\\"\\\\");
  SECTION("to/from string: allow serial \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setSerial(dqb_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow serial \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow serial \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow name \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setName(dqb_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow name \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow name \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow hash \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setHash(dqb_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow hash \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow hash \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow via-port \"<double quote and backslash>\"") {
    const std::vector<std::string> one_dqb_string = { dqb_string };
    rule.setTarget(Rule::Target::Allow);
    rule.attributeViaPort().set(one_dqb_string, Rule::SetOperator::Equals);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow via-port { \"<double quote and backslash>\" \"<double quote and backslash>\" }") {
    const std::vector<std::string> two_dqb_strings = { dqb_string, dqb_string };
    rule.setTarget(Rule::Target::Allow);
    rule.attributeViaPort().set(two_dqb_strings, Rule::SetOperator::OneOf);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port one-of { \"" + dqb_string_escaped + "\" \"" + dqb_string_escaped + "\" }");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port one-of { \"" + dqb_string_escaped + "\" \"" + dqb_string_escaped + "\" }");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
  SECTION("to/from string: allow with-connect-type \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setWithConnectType(dqb_string);
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow with-connect-type \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow with-connect-type \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
}

/* vim: set ts=2 sw=2 et */
