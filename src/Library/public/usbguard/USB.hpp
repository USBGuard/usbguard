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
//          Marek Tamaskovic <mtamasko@redhat.com>
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
  /**
   * @brief Maximum lenght of a string that is read from a USB descriptor.
   *
   * Since the string descriptors have an 8-bit length field, the
   * maximum lenght of a string stored in a string descriptor is
   * \p UINT8_MAX minus the size of the length field (1 Byte) and the
   * size of the type field (1 Byte).
   */
  const size_t USB_GENERIC_STRING_MAX_LENGTH = UINT8_MAX - 2;

  /**
   * @brief Maximum length of the vendor id in string form.
   */
  const size_t USB_VID_STRING_MAX_LENGTH = 4;

  /**
   * @brief Maximum lenght of the product id in string form.
   */
  const size_t USB_PID_STRING_MAX_LENGTH = 4;

  /**
   * @brief Maximum lenght of the port in string form.
   */
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

  /**
   * @brief Represents ID of a USB device.
   *
   * Format: \<\p vendor \p id\>:\<\p product \p id\>
   * \n
   * Example: ffff:ffff
   */
  class DLL_PUBLIC USBDeviceID
  {
  public:
    /**
     * @brief Default constructor.
     */
    USBDeviceID() = default;

    /**
     * @brief Constructs a new ID with \p vendor_id and \p product_id.
     *
     * Also performs \link checkDeviceID() checkDeviceID()\endlink.
     *
     * @param vendor_id Vendor ID.
     * @param product_id Product ID.
     * @throw runtime_error If ID would have invalid format.
     */
    USBDeviceID(const std::string& vendor_id, const std::string& product_id = std::string());

    /**
     * @brief Default copy constructor.
     *
     * @param rhs USBDeviceID to copy.
     */
    USBDeviceID(const USBDeviceID& rhs) = default;

    /**
     * @brief Default copy assignment.
     *
     * @param rhs USBDeviceID to copy.
     * @return New USBDeviceID object.
     */
    USBDeviceID& operator=(const USBDeviceID& rhs) = default;

    /**
     * @brief Checks if ID has a valid format.
     *
     * If vendor id is empty or \p * then product id must also be empty
     * or \p *. Both vendor id and product id can not be larger then
     * their maximum size.
     *
     * @param vendor_id Vendor ID.
     * @param product_id Product ID.
     * @throw runtime_error If ID has invalid format.
     */
    static void checkDeviceID(const std::string& vendor_id, const std::string& product_id);

    /**
     * @brief Sets vendor ID.
     *
     * Also performs \link checkDeviceID() checkDeviceID()\endlink.
     *
     * @param vendor_id Vendor ID.
     * @throw runtime_error If vendor ID invalidates ID format.
     * Respectively, if setting \p vendor_id would cause
     * \link checkDeviceID() checkDeviceID()\endlink to fail.
     */
    void setVendorID(const std::string& vendor_id);

    /**
     * @brief Sets product ID.
     *
     * Also performs \link checkDeviceID() checkDeviceID()\endlink.
     *
     * @param product_id Product ID.
     * @throw runtime_error If product ID invalidates ID format.
     * Respectively, if setting \p product_id would cause \link checkDeviceID()
     * checkDeviceID()\endlink to fail.
     */
    void setProductID(const std::string& product_id);

    /**
     * @brief Returns vendor ID.
     *
     * @return Vendor ID.
     */
    const std::string& getVendorID() const;

    /**
     * @brief Returns product ID.
     *
     * @return Product ID.
     */
    const std::string& getProductID() const;

    /**
     * @brief Returns ID in form of a string.
     *
     * String has the form: \<vendor id\>:\<product id\>.
     *
     * @return ID in form of a string.
     */
    std::string toRuleString() const;

    /**
     * @brief Same as toRuleString().
     *
     * @return ID in form of a string.
     */
    std::string toString() const;

    /**
     * @brief Checks if ID is subset of other ID.
     *
     * Basically checks if both IDs have the same vendor and product ID.
     * Either vendor or product ID can be empty and it might still be a subset.
     *
     * @param rhs ID to compare with.
     * @return True if this ID is subset of rhs ID, false otherwise.
     */
    bool isSubsetOf(const USBDeviceID& rhs) const;

  private:
    std::string _vendor_id; /**< Vendor ID */
    std::string _product_id; /**< Product ID */
  };

  namespace Predicates DLL_PUBLIC
  {
    /**
     * @brief Checks if source ID is subset of target ID.
     *
     * Basically checks if both IDs have the same vendor and product ID.
     * Either vendor or product ID can be empty and it might still be a subset.
     *
     * @param source First USB device ID.
     * @param target Second USB device ID.
     * @return True if source ID is subset of target ID, false otherwise.
     */
    template<>
    bool isSubsetOf(const USBDeviceID& source, const USBDeviceID& target);

    /**
     * @brief Checks if source ID is superset of target ID.
     *
     * Basically checks if source applies to target.
     *
     * @param source First USB device ID.
     * @param target Second USB device ID.
     * @return True if source ID is subset of target ID, false otherwise.
     */
    template<>
    bool isSupersetOf(const USBDeviceID& source, const USBDeviceID& target);
  }

  /**
   * @brief Represents USB interface.
   *
   * Format: three 8b numbers separated by a colon, where first number
   * represents interface class, second number represents interface subclass
   * and third number represents interface protocol.\n
   * Example: ff:ff:ff
   */
  class DLL_PUBLIC USBInterfaceType
  {
  public:
    static const uint8_t MatchClass = 1<<0;
    static const uint8_t MatchSubClass = 1<<1;
    static const uint8_t MatchProtocol = 1<<2;
    static const uint8_t MatchAll = MatchClass|MatchSubClass|MatchProtocol;

    /**
     * @brief Constructs new interface by setting everything to zero.
     */
    USBInterfaceType();

    /**
     * @brief Constructs new interface with given class, subclass,
     * protocol and mask.
     *
     * Mask by default masks everything.
     *
     * @param bClass Interface mask.
     * @param bSubClass Interface subclass.
     * @param bProtocol Interface protocol.
     * @param mask Interface mask.
     */
    USBInterfaceType(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask = MatchAll);

    /**
     * @brief Constructs new interface from existing interface descriptor
     * and sets mask.
     *
     * Mask by default masks everything.
     *
     * @param descriptor Interface descriptor.
     * @param mask Interface mask.
     */
    USBInterfaceType(const USBInterfaceDescriptor& descriptor, uint8_t mask = MatchAll);

    /**
     * @brief Constructs new interface from a string.
     *
     * @throw runtime_error If string is not in the correct interface format.
     */
    USBInterfaceType(const std::string& type_string);

    /**
     * @brief Compares two interfaces for equality.
     *
     * Interfaces are equal if they have the same class, subclass,
     * protocol and mask.
     *
     * @param rhs Interface to compare with.
     * @return True if interfaces are equal, false otherwise.
     */
    bool operator==(const USBInterfaceType& rhs) const;

    /**
     * @brief Checks whether non-masked parts of interface match rhs interface.
     *
     * @param rhs Interface to compare with.
     * @return True if non-masked parts of interface match rhs interface,
     * false otherwise.
     */
    bool appliesTo(const USBInterfaceType& rhs) const;

    /**
     * @brief Returns string representation of interface.
     *
     * Internally calls static version of
     * \link typeString() typeString()\endlink.
     *
     * @return String representation of interface.
     */
    const std::string typeString() const;

    /**
     * @brief Returns string representation of interface.
     *
     * Synonym for \link typeString() typeString()\endlink.
     *
     * @return String representation of interface.
     */
    const std::string toRuleString() const;

    /**
     * @brief Returns string representation of interface.
     *
     * Masked parts of interface are exchanged with \p *.\n
     * Example: 00:*:*
     *
     * @param bClass Interface class.
     * @param bSubClass Interface subclass.
     * @param bProtocol Interface protocol.
     * @param mask Interface mask, default value is to mask everything.
     * @return String representation of interface.
     */
    static const std::string typeString(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask = MatchAll);

  private:
    uint8_t _bClass; /**< Interface class. */
    uint8_t _bSubClass; /**< Interface subclass */
    uint8_t _bProtocol; /**< Interface protocol */
    uint8_t _mask; /**< least significant bit masks interface class,
                        2nd least significant bit masks interface subclass and
                        3rd least significant bit masks interface protocol. */
  };

  namespace Predicates DLL_PUBLIC
  {
    /**
     * @brief Checks whether non-masked parts of interface source match
     * target interface.
     *
     * @param source First interface.
     * @param target Second interface.
     * @return True if non-masked parts of interface source match
     * targer interface.
     */
    template<>
    bool isSubsetOf(const USBInterfaceType& source, const USBInterfaceType& target);
    template<>
    bool isSupersetOf(const USBInterfaceType& source, const USBInterfaceType& target);
  }

  class USBDescriptorParser;

  class DLL_PUBLIC USBDescriptorParserHooks
  {
  public:
    virtual void parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_in, USBDescriptor* descriptor_out);
    virtual void loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor);
  };

  /**
   * @brief Parses USB descriptors.
   */
  class DLL_PUBLIC USBDescriptorParser
  {
  public:
    USBDescriptorParser(USBDescriptorParserHooks& hooks);

    /**
     * @brief Initiate parsing of USB descriptors from an input stream.
     *
     * @param stream Stream to parse.
     * @return Number of bytes succesfully parsed/processed from the stream.
     */
    size_t parse(std::istream& stream);

    /**
     * @brief Returns a pointer to a USBDescriptor of type bDescriptorType that
     * is stored in the USB descriptor state.
     *
     * If there's no such descriptor, then \p nullptr is returned.
     *
     * @param bDescriptorType Type of descriptor.
     * @return Pointer to a USBDescriptor of type \p bDescriptorType
     * or \p nullptr if there's no such descriptor.
     */
    const std::vector<USBDescriptor>* getDescriptor(uint8_t bDescriptorType) const;

    /**
     * @brief Set the active instance of an USB descriptor
     * of type \p bDescriptorType.
     *
     * @param bDescriptorType Type of descriptor.
     * @descriptor Descriptor to set.
     */
    void setDescriptor(uint8_t bDescriptorType, const USBDescriptor& descriptor);

    /**
     * @brief Delete the active instance of an USB descriptor
     * of \p bDescriptorType type.
     *
     * @param bDescriptorType Type of descriptor to remove.
     */
    void delDescriptor(uint8_t bDescriptorType);

    /**
     *
     * @brief Checks whether descriptor state contains a USB descriptor of type
     * \p bDescriptorType.
     *
     * @param bDescriptorType Type of descriptor.
     * @return True if the descriptor state contains a USB descriptor of type
     * bDescriptorType, false otherwise.
     */
    bool haveDescriptor(uint8_t bDescriptorType) const;

    /**
     * @brief Returns a vector of (\p bDescriptorType, \p count) pairs.
     *
     * @return Vector of (\p bDescriptorType, \p count) pairs.
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
