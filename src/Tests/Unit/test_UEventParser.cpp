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
#include <catch.hpp>
#include <UEventParser.cpp>

using namespace usbguard;

TEST_CASE("UEvent parsing", "[UEventParser]")
{
  const char uevent_string[] = {
    'a', 'd', 'd', '@', '/', 'd', 'e', 'v',
    'i', 'c', 'e', 's', '/', 'p', 'l', 'a',
    't', 'f', 'o', 'r', 'm', '/', 'd', 'u',
    'm', 'm', 'y', '_', 'h', 'c', 'd', '.',
    '0', '/', 'u', 's', 'b', '1', 0x00, 'A',
    'C', 'T', 'I', 'O', 'N', '=', 'a', 'd',
    'd', 0x00, 'D', 'E', 'V', 'P', 'A', 'T',
    'H', '=', '/', 'd', 'e', 'v', 'i', 'c',
    'e', 's', '/', 'p', 'l', 'a', 't', 'f',
    'o', 'r', 'm', '/', 'd', 'u', 'm', 'm',
    'y', '_', 'h', 'c', 'd', '.', '0', '/',
    'u', 's', 'b', '1', 0x00, 'S', 'U', 'B',
    'S', 'Y', 'S', 'T', 'E', 'M', '=', 'u',
    's', 'b', 0x00, 'M', 'A', 'J', 'O', 'R',
    '=', '1', '8', '9', 0x00, 'M', 'I', 'N',
    'O', 'R', '=', '0', 0x00, 'D', 'E', 'V',
    'N', 'A', 'M', 'E', '=', 'b', 'u', 's',
    '/', 'u', 's', 'b', '/', '0', '0', '1',
    '/', '0', '0', '1', 0x00, 'D', 'E', 'V',
    'T', 'Y', 'P', 'E', '=', 'u', 's', 'b',
    '_', 'd', 'e', 'v', 'i', 'c', 'e', 0x00,
    'D', 'R', 'I', 'V', 'E', 'R', '=', 'u',
    's', 'b', 0x00, 'P', 'R', 'O', 'D', 'U',
    'C', 'T', '=', '1', 'd', '6', 'b', '/',
    '2', '/', '4', '0', '1', 0x00, 'T', 'Y',
    'P', 'E', '=', '9', '/', '0', '/', '1',
    0x00, 'B', 'U', 'S', 'N', 'U', 'M', '=',
    '0', '0', '1', 0x00, 'D', 'E', 'V', 'N',
    'U', 'M', '=', '0', '0', '1', 0x00, 'S',
    'E', 'Q', 'N', 'U', 'M', '=', '1', '1',
    '1', '7', 0x00
  };
  const size_t uevent_strlen = sizeof uevent_string;
  UEvent uevent;
  REQUIRE_NOTHROW(uevent = UEvent::fromString(std::string(uevent_string, uevent_strlen),
        /*attributes_only=*/false,
        /*trace=*/true));
  REQUIRE(uevent.hasRequiredAttributes());
  REQUIRE(uevent.getAttribute("ACTION") == "add");
  REQUIRE(uevent.getAttribute("DEVPATH") == "/devices/platform/dummy_hcd.0/usb1");
  REQUIRE(uevent.getAttribute("SUBSYSTEM") == "usb");
  REQUIRE(uevent.getAttribute("MAJOR") == "189");
  REQUIRE(uevent.getAttribute("MINOR") == "0");
  REQUIRE(uevent.getAttribute("DEVNAME") == "bus/usb/001/001");
  REQUIRE(uevent.getAttribute("DEVTYPE") == "usb_device");
  REQUIRE(uevent.getAttribute("DRIVER") == "usb");
  REQUIRE(uevent.getAttribute("PRODUCT") == "1d6b/2/401");
  REQUIRE(uevent.getAttribute("TYPE") == "9/0/1");
  REQUIRE(uevent.getAttribute("BUSNUM") == "001");
  REQUIRE(uevent.getAttribute("DEVNUM") == "001");
  REQUIRE(uevent.getAttribute("SEQNUM") == "1117");
  REQUIRE_NOTHROW(uevent.getHeaderLine());
  REQUIRE_NOTHROW(uevent.toString());
}

/* vim: set ts=2 sw=2 et */
