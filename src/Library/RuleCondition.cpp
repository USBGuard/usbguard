#include "RuleCondition.hpp"
#include "Rule.hpp"
#include "LoggerPrivate.hpp"

#include <unordered_map>
#include <functional>

namespace usbguard
{
  RuleCondition::RuleCondition(const String& identifier, const String& parameter, bool negated)
    : _identifier(identifier),
      _parameter(parameter),
      _negated(negated)
  {
  }

  RuleCondition::RuleCondition(const String& identifier, bool negated)
    : _identifier(identifier),
      _negated(negated)
  {
  }

  RuleCondition::RuleCondition(const RuleCondition& rhs)
    : _identifier(rhs._identifier),
      _parameter(rhs._parameter),
      _negated(rhs._negated)
  {
  }

  RuleCondition::~RuleCondition()
  {
  }

  void RuleCondition::init(Interface * const interface_ptr)
  {
    (void)interface_ptr;
  }

  void RuleCondition::fini()
  {
  }

  bool RuleCondition::evaluate()
  {
    return isNegated() ? !update() : update();
  }

  const String& RuleCondition::identifier() const
  {
    return _identifier;
  }

  const String& RuleCondition::parameter() const
  {
    return _parameter;
  }

  bool RuleCondition::hasParameter() const
  {
    return !_parameter.empty();
  }

  bool RuleCondition::isNegated() const
  {
    return _negated;
  }

  const String RuleCondition::toString() const
  {
    String condition_string;
    
    if (isNegated()) {
      condition_string.append("!");
    }
    
    condition_string.append(identifier());
   
    if (hasParameter()) {
      condition_string.append("(");
      condition_string.append(parameter()); /* TODO: Escape parameter string */
      condition_string.append(")");
    }

    return condition_string;
  }
} /* namespace usbguard */

#include "AllowedMatchesCondition.hpp"
#include "LocaltimeCondition.hpp"
#include "FixedStateCondition.hpp"
#include "RandomStateCondition.hpp"
#include <iostream>

namespace usbguard
{
  RuleCondition * RuleCondition::getImplementation(const String& identifier, const String& parameter, bool negated)
  {
    if (identifier == "allowed-matches") {
      return new AllowedMatchesCondition(parameter, negated);
    }
    if (identifier == "localtime") {
      return new LocaltimeCondition(parameter, negated);
    }
    if (identifier == "true") {
      return new FixedStateCondition(true, negated);
    }
    if (identifier == "false") {
      return new FixedStateCondition(false, negated);
    }
    if (identifier == "random") {
      return new RandomStateCondition(parameter, negated);
    }
    throw std::runtime_error("Unknown rule condition");
  }
} /* namespace usbguard */

