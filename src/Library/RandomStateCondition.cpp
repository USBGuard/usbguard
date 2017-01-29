//
// Copyright (C) 2016 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#include "RandomStateCondition.hpp"
#include "RuleParser.hpp"
#include <string>

namespace usbguard
{
  RandomStateCondition::RandomStateCondition(const String& true_probability, bool negated)
    : RuleConditionBase("random", true_probability, negated),
      _rng_gen(_rng_device()),
      _true_probability(true_probability.empty() ? 0.5 : std::stod(true_probability)),
      _rng_dist(_true_probability)
  {
  }

  RandomStateCondition::RandomStateCondition(const RandomStateCondition& rhs)
    : RuleConditionBase(rhs),
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

  RuleConditionBase * RandomStateCondition::clone() const
  {
    return new RandomStateCondition(*this);
  }
} /* namespace usbguard */

