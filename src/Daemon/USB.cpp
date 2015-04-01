#include "USB.hpp"
#include "ByteOrder.hpp"
#include "Common/Utility.hpp"
#include <stdexcept>

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

  const USBDeviceDescriptor USBParseDeviceDescriptor(const void *data, size_t size, size_t *real_size)
  {
    USBDeviceDescriptor descriptor;
    const USBDeviceDescriptor* rawptr;

    if (size < sizeof(USBDeviceDescriptor)) {
      throw std::runtime_error("Invalid binary descriptor data: too small");
    }
    else {
      rawptr = static_cast<const USBDeviceDescriptor *>(data);
    }

    /* 1:1 copy */
    descriptor = *rawptr;
    /* Convert multibyte field to host endianness */
    descriptor.bcdUSB = busEndianToHost(rawptr->bcdUSB);
    descriptor.idVendor = busEndianToHost(rawptr->idVendor);
    descriptor.idProduct = busEndianToHost(rawptr->idProduct);
    descriptor.bcdDevice = busEndianToHost(rawptr->bcdDevice);

    /* Sanity checks */
    if (descriptor.bLength != sizeof(USBDeviceDescriptor)) {
      throw std::runtime_error("Invalid binary descriptor data: invalid bLenght value");
    }

    return std::move(descriptor);
  }

  const USBConfigurationDescriptor USBParseConfigurationDescriptor(const void *data, size_t size, size_t *real_size)
  {
    USBConfigurationDescriptor descriptor;
    const USBConfigurationDescriptor* rawptr;

    if (size < sizeof(USBConfigurationDescriptor)) {
      throw std::runtime_error("Invalid binary descriptor data: too small");
    }
    else {
      rawptr = static_cast<const USBConfigurationDescriptor *>(data);
    }

    /* 1:1 copy */
    descriptor = *rawptr;
    descriptor.wTotalLength = busEndianToHost(rawptr->wTotalLength);

    return std::move(descriptor);
  }

  const USBInterfaceDescriptor USBParseInterfaceDescriptor(const void *data, size_t size, size_t *real_size)
  {
    USBInterfaceDescriptor descriptor;
    const USBInterfaceDescriptor* rawptr;

    if (size < sizeof(USBInterfaceDescriptor)) {
      throw std::runtime_error("Invalid binary descriptor data: too small");
    }
    else {
      rawptr = static_cast<const USBInterfaceDescriptor *>(data);
    }

    /* 1:1 copy */
    descriptor = *rawptr;

    return std::move(descriptor);
  }

} /* namespace usbguard */
