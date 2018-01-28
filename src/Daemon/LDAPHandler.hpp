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

#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#ifdef HAVE_LDAP

#include <map>
#include <ctime>
#include <ldap.h>

#include "usbguard/KeyValueParser.hpp"
#include "usbguard/Rule.hpp"

namespace usbguard
{

  class LDAPHandler
  {
  public:
    struct LDAPDeleter {
      void operator()(LDAP* p)
      {
        ldap_unbind_ext_s(p, nullptr, nullptr);
      }

      void operator()(LDAPMessage* p)
      {
        ldap_msgfree(p);
      }
    };

    LDAPHandler();
    ~LDAPHandler();

    std::shared_ptr<LDAPMessage> query(const std::string filter);
    std::string getRuleQuery();
    std::time_t getUpdateInterval();

    std::vector<std::pair<long, std::string>> ldapToRules(std::shared_ptr<LDAPMessage> message);

    static std::vector<std::string> _configValues;

  private:
    void parseConf(const std::string path);
    void validateConf();

    std::map<std::string, std::string> _parsedOptions;
    KeyValueParser _parser;

    std::unique_ptr<LDAP, LDAPDeleter> _ldap_ptr;
    std::string _ldap_file;
    std::string _hostname;

    std::time_t _updateInterval;
  };
}

#endif

/* vim: set ts=2 sw=2 et */
