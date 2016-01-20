#pragma once

#include "Typedefs.hpp"

namespace usbguard
{
  class Interface;
  class RuleCondition
  {
  public:
    RuleCondition(const String& identifier, const String& parameter, bool negated = false);
    RuleCondition(const String& identifier, bool negated = false);
    RuleCondition(const RuleCondition& rhs);
    virtual ~RuleCondition();

    virtual void init(Interface * const interface_ptr);
    virtual void fini();
    virtual bool update() = 0;
    virtual RuleCondition * clone() const = 0;

    bool evaluate();
    const String& identifier() const;
    const String& parameter() const;
    bool hasParameter() const;
    bool isNegated() const;
    const String toString() const;

    static RuleCondition * getImplementation(const String& identifier, const String& parameter, bool negated);

  private:
    const String _identifier;
    const String _parameter;
    const bool _negated;
  };
} /*namespace usbguard */

