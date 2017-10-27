//
// Copyright (C) 2016 Red Hat, Inc.
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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard.hpp"
#include "usbguard-read-descriptor.hpp"

#include "usbguard/USB.hpp"
#include "usbguard/Exception.hpp"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cinttypes>

namespace usbguard
{
  static const char* options_short = "h";

  static const struct ::option options_long[] = {
    { "help", no_argument, nullptr, 'h' },
    { nullptr, 0, nullptr, 0 }
  };

  static void showHelp(std::ostream& stream)
  {
    stream << " Usage: " << usbguard_arg0 << " read-descriptor [OPTIONS] <file>" << std::endl;
    stream << std::endl;
    stream << " Options:" << std::endl;
    stream << "  -h, --help       Show this help." << std::endl;
    stream << std::endl;
  }

  static void printDeviceDescriptor(const USBDescriptor*);
  static void printConfigurationDescriptor(const USBDescriptor*);
  static void printInterfaceDescriptor(const USBDescriptor*);
  static void printEndpointDescriptor(const USBDescriptor*);
  static void printAudioEndpointDescriptor(const USBDescriptor*);
  static void printUnknownDescriptor(const USBDescriptor*);

  class USBDescriptorPrinter : public USBDescriptorParserHooks
  {
  public:
    void loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor) override
    {
      (void)parser;

      switch (static_cast<USBDescriptorType>(descriptor->bHeader.bDescriptorType)) {
      case USBDescriptorType::Device:
        printDeviceDescriptor(descriptor);
        break;

      case USBDescriptorType::Configuration:
        printConfigurationDescriptor(descriptor);
        break;

      case USBDescriptorType::Interface:
        printInterfaceDescriptor(descriptor);
        break;

      case USBDescriptorType::Endpoint:
        switch (descriptor->bHeader.bLength) {
        case sizeof(USBEndpointDescriptor):
          printEndpointDescriptor(descriptor);
          break;

        case sizeof(USBAudioEndpointDescriptor):
          printAudioEndpointDescriptor(descriptor);
          break;

        default:
          printUnknownDescriptor(descriptor);
        }

        break;

      case USBDescriptorType::String:
      case USBDescriptorType::AssociationInterface:
      case USBDescriptorType::Unknown:
      default:
        printUnknownDescriptor(descriptor);
        break;
      }
    }
  };

  int usbguard_read_descriptor(int argc, char* argv[])
  {
    int opt = 0;

    while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
      switch (opt) {
      case 'h':
        showHelp(std::cout);
        return EXIT_SUCCESS;

      case '?':
        showHelp(std::cerr);

      default:
        return EXIT_FAILURE;
      }
    }

    argc -= optind;
    argv += optind;

    if (argc != 1) {
      showHelp(std::cerr);
      return EXIT_FAILURE;
    }

    std::ifstream descriptor_stream(argv[0], std::ifstream::binary);

    if (!descriptor_stream.good()) {
      throw ErrnoException("read-descriptor", argv[0], errno);
    }
    else {
      USBDescriptorPrinter printer;
      USBDescriptorParser parser(printer);
      const size_t size_parsed = parser.parse(descriptor_stream);
      printf("Bytes parsed: %zu\n", size_parsed);

      for (auto const& count : parser.getDescriptorCounts()) {
        printf("Descriptor type 0x%02" PRIx8 " count: %zu\n", count.first, count.second);
      }
    }

    return EXIT_SUCCESS;
  }

#define PRINTF_LEVEL(level) for (int l = level; l > 0; --l) printf("\t")

#define PRINTF_HEADER(sp, label, level) \
  do { \
    PRINTF_LEVEL(level); \
    printf("%s: type=%02" PRIx8 " length=%" PRIu8 "\n", label, \
      (sp)->bHeader.bDescriptorType, (sp)->bHeader.bLength); \
  } while(0)

