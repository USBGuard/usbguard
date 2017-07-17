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
// Authors: Radovan Sroka <rsroka@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
#include <build-config.h>
#endif

#include <string>
#include <vector>

#include "usbguard/Typedefs.hpp"
#include "../RuleSet.hpp"

#include <pegtl.hh>

namespace usbguard {

  namespace NSSwitchParser {

    struct blanked
      : pegtl::star< pegtl::blank > {};

    struct prop_name
      : pegtl::istring< 'u', 's', 'b', 'g', 'u', 'a', 'r', 'd' > {};

    struct value
      : pegtl::plus< pegtl::alpha > {};

    struct grammar
      : pegtl::must< blanked, prop_name, blanked, pegtl::one< ':' >, blanked, value, blanked, pegtl::eof > {};

    template< typename Rule >
    struct action
      : pegtl::nothing< Rule > {};

    template<>
    struct action< value >
    {
       template < typename Input >
       static void apply(const Input & in, std::string & str )
       {
          std::locale loc;
          str = in.string();
          for (unsigned i = 0; i < str.length(); ++i)
          {
             str[i] = std::tolower(str[i], loc);
          }
       }
    };
  }

  class Interface;
  class DLL_PUBLIC NSHandler
    {
      public:

        enum class SourceType
        {
          LOCAL,
          LDAP,
          SSSD
        };

      NSHandler();

      void setRulesPath(const std::string& path);
      std::shared_ptr<RuleSet> getRuleSet(Interface * const interface_ptr);
      void parseNSSwitch();

      void setNSSwitchPath(const std::string& path);
      void setPropertyName(const std::string& name);

    private:
      std::string _prop_name;
      std::string _nsswitch_path;
      unsigned _num_possible_values;
      std::vector<std::string> _possible_values;

      SourceType _source;

      std::string _rulesPath;
    };
}

/* vim: set ts=2 sw=2 et */
