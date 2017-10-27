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
#include "usbguard/Rule.hpp"

#include <catch.hpp>

using namespace usbguard;

TEST_CASE("Parse rule with an id attribute", "[Parse]")
{
  Rule rule;
  SECTION("old syntax") {
    REQUIRE_NOTHROW(rule = Rule::fromString("allow 1234:1234"));
    REQUIRE(rule.getDeviceID().getVendorID() == "1234");
    REQUIRE(rule.getDeviceID().getProductID() == "1234");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow 1234:*"));
    REQUIRE(rule.getDeviceID().getVendorID() == "1234");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow *:*"));
    REQUIRE(rule.getDeviceID().getVendorID() == "*");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow feab:cd05"));
    REQUIRE(rule.getDeviceID().getVendorID() == "feab");
    REQUIRE(rule.getDeviceID().getProductID() == "cd05");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow 7896:*"));
    REQUIRE(rule.getDeviceID().getVendorID() == "7896");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
  }
  SECTION("new syntax") {
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1234:1234"));
    REQUIRE(rule.getDeviceID().getVendorID() == "1234");
    REQUIRE(rule.getDeviceID().getProductID() == "1234");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1234:*"));
    REQUIRE(rule.getDeviceID().getVendorID() == "1234");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id *:*"));
    REQUIRE(rule.getDeviceID().getVendorID() == "*");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id feab:cd05"));
    REQUIRE(rule.getDeviceID().getVendorID() == "feab");
    REQUIRE(rule.getDeviceID().getProductID() == "cd05");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 7896:*"));
    REQUIRE(rule.getDeviceID().getVendorID() == "7896");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
  }
  SECTION("set syntax") {
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id one-of { 1234:1234 }"));
    REQUIRE(rule.getDeviceID().getVendorID() == "1234");
    REQUIRE(rule.getDeviceID().getProductID() == "1234");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id one-of { 1234:* }"));
    REQUIRE(rule.getDeviceID().getVendorID() == "1234");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id one-of { *:* }"));
    REQUIRE(rule.getDeviceID().getVendorID() == "*");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id one-of { feab:cd05 }"));
    REQUIRE(rule.getDeviceID().getVendorID() == "feab");
    REQUIRE(rule.getDeviceID().getProductID() == "cd05");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id one-of { 7896:* }"));
    REQUIRE(rule.getDeviceID().getVendorID() == "7896");
    REQUIRE(rule.getDeviceID().getProductID() == "*");
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id one-of { 1234:1234 abcd:effa aaaa:* }"));
    REQUIRE_THROWS(rule.getDeviceID().getVendorID());
    REQUIRE_THROWS(rule.getDeviceID().getProductID());
    REQUIRE(rule.attributeDeviceID().count() == 3);
    REQUIRE_THROWS(rule.attributeDeviceID().get());
    REQUIRE(rule.attributeDeviceID().get(0).getVendorID() == "1234");
    REQUIRE(rule.attributeDeviceID().get(0).getProductID() == "1234");
    REQUIRE(rule.attributeDeviceID().get(1).getVendorID() == "abcd");
    REQUIRE(rule.attributeDeviceID().get(1).getProductID() == "effa");
    REQUIRE(rule.attributeDeviceID().get(2).getVendorID() == "aaaa");
    REQUIRE(rule.attributeDeviceID().get(2).getProductID() == "*");
  }
}

TEST_CASE("Match id attribute", "[Match]")
{
  Rule device_rule = \
    Rule::fromString("device id 1d6b:0002 serial \"dummy_hcd.0\" name \"Dummy host controller\" hash \"FSgk48/lKiTJWdqOqkHLuMQr155m+ux+ozIb17HHcKs=\" parent-hash \"zUhq0ExS41FTVEO5sce8eOOyMrPsfwQEeVz0kH6IAhc=\" via-port \"usb1\" with-interface 09:00:00");
  Rule rule;
  SECTION("vendor and product") {
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1d6b:0002"));
    REQUIRE(rule.appliesTo(device_rule));
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1d6b:0003"));
    REQUIRE(!rule.appliesTo(device_rule));
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1d6a:0002"));
    REQUIRE(!rule.appliesTo(device_rule));
  }
  SECTION("vendor only") {
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1d6b:*"));
    REQUIRE(rule.appliesTo(device_rule));
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id 1d6a:*"));
    REQUIRE(!rule.appliesTo(device_rule));
  }
  SECTION("any") {
    REQUIRE_NOTHROW(rule = Rule::fromString("allow id *:*"));
    REQUIRE(rule.appliesTo(device_rule));
  }
}

/* vim: set ts=2 sw=2 et */
