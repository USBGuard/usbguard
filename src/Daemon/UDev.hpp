#ifndef USBFW_UDEV_HPP
#define USBFW_UDEV_HPP

#include "USBFirewall.hpp"
#include <libudev.h>

namespace usbguard
{

  Firewall::Rule UDevDeviceToDeviceRule(struct udev_device *dev);
  std::string UDevDeviceHash(struct udev_device *dev);

} /* namespace usbguard */

#endif /* USBFW_UDEV_HPP */
