#include "RandomStateCondition.hpp"
#include "RuleParser.hpp"
#include <string>

namespace usbguard
{
  RandomStateCondition::RandomStateCondition(const String& true_probability, bool negated)
    : RuleCondition("random", true_probability, negated),
      _rng_gen(_rng_device()),
      _true_probability(true_probability.empty() ? 0.5 : std::stod(true_probability)),
      _rng_dist(_true_probability)
  {
  }

  RandomStateCondition::RandomStateCondition(const RandomStateCondition& rhs)
    : RuleCondition(rhs),
      _rng_gen(_rng_device()),
      _true_probability(rhs._true_probability),
      _rng_dist(_true_probability)
  {
  }

  bool RandomStateCondition::update(const Rule& rule)
  {
    (void)rule;
    return _rng_dist(_rng_gen);
  }

  RuleCondition * RandomStateCondition::clone() const
  {
    return new RandomStateCondition(*this);
  }
} /* namespace usbguard */

