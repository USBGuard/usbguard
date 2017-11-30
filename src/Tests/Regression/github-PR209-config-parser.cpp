//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "usbguard/ConfigFile.hpp"
#include <catch.hpp>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace usbguard;

static std::string createFile(const std::vector<std::string> lines)
{
  std::string filepath("/tmp/usbguard-test.XXXXXX");
  const int fd = ::mkstemp(&filepath[0]);

  if (fd < 0) {
    throw std::runtime_error("cannot create temporary file");
  }

  for (const auto& line : lines) {
    ::write(fd, line.c_str(), line.size());
    ::write(fd, "\n", 1);
  }

  ::close(fd);
  return filepath;
}

TEST_CASE("Regression: GitHub PR #209", "[regression]")
{
  std::vector<std::string> known_keys = {
    "keyA",
    "keyB",
    "fooB",
    "barF"
  };
  ConfigFile config(known_keys);
  SECTION("key=value inside a comment should be ignored") {
    const std::string path = createFile({ "# keyA=foo", "keyA=bar", "# unknown=foo" });
    REQUIRE_NOTHROW(config.open(path));
  }
  SECTION("empty line is not a syntax error") {
    const std::string path = createFile({ "# comment", "", "# comment", "", "", "fooB=bar" });
    REQUIRE_NOTHROW(config.open(path));
  }
  SECTION("unknown key=value should trigger an error") {
    const std::string path = createFile({ "unknown=foo", "keyA=bar" });
    REQUIRE_THROWS(config.open(path));
  }
  SECTION("known key=value should not trigger an error") {
    const std::string path = createFile({ "keyA=foo", "keyB=bla", "fooB=bar", "barF=asdf" });
    REQUIRE_NOTHROW(config.open(path));
  }
  SECTION("syntax error should trigger an error") {
    const std::string path = createFile({ "keyA=foo", "adfsdlfkj", "keyB=b" });
    REQUIRE_THROWS(config.open(path));
  }
  SECTION("trim value") {
    const std::string path = createFile({ "keyA= foo", "keyB=bar" });
    REQUIRE_NOTHROW(config.open(path));
    REQUIRE(config.getSettingValue("keyA") == "foo");
  }
}

/* vim: set ts=2 sw=2 et */
