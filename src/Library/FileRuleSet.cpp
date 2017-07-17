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
#ifdef HAVE_BUILD_CONFIG_H
#include <build-config.h>
#endif

#include "RulePrivate.hpp"
#include "FileRuleSet.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/RuleParser.hpp"
#include "usbguard/Exception.hpp"

#include <fstream>

namespace usbguard {

  FileRuleSet::FileRuleSet(Interface * const interface_ptr)
    : RuleSet(interface_ptr),
      _rulePath("")
  {
    _default_target = Rule::Target::Block;
    _default_action = std::string();
    _id_next = Rule::RootID + 1;
  }

  FileRuleSet::FileRuleSet(const FileRuleSet& rhs)
    : RuleSet(rhs._interface_ptr)
  {
    *this = rhs;
  }

  const FileRuleSet& FileRuleSet::operator=(const FileRuleSet& rhs)
  {
    RuleSet::operator = (rhs);
    return *this;
  }


  void FileRuleSet::load()
  {
    load(_rulePath);
  }

  void FileRuleSet::save()
  {
    save(_rulePath);
  }

  void FileRuleSet::setRulePath(const std::string& path)
  {
    _rulePath = path;
  }

  void FileRuleSet::load(const std::string& path)
  {
    std::ifstream stream(path);
    if (!stream.is_open()) {
      throw ErrnoException("FileRuleSet loading", path, errno);
    }
    load(stream);
  }

  void FileRuleSet::load(std::istream& stream)
  {
    std::unique_lock<std::mutex> lock(_io_mutex);
    std::string line_string;
    size_t line_number = 0;

    do {
      ++line_number;
      std::getline(stream, line_string);
      const Rule rule = parseRuleFromString(line_string, "", line_number);
      if (rule) {
	appendRule(rule);
      }
    } while(stream.good());
  }

  void FileRuleSet::save(const std::string& path) const
  {
    std::ofstream stream(path, std::fstream::trunc);
    if (!stream.is_open()) {
      throw ErrnoException("FileRuleSet saving", path, errno);
    }
    save(stream);
  }

  void FileRuleSet::save(std::ostream& stream) const
  {
    std::unique_lock<std::mutex> io_lock(_io_mutex);
    std::unique_lock<std::mutex> op_lock(_op_mutex);

    for (auto const& rule : _rules) {
      const std::string rule_string = rule->toString();
      stream << rule_string << std::endl;
    }
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
