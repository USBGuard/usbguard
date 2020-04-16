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
//          Radovan Sroka <rsroka@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-generate-policy.hpp"
#include "PolicyGenerator.hpp"
#include "Common/Utility.hpp"

#include "usbguard/DeviceManager.hpp"
#include "Common/LDAPUtil.hpp"

#include <iostream>
#include <unistd.h>

namespace usbguard
{
  static const char* options_short = "hpPd:t:HXLb:o:n:";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "with-ports", no_argument, nullptr, 'p' },
    { "no-ports-sn", no_argument, nullptr, 'P' },
    { "devpath", required_argument, nullptr, 'd' },
    { "target", required_argument, nullptr, 't' },
    { "hash-only", no_argument, nullptr, 'H' },
    { "no-hashes", no_argument, nullptr, 'X' },
    { "ldif", no_argument, nullptr, 'L' },
    { "usbguardbase", required_argument, nullptr, 'b' },
    { "objectclass", required_argument, nullptr, 'o' },
    { "name-prefix", required_argument, nullptr, 'n' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " generate-policy [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -p, --with-ports       Generate port specific rules for all devices." << std::endl;
    stream << "  -P, --no-ports-sn      Don't generate port specific rule for devices" << std::endl;
    stream << "                         without an iSerial value." << std::endl;
    stream << "  -d, --devpath <D>      Only generate a rule for the device at the specified" << std::endl;
    stream << "                         sub path of /sys." << std::endl;
    stream << "  -t, --target <T>       Generate an explicit \"catch all\" rule with the" << std::endl;
    stream << "                         specified target. Possible targets: allow, block, reject." << std::endl;
    stream << "  -X, --no-hashes        Don't generate a hash attribute for each device." << std::endl;
    stream << "  -H, --hash-only        Generate a hash-only policy." << std::endl;
    stream << "  -L, --ldif             Generate a ldif policy for LDAP." << std::endl;
    stream << "  -b, --usbguardbase <B> Generate a ldif policy for LDAP with this base" << std::endl;
    stream << "                         this option is required when --ldif was specified" << std::endl;
    stream << "  -o, --objectclass <O>  Generate a ldif policy for LDAP with this objectClass" << std::endl;
    stream << "  -n, --name-prefix <N>  Generate a ldif policy for LDAP with this name prefix" << std::endl;
    stream << "  -h, --help             Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_generate_policy(int argc, char** argv)
  {
    bool port_specific = false;
    bool port_specific_noserial = true;
    std::string devpath = "";
    bool with_catchall = false;
    std::string catchall_target = "block";
    bool with_hashes = true;
    bool only_hashes = false;
    bool ldif = false;
    bool with_base = false;
    std::string base = "";
    std::string objclass = "USBGuardPolicy";
    std::string name_prefix = "Rule";
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

      case 'd':
        devpath = optarg;
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

      case 'L':
        ldif = true;
        break;

      case 'b':
        with_base = true;
        base = optarg;
        break;

      case 'o':
        objclass = optarg;
        break;

      case 'n':
        name_prefix = optarg;
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
    generator.setDevpath(devpath);
    generator.setExplicitCatchAllRule(with_catchall,
      Rule::targetFromString(catchall_target));
    generator.generate();
    auto rulesets = generator.refRuleSet();
    char array[HOST_NAME_MAX];
    int rc = gethostname(array, HOST_NAME_MAX);
    std::string hostname;

    if (rc != -1) {
      hostname = array;
    }
    else {
      /*TODO maybe some option for setting hostname explicitly
      * without hostname LDAP query will not work!
      */
      std::cerr << "Cannot get hostname" << std::endl;
    }

    if (!ldif) {
      for (auto ruleset : rulesets) {
        ruleset->serialize(std::cout);
      }
    }
    else if (ldif && with_base) {
      std::map<std::string, std::string> values = {
        {"HOSTNAME", hostname},
        {"NAME_PREFIX", name_prefix},
        {"USBGUARD_BASE", base},
        {"OBJCLASS", objclass}
      };
      LDAPUtil::serializeLDIF(rulesets, std::cout, values);
    }
    else {
      std::cerr << "If you want to generate ldif you must specify USBGuardBase" << std::endl;
      std::cerr << "USBGuardBase is usually \"ou=USBGuard,\" followed by your LDAP base" << std::endl;
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
