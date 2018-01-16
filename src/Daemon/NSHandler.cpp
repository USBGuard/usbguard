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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <locale>

#include "NSHandler.hpp"

#include "FileRuleSet.hpp"
#include "LDAPRuleSet.hpp"

#include "usbguard/Exception.hpp"
#include "usbguard/KeyValueParser.hpp"
#include "usbguard/Logger.hpp"
#include "usbguard/RuleSet.hpp"

namespace usbguard
{

  NSHandler::NSHandler()
    : _parser( {"usbguard"}, ":", /*case_sensitive?*/false, /*validate_keys?*/false),
  _nsswitch_path("/etc/nsswitch.conf"),
  _rulesPath(""),
  _ldap(nullptr)
  {
    USBGUARD_LOG(Info) << "NSHandler Loading...";
    _source = SourceType::LOCAL;
    _parser.viewConfig();
    USBGUARD_LOG(Info) << "NSHandler Loaded";
  }

  NSHandler::~NSHandler()
  {
  }

  void NSHandler::setNSSwitchPath(const std::string& path)
  {
    _nsswitch_path = path;
  }

  std::string NSHandler::getSourceInfo()
  {
    std::string ret = "unspecified";

    switch (_source) {
    case NSHandler::SourceType::LOCAL:
      if (_rulesPath != "") {
        ret = "SourceLOCAL::" + _rulesPath;
      }
      else {
        ret = "SourceLOCAL::uknown";
      }

      break;

    case NSHandler::SourceType::LDAP:
      ret = "SourceLDAP";
      break;

    // case NSHandler::SourceType::SSSD:
    //   ret = "SourceSSSD";
    //   break;

    default:
      ret = "SourceUnknown";
      break;
    }

    return ret;
  }

  std::shared_ptr<RuleSet> NSHandler::generateMEMRuleSet(Interface* const interface_ptr)
  {
    auto rule_set = std::make_shared<RuleSet>(interface_ptr);
    return rule_set;
  }

  std::shared_ptr<RuleSet> NSHandler::generateLOCAL(Interface* const interface_ptr)
  {
    auto rule_set = std::make_shared<FileRuleSet>(interface_ptr, _rulesPath);
    return std::dynamic_pointer_cast<RuleSet>(rule_set);
  }

  std::shared_ptr<RuleSet> NSHandler::generateLDAP(Interface* const interface_ptr)
  {
    _ldap = std::make_shared<LDAPHandler>();
    auto rule_set = std::make_shared<LDAPRuleSet>(interface_ptr, _ldap);
    return std::dynamic_pointer_cast<RuleSet>(rule_set);
  }

  // std::shared_ptr<RuleSet> NSHandler::generateSSSD(Interface* const interface_ptr)
  // {
  //   auto rule_set = std::make_shared<RuleSet>(interface_ptr);
  //   return rule_set;
  // }

  std::shared_ptr<RuleSet> NSHandler::getRuleSet(Interface* const interface_ptr)
  {
    switch (_source) {
    case SourceType::LOCAL:
      if (_rulesPath != "") {
        return generateLOCAL(interface_ptr);
      }
      else {
        USBGUARD_LOG(Warning) << "RuleFile not set; Modification of the permanent policy won't be possible.";
        return generateMEMRuleSet(interface_ptr);
      }

      break;

    case SourceType::LDAP:
      return generateLDAP(interface_ptr);
      break;

    // case SourceType::SSSD:
    //   return generateSSSD(interface_ptr);
    //   break;

    default:
      return generateMEMRuleSet(interface_ptr);
      break;
    }

    return std::shared_ptr<RuleSet>(nullptr);
  }

  void NSHandler::setRulesPath(const std::string& path)
  {
    _rulesPath = path;
  }

  void NSHandler::parseNSSwitch()
  {
    USBGUARD_LOG(Info) << "Loading nsswitch from " << _nsswitch_path;
    std::ifstream nss(_nsswitch_path);

    if (!nss.is_open()) {
      throw ErrnoException("NSSwitch parsing", _nsswitch_path, errno);
    }

    _parser.parseStream(nss);
    _parsedOptions = _parser.getMap();
    nss.close();
    USBGUARD_LOG(Debug) << "Map contains:";

    for (auto x: _parsedOptions) {
      USBGUARD_LOG(Debug) << "--> " << x.first << " -> " << x.second << " <--";
    }

    std::locale loc;

    for (unsigned i = 0 ; i < _parsedOptions["USBGUARD"].length() ; i++) {
      _parsedOptions["USBGUARD"][i] = std::toupper(_parsedOptions["USBGUARD"][i], loc);
    }

    USBGUARD_LOG(Info) << "Fetched value is -> " << _parsedOptions["USBGUARD"] << " <-";

    if (_parsedOptions["USBGUARD"] == "FILES") {
      _source = SourceType::LOCAL;
    }
    else if (_parsedOptions["USBGUARD"] == "LDAP") {
      _source = SourceType::LDAP;
    }
    else {
      USBGUARD_LOG(Info) << "Value is not valid or not set, using default FILES";
      _source = SourceType::LOCAL;
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
