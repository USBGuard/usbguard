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

#include "usbguard.hpp"
#include "usbguard-generate-policy.hpp"
#include "PolicyGenerator.hpp"
#include "Common/Utility.hpp"

namespace usbguard
{
  int usbguard_generate_policy(int argc, char **argv)
  {
    bool port_specific = true;
    bool with_catchall = false;
    std::string catchall_target = "block";
    bool with_hashes = true;
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
} /* namespace usbguard */
