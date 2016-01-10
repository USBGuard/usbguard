#include "usbguard.hpp"
#include "usbguard-remove-rule.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
  };

  int usbguard_remove_rule(int argc, char *argv[])
  {
    usbguard::IPCClient ipc(/*connected=*/true);

    ipc.removeRule(std::stoul(argv[1]));

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
