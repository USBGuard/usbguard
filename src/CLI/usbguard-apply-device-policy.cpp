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
// Authors: Attila Lakatos <alakatos@redhat.com>, Zoltan Fridrich <zfridric@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-apply-device-policy.hpp"
#include "Common/Utility.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>
#include <list>

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
    stream << " Usage: " << usbguard_arg0 << " " << target_string
      << "-device [OPTIONS] (<id> | <rule> | <partial-rule>)" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -p, --permanent  Make the decision permanent. A device specific " << target_string << std::endl;
    stream << "                   rule will be appended to or updated in the current policy." << std::endl;
    stream << "  -h, --help       Show this help." << std::endl;
    stream << std::endl;
  }

  static bool isNumeric(const std::string& s)
  {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
      return !std::isdigit(c);
    }) == s.end();
  }

  int usbguard_apply_device_policy(int argc, char** argv, Rule::Target target)
  {
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

    if (argc == 0) {
      showHelp(std::cerr, target);
      return EXIT_FAILURE;
    }

    usbguard::IPCClient ipc(/*connected=*/true);

    /* If a single numeric argument is supplied interpret it as a rule ID */
    if (argc == 1 && isNumeric(std::string(argv[0]))) {
      uint32_t id = std::stoul(argv[0]);
      ipc.applyDevicePolicy(id, target, permanent);
      return EXIT_SUCCESS;
    }

    /*
     * Interpret arguments as a rule/partial-rule
     */
    std::string rule_string;

    if (argc == 1) {
      rule_string = argv[0];
    }
    else {
      std::vector<std::string> arguments(argv, argv + argc);
      rule_string = joinElements(arguments.begin(), arguments.end());
    }

    try { /* Check whether rule target has been supplied */
      std::string rule_target = rule_string.substr(0, rule_string.find(" "));
      Rule::targetFromString(rule_target);
    }
    catch (const std::runtime_error& ex) {
      /*
       * The rule contains no target => partial rule
       * Supply a default match target
       */
      rule_string.insert(0, Rule::targetToString(Rule::Target::Match) + " ");
    }

    for (auto device_rule : ipc.listDevices(rule_string)) {
      if (target != device_rule.getTarget()) {
        uint32_t id = device_rule.getRuleID();

        try {
          ipc.applyDevicePolicy(id, target, permanent);
        }
        catch (const usbguard::Exception& ex) {
          /*
           * When a parent device is blocked/rejected, all its child
           * devices are removed from the device map. If we try to apply
           * device policy to a device whose parent has been
           * blocked/rejected, therefore this device is not present in
           * the device map anymore, we will receive an exception.
           * We ignore such exceptions.
           */
        }
      }
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
