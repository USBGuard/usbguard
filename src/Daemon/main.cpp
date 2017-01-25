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
#include <build-config.h>

#include <Typedefs.hpp>

#include "Logger.hpp"
#include "Daemon.hpp"
#include "Exception.hpp"
#include "Common/Utility.hpp"
#include "Seccomp.h"

#include <iostream>
#include <getopt.h>

#if defined(HAVE_LIBCAPNG)
# include <cap-ng.h>
static void setupCapabilities(void);
#endif

using namespace usbguard;

const char * const G_optstring = "dskl:p:c:hWC";

static void printUsage(std::ostream& stream, const char *arg0)
{
  stream << std::endl;
  stream << "Usage: " << filenameFromPath(String(arg0), true) << " [OPTIONS]" << std::endl;
  stream << std::endl;
  stream << "  -d         Enable debugging messages in the log." << std::endl;
  stream << "  -s         Log to syslog." << std::endl;
  stream << "  -k         Log to console." << std::endl;
  stream << "  -l <path>  Log to a file at `path'." << std::endl;
  stream << "  -p <path>  Write PID to a file at `path'." << std::endl;
  stream << "  -c <path>  Load configuration from a file at `path'." << std::endl;
  stream << "             (default: /etc/usbguard/usbguard-daemon.conf)" << std::endl;
  stream << "  -C         Drop capabilities to limit privileges of the process." << std::endl;
  stream << "  -W         Use a seccomp whitelist to limit available syscalls to the process." << std::endl;
  stream << "  -h         Show this usage screen." << std::endl;
  stream << std::endl;
}

int main(int argc, char *argv[])
{
  const char * const arg0 = argv[0];
  bool debug_mode = false;
  bool log_syslog = false;
  bool log_console = false;
  bool log_file = false;
  bool use_seccomp_whitelist = false;
  bool drop_capabilities = false;
  String log_file_path;
  String pid_file;
  String conf_file = "/etc/usbguard/usbguard-daemon.conf";
  int opt;

  while ((opt = getopt(argc, argv, G_optstring)) != -1) {
    switch(opt)
      {
      case 'd':
	debug_mode = true;
	break;
      case 's':
	log_syslog = true;
	break;
      case 'k':
	log_console = true;
	break;
      case 'l':
	log_file = true;
	log_file_path = String(optarg);
	break;
      case 'p':
	pid_file = String(optarg);
	break;
      case 'c':
	conf_file = String(optarg);
	break;
      case 'W':
	use_seccomp_whitelist = true;
	break;
      case 'C':
	drop_capabilities = true;
	break;
      case 'h':
	printUsage(std::cout, arg0);
	return EXIT_SUCCESS;
      case '?':
	printUsage(std::cerr, arg0);
	/* FALLTROUGH */
      default:
	return EXIT_FAILURE;
      }
  }

  /* Initialize logging */
  USBGUARD_LOGGER.setEnabled(true, (debug_mode ?
                                    LogStream::Level::Trace
                                    :
                                    LogStream::Level::Warning));

  USBGUARD_LOGGER.setOutputConsole(log_console);
  USBGUARD_LOGGER.setOutputSyslog(log_syslog, "usbguard-daemon");
  USBGUARD_LOGGER.setOutputFile(log_file, log_file_path);

  /* Setup seccomp whitelist & drop capabilities */
  if (use_seccomp_whitelist) {
    if (!setupSeccompWhitelist()) {
      return EXIT_FAILURE;
    }
  }

  if (drop_capabilities) {
#if defined(HAVE_LIBCAPNG)
    setupCapabilities();
#else
    return EXIT_FAILURE;
#endif
  }

  /* Start the daemon */
  int ret = EXIT_FAILURE;
  try {
    usbguard::Daemon daemon;
    if (!conf_file.empty()) {
      daemon.loadConfiguration(conf_file);
    }
    daemon.run();
    ret = EXIT_SUCCESS;
  }
  catch(const usbguard::Exception& ex) {
    USBGUARD_LOG(Error) << "ERROR: " << ex.message();
  }
  catch(const std::exception& ex) {
    USBGUARD_LOG(Error) << "EXCEPTION: " << ex.what();
  }
  catch(...) {
    USBGUARD_LOG(Error) << "EXCEPTION: Unknown excepton caught while starting the process";
  }

  return ret;
}

#if defined(HAVE_LIBCAPNG)
 static void setupCapabilities(void)
 {

   capng_clear(CAPNG_SELECT_BOTH);
   capng_updatev(CAPNG_ADD, (capng_type_t)(CAPNG_EFFECTIVE|CAPNG_PERMITTED),
		 CAP_CHOWN, CAP_FOWNER,-1);
   capng_apply(CAPNG_SELECT_BOTH);
   return;
 }
#endif
