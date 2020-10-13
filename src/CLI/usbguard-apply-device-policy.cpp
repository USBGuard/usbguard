//
// Copyright (C) 2020 Red Hat, Inc.
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
// Authors: Attila Lakatos <alakatos@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-apply-device-policy.hpp"
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

  static void showHelp(std::ostream& stream, Rule::Target target)
  {
    std::string target_string = Rule::targetToString(target);
    stream << " Usage: " << usbguard_arg0 << " " << target_string << "-device [OPTIONS] (<device-id> | <rule>)" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -p, --permanent  Make the decision permanent. A device specific " << target_string << std::endl;
    stream << "                   rule will be appended to or updated in the current policy." << std::endl;
    stream << "  -h, --help       Show this help." << std::endl;
    stream << std::endl;
  }

  static bool isNumeric(const std::string& s)
  {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
  }

  int usbguard_apply_device_policy(int argc, char** argv, Rule::Target target)
  {
    uint32_t id = 0;
    bool permanent = false;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout, target);
        return EXIT_SUCCESS;

      case 'p':
        permanent = true;
        break;

      case '?':
        showHelp(std::cerr, target);

      default:
        return EXIT_FAILURE;
      }
    }

    argc -= optind;
    argv += optind;
    usbguard::IPCClient ipc(/*connected=*/true);

    if (argc == 0) {
      showHelp(std::cerr, target);
      return EXIT_FAILURE;
    }
    else if (argc == 1 && isNumeric(std::string(argv[0]))) { /* Change device policy by ID */
      id = std::stoul(argv[0]);
      ipc.applyDevicePolicy(id, target, permanent);
    }
    else { /* Change device policy by Rule */
      std::string rule_string;
      if (argc == 1)
        rule_string = argv[0];
      else {
        std::vector<std::string> arguments(argv, argv + argc);
        rule_string = joinElements(arguments.begin(), arguments.end());
      }

      usbguard::Rule rule;
      try {
        rule = Rule::fromString(rule_string);
      }
      catch (const usbguard::RuleParserError& ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        showHelp(std::cerr, target);
        return EXIT_FAILURE;
      }

      std::string rule_target = rule_string.substr(0, rule_string.find(" "));
      for (auto rule_device : ipc.listDevices(rule_target)) {
        if (rule.appliesTo(rule_device)) {
          id = rule_device.getRuleID();
          try {
            ipc.applyDevicePolicy(id, target, permanent);
          }
          catch (const usbguard::Exception& ex) {}
        }
      }
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
