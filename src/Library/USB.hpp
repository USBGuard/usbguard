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
#pragma once
#include "Predicates.hpp"

#include <Typedefs.hpp>
#include <cstdint>
#include <climits>
#include <unordered_map>
#include <functional>

namespace usbguard {
  /*
   * Maximum lenght of a string that is read from a USB descriptor
   * Since the string descriptors have an 8-bit length field, the
   * maximum lenght of a string stored in a string descriptor is
   * UINT8_MAX minus the size of the length field (1 byte) and the
   * size of the type field (1 byte).
   */
  const size_t USB_GENERIC_STRING_MAX_LENGTH = UINT8_MAX - 2;

  /* Maximum lenght of the vendor id in string form */
  const size_t USB_VID_STRING_MAX_LENGTH = 4;

  /* Maximum lenght of the product id in string form */
  const size_t USB_PID_STRING_MAX_LENGTH = 4;

  /* Maximum lenght of the port in string form */
  const size_t USB_PORT_STRING_MAX_LENGTH = PATH_MAX;

  /*
   * NOTE: The USB specification dictates that all multibyte data fields
   *       are in little-endian form. The structures defined bellow are
   *       used for platform-independed passing of the USB descriptor
   *       data to the Device class. The Device class assumes that the
   *       values are in host-specific endianness.
   */
  const uint8_t USB_DESCRIPTOR_TYPE_UNKNOWN = 0x00;
  const uint8_t USB_DESCRIPTOR_TYPE_DEVICE = 0x01;
  const uint8_t USB_DESCRIPTOR_TYPE_CONFIGURATION = 0x02;
  const uint8_t USB_DESCRIPTOR_TYPE_STRING = 0x03;
  const uint8_t USB_DESCRIPTOR_TYPE_INTERFACE = 0x04;
  const uint8_t USB_DESCRIPTOR_TYPE_ENDPOINT = 0x05;
  const uint8_t USB_DESCRIPTOR_TYPE_ASSOCIATION_INTERFACE = 0x0b;

  enum class USBDescriptorType : uint8_t {
    Device = 0x01,
    Configuration = 0x02,
    String = 0x03,
    Interface = 0x04,
    Endpoint = 0x05,
    AssociationInterface = 0x0b
  };

  struct DLL_PUBLIC USBDescriptorHeader
  {
    uint8_t bLength;
    uint8_t bDescriptorType;
  } __attribute__((packed));

  struct DLL_PUBLIC USBDescriptor
  {
    struct USBDescriptorHeader bHeader;
    uint8_t bDescriptorData[256-sizeof(USBDescriptorHeader)];
  } __attribute__((packed));

  struct DLL_PUBLIC USBDeviceDescriptor
  {
    struct USBDescriptorHeader bHeader;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
  } __attribute__((packed));

  struct DLL_PUBLIC USBConfigurationDescriptor
  {
    struct USBDescriptorHeader bHeader;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
  } __attribute__((packed));

  struct DLL_PUBLIC USBInterfaceDescriptor
  {
    struct USBDescriptorHeader bHeader; 
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
  } __attribute__((packed));

  struct USBEndpointDescriptor
  {
    struct USBDescriptorHeader bHeader;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
  } __attribute__((packed));

  class DLL_PUBLIC USBDeviceID
  {
  public:
    USBDeviceID();
    USBDeviceID(const String& vendor_id, const String& product_id = String());
    USBDeviceID(const USBDeviceID& rhs);

    static void checkDeviceID(const String& vendor_id, const String& product_id);

    void setVendorID(const String& vendor_id);
    void setProductID(const String& product_id);

    const String& getVendorID() const;
    const String& getProductID() const;

    String toRuleString() const;
    String toString() const;
    bool isSubsetOf(const USBDeviceID& rhs) const;

  private:
    String _vendor_id;
    String _product_id;
  };

