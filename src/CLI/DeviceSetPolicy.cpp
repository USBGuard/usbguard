#include "DeviceSetPolicy.hpp"
#include <iostream>

namespace usbguard
{
  DeviceSetPolicy::DeviceSetPolicy(const Options& options)
  {
    _options = options;
  }

  void DeviceSetPolicy::execute(Interface& usbguard)
  {
    std::cout << "seqn=" << _options.seqn << std::endl;
    std::cout << "timeout=" << _options.timeout << std::endl;
    std::cout << "permanent=" << _options.permanent << std::endl;
  }
} /* namespace usbguard */
