#include "FixedStateCondition.hpp"
#include "RuleParser.hpp"

namespace usbguard
{
  FixedStateCondition::FixedStateCondition(bool state, bool negated)
    : RuleCondition(state ? "true" : "false", negated),
      _state(state)
  {
  }

  FixedStateCondition::FixedStateCondition(const FixedStateCondition& rhs)
    : RuleCondition(rhs),
      _state(rhs._state)
  {
  }

  bool FixedStateCondition::update()
  {
    return _state;
  }

  RuleCondition * FixedStateCondition::clone() const
  {
    return new FixedStateCondition(*this);
  }
} /* namespace usbguard */

