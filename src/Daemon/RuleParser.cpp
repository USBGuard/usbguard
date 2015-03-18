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
#include <build-config.h>

#include "RuleParser.hpp"
#include "RuleParser/Lexer.hpp"

#include "Common/Types.hpp"
#include "Common/Logging.hpp"

#include <stdexcept>
#include <stdlib.h>
#include <sstream>
#include <cassert>

namespace usbguard
{
#include "RuleParser/Parser.c"

  static void RuleParserDeleter(void *p)
  {
    RuleParserFree(p, &free);    
  }

  Firewall::Rule parseRuleSpecification(const String& rule_spec)
  {
    Firewall::Rule rule;
    std::istringstream ss(rule_spec);
    quex::Lexer lexer(&ss);
    quex::Token *token_ptr = nullptr;
    UniquePointer<void,void(*)(void *)> parser_data(RuleParserAlloc(&malloc),
						    RuleParserDeleter);

    log->debug("Parsing rule: '{}'", rule_spec);

    for (;;) {
      lexer.receive(&token_ptr);
      if (token_ptr->type_id() != RULE_TOKEN_TERMINATION) {
	RuleParser(parser_data.get(),
		   token_ptr->type_id(), new QUEX_TYPE_TOKEN(*token_ptr), &rule);
      } else {
	RuleParser(parser_data.get(), 0, nullptr, &rule);
	break;
      }
    }

    return std::move(rule);
  }

} /* namespace usbguard */
