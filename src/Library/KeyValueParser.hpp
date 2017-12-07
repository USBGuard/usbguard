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
// Authors: Marek Tamaskovic <mtamasko@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <memory>

namespace usbguard
{

  struct parsed_t {
    const std::string lvalue;
    const std::string rvalue;
    parsed_t(const std::string a, const std::string b): lvalue(a), rvalue(b) {};
  };

  class KeyValueParser
  {
    std::vector<std::string> keys;
    std::string separator {""};
    std::map<std::string, std::string> output_map;
    bool checkKeyValidity(const std::string& key);
    virtual bool checkMapValidity();

  public:
    KeyValueParser(const std::vector<std::string>& v);
    KeyValueParser(const std::vector<std::string>& v, const std::string& sep);
    std::unique_ptr<struct parsed_t> parseString(std::string& str);
    bool parseStream(std::fstream& stream);
    std::map<std::string, std::string> getMap();
    void viewConfig();
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */

