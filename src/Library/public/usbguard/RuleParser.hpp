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
#pragma once

#include "Typedefs.hpp"
#include "Rule.hpp"

#include <stdexcept>
#include <string>

#include <cstddef>

namespace usbguard
{
  class RuleParserError : public std::exception
  {
  public:
    RuleParserError(const std::string& rule_spec, const std::string& hint = "",
      const std::string& file = "", size_t error_line = 0, unsigned int error_offset = 0)
      : _rule_spec(rule_spec),
        _hint(hint),
        _offset(error_offset),
        _file(file),
        _line(error_line)
    {
    }

    void setHint(const std::string& hint)
    {
      _hint = hint;
    }

    void setOffset(size_t offset)
    {
      _offset = offset;
    }

    void setFileInfo(const std::string& file, size_t error_line, size_t error_offset = 0)
    {
      _file = file;
      _line = error_line;
      _offset = error_offset;
    }

    const char* what() const noexcept
    {
      return "RuleParserError";
    }

    const std::string& rule() const
    {
      return _rule_spec;
    }

    const std::string& hint() const
    {
      return _hint;
    }

    bool hasFileInfo() const
    {
      return !_file.empty();
    }

    const std::string fileInfo() const
    {
      return _file + ": line=" + std::to_string(_line) + ": offset=" + std::to_string(_offset);
    }

    const std::string& file() const
    {
      return _file;
    }

    size_t line() const
    {
      return _line;
    }

    size_t offset() const
    {
      return _offset;
    }

  protected:
    const std::string _rule_spec;
    std::string _hint;
    size_t _offset;
    std::string _file;
    size_t _line;
  };

  DLL_PUBLIC Rule parseRuleFromString(const std::string& rule_spec, const std::string& file = std::string(), size_t line = 0,
    bool trace = false);
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
