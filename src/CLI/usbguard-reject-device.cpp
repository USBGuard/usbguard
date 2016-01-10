#include "usbguard.hpp"
#include "usbguard-reject-device.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
  };

  int usbguard_reject_device(int argc, char *argv[])
  {
    usbguard::IPCClient ipc(/*connected=*/true);

    const uint32_t seqn = std::stoul(argv[1]);
    const bool append = false;

    ipc.rejectDevice(seqn, append, 0);

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
