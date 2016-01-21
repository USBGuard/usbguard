#pragma once
#include "Typedefs.hpp"
#include "RuleCondition.hpp"
#include "Rule.hpp"
#include <random>

namespace usbguard
{
  class RandomStateCondition : public RuleCondition
  {
  public:
    RandomStateCondition(const String& true_probability, bool negated = false);
    RandomStateCondition(const RandomStateCondition& rhs);
    bool update(const Rule& rule);
    RuleCondition * clone() const;
  private:
    std::random_device _rng_device;
    std::mt19937 _rng_gen;
    double _true_probability;
    std::bernoulli_distribution _rng_dist;
  };
} /* namespace usbguard */

