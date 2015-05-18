#pragma once

#include <Interface.hpp>

namespace usbguard {
  class CLICommand
  {
  public:
    CLICommand();
    virtual ~CLICommand();

    virtual void prepare();
    virtual void execute(Interface&) = 0;
  };
} /* namespace usbguard */
