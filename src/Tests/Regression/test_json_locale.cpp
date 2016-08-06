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
#include <catch.hpp>
#include <Common/JSON.hpp>
#include <locale>

TEST_CASE("Regression: nlohmann/json locale", "[regression]") {
  REQUIRE_NOTHROW(std::locale::global(std::locale::classic()));

  try {
    auto locale_cs = std::locale("cs_CZ.UTF-8");
    auto locale_de = std::locale("de_DE.UTF-8");
  }
  catch(...) {
    FAIL("Cannot execute test: missing required cs_CZ.UTF-8 and/or de_DE.UTF-8 locale");
  }

  const std::string method_name = "fooBar";
  const std::string rule_spec = "allow";
  const uint64_t parent_id = 1234;
  const uint64_t timeout_sec = 0;
  const uint64_t unique_id = 17;

  const json jobj = {
    {          "_m", method_name },
    {   "rule_spec", rule_spec },
    { "parent_id", parent_id },
    { "timeout_sec", timeout_sec },
    {          "_i", unique_id },
    { "boolean_true", true },
    { "boolean_false", false },
  };

  SECTION("Serialize in C => Parse & Inspect in cs_CZ.UTF-8") {
    std::locale::global(std::locale::classic());
    const std::string jobj_string = jobj.dump();

    std::locale::global(std::locale("cs_CZ.UTF-8"));
    const json jobj_from_string = json::parse(jobj_string);

    REQUIRE(method_name == jobj_from_string["_m"]);
    REQUIRE(rule_spec == jobj_from_string["rule_spec"]);
    REQUIRE(parent_id == jobj_from_string["parent_id"].get<uint64_t>());
    REQUIRE(timeout_sec == jobj_from_string["timeout_sec"].get<uint64_t>());
    REQUIRE(unique_id == jobj_from_string["_i"].get<uint64_t>());
    REQUIRE(true == jobj_from_string["boolean_true"].get<bool>());
    REQUIRE(false == jobj_from_string["boolean_false"].get<bool>());
  }

  SECTION("Serialize in cs_CZ.UTF-8 => Parse & Inspect in C") {
    std::locale::global(std::locale("cs_CZ.UTF-8"));
    const std::string jobj_string = jobj.dump();

    std::locale::global(std::locale::classic());
    const json jobj_from_string = json::parse(jobj_string);

    REQUIRE(method_name == jobj_from_string["_m"]);
    REQUIRE(rule_spec == jobj_from_string["rule_spec"]);
    REQUIRE(parent_id == jobj_from_string["parent_id"].get<uint64_t>());
    REQUIRE(timeout_sec == jobj_from_string["timeout_sec"].get<uint64_t>());
    REQUIRE(unique_id == jobj_from_string["_i"].get<uint64_t>());
    REQUIRE(true == jobj_from_string["boolean_true"].get<bool>());
    REQUIRE(false == jobj_from_string["boolean_false"].get<bool>());
  }

  SECTION("Serialize in C => Parse & Inspect in de_DE.UTF-8") {
    std::locale::global(std::locale::classic());
    const std::string jobj_string = jobj.dump();

    std::locale::global(std::locale("de_DE.UTF-8"));
    const json jobj_from_string = json::parse(jobj_string);

    REQUIRE(method_name == jobj_from_string["_m"]);
    REQUIRE(rule_spec == jobj_from_string["rule_spec"]);
    REQUIRE(parent_id == jobj_from_string["parent_id"].get<uint64_t>());
    REQUIRE(timeout_sec == jobj_from_string["timeout_sec"].get<uint64_t>());
    REQUIRE(unique_id == jobj_from_string["_i"].get<uint64_t>());
    REQUIRE(true == jobj_from_string["boolean_true"].get<bool>());
    REQUIRE(false == jobj_from_string["boolean_false"].get<bool>());
  }

  SECTION("Serialize in de_DE.UTF-8 => Parse & Inspect in C") {
    std::locale::global(std::locale("de_DE.UTF-8"));
    const std::string jobj_string = jobj.dump();

    std::locale::global(std::locale::classic());
    const json jobj_from_string = json::parse(jobj_string);

    REQUIRE(method_name == jobj_from_string["_m"]);
    REQUIRE(rule_spec == jobj_from_string["rule_spec"]);
    REQUIRE(parent_id == jobj_from_string["parent_id"].get<uint64_t>());
    REQUIRE(timeout_sec == jobj_from_string["timeout_sec"].get<uint64_t>());
    REQUIRE(unique_id == jobj_from_string["_i"].get<uint64_t>());
    REQUIRE(true == jobj_from_string["boolean_true"].get<bool>());
    REQUIRE(false == jobj_from_string["boolean_false"].get<bool>());
  }

  SECTION("Serialize in cs_CZ.UTF-8 => Parse & Inspect in cs_CZ.UTF-8") {
    std::locale::global(std::locale("cs_CZ.UTF-8"));
    const std::string jobj_string = jobj.dump();
    const json jobj_from_string = json::parse(jobj_string);

    REQUIRE(method_name == jobj_from_string["_m"]);
    REQUIRE(rule_spec == jobj_from_string["rule_spec"]);
    REQUIRE(parent_id == jobj_from_string["parent_id"].get<uint64_t>());
    REQUIRE(timeout_sec == jobj_from_string["timeout_sec"].get<uint64_t>());
    REQUIRE(unique_id == jobj_from_string["_i"].get<uint64_t>());
    REQUIRE(true == jobj_from_string["boolean_true"].get<bool>());
    REQUIRE(false == jobj_from_string["boolean_false"].get<bool>());
  }

  SECTION("Serialize in de_DE.UTF-8 => Parse & Inspect in de_DE.UTF-8") {
    std::locale::global(std::locale("de_DE.UTF-8"));
    const std::string jobj_string = jobj.dump();
    const json jobj_from_string = json::parse(jobj_string);

    REQUIRE(method_name == jobj_from_string["_m"]);
    REQUIRE(rule_spec == jobj_from_string["rule_spec"]);
    REQUIRE(parent_id == jobj_from_string["parent_id"].get<uint64_t>());
    REQUIRE(timeout_sec == jobj_from_string["timeout_sec"].get<uint64_t>());
    REQUIRE(unique_id == jobj_from_string["_i"].get<uint64_t>());
    REQUIRE(true == jobj_from_string["boolean_true"].get<bool>());
    REQUIRE(false == jobj_from_string["boolean_false"].get<bool>());
  }
}

