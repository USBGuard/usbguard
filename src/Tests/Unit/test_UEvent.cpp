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
#include <UEvent.cpp>

using namespace usbguard;

TEST_CASE("Default constructed UEvent", "[UEvent]")
{
  UEvent uevent;
  SECTION("empty") {
    REQUIRE_NOTHROW(uevent.getAttribute("KEY1"));
    REQUIRE(uevent.getAttribute("KEY1") == std::string());
    REQUIRE_FALSE(uevent.hasAttribute("KEY1"));
    REQUIRE_FALSE(uevent.hasRequiredAttributes());
    REQUIRE_NOTHROW(uevent.clear());
  }
  SECTION("set/get attribute") {
    REQUIRE_NOTHROW(uevent.setAttribute("KEY1", "value1"));
    REQUIRE(uevent.getAttribute("KEY1") == "value1");
    REQUIRE(uevent.hasAttribute("KEY1"));
    REQUIRE_FALSE(uevent.hasRequiredAttributes());
    REQUIRE_NOTHROW(uevent.clear());
  }
  SECTION("string serialization") {
    REQUIRE_THROWS(uevent.getHeaderLine());
    REQUIRE_THROWS(uevent.toString());
  }
}

TEST_CASE("UEvent with required attributes", "[UEvent]")
{
  UEvent uevent;
  REQUIRE_NOTHROW(uevent.setAttribute("ACTION", "add"));
  REQUIRE_NOTHROW(uevent.setAttribute("DEVPATH", "/bus/usb/devices/usb1"));
  REQUIRE_NOTHROW(uevent.setAttribute("SUBSYSTEM", "usb_device"));
  SECTION("non-existing attribute") {
    REQUIRE_NOTHROW(uevent.getAttribute("KEY1"));
    REQUIRE(uevent.getAttribute("KEY1") == std::string());
    REQUIRE_FALSE(uevent.hasAttribute("KEY1"));
  }
  SECTION("set/get attribute") {
    REQUIRE_NOTHROW(uevent.setAttribute("KEY1", "value1"));
    REQUIRE(uevent.getAttribute("KEY1") == "value1");
    REQUIRE(uevent.hasAttribute("KEY1"));
  }
  SECTION("string serialization") {
    REQUIRE(uevent.hasRequiredAttributes());
    REQUIRE_NOTHROW(uevent.getHeaderLine());
    REQUIRE_NOTHROW(uevent.toString());
  }
}

/* vim: set ts=2 sw=2 et */
