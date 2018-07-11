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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Exception.hpp"
#include "usbguard/Typedefs.hpp"

#include <functional>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace usbguard
{
  /**
   * Wrappers for the __builtin_expect function.
   */
#if defined(__GNUC__)
  #define likely(expr) __builtin_expect(!!(expr), 1)
  #define unlikely(expr) __builtin_expect(!!(expr), 0)
#else
  #define likely(expr) (expr)
  #define unlikely(expr) (expr)
#endif

  int runCommand(const char* path, const char* arg1, int timeout_secs = 10);
  int runCommand(const char* path, const char* arg1, const char* arg2, int timeout_secs = 10);
  int runCommand(const std::string& path, const std::vector<std::string>& args = std::vector<std::string>(),
    int timeout_secs = 10);

  /**
   * Tokenize a std::string compatible type using delimiters specified in a string.
   *
   * Splits the given string using delimiters found in the delimiter string (each
   * character in the delimiter string is considered as a delimiter) and stores the
   * token in a vector. If trim_empty is set to true, empty tokens won't be included
   * in the vector.
   */
  template<typename StringType>
  void tokenizeString(const StringType& str, std::vector<StringType>& tokens,
    const typename std::vector<StringType>::value_type delimiters, const bool trim_empty = false)
  {
    typename StringType::size_type pos, last_pos = 0;

    while (true) {
      pos = str.find_first_of(delimiters, last_pos);

      if (pos == StringType::npos) {
        pos = str.length();

        if (pos != last_pos || !trim_empty) {
          tokens.push_back(StringType(str.data() + last_pos, pos - last_pos));
        }

        break;
      }
      else {
        if (pos != last_pos || !trim_empty) {
          tokens.push_back(StringType(str.data() + last_pos, pos - last_pos));
        }
      }

      last_pos = pos + 1;
    }
  }

  /**
   * Convert a number of type T to its string
   * representation.
   */
  template<typename T>
  std::string numberToString(const T number, const std::string& prefix = std::string(), const int base = 10, const int align = -1,
    const char align_char = ' ')
  {
    std::ostringstream ss;
    ss << std::setbase(base);
    ss << number;
    const std::string number_string = ss.str();
    std::string result;
    result.append(prefix);

    if (align > 0 && number_string.size() < (size_t)align) {
      size_t chars_to_add = (size_t)align - number_string.size();

      for (; chars_to_add > 0; --chars_to_add) {
        result += align_char;
      }
    }

    result.append(number_string);
    return result;
  }

  template<>
  std::string numberToString(const uint8_t number, const std::string& prefix, const int base, const int align,
    const char align_char);

  /**
   * Convert a string representation of a number
   * to a number of type T.
   */
  template<typename T>
  T stringToNumber(const std::string& s, const int base = 10)
  {
    std::istringstream ss(s);
    T num;
    ss >> std::setbase(base) >> num;
    return num;
  }

  template<>
  uint8_t stringToNumber(const std::string& s, const int base);

  bool isNumericString(const std::string& s);

  /**
   * Return the filename part of a path. If include_extension is set to
   * false, then any characters after the last dot character '.' will be
   * removed including the dot.
   *
   * Examples: "/foo/bar/baz" => "baz"
   *           "/foo/bar/baz.txt" (include_extension=false) => "baz"
   *           "/foo/bar/baz.txt" (include_extension=true) => "baz.txt"
   *           "/foo/bar/baz.woo.txt" (include_extension=false) => "baz.woo"
   *           "foo.txt" (include_extension=true) => "foo.txt"
   */
  std::string filenameFromPath(const std::string& filepath, bool include_extension = false);

  /**
   * Return the parent path part of a path.
   */
  std::string parentPath(const std::string& path);

  /**
   * Retrun the number of path components.
   */
  std::size_t countPathComponents(const std::string& path);

  /**
   * Remove whitespace characters from the right side of a string.
   */
  std::string trimRight(const std::string& s, const std::string& delimiters = " \f\n\r\t\v");

  /**
   * Remove whitespace characters from the left side of a string.
   */
  std::string trimLeft(const std::string& s, const std::string& delimiters = " \f\n\r\t\v");

  /**
   * Remove whitespace characters from the left & right side of a string.
   */
  std::string trim(const std::string& s, const std::string& delimiters = " \f\n\r\t\v");

  /**
   * Call a void(*)(const std::string&) compatible method for every file in a directory
   * matching a regular expression. The function does not recursively descent into
   * subdirectories.
   */
  int loadFiles(const std::string& directory,
    std::function<std::string(const std::string&, const struct dirent*)> filter,
    std::function<int(const std::string&, const std::string&)> loader,
    std::function<bool(const std::pair<std::string, std::string>&, const std::pair<std::string, std::string>&)> sorter = \
      [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b)
  {
    return a.first < b.first;
  },
  bool directory_required = false);

  /**
   * Remove prefix from string.
   */
  std::string removePrefix(const std::string& prefix, const std::string& value);

  /**
   * Test whether a string value has a prefix/suffix.
   */
  bool hasSuffix(const std::string& value, const std::string& suffix);
  bool hasPrefix(const std::string& value, const std::string& prefix);

  /**
   * Read symlink destination.
   */
  std::string symlinkPath(const std::string& linkpath, struct stat* st_user = nullptr);

  /*
   * Normalize path string:
   *  - remove //
   *  - remove ./
   *  - resolve foo/../bar/ to bar
   *  - resolve /foo/../bar/ to /bar
   */
  std::string normalizePath(const std::string& path);

  /*
   * Restorer class
   */
  template<typename Tvar, typename Tval>
  class Restorer
  {
  public:
    Restorer(Tvar& var, Tval transient, Tval restored)
      : _ref(var),
        _val(restored)
    {
      _ref = transient;
    }

    ~Restorer()
    {
      _ref = _val;
    }
  private:
    Tvar& _ref;
    Tval _val;
  };

  struct FreeDeleter {
    void operator()(void* p)
    {
      free(p);
    }
  };

  template<typename T, typename... Params>
  std::unique_ptr<T> make_unique(Params&& ... params)
  {
    return std::unique_ptr<T>(new T(std::forward<Params>(params)...));
  }

  /*
   * Scoped File Descriptor
   */
  class ScopedFD
  {
  public:
    ScopedFD(int fd)
      : _fd(fd) {}

    ScopedFD(const ScopedFD&) = delete;

    ~ScopedFD()
    {
      if (_fd >= 0) {
        ::close(_fd);
        _fd = -1;
      }
    }

    operator int() const noexcept
    {
      return _fd;
    }

    bool operator<(int rhs) const noexcept
    {
      return _fd < rhs;
    }

    bool operator==(int rhs) const noexcept
    {
      return _fd == rhs;
    }

  private:
    int _fd{-1};
  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
