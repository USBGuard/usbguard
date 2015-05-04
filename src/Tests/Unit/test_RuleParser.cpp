#include <catch.hpp>
#include <Rule.hpp>

using namespace usbguard;

TEST_CASE("Non-printable characters in a rule string", "[RuleParser]") {
  Rule rule, rule_from;
  std::string rule_string;
  
  const uint8_t non_printable_cstr[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xAA, 0xBB, 0xFF };
  const size_t non_printable_size = sizeof non_printable_cstr;
  const std::string non_printable_string((const char *)non_printable_cstr, non_printable_size);

  SECTION("to/from string: allow serial \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setSerialNumber(non_printable_string);
    
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow serial \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow serial \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
}
