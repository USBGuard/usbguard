#include "USB.hpp"
#include "Common/ByteOrder.hpp"
#include "Common/Utility.hpp"
#include <stdexcept>
#include <iostream>

namespace usbguard {
  USBInterfaceType::USBInterfaceType()
  {
    _bClass = 0;
    _bSubClass = 0;
    _bProtocol = 0;
    _mask = 0;
    return;
  }

  USBInterfaceType::USBInterfaceType(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask)
  {
    _bClass = bClass;
    _bSubClass = bSubClass;
    _bProtocol = bProtocol;
    _mask = mask;

    return;
  }

  USBInterfaceType::USBInterfaceType(const USBInterfaceDescriptor& descriptor, uint8_t mask)
  {
    _bClass = descriptor.bInterfaceClass;
    _bSubClass = descriptor.bInterfaceSubClass;
    _bProtocol = descriptor.bInterfaceProtocol;
    _mask = mask;
    return;
  }

  USBInterfaceType::USBInterfaceType(const std::string& type_string)
  {
    std::vector<std::string> tokens;
    tokenizeString(type_string, tokens, ":", /*trim_empty=*/false);

    _bClass = 0;
    _bSubClass = 0;
    _bProtocol = 0;
    _mask = 0;

    if (tokens.size() != 3) {
      throw std::runtime_error("Invalid type_string");
    }

    if (tokens[0].size() != 2) {
      throw std::runtime_error("Invalid type_string");
    }
    else {
      _bClass = stringToNumber<uint8_t>(tokens[0], 16);
      _mask |= MatchClass;
    }

    if (tokens[1] != "*") {
      if (tokens[1].size() != 2) {
	throw std::runtime_error("Invalid type_string");
      }
      else {
	_bSubClass = stringToNumber<uint8_t>(tokens[1], 16);
	_mask |= MatchSubClass;
      }
    }

    if (tokens[2] != "*") {
      if (tokens[2].size() != 2) {
	throw std::runtime_error("Invalid type_string");
      }
      else {
	_bProtocol = stringToNumber<uint8_t>(tokens[2], 16);
	_mask |= MatchProtocol;
      }
    }

    if (!(_mask == (MatchAll) ||
	  _mask == (MatchClass|MatchSubClass) ||
	  _mask == (MatchClass))) {
      throw std::runtime_error("Invalid type_string");
    }

    return;
  }

  bool USBInterfaceType::operator==(const USBInterfaceType& rhs) const
  {
    return (_bClass == rhs._bClass &&
	    _bSubClass == rhs._bSubClass &&
	    _bProtocol == rhs._bProtocol &&
	    _mask == rhs._mask);
  }

  bool USBInterfaceType::appliesTo(const USBInterfaceType& rhs) const
  {
    if (_mask & MatchClass) {
      if (_bClass != rhs._bClass) {
	return false;
      }
    }
    if (_mask & MatchSubClass) {
      if (_bSubClass != rhs._bSubClass) {
	return false;
      }
    }
    if (_mask & MatchProtocol) {
      if (_bProtocol != rhs._bProtocol) {
	return false;
      }
    }
    return true;
  }

  const String USBInterfaceType::typeString() const
  {
    return USBInterfaceType::typeString(_bClass, _bSubClass, _bProtocol, _mask);
  }

  const String USBInterfaceType::typeString(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask)
  {
    String type_string("");

    if (mask & MatchClass) {
      type_string.append(numberToString(bClass, "", 16, 2, '0') + ":");

      if (mask & MatchSubClass) {
	type_string.append(numberToString(bSubClass, "", 16, 2, '0') + ":");

	if (mask & MatchProtocol) {
	  type_string.append(numberToString(bProtocol, "", 16, 2, '0'));
	}
	else {
	  type_string.append("*");
	}
      }
      else {
	type_string.append("*:*");
      }
    }
    else {
      throw std::runtime_error("BUG: cannot create type string, invalid mask");
    }

    return std::move(type_string);
  }

  template<>
  bool matches(const USBInterfaceType& a, const USBInterfaceType& b)
  {
    return a.appliesTo(b);
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
