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
#include <algorithm>

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

    return type_string;
  }

  template<>
  bool matches(const USBInterfaceType& a, const USBInterfaceType& b)
  {
    return a.appliesTo(b);
  }

  void USBParseDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out)
  {
    const USBDeviceDescriptor* device_raw = reinterpret_cast<const USBDeviceDescriptor*>(descriptor_raw);
    USBDeviceDescriptor* device_out = reinterpret_cast<USBDeviceDescriptor*>(descriptor_out);

    /* Copy 1:1 */
    *device_out = *device_raw;

    /* Convert multibyte field to host endianness */
    device_out->bcdUSB = busEndianToHost(device_raw->bcdUSB);
    device_out->idVendor = busEndianToHost(device_raw->idVendor);
    device_out->idProduct = busEndianToHost(device_raw->idProduct);
    device_out->bcdDevice = busEndianToHost(device_raw->bcdDevice);

    return;
  }

  void USBParseConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out)
  {
    const USBConfigurationDescriptor* configuration_raw = reinterpret_cast<const USBConfigurationDescriptor*>(descriptor_raw);
    USBConfigurationDescriptor* configuration_out = reinterpret_cast<USBConfigurationDescriptor*>(descriptor_out);

    /* Copy 1:1 */
    *configuration_out = *configuration_raw;

    /* Convert multibyte field to host endianness */
    configuration_out->wTotalLength = busEndianToHost(configuration_raw->wTotalLength);

    return;
  }

  void USBParseInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out)
  {
    const USBInterfaceDescriptor* interface_raw = reinterpret_cast<const USBInterfaceDescriptor*>(descriptor_raw);
    USBInterfaceDescriptor* interface_out = reinterpret_cast<USBInterfaceDescriptor*>(descriptor_out);

    /* Copy 1:1 */
    *interface_out = *interface_raw;

    return;
  }

  void USBParseEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out)
  {
    const USBEndpointDescriptor* endpoint_raw = reinterpret_cast<const USBEndpointDescriptor*>(descriptor_raw);
    USBEndpointDescriptor* endpoint_out = reinterpret_cast<USBEndpointDescriptor*>(descriptor_out);

    *endpoint_out = *endpoint_raw;
    endpoint_out->wMaxPacketSize = busEndianToHost(endpoint_raw->wMaxPacketSize);

    return;
  }

  /*
  USBDescriptorParser::USBDescriptorParser()
  {
  }
  */

  const USBDescriptorParser::Handler* USBDescriptorParser::getDescriptorTypeHandler(uint8_t bDescriptorType) const
  {
    const auto it = _handler_map.find(bDescriptorType);
    if (it == _handler_map.end()) {
      return nullptr;
    }
    return &it->second;
  }

  size_t USBDescriptorParser::parse(std::istream& stream)
  {
    size_t size_processed = 0;

    while (stream.good()) {
      USBDescriptorHeader header;
      stream.read(reinterpret_cast<char*>(&header), sizeof header);

      if (stream.gcount() != sizeof header) {
        /*
         * If we read nothing and the stream if at EOF, just break
         * the loop and return normally. Checking the sanity of the
         * parsed descriptor data is up to the higher layers.
         */
        if (stream.gcount() == 0 && stream.eof()) {
          break;
        }
        /*
         * Otherwise throw an exception because there's unknown garbage
         * in the stream which cannot be a valid USB descriptor.
         */
        else {
          throw std::runtime_error("Cannot parse descriptor data: partial read while reading header data");
        }
      }

      /*
       * The bLength value has to be at least 2, because that is the size of the USB
       * descriptor header.
       */
      if (header.bLength < sizeof(USBDescriptorHeader)) {
        throw std::runtime_error("Invalid descriptor data: bLength is less than the size of the header");
      }

      /*
       * Let's try to read the rest of the descriptor data before we start looking
       * for the descriptor type handler. If there's not enough data in the stream,
       * then there's no point for searching for the handler.
       */
      USBDescriptor descriptor;

      descriptor.bHeader = header;
      memset(&descriptor.bDescriptorData, 0, sizeof descriptor.bDescriptorData);

      /*
       * We read (bLength - header_size) amount of data here because the bLength value
       * counts in the size of the header too and we already read it from the stream.
       */
      stream.read(reinterpret_cast<char*>(&descriptor.bDescriptorData), header.bLength - sizeof(USBDescriptorHeader));

      if (stream.gcount() != (std::streamsize)(header.bLength - sizeof(USBDescriptorHeader))) {
        throw std::runtime_error("Invalid descriptor data: bLength value larger than the amount of available data");
      }

      /*
       * Find the descriptor type handler.
       */
      const Handler* h = getDescriptorTypeHandler(header.bDescriptorType);
      if (!h) {
        h = getDescriptorTypeHandler(USB_DESCRIPTOR_TYPE_UNKNOWN);
        /*
         * If there's no handler for this type of descriptor, skip to the next one.
         */
        if (!h) {
          continue;
        }
      }

      /*
       * Check whether the descriptor is of expected length.
       */
      if (h->bLengthExpected > 0) {
        if (header.bLength != h->bLengthExpected) {
          throw std::runtime_error("Invalid descriptor data: bLength doesn't match expected bLenght for this descriptor type");
        }
      }

      USBDescriptor descriptor_parsed;
      descriptor_parsed.bHeader = header;
      memset(&descriptor_parsed.bDescriptorData, 0, sizeof descriptor_parsed.bDescriptorData);

      if (h->parser) {
        h->parser(this, &descriptor, &descriptor_parsed); 
      }
      if (h->callback) {
        h->callback(this, &descriptor_parsed);
      }

      setDescriptor(header.bDescriptorType, descriptor_parsed);
      size_processed += header.bLength;
    }

    return size_processed;
  }

  void USBDescriptorParser::setHandler(uint8_t bDescriptorType, uint8_t bLengthExpected, ParserFunction parser, CallbackFunction callback)
  {
    auto& handler = _handler_map[bDescriptorType];
    handler.parser = parser;
    handler.callback = callback;
    handler.bLengthExpected = bLengthExpected;
    return;
  }

  const USBDescriptor* USBDescriptorParser::getDescriptor(uint8_t bDescriptorType) const
  {
    auto const& it = _dstate_map.find(bDescriptorType);
    if (it == _dstate_map.end()) {
      return nullptr;
    }
    return &it->second;
  }

  void USBDescriptorParser::setDescriptor(uint8_t bDescriptorType, const USBDescriptor& descriptor)
  {
    _dstate_map[bDescriptorType] = descriptor;
    ++_count_map[bDescriptorType];
  }

  void USBDescriptorParser::delDescriptor(uint8_t bDescriptorType)
  {
    _dstate_map.erase(bDescriptorType);
  }

  bool USBDescriptorParser::haveDescriptor(uint8_t bDescriptorType) const
  {
    return _dstate_map.count(bDescriptorType) == 1;
  }

  const std::vector<std::pair<uint8_t,size_t>> USBDescriptorParser::getDescriptorCounts() const
  {
    std::vector<std::pair<uint8_t,size_t>> counts;

    for (auto const& kv : _count_map) {
      counts.push_back(std::make_pair(kv.first, kv.second));
    }

    std::sort(counts.begin(), counts.end());

    return counts;
  }

} /* namespace usbguard */
