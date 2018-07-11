//
// Copyright (C) 2018 Red Hat, Inc.
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
#include <UMockdevDeviceDefinition.hpp>

#include "test_UMockdevDeviceDefinition.data.hpp"

using namespace usbguard;

TEST_CASE("Default constructed UMockdevDeviceDefinition", "[UMockdev]")
{
  UMockdevDeviceDefinition d;
  SECTION("empty") {
    REQUIRE(d.getUMockdevName() == std::string());
    REQUIRE(d.getSysfsPath() == std::string());
    REQUIRE(d.getDevfsNode() == std::string());
    REQUIRE(d.getUMockdevDefinition() == std::string());
    REQUIRE_FALSE(d.hasDevfsNode());
    REQUIRE_FALSE(d);
  }
  SECTION("set/get/has") {
    REQUIRE(d.getUMockdevName() == std::string());
    REQUIRE_NOTHROW(d.setUMockdevName("foo.umockdev"));
    REQUIRE(d.getUMockdevName() == "foo.umockdev");
    REQUIRE(d.getSysfsPath() == std::string());
    REQUIRE_NOTHROW(d.setSysfsPath("/devices/usb/usb1"));
    REQUIRE(d.getSysfsPath() == "/devices/usb/usb1");
    REQUIRE(d.getDevfsNode() == std::string());
    REQUIRE_NOTHROW(d.setDevfsNode("/bus/usb/001/001"));
    REQUIRE(d.getDevfsNode() == "/bus/usb/001/001");
    REQUIRE(d.hasDevfsNode());
    REQUIRE(d.getUMockdevDefinition() == std::string());
    REQUIRE_FALSE(d);
    REQUIRE_NOTHROW(d.setUMockdevDefinition("P:/devices/usb/usb1\nN:/bus/usb/001/001=348023948039809\n"));
    REQUIRE(d.getUMockdevDefinition() == "P:/devices/usb/usb1\nN:/bus/usb/001/001=348023948039809\n");
    REQUIRE(d);
  }
  SECTION("parsing") {
    const std::string s(reinterpret_cast<const char*>(definitions_string), definitions_string_len);
    std::vector<std::unique_ptr<UMockdevDeviceDefinition>> definitions;
    REQUIRE_NOTHROW(definitions = UMockdevDeviceDefinition::parseFromString(s));
    REQUIRE(definitions.size() == 2);
    REQUIRE(s == (definitions[0]->getUMockdevDefinition() + "\n\n" + definitions[1]->getUMockdevDefinition() + "\n\n"));
    REQUIRE(definitions[0]->getSysfsPath() == "/devices/platform/dummy_hcd.0/usb1");
    REQUIRE(definitions[1]->getSysfsPath() == "/devices/platform/dummy_hcd.0");
  }
}
/* vim: set ts=2 sw=2 et */
