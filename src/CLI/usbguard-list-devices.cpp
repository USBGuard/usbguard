#include "usbguard.hpp"
#include <IPCClient.hpp>
#include <iostream>

static const char *options_short = "hab";

static const struct ::option options_long[] = {
  { "help", no_argument, nullptr, 'h' },
  { "blocked", no_argument, nullptr, 'a' },
  { "allowed", no_argument, nullptr, 'b' }
};

int usbguard_list_devices(int argc, char *argv[])
{
  usbguard::IPCClient ipc(/*connected=*/true);

  for (auto device : ipc.listDevices()) {
    std::cout << device << std::endl;
  }

  return EXIT_SUCCESS;
}
