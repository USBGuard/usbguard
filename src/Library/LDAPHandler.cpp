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
#include <string.h>

#include "usbguard/Exception.hpp"
#include "usbguard/Logger.hpp"

#include "Common/LDAPUtil.hpp"
#include "LDAPHandler.hpp"

#include <ldap.h>
#include <unistd.h>

namespace usbguard
{

  std::map<std::string, bool> LDAPHandler::_configValues = {
    {"URI", false},
    {"ROOTDN", false},
    {"BASE", false},
    {"ROOTPW", false},
    {"USBGUARDBASE", false},
    {"RULEQUERY", false}
  };

  LDAPHandler::LDAPHandler()
    : _ldap_file("/etc/usbguard/usbguard-ldap.conf")
  {
    USBGUARD_LOG(Info) << "LDAPHandler Loading...";
    char array[HOST_NAME_MAX];
    int rc = gethostname(array, HOST_NAME_MAX);

    if (rc != -1) {
      _hostname = array;
    }
    else {
      /*TODO maybe some option for setting hostname explicitly
      * without hostname LDAP query will not work!
      */
      USBGUARD_LOG(Debug) << "Cannot get hostname";
    }

    USBGUARD_LOG(Debug) << "Hostname is: " << _hostname;
    parseConf(_ldap_file);
    validateConf();
    LDAP* ptr = nullptr;

    if (ldap_initialize(&ptr, _parsedOptions["URI"].c_str()) != LDAP_SUCCESS) {
      throw ErrnoException("LDAPHandler initialization", "ldap_initialize", errno);
    }

    _ldap_ptr.reset(ptr);
    int version = LDAP_VERSION3;
    ldap_set_option(_ldap_ptr.get(), LDAP_OPT_PROTOCOL_VERSION, &version);
    rc = LDAP_SUCCESS;
    struct berval passwd;
    passwd.bv_val = strdup(_parsedOptions["ROOTPW"].c_str());
    passwd.bv_len = _parsedOptions["ROOTPW"].length();
    rc = ldap_sasl_bind_s(_ldap_ptr.get(), _parsedOptions["ROOTDN"].c_str(), LDAP_SASL_SIMPLE,
        &passwd, nullptr, nullptr, nullptr);
    free(passwd.bv_val);

    if (rc != LDAP_SUCCESS) {
      throw Exception("LDAPHandler initialization", "ldap_sasl_bind_s", ldap_err2string(rc));
    }

    USBGUARD_LOG(Info) << "LDAPHandler Loaded";
  }

  LDAPHandler::~LDAPHandler()
  {
  }

  std::string LDAPHandler::getRuleQuery()
  {
    return _parsedOptions["RULEQUERY"];
  }

  std::shared_ptr<LDAPMessage> LDAPHandler::query(const std::string filter)
  {
    USBGUARD_LOG(Debug) << "Trying to fetch LDAP data, query: " << filter;
    LDAPMessage* res = nullptr;
    int rc = ldap_search_ext_s(_ldap_ptr.get(), _parsedOptions["USBGUARDBASE"].c_str(),
        LDAP_SCOPE_SUBTREE, filter.c_str(), nullptr, false,
        nullptr, nullptr, nullptr, 0, &res);

    if (rc != LDAP_SUCCESS) {
      throw Exception("LDAPHandler query", "ldap_search_ext_s", ldap_err2string(rc));
    }

    std::shared_ptr<LDAPMessage> ptr(res, LDAPHandler::LDAPDeleter());
    USBGUARD_LOG(Debug) << "Fetched LDAP DNs: ";
    char* dn = nullptr;

    for ( LDAPMessage* e = ldap_first_entry( _ldap_ptr.get(), ptr.get() ); e != nullptr;
      e = ldap_next_entry( _ldap_ptr.get(), e ) ) {
      if ( (dn = ldap_get_dn( _ldap_ptr.get(), e )) != NULL ) {
        USBGUARD_LOG(Debug) << "dn: " << dn;
        ldap_memfree( dn );
      }
    }

    return ptr;
  }