#define PRINTF_MEMBER(sp, m, fmt, level) \
  do { \
    PRINTF_LEVEL(level); \
    printf("%20s: " fmt "\n", #m, sp->m); \
  } while(0)

  void printDeviceDescriptor(const USBDescriptor* descriptor_base)
  {
    const USBDeviceDescriptor* descriptor = reinterpret_cast<const USBDeviceDescriptor*>(descriptor_base);
    PRINTF_HEADER(descriptor, "Device Descriptor", 0);
    PRINTF_MEMBER(descriptor, bcdUSB, "0x%04" PRIx16, 0);
    PRINTF_MEMBER(descriptor, bDeviceClass, "0x%02" PRIx8, 0);
    PRINTF_MEMBER(descriptor, bDeviceSubClass, "0x%02" PRIx8, 0);
    PRINTF_MEMBER(descriptor, bDeviceProtocol, "0x%02" PRIx8, 0);
    PRINTF_MEMBER(descriptor, bMaxPacketSize, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, idVendor, "0x%04" PRIx16, 0);
    PRINTF_MEMBER(descriptor, idProduct, "0x%04" PRIx16, 0);
    PRINTF_MEMBER(descriptor, bcdDevice, "0x%04" PRIx16, 0);
    PRINTF_MEMBER(descriptor, iManufacturer, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, iSerialNumber, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bNumConfigurations, "%" PRIu8, 0);
    printf("\n");
  }

  void printConfigurationDescriptor(const USBDescriptor* descriptor_base)
  {
    const USBConfigurationDescriptor* descriptor = reinterpret_cast<const USBConfigurationDescriptor*>(descriptor_base);
    PRINTF_HEADER(descriptor, "Configuration Descriptor", 0);
    PRINTF_MEMBER(descriptor, wTotalLength, "%" PRIu16, 0);
    PRINTF_MEMBER(descriptor, bNumInterfaces, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bConfigurationValue, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, iConfiguration, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bmAttributes, "%02" PRIx8, 0);
    PRINTF_MEMBER(descriptor, bMaxPower, "%" PRIu8, 0);
    printf("\n");
  }

  void printInterfaceDescriptor(const USBDescriptor* descriptor_base)
  {
    const USBInterfaceDescriptor* descriptor = reinterpret_cast<const USBInterfaceDescriptor*>(descriptor_base);
    PRINTF_HEADER(descriptor, "Interface Descriptor", 0);
    PRINTF_MEMBER(descriptor, bInterfaceNumber, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bAlternateSetting, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bNumEndpoints, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bInterfaceClass, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bInterfaceSubClass, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bInterfaceProtocol, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, iInterface, "%" PRIu8, 0);
    printf("\n");
  }

  void printEndpointDescriptor(const USBDescriptor* descriptor_base)
  {
    const USBEndpointDescriptor* descriptor = reinterpret_cast<const USBEndpointDescriptor*>(descriptor_base);
    PRINTF_HEADER(descriptor, "Endpoint Descriptor", 0);
    PRINTF_MEMBER(descriptor, bEndpointAddress, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bmAttributes, "0x%02" PRIx8, 0);
    PRINTF_MEMBER(descriptor, wMaxPacketSize, "%" PRIu16, 0);
    PRINTF_MEMBER(descriptor, bInterval, "%" PRIu8, 0);
    printf("\n");
  }

  void printAudioEndpointDescriptor(const USBDescriptor* descriptor_base)
  {
    const USBAudioEndpointDescriptor* descriptor = reinterpret_cast<const USBAudioEndpointDescriptor*>(descriptor_base);
    PRINTF_HEADER(descriptor, "Endpoint Descriptor (Audio)", 0);
    PRINTF_MEMBER(descriptor, bEndpointAddress, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bmAttributes, "0x%02" PRIx8, 0);
    PRINTF_MEMBER(descriptor, wMaxPacketSize, "%" PRIu16, 0);
    PRINTF_MEMBER(descriptor, bInterval, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bRefresh, "%" PRIu8, 0);
    PRINTF_MEMBER(descriptor, bSynchAddress, "%" PRIu8, 0);
    printf("\n");
  }

  void printUnknownDescriptor(const USBDescriptor* descriptor)
  {
    PRINTF_HEADER(descriptor, "UNKNOWN Descriptor", 0);
    printf("\n");
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
