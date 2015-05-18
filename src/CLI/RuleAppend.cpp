#include "RuleAppend.hpp"
#include <iostream>

namespace usbguard
{
  RuleAppend::RuleAppend(const String& rule_spec, const Options& options)
  {
    _options = options;
    _rule_spec = rule_spec;
  }

  void RuleAppend::execute(Interface& usbguard)
  {
    usbguard.appendRule(_rule_spec, _options.parent_seqn, _options.timeout_sec);
  }
} /* namespace usbguard */
