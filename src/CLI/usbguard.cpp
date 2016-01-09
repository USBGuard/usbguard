#include <build-config.h>
#include <map>

#include <Logger.hpp>
#include "usbguard.hpp"
#include "usbguard-list-devices.hpp"

namespace usbguard
{
  const char *usbguard_arg0 = nullptr;

  static const std::map<const std::string,int(*)(int, char**)> cmd_handler_map = {
    { "list-devices", &usbguard_list_devices }
  };

  static void showTopLevelHelp()
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
  return usbguard::usbguard_cli(argc, argv);
}
