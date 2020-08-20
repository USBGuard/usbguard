//
// Copyright (C) 2016 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-allow-device.hpp"
#include "usbguard/RuleParser.hpp"
#include "Common/Utility.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>

namespace usbguard
{
  static const char* options_short = "hp";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "permanent", no_argument, nullptr, 'p' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " allow-device [OPTIONS] (<device-id> | <rule>)" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -p, --permanent  Make the decision permanent. A device specific allow" << std::endl;
    stream << "                   rule will be appended to or updated in the current policy." << std::endl;
    stream << "  -h, --help       Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_allow_device(int argc, char* argv[])
  {
    uint32_t id = 0;
    bool permanent = false;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'p':
        permanent = true;
        break;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    argc -= optind;
    argv += optind;
    usbguard::IPCClient ipc(/*connected=*/true);

    if (argc == 0) {
      showHelp(std::cerr);
      return EXIT_FAILURE;
    }
    else if (argc == 1) { /* Allow device by ID */
      id = std::stoul(argv[0]);
      ipc.applyDevicePolicy(id, Rule::Target::Allow, permanent);
    }
    else { /* Allow device by Rule */
      //Create a string containing each argument(rule)
      std::vector<std::string> arguments(argv, argv + argc);
      std::string rule_string = joinElements(arguments.begin(), arguments.end());
      usbguard::Rule rule;

      try {
        rule = Rule::fromString(rule_string);
      }
      catch (const usbguard::RuleParserError& ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        showHelp(std::cerr);
        return EXIT_FAILURE;
      }

      for (auto rule_device : ipc.listDevices(argv[0])) {
        if (rule.appliesTo(rule_device)) {
          id = rule_device.getRuleID();
          try {
            ipc.applyDevicePolicy(id, Rule::Target::Allow, permanent);
          }
          catch (const usbguard::Exception& ex) {}
        }
      }
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
