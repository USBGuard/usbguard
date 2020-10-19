//
// Copyright (C) 2017 Red Hat, Inc.
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
#include "usbguard-remove-user.hpp"

#include "usbguard/USBGuard.hpp"
#include "usbguard/IPCServer.hpp"

#include <iostream>
#include <unistd.h>

namespace usbguard
{
  static const char* options_short = "hug";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { "user", no_argument, nullptr, 'u' },
    { "group", no_argument, nullptr, 'g' },
    { nullptr, 0, nullptr, 0  }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " remove-user [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -u, --user           The specified name represents a username or UID (default)." << std::endl;
    stream << "  -g, --group          The specified name represents a groupname or GID." << std::endl;
    stream << "  -h, --help           Show this help." << std::endl;
    stream << std::endl;
  }

  static void removeIPCAccessControlFile(const std::string& path, const std::string& name, bool is_group)
  {
    IPCServer::checkAccessControlName(name);
    const std::string basename = getIPCAccessControlFileBasename(name, is_group);
    const std::string separator = (path.at(path.size() - 1) == '/' ? "" : "/");
    const std::string filepath = path + separator + basename;

    if (unlink(filepath.c_str()) != 0) {
      throw ErrnoException("removeIPCAccessControlFile", filepath, errno);
    }
  }

  int usbguard_remove_user(int argc, char* argv[])
  {
    int opt = 0;
    bool opt_is_group = false;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'u':
        opt_is_group = false;
        break;

      case 'g':
        opt_is_group = true;
        break;

      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    argv += optind;
    argc -= optind;

    if (argc != 1) {
      showHelp(std::cerr);
      return EXIT_FAILURE;
    }

    if (!(getuid() == 0 && geteuid() == 0)) {
      USBGUARD_LOG(Error) << "This subcommand requires root privileges. Please retry as root.";
      return EXIT_FAILURE;
    }

    const std::string name(argv[0]);
    const std::string path(getIPCAccessControlFilesPath());
    removeIPCAccessControlFile(path, name, opt_is_group);
    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
