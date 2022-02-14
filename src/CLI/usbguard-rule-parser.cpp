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

#include "usbguard/Exception.hpp"
#include "usbguard/Rule.hpp"
#include "usbguard/RuleParser.hpp"

#ifdef HAVE_GNU_BASENAME
  /* GNU version of basename(3) */
  #ifndef _GNU_SOURCE
    #define _GNU_SOURCE
  #endif
  #include <cstring>
#else
  /* POSIX version of basename(3) */
  #include <libgen.h>
#endif

#include <cstdlib>  // for free(3)
#include <cstring>  // for strdup(3)
#include <iostream>
#include <fstream>

#include <getopt.h>

static const char* options_short = "hft";

static const struct ::option options_long[] = {
  { "help", no_argument, nullptr, 'h' },
  { "file", no_argument, nullptr, 'f' },
  { "trace", no_argument, nullptr, 't' },
  { nullptr, 0, nullptr, 0 }
};

static void showHelp(std::ostream& stream, const char* usbguard_arg0)
{
  char* const writeable_arg0 = ::strdup(usbguard_arg0);
  stream << " Usage: " << ::basename(writeable_arg0) << " [OPTIONS] <rule_spec>" << std::endl;
  stream << " Usage: " << ::basename(writeable_arg0) << " [OPTIONS] -f <file>" << std::endl;
  stream << std::endl;
  stream << " Options:" << std::endl;
  stream << "  -f, --file       Interpret the argument as a path to a file that should be parsed." << std::endl;
  stream << "  -t, --trace      Enable parser tracing." << std::endl;
  stream << "  -h, --help       Show this help." << std::endl;
  stream << std::endl;
  ::free(writeable_arg0);
}

int main(int argc, char** argv)
{
  const char* usbguard_arg0 = argv[0];
  bool trace = false;
  bool from_file = false;
  int opt = 0;

  while ((opt = getopt_long(argc, argv, options_short, options_long, nullptr)) != -1) {
    switch (opt) {
    case 'h':
      showHelp(std::cout, usbguard_arg0);
      return EXIT_SUCCESS;

    case 'f':
      from_file = true;
      break;

    case 't':
      trace = true;
      break;

    case '?':
    default:
      showHelp(std::cout, usbguard_arg0);
      return EXIT_FAILURE;
    }
  }

  argc -= optind;
  argv += optind;

  if (argc != 1) {
    showHelp(std::cerr, usbguard_arg0);
    return EXIT_FAILURE;
  }

  std::string rule_spec;

  try {
    if (from_file) {
      const char* const rule_file_filename = argv[0];
      const std::string rule_file(rule_file_filename);
      std::ifstream stream(rule_file);

      if (! stream.is_open()) {
        throw usbguard::ErrnoException("Rules file could not be opened", rule_file_filename, errno);
      }

      size_t line = 0;

      while (stream.good()) {
        rule_spec.clear();
        std::getline(stream, rule_spec);
        ++line;

        if (rule_spec.empty()) {
          continue;
        }

        std::cout << "INPUT: " << rule_spec << std::endl;
        const usbguard::Rule rule = usbguard::parseRuleFromString(rule_spec, rule_file, line, trace);
        std::cout << "OUTPUT: " << rule.toString() << std::endl;
      }

      return EXIT_SUCCESS;
    }
    else {
      rule_spec = argv[0];
      std::cout << "INPUT: " << rule_spec << std::endl;
      const usbguard::Rule rule = usbguard::parseRuleFromString(rule_spec, "<argv>", 0, trace);
      std::cout << "OUTPUT: " << rule.toString() << std::endl;
      return EXIT_SUCCESS;
    }
  }
  catch (const usbguard::RuleParserError& ex) {
    std::cerr << "! ERROR: " << ex.what() << std::endl;
    std::cerr << "!!  " << rule_spec << std::endl;
    std::cerr << "!!  ";
    std::cerr.width(4 + ex.offset());
    std::cerr << "^-- " << ex.hint() << std::endl;
    std::cerr.width(1);
  }
  catch (const usbguard::Exception& ex) {
    std::cerr << "! ERROR: " << ex.message() << std::endl;
  }
  catch (const std::exception& ex) {
    std::cerr << "! EXCEPTION: " << ex.what() << std::endl;
  }
  catch (...) {
    std::cerr << "! EXCEPTION: Unknown" << std::endl;
  }

  return EXIT_FAILURE;
}

/* vim: set ts=2 sw=2 et */
