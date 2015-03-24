#ifndef USBFW_UDEV_HPP
#define USBFW_UDEV_HPP

#include "Rule.hpp"
#include <libudev.h>

namespace usbguard
{

  Rule UDevDeviceToDeviceRule(struct udev_device *dev);
  std::string UDevDeviceHash(struct udev_device *dev);

} /* namespace usbguard */

#endif /* USBFW_UDEV_HPP */
