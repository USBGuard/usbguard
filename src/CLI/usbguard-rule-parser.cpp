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
#include <iostream>
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <cstring>
#include "Rule.hpp"
#include "RuleParser.hpp"

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << ::basename(argv[0]) << " <rulespec>" << std::endl;
  }
  else {
    try {
      const std::string rule_spec(argv[1]);
      std::cout << "INPUT: " << rule_spec << std::endl;
      const usbguard::Rule rule = usbguard::parseRuleSpecification(rule_spec);
      std::cout << "OUTPUT: " << rule.toString() << std::endl;
      return EXIT_SUCCESS;
    }
    catch(const usbguard::RuleParserError& ex) {
      std::cerr << "ERROR: " << ex.what() << std::endl;
      std::cerr << "HINT: " << ex.hint() << std::endl;
      if (ex.hasFileInfo()) {
        std::cerr << "FILE: " << ex.fileInfo() << std::endl;
      }
    }
    catch(const std::exception& ex) {
      std::cerr << "EXCEPTION: " << ex.what() << std::endl;
    }
    catch(...) {
      std::cerr << "EXCEPTION: Unknown" << std::endl;
    }
  }
  return EXIT_FAILURE;
}
