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
#include "usbguard-append-rule.hpp"

#include "usbguard/IPCClient.hpp"

#include <iostream>

namespace usbguard
{
  static const char* options_short = "ha:t";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "after", required_argument, nullptr, 'a' },
    { "temporary", no_argument, nullptr, 't' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " append-rule [OPTIONS] <rule>" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -a, --after <id>  Append the new rule after a rule with the specified id" << std::endl;
    stream << "                    instead of appending it at the end of the rule set." << std::endl;
    stream << "  -t, --temporary   Make the decision temporary. The rule policy file will not" << std::endl;
    stream << "                    be updated." << std::endl;
    stream << "  -h, --help        Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_append_rule(int argc, char* argv[])
  {
    uint32_t parent_id = usbguard::Rule::LastID;
    bool permanent = true;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'a':
        parent_id = std::stoul(optarg);
        break;

      case 't':
        permanent = false;
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
    const uint32_t id = ipc.appendRule(rule_spec, parent_id, permanent);
    std::cout << id << std::endl;
    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
