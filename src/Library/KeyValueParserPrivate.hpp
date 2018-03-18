//
// Copyright (C) 2015 Red Hat, Inc.
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
//          Marek Tamaskovic <mtamasko@redhat.com>
//          Radovan Sroka <rsroka@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <istream>
#include <map>
#include <memory>
#include <locale>

#include "usbguard/KeyValueParser.hpp"

namespace usbguard
{

  class KeyValueParserPrivate
  {
    std::vector<std::string> _keys;
    std::string _separator {""};
    std::map<std::string, std::string> _output_map;

    KeyValueParser& _p_instance;

    std::locale _loc;
    bool _case_sensitive;
    bool _validate_keys;

    bool checkKeyValidity(const std::string& key);

  public:
    KeyValueParserPrivate(KeyValueParser& p_instance, const std::vector<std::string>& v, bool case_sensitive, bool validate_keys);
    KeyValueParserPrivate(KeyValueParser& p_instance, const std::vector<std::string>& v, const std::string& sep,
      bool case_sensitive, bool validate_keys);
    std::pair<std::string, std::string> parseLine(std::string& str);
    void parseStream(std::istream& stream);
    std::map<std::string, std::string> getMap();
    void viewConfig();
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
