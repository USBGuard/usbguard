#include "usbguard.hpp"
#include "usbguard-append-rule.hpp"

#include <IPCClient.hpp>
#include <iostream>

namespace usbguard
{
  static const char *options_short = "hab";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
  };

  int usbguard_append_rule(int argc, char *argv[])
  {
    usbguard::IPCClient ipc(/*connected=*/true);

    const std::string rule_spec = argv[1];
    const uint32_t parent_seqn = usbguard::Rule::SeqnLast;
    const uint32_t seqn = ipc.appendRule(rule_spec, parent_seqn, 0);

    std::cout << seqn << std::endl;

    return EXIT_SUCCESS;
  }
} /* namespace usbguard */
