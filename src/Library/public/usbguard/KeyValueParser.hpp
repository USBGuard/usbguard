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

#include <iostream>
#include <vector>
#include <string>
#include <istream>
#include <map>
#include <memory>
#include <locale>

#include "usbguard/Typedefs.hpp"

namespace usbguard
{
  class KeyValueParserPrivate;

  /**
   * @brief Serves as a config file parser.
   */
  class DLL_PUBLIC KeyValueParser
  {
  public:
    /**
     * @brief Constructs object of KeyValueParser.
     *
     * @param v Vector of known key names.
     * @param case_sensitive Determines if key names should be parsed as
     *  case sensitive. If enabled, then all key names in vector \p v must
     *  be upper case.
     * @param validate_keys When enabled, every key that is parsed must be
     *  known (present in vector of known key names).
     */
    KeyValueParser(const std::vector<std::string>& v, bool case_sensitive, bool validate_keys);

    /**
     * @brief Constructs object of KeyValueParser.
     *
     * @param v Vector of known key names.
     * @param sep Separator of keys and values in key-value pairs.
     * @param case_sensitive Determines if key names should be parsed as
     *  case sensitive. If enabled, then all key names in vector \p v must
     *  be upper case.
     * @param validate_keys When enabled, every key that is parsed must be
     *  known (present in vector of known key names).
     */
    KeyValueParser(const std::vector<std::string>& v, const std::string& sep, bool case_sensitive, bool validate_keys);

    /**
     * @brief Default destructor of KeyValueParser.
     */
    ~KeyValueParser();

    /**
     * @brief Retreives key-value pair from given string.
     *
     * This function serves to parse single line of config file
     * and return key-value pair. If given string does not contain
     * valid key-value pair, then exception is thrown.
     *
     * @param str String to be parsed for key-value pair.
     * @return Key-value pair present in given string.
     */
    std::pair<std::string, std::string> parseLine(std::string& str);

    /**
     * @brief Parses given stream for key-value pairs.
     *
     * Given stream is parsed line by line using function parseLine.
     * Lines that start with \p # are ignored. Parsed key-value pairs
     * can be retreived using function \link getMap() getMap()\endlink.
     *
     * @see \link parseLine() parseLine()\endlink
     * @see \link getMap() getMap()\endlink
     * @param stream Stream to be parsed.
     */
    void parseStream(std::istream& stream);

    /**
     * @brief Retreives map of key-value pairs.
     *
     * This map is filled by function \link parseStream()
     * parseStream()\endlink.
     *
     * @see \link parseStream() parseStream()\endlink
     * @return Map of key-value pairs
     */
    std::map<std::string, std::string> getMap();

    /**
     * @brief Prints key-value separator and known key names into the log.
     */
    void viewConfig();

  private:
    std::unique_ptr<KeyValueParserPrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
