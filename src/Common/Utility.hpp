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
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <iomanip>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>

namespace usbguard
{
  /**
   * Create a background process.
   *
   * Performs the following actions:
   * 1) fork a new process (parent process exists with 0)
   * 2) chdir to /
   * 3) creates a new process session
   * 4) resets umask
   * 5) closes all file descriptors
   * 6) Reinitialize logging for the child
   */
  void daemonize(void);

  /**
   * Writes the current PID to a file at filepath.
   * Returns true on success, otherwise returns false.
   */
  bool writePID(const String& filepath);

  /**
   * Wrappers for the __builtin_expect function.
   */
#if defined(__GNUC__)
# define likely(expr) __builtin_expect(!!(expr), 1)
# define unlikely(expr) __builtin_expect(!!(expr), 0)
#else
# define likely(expr) (expr)
# define unlikely(expr) (expr)
#endif

  int runCommand(const char *path, const char *arg1, int timeout_secs = 10);
  int runCommand(const char *path, const char *arg1, const char *arg2, int timeout_secs = 10);
  int runCommand(const String& path, const std::vector<String>& args = std::vector<String>(), int timeout_secs = 10);

  /**
   * Tokenize a std::string compatible type using delimiters specified in a string.
   *
   * Splits the given string using delimiters found in the delimiter string (each
   * character in the delimiter string is considered as a delimiter) and stores the
   * token in a vector. If trim_empty is set to true, empty tokens won't be included
   * in the vector.
   */
  template<typename StringType>
  void tokenizeString(const StringType& str, std::vector<StringType>& tokens, const typename std::vector<StringType>::value_type delimiters, const bool trim_empty = false)
  {
    typename StringType::size_type pos, last_pos = 0;
    while(true) {
      pos = str.find_first_of(delimiters, last_pos);
      if(pos == StringType::npos) {
	pos = str.length();

	if(pos != last_pos || !trim_empty) {
	  tokens.push_back(StringType(str.data() + last_pos, pos - last_pos));
	}

	break;
      } else {
	if(pos != last_pos || !trim_empty) {
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
  String numberToString(const T number, const String& prefix = String(), const int base = 10, const int align = -1, const char align_char = ' ')
  {
    std::ostringstream ss;

    ss << std::setbase(base);
    ss << number;

    const String number_string = ss.str();
    String result;
    result.append(prefix);

    if (align > 0 && number_string.size() < (size_t)align) {
      size_t chars_to_add = (size_t)align - number_string.size();
      for (;chars_to_add > 0; --chars_to_add) {
	result += align_char;
      }
    }

    result.append(number_string);
    return result;
  }

  template<>
  String numberToString(const uint8_t number, const String& prefix, const int base, const int align, const char align_char);

  /**
   * Convert a string representation of a number
   * to a number of type T.
   */
  template<typename T>
  T stringToNumber(const String& s, const int base = 10)
  {
    std::istringstream ss(s);
    T num;
    ss >> std::setbase(base) >> num;
    return num;
  }

  template<>
  uint8_t stringToNumber(const String& s, const int base);

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
  String filenameFromPath(const String& filepath, bool include_extension = false);

  /**
   * Remove whitespace characters from the right side of a string.
   */
  template<class StringType>
  StringType trimRight(const StringType& s, const StringType& delimiters = " \f\n\r\t\v" )
  {
    const size_t substr_to = s.find_last_not_of(delimiters)+1;
    return s.substr(0, substr_to);
  }

  /**
   * Remove whitespace characters from the left side of a string.
   */
  template<class StringType>
  StringType trimLeft(const StringType& s, const StringType& delimiters = " \f\n\r\t\v" )
  {
    const size_t substr_from = s.find_first_not_of(delimiters);
    if (substr_from == StringType::npos) {
      return s;
    } else {
      return s.substr(substr_from);
    }
  }

  /**
   * Remove whitespace characters from the left & right side of a string.
   */
  template<class StringType>
  StringType trim(const StringType& s, const StringType& delimiters = " \f\n\r\t\v" )
  {
    return trimRight(trimLeft(s, delimiters), delimiters);
  }

  /**
   * Call a void(*)(const String&) compatible method for every file in a directory
   * matching a regular expression. The function does not recursively descent into
   * subdirectories.
   */
  template<class T>
  void loadFiles(const String& directory, const std::regex& filename_regex,
		 void(T::*load_method_ptr)(const String& filepath), T* load_method_obj)
  {
    DIR* dirobj;

    if ((dirobj = opendir(directory.c_str())) == nullptr) {
      throw std::runtime_error("Cannot open directory");
    }

    struct dirent entry, *entry_ptr = nullptr;
    int error;

    while((error = readdir_r(dirobj, &entry, &entry_ptr)) == 0) {
      if (entry_ptr == nullptr) {
	// No more directory entries
	break;
      } else {
	const String filename(entry_ptr->d_name);
	const String fullpath = directory + "/" + filename;
#if defined(_DIRENT_HAVE_D_TYPE)
	if (entry_ptr->d_type != DT_UNKNOWN) {
	  if (entry_ptr->d_type != DT_REG) {
	    // Not a regular file, skip it.
	    continue;
	  }
	} else {
#endif
	  struct stat st;
	  if (lstat(fullpath.c_str(), &st) != 0) {
	    // Cannot stat, skip this entry
	    continue;
	  }
	  if (!S_ISREG(st.st_mode)) {
	    // Not a regular file, skip it.
	    continue;
	  }
#if defined(_DIRENT_HAVE_D_TYPE)
	}
#endif
	if (std::regex_match(filename, filename_regex)) {
	  (*load_method_obj.*load_method_ptr)(fullpath);
	}
      }
    }

    if (error > 0) {
      closedir(dirobj);
      throw std::runtime_error("Error while reading directory entries");
    }

    closedir(dirobj);
    return;
  }

} /* namespace usbguard */
