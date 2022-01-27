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
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "RuleParser.hpp"
#include "RuleParser/Grammar.hpp"
#include "RuleParser/Actions.hpp"
#include "RulePrivate.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/USB.hpp"
#include "usbguard/Logger.hpp"
#include "Common/Utility.hpp"

#include <cstddef>
#include <stdexcept>
#include <stdlib.h>

#if TAO_PEGTL_VERSION_MAJOR >= 3
  #include <tao/pegtl/contrib/trace.hpp>
#else
  #include <tao/pegtl/contrib/tracer.hpp>
#endif

namespace usbguard
{
  Rule parseRuleFromString(const std::string& rule_spec, const std::string& file, size_t line, bool trace)
  {
    try {
      Rule rule;
      tao::pegtl::string_input<> input(rule_spec, file);

      if (!trace) {
        tao::pegtl::parse<RuleParser::rule_grammar, RuleParser::rule_parser_actions>(input, rule);
      }
      else {
#if TAO_PEGTL_VERSION_MAJOR >= 3
        tao::pegtl::complete_trace<RuleParser::rule_grammar, RuleParser::rule_parser_actions>(input, rule);
#else
        tao::pegtl::parse<RuleParser::rule_grammar, RuleParser::rule_parser_actions, tao::pegtl::tracer>(input, rule);
#endif
      }

      return rule;
    }
    catch (const tao::pegtl::parse_error& ex) {
      RuleParserError error(rule_spec);
      error.setHint(ex.what());
#if TAO_PEGTL_VERSION_MAJOR >= 3
      error.setOffset(ex.positions().front().column);
#else
      error.setOffset(ex.positions[0].byte_in_line);
#endif

      if (!file.empty() || line != 0) {
        error.setFileInfo(file, line);
      }

      throw error;
    }
    catch (const std::exception& ex) {
      throw;
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
