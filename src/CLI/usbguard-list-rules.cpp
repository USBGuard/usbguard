#include "usbguard.hpp"
#include "usbguard-list-rules.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "h";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " list-rules [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -h, --help  Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_list_rules(int argc, char *argv[])
  {
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch(opt) {
        case 'h':
          showHelp(std::cout);
          return EXIT_SUCCESS;
        case '?':
          showHelp(std::cerr);
        default:
          return EXIT_FAILURE;
      }
    }

    usbguard::IPCClient ipc(/*connected=*/true);

    for (auto map_entry : ipc.listRules()) {
      std::cout << map_entry.first << ": " << map_entry.second << std::endl;
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
