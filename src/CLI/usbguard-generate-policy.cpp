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
#include <iostream>
#include <DeviceManager.hpp>
#include <unistd.h>

#include "PolicyGenerator.hpp"
#include "Common/Utility.hpp"

using namespace std;
using namespace usbguard;

static void printUsage(std::ostream& stream, const char *arg0)
{
  stream << std::endl;
  stream << "Usage: " << filenameFromPath(arg0) << " [OPTIONS]" << std::endl;
  stream << std::endl;
  stream << "\t -P           " << "Don't generate port specific rules for devices without an iSerial value." << std::endl;
  stream << "\t -t <target>  " << "Generate an explicit \"catch all\" rule with the specified target." << std::endl;
  stream << "\t -H           " << "Don't include the hash attribute in the generated rules." << std::endl;
  stream << "\t -h           " << "Show this help screen." << std::endl;
  stream << std::endl;
  return;
}

int main(int argc, char *argv[])
{
  int opt;
  bool port_specific = true;
  bool with_catchall = false;
  std::string catchall_target = "block";
  bool with_hashes = true;

  while ((opt = ::getopt(argc, argv, "Pt:H")) != -1) {
    switch (opt) {
    case 'P':
      port_specific = false;
      break;
    case 't':
      with_catchall = true;
      catchall_target = optarg;
      break;
    case 'H':
      with_hashes = false;
      break;
    case 'h':
      printUsage(std::cout, argv[0]);
      return EXIT_SUCCESS;
    default: /* '?' */
      printUsage(std::cerr, argv[0]);
      return EXIT_FAILURE;
    }
  }

  PolicyGenerator generator;

  generator.setWithHashAttribute(with_hashes);
  generator.setPortSpecificRules(port_specific);
  generator.setExplicitCatchAllRule(with_catchall,
				    Rule::targetFromString(catchall_target));
  generator.generate();
  const RuleSet& ruleset = generator.refRuleSet();

  ruleset.save(std::cout);

  return EXIT_SUCCESS;
}
