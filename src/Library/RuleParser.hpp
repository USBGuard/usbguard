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

namespace usbguard
{
  class RuleParserError : public std::exception
  {
  public:
    RuleParserError(const std::string& rule_spec, const std::string& hint = "",
                    const std::string& file = "", unsigned int file_row = 0, unsigned int file_col = 0)
      : _rule_spec(rule_spec),
        _hint(hint),
        _file(file),
        _file_row(file_row),
        _file_col(file_col)
    {
    }

    void setHint(const std::string& hint)
    {
      _hint = hint;
    }

    void setFileInfo(const std::string& file, unsigned int row, unsigned int col = 0)
    {
      _file = file;
      _file_row = row;
      _file_col = col;
    }

    const char *what() const noexcept
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
      return _file + ": line=" + std::to_string(_file_row) + ": column=" + std::to_string(_file_col);
    }

    const std::string& file() const
    {
      return _file;
    }

    unsigned int fileRow() const
    {
      return _file_row;
    }

    unsigned int fileColumn() const
    {
      return _file_col;
    }

  protected:
    const std::string _rule_spec;
    std::string _hint;
    std::string _file;
    unsigned int _file_row;
    unsigned int _file_col;
  };

  Rule parseRuleSpecification(const String& rule_spec, const std::string * const file = nullptr, unsigned int line = 0);
} /* namespace usbguard */
