#include <map>

#include "usbguard.hpp"
#include "usbguard-device-list.hpp"

std::map<const char *,int(*)(int, char**)> cmd_handler_map = {
  { "device-list", &usbguard_device_list }
};

const char *usbguard_arg0 = nullptr;

static void showTopLevelHelp()
{
}

int main(int argc, char *argv[])
{
  usbguard_arg0 = argv[0];

  if (argc == 1) {
    showTopLevelHelp();
    return EXIT_SUCCESS;
  }

  const char * subcommand_key = argv[1];
  const auto iterator = cmd_handler_map.find(subcommand_key);

  /* Not such key in the map, show list of commands */
  if (iterator == cmd_handler_map.end()) {
    showTopLevelHelp();
    return EXIT_FAILURE;
  }

  auto subcommand = iterator->second;
  return subcommand(argc - 2, argv + 2);
}
