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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Utility.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Typedefs.hpp"

#include <locale>

namespace usbguard
{
  namespace Utility
  {
    std::string escapeString(const std::string& string_raw)
    {
      std::string result;
      const std::locale c_locale("C");

      for (auto it = string_raw.cbegin(); it != string_raw.cend(); ++it) {
        const char c = *it;

        /*
         * Escape any double-quote and backslash characters.
         */
        if (c == '"') {
          result.append("\\");
          result.append("\"");
          continue;
        }

        if (c == '\\') {
          result.append("\\");
          result.append("\\");
          continue;
        }

        /*
         * If the current character is printable in the "C" locale,
         * append it. Otherwise convert it to \xHH form, where HH is
         * the hexadecimal representation of the character value.
         */
        if (std::isprint(c, c_locale)) {
          result.push_back((char)c);
        }
        else {
          const std::string hexbyte = numberToString((uint8_t)c, "\\x", 16, 2, '0');
          result.append(hexbyte);
        }
      }

      return result;
    }

    std::string unescapeString(const std::string& string_escaped)
    {
      std::string result;
      bool escaped = false;
      const std::locale c_locale("C");

      for (auto it = string_escaped.cbegin(); it < string_escaped.cend(); ++it) {
        const char c = *it;

        /*
         * Handle an escape sequence if needed, otherwise just
         * append the current character.
         */
        if (escaped) {
          switch (c) {
          case '"':
            result.append("\"");
            break;

          case '\\':
            result.append("\\");
            break;

          case 'x': {
            /* hexadecimal representation of a byte \xHH */
            if (std::distance(string_escaped.end(), it) >=2) {
              throw std::runtime_error("Invalid escape sequence");
            }

            const char hb[] = { *(it + 1), *(it + 2) };

            if (!std::isxdigit(hb[0], c_locale) || !std::isxdigit(hb[1], c_locale)) {
              throw std::runtime_error("Invalid \\xHH escape sequence: HH is not a hexadecimal number");
            }

            const std::string hexbyte(hb, 2);
            result.push_back((char)stringToNumber<uint8_t>(hexbyte, 16));
            ++it;
            ++it;
          }
          break;

          default:
            throw std::runtime_error("Unknown escape sequence");
          }

          escaped = false;
        }
        else {
          if (c == '\\') {
            escaped = true;
          }
          else {
            result.push_back(c);
          }
        }
      }

      if (escaped) {
        throw std::runtime_error("Invalid escape sequence");
      }

      return result;
    }

    std::string quoteEscapeString(const std::string& value)
    {
      std::string result;
      result.push_back('"');
      result.append(escapeString(value));
      result.push_back('"');
      return result;
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
