#pragma once
#include "Typedefs.hpp"
#include "RuleCondition.hpp"
#include "Rule.hpp"
#include <chrono>

namespace usbguard
{
  class RuleAppliedCondition : public RuleCondition
  {
  public:
    RuleAppliedCondition(const String& elapsed_time, bool negated = false);
    RuleAppliedCondition(const RuleAppliedCondition& rhs);
    bool update(const Rule& rule);
    RuleCondition * clone() const;
  protected:
    static uint64_t stringToSeconds(const String& string);
  private:
    std::chrono::steady_clock::duration _elapsed_time;
  };
} /* namespace usbguard */

