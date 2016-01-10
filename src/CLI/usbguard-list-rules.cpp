#include "usbguard.hpp"
#include "usbguard-list-rules.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
  };

  int usbguard_list_rules(int argc, char *argv[])
  {
    usbguard::IPCClient ipc(/*connected=*/true);

    for (auto map_entry : ipc.listRules()) {
      std::cout << map_entry.first << ": " << map_entry.second << std::endl;
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
