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
#include "Typedefs.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <climits>
#include <cstddef>
#include <cstdint>

namespace usbguard
{
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
    Unknown = USB_DESCRIPTOR_TYPE_UNKNOWN,
    Device = USB_DESCRIPTOR_TYPE_DEVICE,
    Configuration = USB_DESCRIPTOR_TYPE_CONFIGURATION,
    String = USB_DESCRIPTOR_TYPE_STRING,
    Interface = USB_DESCRIPTOR_TYPE_INTERFACE,
    Endpoint = USB_DESCRIPTOR_TYPE_ENDPOINT,
    AssociationInterface = USB_DESCRIPTOR_TYPE_ASSOCIATION_INTERFACE
  };

  struct DLL_PUBLIC USBDescriptorHeader {
    uint8_t bLength;
    uint8_t bDescriptorType;
  } __attribute__((packed));

  struct DLL_PUBLIC USBDescriptor {
    struct USBDescriptorHeader bHeader;
    uint8_t bDescriptorData[256-sizeof(USBDescriptorHeader)];
  } __attribute__((packed));

  struct DLL_PUBLIC USBDeviceDescriptor {
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

  struct DLL_PUBLIC USBConfigurationDescriptor {
    struct USBDescriptorHeader bHeader;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
  } __attribute__((packed));

  struct DLL_PUBLIC USBInterfaceDescriptor {
    struct USBDescriptorHeader bHeader;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
  } __attribute__((packed));

  struct USBEndpointDescriptor {
    struct USBDescriptorHeader bHeader;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
  } __attribute__((packed));

  struct USBAudioEndpointDescriptor {
    struct USBDescriptorHeader bHeader;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
    uint8_t bRefresh;
    uint8_t bSynchAddress;
  } __attribute__((packed));

  class DLL_PUBLIC USBDeviceID
  {
  public:
    USBDeviceID();
    USBDeviceID(const std::string& vendor_id, const std::string& product_id = std::string());
    USBDeviceID(const USBDeviceID& rhs);

    static void checkDeviceID(const std::string& vendor_id, const std::string& product_id);

    void setVendorID(const std::string& vendor_id);
    void setProductID(const std::string& product_id);

    const std::string& getVendorID() const;
    const std::string& getProductID() const;

    std::string toRuleString() const;
    std::string toString() const;
    bool isSubsetOf(const USBDeviceID& rhs) const;

  private:
    std::string _vendor_id;
    std::string _product_id;
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

    const std::string typeString() const;
    const std::string toRuleString() const;
    static const std::string typeString(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask = MatchAll);

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

  class USBDescriptorParser;

  class DLL_PUBLIC USBDescriptorParserHooks
  {
  public:
    virtual void parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_in, USBDescriptor* descriptor_out);
    virtual void loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
  };

  class DLL_PUBLIC USBDescriptorParser
  {
  public:
    USBDescriptorParser(USBDescriptorParserHooks& hooks);

    /**
     * Initiate parsing of USB descriptors from an input stream.
     *
     * Returns number of bytes succesfully parsed/processed from
     * the stream.
     */
    size_t parse(std::istream& stream);

    /**
     * Return a pointer to a USBDescriptor of type bDescriptorType that
     * is stored in the USB descriptor state. If there's no such descriptor,
     * then nullptr is returned.
     */
    const std::vector<USBDescriptor>* getDescriptor(uint8_t bDescriptorType) const;

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
    const std::vector<std::pair<uint8_t, size_t>> getDescriptorCounts() const;

  private:
    USBDescriptorParserHooks& _hooks;

    std::unordered_map<uint8_t, std::vector<USBDescriptor>> _dstate_map; /**< Descriptor State Map */
    std::unordered_map<uint8_t, size_t> _count_map;
  };

  void DLL_PUBLIC USBParseDeviceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out);
  void DLL_PUBLIC USBParseConfigurationDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out);
  void DLL_PUBLIC USBParseInterfaceDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out);
  void DLL_PUBLIC USBParseEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out);
  void DLL_PUBLIC USBParseAudioEndpointDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out);
  void DLL_PUBLIC USBParseUnknownDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out);

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
