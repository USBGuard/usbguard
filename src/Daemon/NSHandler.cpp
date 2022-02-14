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

#ifdef HAVE_LDAP
  #include "LDAPRuleSet.hpp"
#endif

#include "usbguard/Exception.hpp"
#include "usbguard/KeyValueParser.hpp"
#include "usbguard/Logger.hpp"
#include "usbguard/RuleSet.hpp"
#include "usbguard/MemoryRuleSet.hpp"

#include "RuleSetFactory.hpp"

namespace usbguard
{

  NSHandler* NSHandler::_self = nullptr;

  NSHandler::NSHandler()
    : _parser( {"usbguard"}, ":", /*case_sensitive?*/false, /*validate_keys?*/false),
  _nsswitch_path("/etc/nsswitch.conf"),
  _rulesPath("")
  {
    USBGUARD_LOG(Info) << "NSHandler Loading...";
    _source = SourceType::LOCAL;
    _parser.viewConfig();
    _self = this;
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
        ret = "SourceLOCAL::unknown";
      }

      break;
#ifdef HAVE_LDAP

    case NSHandler::SourceType::LDAP:
      ret = "SourceLDAP";
      break;
#endif

    // case NSHandler::SourceType::SSSD:
    //   ret = "SourceSSSD";
    //   break;

    default:
      ret = "SourceUnknown";
      break;
    }

    return ret;
  }

#if HAVE_LDAP
  std::shared_ptr<LDAPHandler> NSHandler::getLDAPHandler()
  {
    if (!_ldap.get()) {
      _ldap = _ldap = std::make_shared<LDAPHandler>();
    }

    return _ldap;
  }
#endif

  NSHandler& NSHandler::getRef()
  {
    if (!NSHandler::_self) {
      throw Exception("NSHandler", "getRef", "uninitialized static _self parameter");
    }

    return *NSHandler::_self;
  }

  std::vector<std::shared_ptr<RuleSet>> NSHandler::getRuleSet(Interface* const interface_ptr)
  {
    RuleSetFactory::setInterface(interface_ptr);
    return RuleSetFactory::generateRuleSetBySource(_source);
  }

  void NSHandler::setRulesPath(const std::string& path)
  {
    _rulesPath = path;
  }

  void NSHandler::setRulesDirPath(std::string& ruledir_path)
  {
    _rulesDirPath = ruledir_path;
  }

  std::string& NSHandler::getRulesPath()
  {
    return _rulesPath;
  }

  std::string& NSHandler::getRulesDirPath()
  {
    return _rulesDirPath;
  }

  void NSHandler::parseNSSwitch()
  {
    USBGUARD_LOG(Info) << "Loading nsswitch from " << _nsswitch_path;
    std::ifstream nss(_nsswitch_path);

    if (!nss.is_open()) {
      USBGUARD_LOG(Info) << "Error when opening nsswitch file: " << _nsswitch_path << ": " << ErrnoException::reasonFromErrno(errno);
      USBGUARD_LOG(Info) << "Using default value FILES";
      _source = SourceType::LOCAL;
      return;
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
#ifdef HAVE_LDAP
      _source = SourceType::LDAP;
#else
      USBGUARD_LOG(Info) << "USBGuard has been compiled without LDAP support";
      USBGUARD_LOG(Info) << "Using default value FILES";
      _source = SourceType::LOCAL;
#endif
    }
    else {
      USBGUARD_LOG(Info) << "Value is not valid or not set, using default FILES";
      _source = SourceType::LOCAL;
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
