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
#include "usbguard/IPCServer.hpp"

#include <catch.hpp>
#include <sstream>

using namespace usbguard;

TEST_CASE("IPCServer::AccessControl", "[Unit]")
{
  SECTION("default constructed") {
    IPCServer::AccessControl ac;
    REQUIRE_THROWS(ac.hasPrivilege(IPCServer::AccessControl::Section::ALL,
        IPCServer::AccessControl::Privilege::LIST));

    for (const auto& section : {
        IPCServer::AccessControl::Section::DEVICES,
        IPCServer::AccessControl::Section::POLICY,
        IPCServer::AccessControl::Section::PARAMETERS,
        IPCServer::AccessControl::Section::EXCEPTIONS
      }) {
      for (const auto& privilege : {
          IPCServer::AccessControl::Privilege::MODIFY,
          IPCServer::AccessControl::Privilege::LIST,
          IPCServer::AccessControl::Privilege::LISTEN
        }) {
        REQUIRE(!ac.hasPrivilege(section, privilege));
      }
    }
  }
  SECTION("constructed") {
    IPCServer::AccessControl ac(IPCServer::AccessControl::Section::DEVICES,
      IPCServer::AccessControl::Privilege::LIST);
    REQUIRE(ac.hasPrivilege(IPCServer::AccessControl::Section::DEVICES,
        IPCServer::AccessControl::Privilege::LIST));

    for (const auto& section : {
        IPCServer::AccessControl::Section::POLICY,
        IPCServer::AccessControl::Section::PARAMETERS,
        IPCServer::AccessControl::Section::EXCEPTIONS
      }) {
      for (const auto& privilege : {
          IPCServer::AccessControl::Privilege::MODIFY,
          IPCServer::AccessControl::Privilege::LIST,
          IPCServer::AccessControl::Privilege::LISTEN
        }) {
        REQUIRE(!ac.hasPrivilege(section, privilege));
      }
    }
    REQUIRE_NOTHROW(ac.clear());
    REQUIRE(!ac.hasPrivilege(IPCServer::AccessControl::Section::DEVICES,
        IPCServer::AccessControl::Privilege::LIST));
  }
  SECTION("parsed") {
    std::stringstream ss;
    ss << "Devices=list listen\n";
    ss << "Policy=list modify\n";
    IPCServer::AccessControl ac;
    REQUIRE_NOTHROW(ac.load(ss));
    REQUIRE(ac.hasPrivilege(IPCServer::AccessControl::Section::DEVICES,
        IPCServer::AccessControl::Privilege::LIST));
    REQUIRE(ac.hasPrivilege(IPCServer::AccessControl::Section::DEVICES,
        IPCServer::AccessControl::Privilege::LISTEN));
    REQUIRE(!ac.hasPrivilege(IPCServer::AccessControl::Section::DEVICES,
        IPCServer::AccessControl::Privilege::MODIFY));
    REQUIRE(ac.hasPrivilege(IPCServer::AccessControl::Section::POLICY,
        IPCServer::AccessControl::Privilege::LIST));
    REQUIRE(!ac.hasPrivilege(IPCServer::AccessControl::Section::POLICY,
        IPCServer::AccessControl::Privilege::LISTEN));
    REQUIRE(ac.hasPrivilege(IPCServer::AccessControl::Section::POLICY,
        IPCServer::AccessControl::Privilege::MODIFY));
    REQUIRE(!ac.hasPrivilege(IPCServer::AccessControl::Section::PARAMETERS,
        IPCServer::AccessControl::Privilege::LIST));
    REQUIRE(!ac.hasPrivilege(IPCServer::AccessControl::Section::PARAMETERS,
        IPCServer::AccessControl::Privilege::LISTEN));
    REQUIRE(!ac.hasPrivilege(IPCServer::AccessControl::Section::PARAMETERS,
        IPCServer::AccessControl::Privilege::MODIFY));
  }
  SECTION("parse error") {
    std::stringstream ss;
    ss << "Devices=lisp drog listne\n";
    ss << "Pulicy=list listen\n";
    IPCServer::AccessControl ac;
    REQUIRE_THROWS(ac.load(ss));
  }
}

/* vim: set ts=2 sw=2 et */
