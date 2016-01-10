#include <build-config.h>
#include <map>
#include <iostream>
#include <Logger.hpp>
#include <IPC.hpp>

#include "usbguard.hpp"
#include "usbguard-list-devices.hpp"
#include "usbguard-list-rules.hpp"
#include "usbguard-generate-policy.hpp"
#include "usbguard-allow-device.hpp"
#include "usbguard-block-device.hpp"
#include "usbguard-reject-device.hpp"
#include "usbguard-append-rule.hpp"
#include "usbguard-remove-rule.hpp"
#include "usbguard-watch.hpp"

namespace usbguard
{
  const char *usbguard_arg0 = nullptr;

  static const std::map<const std::string,int(*)(int, char**)> cmd_handler_map = {
    { "list-devices", &usbguard_list_devices },
    { "allow-device", &usbguard_allow_device },
    { "block-device", &usbguard_block_device },
    { "reject-device", &usbguard_reject_device },
    { "list-rules", &usbguard_list_rules },
    { "append-rule", &usbguard_append_rule },
    { "remove-rule", &usbguard_remove_rule },
    { "generate-policy", &usbguard_generate_policy },
    { "watch", &usbguard_watch }
  };

  static void showTopLevelHelp(std::ostream& stream = std::cout)
  {
  }

  static int usbguard_cli(int argc, char *argv[])
  {
    usbguard_arg0 = argv[0];

    Logger::setVerbosityLevel(Logger::Debug);
    Logger::setConsoleOutput(true);

    if (argc == 1) {
      showTopLevelHelp();
      return EXIT_SUCCESS;
    }

    const char * subcommand_key = argv[1];
    const auto iterator = cmd_handler_map.find(subcommand_key);

    /* Not such key in the map, show list of commands */
    if (iterator == cmd_handler_map.cend()) {
      logger->error("Unknown subcommand: {}", subcommand_key);
      showTopLevelHelp();
      return EXIT_FAILURE;
    }

    logger->debug("Executing subcommand \"{}\"", subcommand_key);
    auto subcommand = iterator->second;
    return subcommand(argc - 1, argv + 1);
  }
} /* namespace usbguard */

int main(int argc, char *argv[])
{
  try {
    return usbguard::usbguard_cli(argc, argv);
  }
  catch(const usbguard::IPCException& ex) {
    std::cerr << "ERROR: ";

    switch(ex.code()) {
      case usbguard::IPCException::ConnectionError:
        std::cerr << "Unable to connect to the IPC socket! ";
        std::cerr << "The requested action cannot be completed without a running usbguard-daemon instance." << std::endl;
        std::cerr << std::endl;
        std::cerr << "Possible reasons:" << std::endl;
        std::cerr << " 1) usbguard-daemon is NOT running." << std::endl;
        std::cerr << " 2) You don't have permissions to use the usbguard IPC interface." << std::endl;
        std::cerr << "    Check/Set the IPCAllowedUsers and IPCAllowedGroups settings" << std::endl;
        std::cerr << "    of the usbguard-daemon component." << std::endl;
        std::cerr << std::endl;
        break;
      case usbguard::IPCException::ProtocolError:
        std::cerr << "IPC protocol violation! The CLI and the usbguard-daemon aren't compatible." << std::endl;
        break;
      case usbguard::IPCException::InvalidArgument:
        std::cerr << "Invalid argument.";
        break;
      case usbguard::IPCException::PermissionDenied:
        std::cerr << "Permission denied: the usbguard-daemon cannot perform the requested action." << std::endl;
        break;
      case usbguard::IPCException::TransientError:
        std::cerr << "Transient error. Please try again." << std::endl;
        break;
      case usbguard::IPCException::NotFound:
        std::cerr << "Not found: the requested resource WAS NOT found." << std::endl;
        break;
      default:
        std::cerr << "BUG: Unknown error code. Please file a bug report at: " << std::endl;
        std::cerr << std::endl;
        std::cerr << "    https://github.com/dkopecek/usbguard/issues." << std::endl;
        std::cerr << std::endl;
    }
  }
  catch(const std::exception& ex) {
    std::cerr << "EXCEPTION: " << ex.what() << std::endl;
  }
  catch(...) {

  }
  return EXIT_FAILURE;
}
