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
#include <Base64.cpp>

using namespace usbguard;

TEST_CASE("Base64", "[Utility]")
{
  SECTION("encoding/decoding empty value throws an exception") {
    std::string decoded;
    std::string encoded;
    REQUIRE_THROWS(encoded = base64Encode(decoded));
    REQUIRE_THROWS(decoded = base64Decode(encoded));
  }
  SECTION("encoded/decoded size estimation") {
    REQUIRE(base64EncodedSize(1) == 4);
    REQUIRE(base64EncodedSize(2) == 4);
    REQUIRE(base64EncodedSize(3) == 4);
    REQUIRE(base64EncodedSize(4) == 8);
    REQUIRE(base64EncodedSize(5) == 8);
    REQUIRE(base64EncodedSize(6) == 8);
    REQUIRE(base64DecodedSize(4) == 3);
    REQUIRE(base64DecodedSize(8) == 6);
    REQUIRE(base64DecodedSize(12) == 9);
    REQUIRE(base64DecodedSize(16) == 12);
  }
  SECTION("encoding and decoding") {
    std::vector<std::pair<std::string, std::string>> test_pairs = {
      { "a", "YQ==" },
      { std::string("\x01", 1), "AQ==" },
      { std::string("\x10", 1), "EA==" },
      { "base64", "YmFzZTY0" },
      { std::string("\x01\x23\x45\x67\x89", 5), "ASNFZ4k=" },
      { "b", "Yg==" },
      { "c", "Yw==" },
      { "aa", "YWE=" },
      { "ab", "YWI=" },
      { "ba", "YmE=" },
      { "bb", "YmI=" },
      { "abc", "YWJj" },
      { "bac", "YmFj" },
      { "bca", "YmNh" },
      { "cab", "Y2Fi" },
      { "cba", "Y2Jh" },
      { "A", "QQ==" },
      { "Z", "Wg==" },
      { "AA", "QUE=" },
      { "AZ", "QVo=" },
      { "ZZ", "Wlo=" },
      { "/", "Lw==" },
      { "//", "Ly8=" },
      { "///", "Ly8v" },
      { "+", "Kw==" },
      { "++", "Kys=" },
      { "+++", "Kysr" },
      { std::string("\x00", 1), "AA==" },
      { std::string("\xff", 1), "/w==" },
    };

    for (auto test_pair : test_pairs) {
      REQUIRE(test_pair.second == base64Encode(test_pair.first));
      REQUIRE(test_pair.first == base64Decode(test_pair.second));
    }
  }
  SECTION("decoding invalid inputs") {
    std::vector<std::string> test_inputs = {
      { "Y=Q=" },
      { "Y~g=" },
      { ".Yw=" },
      { "Y:E=" },
      { "YWI:" },
      { "YmVpc*Bv" },
      { "cHdxbw@=" },
      { "aX@3b2VyaQ==" },
      { "cG9zb@tk" },
      { "Zmxs;:pxOTIw" },
      { "cHd<ZWty" },
      { "cG93>XBy" },
      { "<2l3cGVyaQ==" },
      { ">nBvZXc=" },
      { "c?93cm93aWVycA==" },
      { "aTItNA=?" },
      { "cG9?ZnNkb2lmd3BvZWly" },
      { "cG9,,3Jw" },
      { "b2l3,A==" },
      { ",2Zzd2hyaTJndWdpdXBhW3B3cW9b" },
      { "[1t3cHI=" },
      { "]3B3Yw==" },
      { "Ym[h" },
      { "Y2F]" },
      { "`2Jh" },
      { "Q`==" },
      { "Wg=`" },
      { "QUE`" },
      { "`Vo=" },
      { "W_o=" },
      { "L ==" },
      { "Ly8 " },
      { " y8v" },
      { "K{}=" },
      { "Kys{" },
      { "Kys}" },
      { "{w==" },
      { "a" },
      { "aa" },
      { "aaa" },
      { "bbbbb" },
      { "bbbbbb" }
    };

    for (auto test_input : test_inputs) {
      INFO("Testing input: " + test_input);
      REQUIRE_THROWS(base64Decode(test_input));
    }
  }
}

/* vim: set ts=2 sw=2 et */
