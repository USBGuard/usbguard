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
#include "usbguard-watch.hpp"

#include <IPCSignalWatcher.hpp>
#include <iostream>
#include <unistd.h>

namespace usbguard
{
  static const char* options_short = "wohe:";

  static const struct ::option options_long[] = {
    { "wait", no_argument, nullptr, 'w' },
    { "once", no_argument, nullptr, 'o' },
    { "help", no_argument, nullptr, 'h' },
    { "exec", required_argument, nullptr, 'e' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " watch [OPTIONS]" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -w, --wait         Wait for IPC connection to become available." << std::endl;
    stream << "  -o, --once         Wait only when starting, if needed. Exit when the connection is lost." << std::endl;
    stream << "  -e, --exec <path>  Run an executable file located at <path> for every event. Pass event" << std::endl;
    stream << "                     data to the process via environment variables." << std::endl;
    stream << "  -h, --help         Show this help." << std::endl;
    stream << std::endl;
  }

  int usbguard_watch(int argc, char* argv[])
  {
    int opt = 0;
    bool do_wait = false;
    bool wait_once = false;
    std::string exec_path;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'w':
        do_wait = true;
        break;

      case 'o':
        wait_once = do_wait = true;
        break;

      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case 'e':
        exec_path = std::string(optarg);
        break;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    IPCSignalWatcher watcher;

    if (!exec_path.empty()) {
      watcher.setExecutable(exec_path);
    }

    bool connect_waiting = false;
    std::string connect_last_exception;

    while (true) {
      try {
        watcher.connect();
        connect_waiting = false;

        if (wait_once) {
          do_wait = false;
        }

        watcher.wait();
      }
      catch (const Exception& ex) {
        /*
         * Re-throw if we won't be waiting for the connection
         * to become available.
         */
        if (!do_wait) {
          throw;
        }

        /*
         * Save the exception message. If the message changes, we'll
         * write an updated waiting message to the output.
         */
        const std::string connect_exception = ex.message();

        /*
         * When transitioning to waiting state or on exception message
         * updates, write a waiting status message to the output.
         */
        if (!connect_waiting || connect_last_exception != connect_exception) {
          std::cerr << "Waiting. IPC not available: " << connect_exception << std::endl;
          connect_waiting = true;
          connect_last_exception = connect_exception;
        }

        /*
         * Wait for some time...
         */
        sleep(1);
      }
    }

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
