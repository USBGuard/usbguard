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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "KeyValueParser.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Exception.hpp"
#include "usbguard/Logger.hpp"

namespace usbguard
{
  KeyValueParser::KeyValueParser(const std::vector<std::string>& v, const std::string& sep):
    keys(v), separator(sep) {}

  KeyValueParser::KeyValueParser(const std::vector<std::string>& v):
    KeyValueParser(v, "=") {}

  void KeyValueParser::viewConfig()
  {
    std::cout << "separator:\t" << this->separator << std::endl;
    std::cout << "keys:\t";

    for (auto item : this->keys) {
      std::cout << item << ", ";
    }

    std::cout << "\b\b " << std::endl;
  }

  std::unique_ptr<struct parsed_t> KeyValueParser::parseString(std::string& str)
  {
    std::string::size_type sep_pos;
    std::string key_c, key, val;
    sep_pos = str.find(this->separator);

    if (sep_pos == std::string::npos) {
      throw Exception("KeyValueParser", "Parser", "Separator not found: syntax error");
    }
    else {
      // std::cout << "found at: " << str.substr(sep_pos) << " " << sep_pos << std::endl;
      key = str.substr(0, sep_pos);
      val = str.substr(sep_pos + 1);
      key = trim(key);
      val = trim(val);

      // USBGUARD_LOG(Error) << "\""<< key <<"\"" << "\""<< val <<"\"";

      // std::cout << "parsed:  '" << key << "'='" << val << "'" << std::endl;

      if (this->checkKeyValidity(key)) {
        USBGUARD_LOG(Error) << "Error: parsed key is not in key set: '" << key << "'";
        throw Exception("KeyValueParser", "Parser", "Invalid key");
      }
      else {
        std::unique_ptr<struct parsed_t> p (new struct parsed_t(key, val));
        return p;
      }
    }

    return nullptr;
  }

  bool KeyValueParser::parseStream(std::fstream& stream)
  {
    std::map<std::string, std::string> m;
    std::map<std::string, std::string>::iterator it;
    std::string line;

    while ( getline (stream, line) ) {
      if ((line.size() < 1) || (line[0] == '#')) {
        continue;
      }

      auto p = this->parseString(line);

      if (p != nullptr) {
        it = m.find(p->lvalue);

        if (it != m.end()) {
          m.emplace(p->lvalue, p->rvalue);
        }
        else {
          m[p->lvalue] = p->rvalue;
        }
      }
    }

    this->output_map = std::move(m);

    if (this->checkMapValidity()) {
      return false;
    }
    else {
      return true;
    }
  }

  bool KeyValueParser::checkKeyValidity(const std::string& key)
  {
    for (auto a: this->keys) {
      if (!key.compare(a)) {
        return false;
      }
    }

    return 1;
  }

  bool KeyValueParser::checkMapValidity()
  {
    USBGUARD_LOG(Error) << "ERROR: Override KeyValueParser::checkMapValidity() method!" << std::endl;
    return false;
  }

  std::map<std::string, std::string> KeyValueParser::getMap()
  {
    return this->output_map;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */