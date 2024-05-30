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

#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"

#include <map>
#include <iostream>

#ifdef HAVE_GNU_BASENAME
  /* GNU version of basename(3) */
  #ifndef _GNU_SOURCE
    #define _GNU_SOURCE
  #endif
  #include <cstring>
#else
  /* POSIX version of basename(3) */
  #include <libgen.h>
#endif

#include "usbguard.hpp"
#include "usbguard-get-parameter.hpp"
#include "usbguard-set-parameter.hpp"
#include "usbguard-list-devices.hpp"
#include "usbguard-list-rules.hpp"
#include "usbguard-generate-policy.hpp"
#include "usbguard-allow-device.hpp"
#include "usbguard-block-device.hpp"
#include "usbguard-reject-device.hpp"
#include "usbguard-append-rule.hpp"
#include "usbguard-remove-rule.hpp"
#include "usbguard-watch.hpp"
#include "usbguard-read-descriptor.hpp"
#include "usbguard-add-user.hpp"
#include "usbguard-remove-user.hpp"
#include "usbguard-print-version.hpp"

namespace usbguard
{
  const char* usbguard_arg0 = nullptr;

  static const std::map<const std::string, int(*)(int, char**)> cmd_handler_map = {
    { "get-parameter", &usbguard_get_parameter },
    { "set-parameter", &usbguard_set_parameter },
    { "list-devices", &usbguard_list_devices },
    { "allow-device", &usbguard_allow_device },
    { "block-device", &usbguard_block_device },
    { "reject-device", &usbguard_reject_device },
    { "list-rules", &usbguard_list_rules },
    { "append-rule", &usbguard_append_rule },
    { "remove-rule", &usbguard_remove_rule },
    { "generate-policy", &usbguard_generate_policy },
    { "watch", &usbguard_watch },
    { "read-descriptor", &usbguard_read_descriptor },
    { "add-user", &usbguard_add_user },
    { "remove-user", &usbguard_remove_user },
    { "--version", &usbguard_print_version }
  };

  static void showTopLevelHelp(std::ostream& stream = std::cout)
  {
    stream << " Usage: " << usbguard_arg0
      << " [OPTIONS] <command> [COMMAND OPTIONS] ..." << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "" << std::endl;
    stream << " Commands:" << std::endl;
    stream << "  get-parameter <name>           Get the value of a runtime parameter." << std::endl;
    stream << "  set-parameter <name> <value>   Set the value of a runtime parameter." << std::endl;
    stream << "  list-devices                   List all USB devices recognized by the USBGuard daemon." << std::endl;
    stream << "  allow-device <id|rule|p-rule>  Authorize a device to interact with the system." << std::endl;
    stream << "  block-device <id|rule|p-rule>  Deauthorize a device." << std::endl;
    stream << "  reject-device <id|rule|p-rule> Deauthorize and remove a device from the system." << std::endl;
    stream << std::endl;
    stream << "  list-rules                     List the rule set (policy) used by the USBGuard daemon." << std::endl;
    stream << "  append-rule <rule>             Append a rule to the rule set." << std::endl;
    stream << "  remove-rule <id>               Remove a rule from the rule set." << std::endl;
    stream << std::endl;
    stream << "  generate-policy                Generate a rule set (policy) based on the connected USB devices." << std::endl;
    stream << "  watch                          Watch for IPC interface events and print them to stdout." << std::endl;
    stream << "  read-descriptor                Read a USB descriptor from a file and print it in human-readable form." <<
      std::endl;
    stream << std::endl;
    stream << "  add-user <name>                Add USBGuard IPC user/group (requires root privileges)" << std::endl;
    stream << "  remove-user <name>             Remove USBGuard IPC user/group (requires root privileges)" << std::endl;
    stream << std::endl;
  }

  static int usbguard_cli(int argc, char* argv[])
  {
    usbguard_arg0 = ::basename(argv[0]);

    if (argc == 1) {
      showTopLevelHelp();
      return EXIT_SUCCESS;
    }

    const char* subcommand_key = argv[1];
    const auto iterator = cmd_handler_map.find(subcommand_key);

    /* Not such key in the map, show list of commands */
    if (iterator == cmd_handler_map.cend()) {
      showTopLevelHelp();
      return EXIT_FAILURE;
    }

    auto subcommand = iterator->second;
    return subcommand(argc - 1, argv + 1);
  }
} /* namespace usbguard */

int main(int argc, char* argv[])
{
  try {
    return usbguard::usbguard_cli(argc, argv);
  }
  catch (const usbguard::IPCException& ex) {
    std::cerr << "IPC ERROR: request id=" << ex.messageID()
      << ": "  << ex.message() << std::endl;
  }
  catch (const usbguard::Exception& ex) {
    std::cerr << "ERROR: " << ex.message() << std::endl;
  }
  catch (const std::exception& ex) {
    std::cerr << "EXCEPTION: " << ex.what() << std::endl;
  }
  catch (...) {
    std::cerr << "BUG: Unknown exception" << std::endl;
  }

  return EXIT_FAILURE;
}

/* vim: set ts=2 sw=2 et */
