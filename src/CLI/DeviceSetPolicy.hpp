#pragma once

#include "CLICommand.hpp"
#include "Rule.hpp"

namespace usbguard
{
  class DeviceSetPolicy : public CLICommand
  {
  public:
    struct Options
    {
      uint32_t seqn;
      uint32_t timeout;
      bool permanent;

      Options()
      {
	seqn = Rule::SeqnLast;
	timeout = 0;
	permanent = true;
      }
    };

    DeviceSetPolicy(const Options& options);
    void execute(Interface&);
  private:
    Options _options;
  };

} /* namespace usbguard */
