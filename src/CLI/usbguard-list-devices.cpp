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
#include "usbguard-list-devices.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>

namespace usbguard
{
  static const char* options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "blocked", no_argument, nullptr, 'b' },
    { "allowed", no_argument, nullptr, 'a' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " list-devices [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -a, --allowed  List allowed devices." << std::endl;
    stream << "  -b, --blocked  List blocked devices." << std::endl;
    stream << "  -h, --help     Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_list_devices(int argc, char* argv[])
  {
    bool list_blocked = false;
    bool list_allowed = false;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'a':
        list_allowed = true;
        break;

      case 'b':
        list_blocked = true;
        break;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    const bool list_everything = (list_blocked == list_allowed);
    std::string query = "match";

    if (!list_everything) {
      if (list_allowed) {
        query = "allow";
      }
      else {
        query = "block";
      }
    }

    usbguard::IPCClient ipc(/*connected=*/true);

    for (auto device_rule : ipc.listDevices(query)) {
      std::cout << device_rule.getRuleID() << ": " << device_rule.toString() << std::endl;
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
