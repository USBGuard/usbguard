#include "RuleEvaluatedCondition.hpp"
#include "RuleParser.hpp"
#include "RulePrivate.hpp"
#include <string>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#include <ctime>
#endif

namespace usbguard
{
  RuleEvaluatedCondition::RuleEvaluatedCondition(const String& elapsed_time, bool negated)
    : RuleCondition("rule-applied", elapsed_time, negated)
  {
    _elapsed_time = std::chrono::steady_clock::duration(stringToSeconds(elapsed_time));
  }

  RuleEvaluatedCondition::RuleEvaluatedCondition(const RuleEvaluatedCondition& rhs)
    : RuleCondition(rhs),
      _elapsed_time(rhs._elapsed_time)
  {
  }

  bool RuleEvaluatedCondition::update(const Rule& rule)
  {
    if (rule.internal()->metadata().counter_evaluated > 0) {
      if (_elapsed_time == std::chrono::steady_clock::duration::zero()) {
        return true;
      }
      else {
        const auto last_evaluated_duration = std::chrono::steady_clock::now() \
                                              - rule.internal()->metadata().tp_last_evaluated;

        if (last_evaluated_duration <= _elapsed_time) {
          return true;
        }
      }
    }
    return false;
  }

  RuleCondition * RuleEvaluatedCondition::clone() const
  {
    return new RuleEvaluatedCondition(*this);
  }

  uint64_t RuleEvaluatedCondition::stringToSeconds(const String& string)
  {
    struct ::tm tm = { 0 };

    if (string.empty() || string == "") {
      return 0;
    }

    if (::strptime(string.c_str(), "%H:%M:%s", &tm) == nullptr) {
      if (::strptime(string.c_str(), "%H:%M", &tm) == nullptr) {
        if (::strptime(string.c_str(), "%s", &tm) == nullptr) {
          throw std::runtime_error("Invalid time string. Expecing either HH:MM, HH:MM:SS or SS format.");
        }
      }
    }

    return tm.tm_sec + 60*tm.tm_min + 60*60*tm.tm_hour;
  }
} /* namespace usbguard */

