//
// Copyright (C) 2015 Red Hat, Inc.
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
#include "usbguard-generate-policy.hpp"
#include "PolicyGenerator.hpp"
#include "Common/Utility.hpp"

#include "usbguard/DeviceManager.hpp"

#include <iostream>
#include <unistd.h>

namespace usbguard
{
  static const char* options_short = "hpPt:HX";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "with-ports", no_argument, nullptr, 'p' },
    { "no-ports-sn", no_argument, nullptr, 'P' },
    { "target", required_argument, nullptr, 't' },
    { "hash-only", no_argument, nullptr, 'H' },
    { "no-hashes", no_argument, nullptr, 'X' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " generate-policy [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -p, --with-ports   Generate port specific rules for all devices." << std::endl;
    stream << "  -P, --no-ports-sn  Don't generate port specific rule for devices" << std::endl;
    stream << "                     without an iSerial value." << std::endl;
    stream << "  -t, --target <T>   Generate an explicit \"catch all\" rule with the" << std::endl;
    stream << "                     specified target. Possible targets: allow, block, reject." << std::endl;
    stream << "  -X, --no-hashes    Don't generate a hash attribute for each device." << std::endl;
    stream << "  -H, --hash-only    Generate a hash-only policy." << std::endl;
    stream << "  -h, --help         Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_generate_policy(int argc, char** argv)
  {
    bool port_specific = false;
    bool port_specific_noserial = true;
    bool with_catchall = false;
    std::string catchall_target = "block";
    bool with_hashes = true;
    bool only_hashes = false;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'p':
        port_specific = true;
        break;

      case 'P':
        port_specific_noserial = false;
        break;

      case 't':
        with_catchall = true;
        catchall_target = optarg;
        break;

      case 'H':
        only_hashes = true;
        break;

      case 'X':
        with_hashes = false;
        break;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    PolicyGenerator generator;
    generator.setWithHashAttribute(with_hashes);
    generator.setHashOnly(only_hashes);
    generator.setPortSpecificRules(port_specific);
    generator.setPortSpecificNoSerialRules(port_specific_noserial);
    generator.setExplicitCatchAllRule(with_catchall,
      Rule::targetFromString(catchall_target));
    generator.generate();

    auto ruleset = generator.refRuleSet();
    ruleset->save(std::cout);

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
