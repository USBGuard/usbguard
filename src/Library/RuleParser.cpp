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
#include "RuleParser.hpp"
#include "RuleParser/Lexer.hpp"
#include "Typedefs.hpp"

#include "RulePrivate.hpp"
#include "USB.hpp"
#include "Lexer.hpp"
#include "Common/Utility.hpp"
#include "LoggerPrivate.hpp"

#include <cstddef>
#include <stdexcept>
#include <stdlib.h>
#include <sstream>
#include <cassert>

namespace usbguard
{
  struct RuleParserState
  {
    RuleParserState(const String& rule_spec)
      : error(rule_spec)
      {}
    Rule rule;
    RuleParserError error;
  };

#include "RuleParser/Parser.c"

  static void RuleParserDeleter(void *p)
  {
    RuleParserFree(p, &free);
  }

  Rule parseRuleSpecification(const String& rule_spec, const std::string * const file, unsigned int line)
  {
    std::istringstream stream(rule_spec);
    UniquePointer<void,void(*)(void *)> parser_data(RuleParserAlloc(&malloc), RuleParserDeleter);

    logger->debug("Trying to parse rule: \"{}\"", rule_spec);

    try {
      RuleParserState state(rule_spec);
      quex::Lexer lexer(&stream);
      quex::Token *token_ptr = nullptr;

#ifndef NDEBUG
      RuleParserTrace(stderr, (char*)"RuleParser:");
#endif
      for (;;) {
        lexer.receive(&token_ptr);
        if (token_ptr->type_id() != RULE_TOKEN_TERMINATION) {
          RuleParser(parser_data.get(), token_ptr->type_id(), new QUEX_TYPE_TOKEN(*token_ptr), &state);
        } else {
          RuleParser(parser_data.get(), 0, nullptr, &state);
          break;
        }
      }

      return std::move(state.rule);
    }
    catch(RuleParserError& ex) {
      logger->debug("Caught RuleParserError: {}", ex.what());
      /*
       * If the caller provided a file context, add it to the
       * exception.
       */
      if (file != nullptr) {
        ex.setFileInfo(*file, line);
      }
      throw ex;
    }
    catch(const std::exception& ex) {
      logger->debug("Caught std::exception: {}", ex.what());
      throw;
    }

    throw std::runtime_error("BUG in parseRuleSpecification");
  }
} /* namespace usbguard */
