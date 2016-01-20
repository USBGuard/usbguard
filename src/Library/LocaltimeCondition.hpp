#pragma once
#include "Typedefs.hpp"
#include "RuleCondition.hpp"
#include "Rule.hpp"
#include <chrono>

namespace usbguard
{
  class LocaltimeCondition : public RuleCondition
  {
  public:
    LocaltimeCondition(const String& time_range, bool negated = false);
    LocaltimeCondition(const LocaltimeCondition& rhs);
    bool update();
    RuleCondition * clone() const;

  protected:
    static std::chrono::system_clock::time_point stringToTimePoint(const String& string);

  private:
    std::chrono::system_clock::time_point _tp_begin;
    std::chrono::system_clock::time_point _tp_end;
  };
} /* namespace usbguard */

