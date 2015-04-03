#pragma once
#include "Typedefs.hpp"
#include <cstdint>

namespace usbguard {
  /*
   * NOTE: The USB specification dictates that all multibyte data fields
   *       are in little-endian form. The structures defined bellow are
   *       used for platform-independed passing of the USB descriptor
   *       data to the Device class. The Device class assumes that the
   *       values are in host-specific endianness.
   */

  struct USBDeviceDescriptor
  {
    uint8_t bLength;
    uint8_t bDescriptorType;
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

  struct USBConfigurationDescriptor
  {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
  } __attribute__((packed));

  struct USBInterfaceDescriptor
  {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
  } __attribute__((packed));

  class USBInterfaceType
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
    static const String typeString(uint8_t bClass, uint8_t bSubClass, uint8_t bProtocol, uint8_t mask = MatchAll);

  private:
    uint8_t _bClass;
    uint8_t _bSubClass;
    uint8_t _bProtocol;
    uint8_t _mask;
  };

  template<>
  bool matches(const USBInterfaceType& a, const USBInterfaceType& b);

  const USBDeviceDescriptor USBParseDeviceDescriptor(const void *data, size_t size, size_t *real_size = nullptr);
  const USBConfigurationDescriptor USBParseConfigurationDescriptor(const void *data, size_t size, size_t *real_size = nullptr);
  const USBInterfaceDescriptor USBParseInterfaceDescriptor(const void *data, size_t size, size_t *real_size = nullptr);

} /* namespace usbguard */
