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

#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/KeyValueParser.hpp"

#include "Common/Utility.hpp"
#include "usbguard/Exception.hpp"
#include "usbguard/Logger.hpp"

#include "KeyValueParserPrivate.hpp"

#include <locale>

namespace usbguard
{
  KeyValueParser::KeyValueParser(const std::vector<std::string>& v, const std::string& sep, bool case_sensitive,
    bool validate_keys):
    d_pointer(usbguard::make_unique<KeyValueParserPrivate>(*this, v, sep, case_sensitive, validate_keys))
  {
  }

  KeyValueParser::KeyValueParser(const std::vector<std::string>& v, bool case_sensitive, bool validate_keys):
    d_pointer(usbguard::make_unique<KeyValueParserPrivate>(*this, v, "=", case_sensitive, validate_keys))
  {
  }

  void KeyValueParser::viewConfig()
  {
    d_pointer->viewConfig();
  }

  KeyValueParser::~KeyValueParser() = default;

  std::pair<std::string, std::string> KeyValueParser::parseLine(std::string& str)
  {
    return d_pointer->parseLine(str);
  }

  void KeyValueParser::parseStream(std::istream& stream)
  {
    d_pointer->parseStream(stream);
  }

  std::map<std::string, std::string> KeyValueParser::getMap()
  {
    return d_pointer->getMap();
  }

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
