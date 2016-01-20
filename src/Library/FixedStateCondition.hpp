#pragma once
#include "Typedefs.hpp"
#include "RuleCondition.hpp"
#include "Rule.hpp"

namespace usbguard
{
  class FixedStateCondition : public RuleCondition
  {
  public:
    FixedStateCondition(bool state, bool negated = false);
    FixedStateCondition(const FixedStateCondition& rhs);
    bool update();
    RuleCondition * clone() const;
  private:
    const bool _state;
  };
} /* namespace usbguard */

