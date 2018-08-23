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
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Typedefs.hpp"

#include <tao/pegtl.hpp>

namespace usbguard
{
  class UEvent;

  namespace UEventParser
  {
    using namespace tao::pegtl;

    struct value
      : seq<not_one<'\0', '\n'>, star<not_one<'\0', '\n'>>> {};

    struct key
      : seq<ranges<'A', 'Z'>, star<ranges<'A', 'Z', '0', '9'>>> {};

    struct attribute
      : seq<key, one<'='>, value> {};

    struct attributes
      : list<attribute, one<'\0', '\n'>> {};

    struct action
      : seq<ranges<'a', 'z'>, star<ranges<'a', 'z'>>> {};

    struct devpath
      : seq<ascii::print, star<ascii::print>> {};

    struct header
      : seq<action, one<'@'>, devpath, one<'\0'>> {};

    struct uevent
      : seq<header, attributes> {};

    struct grammar
      : until<eof, uevent> {};

  } /* namespace UEventParser */

  void parseUEventFromFile(const std::string& uevent_path, UEvent& uevent, bool attributes_only = false, bool trace = false);
  void parseUEventFromString(const std::string& uevent_string, UEvent& uevent, bool attributes_only = false, bool trace = false);
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
