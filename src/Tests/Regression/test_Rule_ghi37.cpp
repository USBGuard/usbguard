#include <catch.hpp>
#include <Rule.hpp>

using namespace usbguard;

TEST_CASE("Regression: GitHub issue #37", "[regression]") {
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
