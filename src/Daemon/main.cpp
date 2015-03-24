#include "Common/Logging.hpp"
#include "Common/Utility.hpp"
#include "Typedefs.hpp"
#include "Daemon.hpp"

#include <iostream>
#include <getopt.h>
#include <sodium.h>

using namespace usbguard;

const char * const G_optstring = "dskl:p:c:h";

void printUsage(std::ostream& stream, const char *arg0)
{
  stream << std::endl;
  stream << "Usage: " << filenameFromPath(String(arg0), true) << " [OPTIONS]" << std::endl;
  stream << std::endl;
  stream << "   -d        \t Enabled debugging messages in the log." << std::endl;
  stream << "   -s        \t Log to syslog." << std::endl;
  stream << "   -k        \t Log to console." << std::endl;
  stream << "   -l <path> \t Log to a file at `path'." << std::endl;
  stream << "   -p <path> \t Write PID to a file at `path'." << std::endl;
  stream << "   -c <path> \t Load configuration from a file at `path'." << std::endl;
  stream << "             \t (default: /etc/usbguard/usbguard-daemon.conf)" << std::endl;
  stream << "   -h        \t Show this usage screen." << std::endl;
  stream << std::endl;
}

int main(int argc, char *argv[])
{
  const char * const arg0 = argv[0];
  bool debug_mode = false;
  bool log_syslog = false;
  bool log_console = false;
  String log_file;
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
	log_file = String(optarg);
	break;
      case 'p':
	pid_file = String(optarg);
	break;
      case 'c':
	conf_file = String(optarg);
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

  usbguard::setupLogger(debug_mode,
		     log_syslog,
		     log_console,
		     log_file);

  if (sodium_init() == -1) {
    usbguard::log->error("Cannot initialize the sodium library");
    return EXIT_FAILURE;
  }

  int ret = EXIT_SUCCESS;

  try {
    usbguard::Daemon daemon;
    if (!conf_file.empty()) {
      daemon.loadConfiguration(conf_file);
    }
    daemon.run();
  } catch(const std::exception& ex) {
    usbguard::log->critical("Exception: {}", ex.what());
    ret = EXIT_FAILURE;
  }

  return ret;
}
