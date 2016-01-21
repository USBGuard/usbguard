#pragma once
#include "Typedefs.hpp"
#include "RuleCondition.hpp"
#include "Rule.hpp"

namespace usbguard
{
  class AllowedMatchesCondition : public RuleCondition
  {
  public:
    AllowedMatchesCondition(const String& device_spec, bool negated = false);
    AllowedMatchesCondition(const AllowedMatchesCondition& rhs);
    void init(Interface * const interface_ptr);
    bool update(const Rule& rule);
    RuleCondition * clone() const;
  private:
    Rule _device_match_rule;
    Interface * _interface_ptr;
  };
} /* namespace usbguard */

