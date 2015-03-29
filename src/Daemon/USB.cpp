#include "USB.hpp"
#include "Common/Utility.hpp"

namespace usbguard {
  USBInterfaceType::USBInterfaceType()
  {
    bType = 0;
    return;
  }

  bool USBInterfaceType::operator==(const USBInterfaceType& rhs) const
  {
    return (bType == rhs.bType);
  }

  bool USBInterfaceType::sameClass(const USBInterfaceType& rhs) const
  {
    return (bFields.bClass == rhs.bFields.bClass);
  }

  bool USBInterfaceType::sameSubClass(const USBInterfaceType& rhs) const
  {
    return (bFields.bClass == rhs.bFields.bClass &&
	    bFields.bSubClass == rhs.bFields.bSubClass);
  }

  const String USBInterfaceTypeString(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol)
  {
    return (numberToString(bClass, "", 16) + ":" +
	    numberToString(bSubClass, "", 16) + ":" +
	    numberToString(bProtocol, "", 16));
  }

} /* namespace usbguard */













