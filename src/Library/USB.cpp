//
// Copyright (C) 2015 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#include "USB.hpp"
#include "Common/ByteOrder.hpp"
#include "Common/Utility.hpp"
#include "LoggerPrivate.hpp"
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

    logger->trace("Parsing device descriptor from ptr={:p}, size={}, real_size_ptr={:p}",
		  data, size, (void *)real_size);

    if (size < sizeof(USBDeviceDescriptor)) {
      logger->debug("Provided buffer doesn't have a sufficient size to hold a device descriptor");
      logger->debug("Expected size is at least {} bytes", sizeof(USBDeviceDescriptor));
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

    logger->debug("Device descriptor values (host endian):");
    logger->debug(" bcdUSB: {:x}", descriptor.bcdUSB);
    logger->debug(" idVendor: {:x}", descriptor.idVendor);
    logger->debug(" idProduct: {:x}", descriptor.idProduct);
    logger->debug(" bcdDevice: {:x}", descriptor.bcdDevice);
    logger->debug(" bDeviceClass: {:x}", descriptor.bDeviceClass);
    logger->debug(" bDeviceSubClass: {:x}", descriptor.bDeviceSubClass);

    /* Sanity checks */
    if (descriptor.bLength != sizeof(USBDeviceDescriptor)) {
      logger->debug("Device descriptor bLength value ({:d} bytes) doesn't match descriptor size ({})",
		    descriptor.bLength, sizeof(USBDeviceDescriptor));
      throw std::runtime_error("Invalid binary descriptor data: invalid bLenght value");
    }

    return std::move(descriptor);
  }

  const USBConfigurationDescriptor USBParseConfigurationDescriptor(const void *data, size_t size, size_t *real_size)
  {
    USBConfigurationDescriptor descriptor;
    const USBConfigurationDescriptor* rawptr;

    logger->trace("Parsing configuration descriptor from ptr={:p}, size={}, real_size_ptr={:p}",
		  data, size, (void*)real_size);

    if (size < sizeof(USBConfigurationDescriptor)) {
      logger->debug("Provided buffer doesn't have a sufficient size to hold a configuration descriptor");
      logger->debug("Expected size is at least {} bytes", sizeof(USBConfigurationDescriptor));
      throw std::runtime_error("Invalid binary descriptor data: too small");
    }
    else {
      rawptr = static_cast<const USBConfigurationDescriptor *>(data);
    }

    /* 1:1 copy */
    descriptor = *rawptr;
    descriptor.wTotalLength = busEndianToHost(rawptr->wTotalLength);

    logger->debug("Configuration descriptor data (host endian)");
    logger->debug(" bNumInterfaces: {:d}", descriptor.bNumInterfaces);
    logger->debug(" wTotalLength: {:d}", descriptor.wTotalLength);
    logger->debug(" bConfigurationValue: {:d}", descriptor.bConfigurationValue);

    return std::move(descriptor);
  }

  const USBInterfaceDescriptor USBParseInterfaceDescriptor(const void *data, size_t size, size_t *real_size)
  {
    USBInterfaceDescriptor descriptor;
    const USBInterfaceDescriptor* rawptr;

    logger->trace("Parsing interface descriptor from ptr={:p}, size={}, real_size_ptr={:p}",
		  data, size, (void *)real_size);

    if (size < sizeof(USBInterfaceDescriptor)) {
      logger->debug("Provided buffer doesn't have a sufficient size to hold an interface descriptor");
      logger->debug("Expected size is at least {} bytes", sizeof(USBInterfaceDescriptor));
      throw std::runtime_error("Invalid binary descriptor data: too small");
    }
    else {
      rawptr = static_cast<const USBInterfaceDescriptor *>(data);
    }

    /* 1:1 copy */
    descriptor = *rawptr;

    logger->debug("Interface descriptor data (host endian):");
    logger->debug(" bInterfaceClass: {:x}", descriptor.bInterfaceClass);
    logger->debug(" bInterfaceSubClass: {:x}", descriptor.bInterfaceSubClass);
    logger->debug(" bInterfaceProtocol: {:x}", descriptor.bInterfaceProtocol);
    logger->debug(" bInterfaceNumber: {:d}", descriptor.bInterfaceNumber);

    return std::move(descriptor);
  }

} /* namespace usbguard */
