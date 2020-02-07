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
  /**
   * @brief Represents error during the rule parsing.
   */
  class RuleParserError : public std::exception
  {
  public:

    /**
     * @brief Constructs RuleParserError.
     *
     * @param rule_spec String representation of the rule.
     * @param hint Describes what happened.
     * @param file File where the rule is located.
     * @param error_line Line in the file where the error occured.
     * @param error_offset Offset where the error occured.
     */
    RuleParserError(const std::string& rule_spec, const std::string& hint = "",
      const std::string& file = "", size_t error_line = 0, unsigned int error_offset = 0)
      : _rule_spec(rule_spec),
        _hint(hint),
        _offset(error_offset),
        _file(file),
        _line(error_line)
    {
    }

    /**
     * @brief Sets the describtion of the error.
     *
     * @param hint Describes what happened.
     */
    void setHint(const std::string& hint)
    {
      _hint = hint;
    }

    /**
     * @brief Sets the offset where the error occured.
     *
     * @param offset Offset where the error occured.
     */
    void setOffset(size_t offset)
    {
      _offset = offset;
    }

    /**
     * @brief Sets exact location where the error occured.
     *
     * @param file File where the rule is located.
     * @param error_line Line in the file where the error occured.
     * @param error_offset Offset where the error occured.
     */
    void setFileInfo(const std::string& file, size_t error_line, size_t error_offset = 0)
    {
      _file = file;
      _line = error_line;
      _offset = error_offset;
    }

    /**
     * @brief Describes the exception.
     *
     * @return Describes what happened.
     */
    const char* what() const noexcept
    {
      return "RuleParserError";
    }

    /**
     * @brief Returns string representation of the rule.
     *
     * @return string representation of the rule.
     */
    const std::string& rule() const
    {
      return _rule_spec;
    }

    /**
     * @brief Returns error hint.
     *
     * @return Hint.
     */
    const std::string& hint() const
    {
      return _hint;
    }

    /**
     * @brief Decides whether information about error location is set.
     *
     * @return True if information about error location is set,
     * false otherwise.
     */
    bool hasFileInfo() const
    {
      return !_file.empty();
    }

    /**
     * @brief Returns string containing file,
     * line and offset where the error occured.
     *
     * @return String containing file, line and offset where the error occured.
     */
    const std::string fileInfo() const
    {
      return _file + ": line=" + std::to_string(_line) + ": offset=" + std::to_string(_offset);
    }

    /**
     * @brief File where the rule is located.
     *
     * @return File where the rule is located.
     */
    const std::string& file() const
    {
      return _file;
    }

    /**
     * @brief Line number where the rule is located.
     *
     * @return line number where the rule is located.
     */
    size_t line() const
    {
      return _line;
    }

    /**
     * @brief Offset where the error occured.
     *
     * @return offset where the error occured.
     */
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

  /**
   * @brief Parses given string and returns Rule object if the string contains
   * a valid rule.
   *
   * @param rule_spec String representation of the rule.
   * @param file File where the rule is located.
   * @param line Line number in the file where the rule is located.
   * @param trace Trace.
   * @return \link Rule Rule\endlink object.
   * @throw RuleParseError If there is an error in the rule_spec.
   */
  DLL_PUBLIC Rule parseRuleFromString(const std::string& rule_spec, const std::string& file = std::string(), size_t line = 0,
    bool trace = false);
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
