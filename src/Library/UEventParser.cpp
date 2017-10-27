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

#include "UEventParser.hpp"
#include "UEvent.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Logger.hpp"

#include <fstream>
#include <pegtl/trace.hh>

namespace usbguard
{
  namespace UEventParser
  {
    template<typename Rule>
    struct actions
      : pegtl::nothing<Rule> {};

    template<>
    struct actions<attribute> {
      template<typename Input>
      static void apply(const Input& in, UEvent& uevent)
      {
        try {
          const auto p = in.string().find_first_of('=');

          if (p == std::string::npos || p == (in.string().size() - 1)) {
            throw pegtl::parse_error("invalid attribute format", in);
          }

          const std::string key = in.string().substr(0, p);
          const std::string value = trim(in.string().substr(p + 1, std::string::npos), std::string("\n\0", 2));

          for (const std::string header_key : {
          "ACTION", "DEVPATH"
        }) {
            if (key == header_key) {
              if (value != uevent.getAttribute(header_key)) {
                throw pegtl::parse_error("header value mismatch", in);
              }
            }
          }
          uevent.setAttribute(key, value);
        }
        catch (const pegtl::parse_error& ex) {
          throw;
        }
        catch (const std::exception& ex) {
          throw pegtl::parse_error(ex.what(), in);
        }
      }
    };

    template<>
    struct actions<action> {
      template<typename Input>
      static void apply(const Input& in, UEvent& uevent)
      {
        uevent.setAttribute("ACTION", in.string());
      }
    };

    template<>
    struct actions<devpath> {
      template<typename Input>
      static void apply(const Input& in, UEvent& uevent)
      {
        uevent.setAttribute("DEVPATH", in.string());
      }
    };
  } /* namespace UEventParser */

  void parseUEventFromFile(const std::string& uevent_path, UEvent& uevent, bool attributes_only, bool trace)
  {
    std::ifstream uevent_stream(uevent_path);

    if (uevent_stream.good()) {
      std::string uevent_string(4096, 0);
      uevent_stream.readsome(&uevent_string[0], uevent_string.capacity());
      const auto read_size = uevent_stream.gcount();

      if (read_size > 0) {
        uevent_string.resize(read_size);
        parseUEventFromString(uevent_string, uevent, attributes_only, trace);
      }
    }
    else {
      throw std::runtime_error("failed to open uevent source file");
    }
  }

  template<class G>
  void parseUEventFromString(const std::string& uevent_string, UEvent& uevent, bool trace)
  {
    try {
#if HAVE_PEGTL_LTE_1_3_1

      if (!trace) {
        pegtl::parse<G, UEventParser::actions>(uevent_string, std::string(), uevent);
      }
      else {
        pegtl::parse<G, UEventParser::actions, pegtl::tracer>(uevent_string, std::string(), uevent);
      }

#else

      if (!trace) {
        pegtl::parse_string<G, UEventParser::actions>(uevent_string, std::string(), uevent);
      }
      else {
        pegtl::parse_string<G, UEventParser::actions, pegtl::tracer>(uevent_string, std::string(), uevent);
      }

#endif
    }
    catch (...) {
      throw;
    }
  }

  void parseUEventFromString(const std::string& uevent_string, UEvent& uevent, bool attributes_only, bool trace)
  {
    if (attributes_only) {
      parseUEventFromString<UEventParser::attributes>(uevent_string, uevent, trace);
    }
    else {
      parseUEventFromString<UEventParser::grammar>(uevent_string, uevent, trace);
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
