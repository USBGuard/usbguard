#include "usbguard.hpp"
#include "usbguard-list-devices.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "blocked", no_argument, nullptr, 'a' },
    { "allowed", no_argument, nullptr, 'b' }
  };

  int usbguard_list_devices(int argc, char *argv[])
  {
    usbguard::IPCClient ipc(/*connected=*/true);

    for (auto map_entry : ipc.listDevices()) {
      std::cout << map_entry.first << ": " << map_entry.second << std::endl;
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */