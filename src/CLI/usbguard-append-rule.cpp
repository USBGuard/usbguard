#include "usbguard.hpp"
#include "usbguard-append-rule.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "ha:";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "after", required_argument, nullptr, 'a' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " append-rule [OPTIONS] <rule>" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -a, --after <id>  Append the new rule after a rule with the specified id" << std::endl;
    stream << "                    instead of appending it at the end of the rule set." << std::endl;
    stream << "  -h, --help        Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_append_rule(int argc, char *argv[])
  {
    uint32_t parent_id = usbguard::Rule::LastID;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch(opt) {
        case 'h':
          showHelp(std::cout);
          return EXIT_SUCCESS;
        case 'a':
          parent_id = std::stoul(optarg);
          break;
        case '?':
          showHelp(std::cerr);
        default:
          return EXIT_FAILURE;
      }
    }

    argc -= optind;
    argv += optind;

    if (argc != 1) {
      showHelp(std::cerr);
      return EXIT_FAILURE;
    }

    usbguard::IPCClient ipc(/*connected=*/true);

    const std::string rule_spec = argv[0];
    const uint32_t id = ipc.appendRule(rule_spec, parent_id, 0);

    std::cout << id << std::endl;

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
