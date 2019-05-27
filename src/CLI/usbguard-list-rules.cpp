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
#include "usbguard-list-rules.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>

namespace usbguard
{
  static const char* options_short = "hdl:";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "show-devices", no_argument, nullptr, 'd'},
    { "label", required_argument, nullptr, 'l' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " list-rules [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -d, --show-devices  Show all devices which are affected by the specific rule." << std::endl;
    stream << "  -h, --help          Show this help." << std::endl;
    stream << "  -l, --label <label> Only show rules having a specific label." << std::endl;
    stream << std::endl;
  }

  int usbguard_list_rules(int argc, char* argv[])
  {
    bool show_devices = false;
    int opt = 0;
    std::string label;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'd':
        show_devices = true;
        break;

      case 'l':
        label = optarg;
        break;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    usbguard::IPCClient ipc(/*connected=*/true);
    auto rules = ipc.listRules(label);

    for (auto rule : rules) {
      std::cout << rule.getRuleID() << ": " << rule.toString() << std::endl;

      if (show_devices) {
        for (auto device_rule : ipc.listDevices(rule.toString())) {
          std::cout << "\t"<< device_rule.getRuleID() << ": " << device_rule.toString() << std::endl;
        }
      }
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
