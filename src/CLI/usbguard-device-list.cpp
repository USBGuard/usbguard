#include "usbguard.hpp"

static const char *options_short = "hab";

static const struct ::option options_long[] = {
  { "help", no_argument, nullptr, 'h' },
  { "blocked", no_argument, nullptr, 'a' },
  { "allowed", no_argument, nullptr, 'b' }
};

int usbguard_device_list(int argc, char *argv[])
{
  
  return EXIT_FAILURE;
}
