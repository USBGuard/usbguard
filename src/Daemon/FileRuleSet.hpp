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
// Authors: Radovan Sroka <rsroka@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Typedefs.hpp"
#include "usbguard/Rule.hpp"
#include "usbguard/RuleSet.hpp"

#include <istream>
#include <ostream>
#include <mutex>

namespace usbguard
{
  class Interface;
  class FileRuleSet : public RuleSet
  {
  public:
    FileRuleSet(Interface* const interface_ptr, std::string const path);
    FileRuleSet(const FileRuleSet& rhs);
    const FileRuleSet& operator=(const FileRuleSet& rhs);

    void load() override;
    void save() override;

    void load(const std::string& path);
    void load(std::istream& stream);
    void save(const std::string& path) const;
    void save(std::ostream& stream) const;

    void setRulesPath(const std::string& path);

  private:
    std::string _rulesPath;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