  std::vector<std::string> LDAPHandler::ldapToRules(std::shared_ptr<LDAPMessage> message)
  {
    std::vector<std::string> rules;
    USBGUARD_LOG(Info);
    USBGUARD_LOG(Info) << "Complete LDAP Data:";
    USBGUARD_LOG(Info);
    struct berval** entry = nullptr;
    char* dn = nullptr;

    for ( LDAPMessage* e = ldap_first_entry( _ldap_ptr.get(), message.get() ); e != nullptr;
      e = ldap_next_entry( _ldap_ptr.get(), e ) ) {
      std::string rule;

      if ( (dn = ldap_get_dn( _ldap_ptr.get(), e)) != nullptr ) {
        USBGUARD_LOG(Info) << "dn: " << dn;
        ldap_memfree(dn);
      }

      for (size_t i = 0 ; i < LDAPUtil::_ldap_keys.size() ; i++) {
        if ( (entry = ldap_get_values_len( _ldap_ptr.get(), e, LDAPUtil::_ldap_keys[i].c_str() )) != nullptr ) {
          if ((*entry)[0].bv_val == nullptr) {
            continue;
          }

          std::string value((*entry)[0].bv_val);

          switch (static_cast<LDAPHandler::LDAP_KEY_INDEX>(i)) {
          case LDAPHandler::LDAP_KEY_INDEX::RuleType:
            rule += value;
            break;

          case LDAPHandler::LDAP_KEY_INDEX::DeviceID:
          case LDAPHandler::LDAP_KEY_INDEX::DeviceSerial:
          case LDAPHandler::LDAP_KEY_INDEX::DeviceName:
          case LDAPHandler::LDAP_KEY_INDEX::DeviceHash:
          case LDAPHandler::LDAP_KEY_INDEX::DeviceParentHash:
          case LDAPHandler::LDAP_KEY_INDEX::DeviceViaPort:
          case LDAPHandler::LDAP_KEY_INDEX::DeviceWithInterface:
          case LDAPHandler::LDAP_KEY_INDEX::RuleCondition:
            rule += " " + LDAPUtil::_rule_keys[i] + " " + value;
            break;

          case LDAPHandler::LDAP_KEY_INDEX::USBGuardHost:
          default:
            /* code */
            break;
          }

          USBGUARD_LOG(Info) << LDAPUtil::_ldap_keys[i] << ": " << (*entry)[0].bv_val;
          free((*entry)[0].bv_val);
        }
      }

      rules.push_back(rule);
      USBGUARD_LOG(Info);
      USBGUARD_LOG(Info) << rule;
      USBGUARD_LOG(Info);
    }

    return rules;
  }

  void LDAPHandler::parseConf(const std::string path)
  {
    _ldap_file = path;
    USBGUARD_LOG(Info) << "Loading LDAP configuration from " << _ldap_file;
    std::ifstream ldap_file(_ldap_file);

    if (!ldap_file.is_open()) {
      throw ErrnoException("LDAP configuration parsing", _ldap_file, errno);
    }

    std::string line;
    std::pair<std::string, std::string> parsed;
    unsigned line_number = 0;

    while (std::getline(ldap_file, line)) {
      line_number++;

      if (line[0] != '#' && line[0] != 0 ) {
        try {
          pegtl::parse_string< usbguard::LDAPConfParser::grammar, usbguard::LDAPConfParser::action >
          ( line, _ldap_file + ":" + std::to_string(line_number), parsed );
          //pegtl::parse< usbguard::LDAPConfParser::grammar, usbguard::LDAPConfParser::action >( in, parsed ); --> new pegtl
        }
        catch (pegtl::parse_error& e) {
          USBGUARD_LOG(Debug) << "--- Parsing line: " << line_number << "---";
          USBGUARD_LOG(Debug) << line;
          USBGUARD_LOG(Debug) << "--- Nothing to do ---" << std::endl;
          continue;
        }

        _parsedOptions.insert(parsed);
      }
    }

    ldap_file.close();
    USBGUARD_LOG(Debug) << "Map contains:";

    for (auto x: _parsedOptions) {
      USBGUARD_LOG(Debug) << "--> " << x.first << "->" << x.second << " <--";
    }

    USBGUARD_LOG(Debug) << "LDAP conf has been parsed";
  }

  void LDAPHandler::validateConf()
  {
    USBGUARD_LOG(Debug) << "Validating LDAP conf";

    for (auto x: _parsedOptions) {
      if (_configValues.find(x.first) == _configValues.end()) {
        throw Exception("LDAP conf validation", "validateConf", "Parsed option is not valid: " + x.first);
      }
      else {
        _configValues[x.first] = true;
      }
    }

    std::vector<std::string> v = {"URI", "BASE", "ROOTDN", "ROOTPW"};

    for (auto s: v) {
      if (!_configValues[s]) {
        USBGUARD_LOG(Debug) << "Option " << s << " is missing!";
        throw Exception("LDAP conf validation", "validateConf", "Too few options");
      }
    }

    if (!_configValues["USBGUARDBASE"]) {
      _parsedOptions["USBGUARDBASE"] = "ou=USBGuard," + _parsedOptions["BASE"];
      _configValues["USBGUARDBASE"] = true;
      USBGUARD_LOG(Debug) << "Option " << "USBGUARDBASE" << " is missing!";
      USBGUARD_LOG(Debug) << "Using defult: " << _parsedOptions["USBGUARDBASE"];
    }

    if (!_configValues["RULEQUERY"]) {
      _parsedOptions["RULEQUERY"] = "(&(cn=Rule*)(|(USBGuardHost=" + _hostname + ")(&(USBGuardHost=\\*)(!(USBGuardHost=!" + _hostname
        + ")))))";
      _configValues["RULEQUERY"] = true;
      USBGUARD_LOG(Debug) << "Option " << "RULEQUERY" << " is missing!";
      USBGUARD_LOG(Debug) << "Using default: " << _parsedOptions["RULEQUERY"];
    }
  }
}

/* vim: set ts=2 sw=2 et */
