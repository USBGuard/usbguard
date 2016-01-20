#include "AllowedMatchesCondition.hpp"
#include "RuleParser.hpp"

namespace usbguard
{
  AllowedMatchesCondition::AllowedMatchesCondition(const String& device_spec, bool negated)
    : RuleCondition("allowed-matches", device_spec, negated)
  {
    _device_match_rule = parseRuleSpecification(std::string("match ") + device_spec);
    _interface_ptr = nullptr;
  }

  AllowedMatchesCondition::AllowedMatchesCondition(const AllowedMatchesCondition& rhs)
    : RuleCondition(rhs)
  {
    _device_match_rule = rhs._device_match_rule;
    _interface_ptr = rhs._interface_ptr;
  }

  void AllowedMatchesCondition::init(Interface * const interface_ptr)
  {
    _interface_ptr = interface_ptr;
  }

  bool AllowedMatchesCondition::update()
  {
    return false;
  }

  RuleCondition * AllowedMatchesCondition::clone() const
  {
    return new AllowedMatchesCondition(*this);
  }
} /* namespace usbguard */

