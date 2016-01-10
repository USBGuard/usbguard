#include "usbguard.hpp"
#include "usbguard-list-rules.hpp"

#include <IPCSignalWatcher.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
  };

  int usbguard_watch(int argc, char *argv[])
  {
    IPCSignalWatcher watcher;

    watcher.connect();
    watcher.wait();

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
