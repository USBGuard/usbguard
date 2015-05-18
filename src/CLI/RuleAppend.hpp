#pragma once

#include "CLICommand.hpp"
#include "Rule.hpp"

namespace usbguard
{
  class RuleAppend : public CLICommand
  {
  public:
    struct Options
    {
      uint32_t parent_seqn;
      uint32_t timeout_sec;

      Options()
      {
	parent_seqn = Rule::SeqnLast;
	timeout = 0;
      }
    };

    RuleAppend(const String& rule_spec, const Options& options);
    void execute(Interface&);
  private:
    Options _options;
    String _rule_spec;
  };
} /* namespace usbguard */
