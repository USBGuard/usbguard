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
    stream << " Usage: " << usbguard_arg0 << " allow-device [OPTIONS] <device-id>" << std::endl;
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

    if (argc != 1) {
      showHelp(std::cerr);
      return EXIT_FAILURE;
    }

    id = std::stoul(argv[0]);
    usbguard::IPCClient ipc(/*connected=*/true);
    ipc.applyDevicePolicy(id, Rule::Target::Allow, permanent);
    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