  namespace Predicates DLL_PUBLIC
  {
    template<>
    bool isSubsetOf(const USBDeviceID& source, const USBDeviceID& target);
  }

  class DLL_PUBLIC USBInterfaceType
  {
  public:
    static const uint8_t MatchClass = 1<<0;
    static const uint8_t MatchSubClass = 1<<1;
    static const uint8_t MatchProtocol = 1<<2;
    static const uint8_t MatchAll = MatchClass|MatchSubClass|MatchProtocol;

    USBInterfaceType();
    USBInterfaceType(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask = MatchAll);
    USBInterfaceType(const USBInterfaceDescriptor& descriptor, uint8_t mask = MatchAll);
    USBInterfaceType(const std::string& type_string);

    bool operator==(const USBInterfaceType& rhs) const;
    bool appliesTo(const USBInterfaceType& rhs) const;

    const String typeString() const;
    const String toRuleString() const;
    static const String typeString(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask = MatchAll);

  private:
    uint8_t _bClass;
    uint8_t _bSubClass;
    uint8_t _bProtocol;
    uint8_t _mask;
  };

  namespace Predicates DLL_PUBLIC
  {
    template<>
    bool isSubsetOf(const USBInterfaceType& source, const USBInterfaceType& target);
  }

  class DLL_PUBLIC USBDescriptorParser
  {
  public:
    using ParserFunction = std::function<void(USBDescriptorParser *, const USBDescriptor *, USBDescriptor *)>;
    using CallbackFunction = std::function<void(USBDescriptorParser *, const USBDescriptor *)>;

    //USBDescriptorParser(); //size_t max_resident_descriptors = 1024*1024/256 /* 1MiB of descriptors */);

    /**
     * Initiate parsing of USB descriptors from an input stream.
     *
     * Returns number of bytes succesfully parsed/processed from
     * the stream.
     */
    size_t parse(std::istream& stream);

    /**
     * Sets handler functions (parser and callback) for specific USB descriptor types
     *
     * bDescriptorType: Type of the descriptor.
     * bLengthExpected: Expected length (in bytes/octets) of the descriptor. Set to 0 for variable length descriptors.
     *
     */
    void setHandler(uint8_t bDescriptorType, uint8_t bLengthExpected, ParserFunction parser, CallbackFunction callback);

    /**
     * Return a pointer to a USBDescriptor of type bDescriptorType that
     * is stored in the USB descriptor state. If there's no such descriptor,
     * then nullptr is returned.
     */
    const USBDescriptor* getDescriptor(uint8_t bDescriptorType) const;

    /**
     * Set the active instance of an USB descriptor of bDescriptorType type.
     */
    void setDescriptor(uint8_t bDescriptorType, const USBDescriptor& descriptor);

    /**
     * Delete the active instance of an USB descriptor of bDescriptorType type.
     */
    void delDescriptor(uint8_t bDescriptorType);

    /**
     * Returns true if the descriptor state contains a USB descriptor of type bDescriptorType.
     */
    bool haveDescriptor(uint8_t bDescriptorType) const;

    /**
     * Returns a vector of (bDescriptorType, count) pairs.
     */
    const std::vector<std::pair<uint8_t,size_t>> getDescriptorCounts() const;

  private:
    struct Handler
    {
      ParserFunction parser;
      CallbackFunction callback;
      uint8_t bLengthExpected;
    };

    const Handler* getDescriptorTypeHandler(uint8_t bDescriptorType) const;

    std::unordered_map<uint8_t, USBDescriptor> _dstate_map; /**< Descriptor State Map */
    std::unordered_map<uint8_t, Handler> _handler_map;
    std::unordered_map<uint8_t, size_t> _count_map;
 };

 void DLL_PUBLIC USBParseDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out);
 void DLL_PUBLIC USBParseConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out);
 void DLL_PUBLIC USBParseInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out);
 void DLL_PUBLIC USBParseEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out);

} /* namespace usbguard */
