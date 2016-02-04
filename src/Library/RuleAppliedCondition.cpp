#include "RuleAppliedCondition.hpp"
#include "RuleParser.hpp"
#include "RulePrivate.hpp"
#include <string>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#include <ctime>
#endif

namespace usbguard
{
  RuleAppliedCondition::RuleAppliedCondition(const String& elapsed_time, bool negated)
    : RuleCondition("rule-applied", elapsed_time, negated)
  {
    _elapsed_time = std::chrono::steady_clock::duration(stringToSeconds(elapsed_time));
  }

  RuleAppliedCondition::RuleAppliedCondition(const RuleAppliedCondition& rhs)
    : RuleCondition(rhs),
      _elapsed_time(rhs._elapsed_time)
  {
  }

  bool RuleAppliedCondition::update(const Rule& rule)
  {
    if (rule.internal()->metadata().counter_applied > 0) {
      if (_elapsed_time == std::chrono::steady_clock::duration::zero()) {
        return true;
      }
      else {
        const auto last_applied_duration = std::chrono::steady_clock::now() \
                                            - rule.internal()->metadata().tp_last_applied;

        if (last_applied_duration <= _elapsed_time) {
          return true;
        }
      }
    }
    return false;
  }

  RuleCondition * RuleAppliedCondition::clone() const
  {
    return new RuleAppliedCondition(*this);
  }

  uint64_t RuleAppliedCondition::stringToSeconds(const String& string)
  {
    struct ::tm tm = { 0 };

    if (string.empty() || string == "") {
      return 0;
    }

    if (::strptime(string.c_str(), "%H:%M:%s", &tm) == nullptr) {
      if (::strptime(string.c_str(), "%H:%M", &tm) == nullptr) {
        if (::strptime(string.c_str(), "%s", &tm) == nullptr) {
          throw std::runtime_error("Invalid time string. Expecing either HH:MM or HH:MM:SS format.");
        }
      }
    }

    return tm.tm_sec + 60*tm.tm_min + 60*60*tm.tm_hour;
  }
} /* namespace usbguard */

