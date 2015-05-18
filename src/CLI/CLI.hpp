#pragma once

#include <Typedefs.hpp>
#include <IPCClient.hpp>

namespace usbguard
{
  class CLI : public IPCClient
  {
  public:
    CLI();
  };
} /* namespace usbguard */
