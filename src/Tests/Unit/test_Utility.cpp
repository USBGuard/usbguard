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
#include <Common/Utility.cpp>

using namespace usbguard;

TEST_CASE("parentPath", "[Utility]")
{
  REQUIRE(parentPath("/a/b/c/d") == "/a/b/c");
  REQUIRE(parentPath("/a/b/c/d/") == "/a/b/c");
  REQUIRE(parentPath("/a/b/c/d//") == "/a/b/c");
  REQUIRE(parentPath("/a/b/c/d///") == "/a/b/c");
  REQUIRE(parentPath("/a/parent/child") == "/a/parent");
  REQUIRE(parentPath("/foo/bar") == "/foo");
  REQUIRE(parentPath("/foo/bar/") == "/foo");
  REQUIRE(parentPath("/foo/bar//") == "/foo");
  REQUIRE(parentPath("/foo") == std::string());
  REQUIRE(parentPath("/foo/") == std::string());
  REQUIRE(parentPath("/") == std::string());
  REQUIRE(parentPath("//") == std::string());
  REQUIRE(parentPath("///") == std::string());
  REQUIRE(parentPath("//foo") == std::string());
  REQUIRE(parentPath("//foo//") == std::string());
  REQUIRE(parentPath("") == std::string());
  REQUIRE(parentPath(std::string()) == std::string());
}

/* vim: set ts=2 sw=2 et */
