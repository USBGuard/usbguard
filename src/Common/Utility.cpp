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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Logger.hpp"

#include "Common/Utility.hpp"

#include <fstream>
#include <algorithm>

#include <alloca.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace usbguard
{

  static void runCommandExecChild(const std::string& path, const std::vector<std::string>& args)
  {
    struct rlimit rlim;

    // Find out what the maxfd value could be
    if (::getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
      return;
    }

    const int maxfd = (rlim.rlim_max == RLIM_INFINITY ? 4096 : rlim.rlim_max);
    const int nullfd = ::open("/dev/null", O_RDWR);

    if (nullfd < 0) {
      return;
    }

    for (int fd = 0; fd < maxfd; ++fd) {
      if (fd == nullfd) {
        // Don't close our /dev/null fd
        continue;
      }

      switch (fd) {
      case STDERR_FILENO:
      case STDOUT_FILENO:
      case STDIN_FILENO:
        // Redirect the standard descriptors to /dev/null
        ::dup2(nullfd, fd);
        break;

      default:
        // Close everything else
        (void)::close(fd);
      }
    }

    (void)::close(nullfd);

    if (args.size() > 1024) {
      // Looks suspicious...
      return;
    }

    //
    // Allocate space for the argv array on stack
    // +1 ... for argv[0]
    // +1 ... for nullptr termination of the array
    //
    char** const args_cstr = (char**)(::alloca(sizeof(const char*) * (args.size() + 2)));
    unsigned int i;
    args_cstr[0] = const_cast<char*>(path.c_str());

    for (i = 0; i < args.size(); ++i) {
      args_cstr[1+i] = const_cast<char*>(args[i].c_str());
    }

    args_cstr[1+i] = nullptr;
    // TODO: Reset environment?
    (void)::execv(path.c_str(), args_cstr);
  }

  int runCommand(const char* const path, const char* const arg1, const int timeout_secs)
  {
    std::vector<std::string> args;
    args.push_back(arg1);
    return runCommand(path, args, timeout_secs);
  }

  int runCommand(const char* const path, const char* const arg1, const char* const arg2, const int timeout_secs)
  {
    std::vector<std::string> args;
    args.push_back(arg1);
    args.push_back(arg2);
    return runCommand(path, args, timeout_secs);
  }

  int runCommand(const std::string& path, const std::vector<std::string>& args, const int timeout_secs)
  {
    int retval = 0, status = 0;
    bool timedout = false;
    const pid_t child_pid = ::fork();

    switch (child_pid) {
    case 0:
      // Child
      runCommandExecChild(path, args);
      ::_exit(EXIT_FAILURE);

    case -1:
    default:
      break;
    }

    // Parent - wait for the child to exit (up to timeout seconds)
    int waitpid_time_usec = timeout_secs * 1000 * 1000;

    while (waitpid_time_usec > 0) {
      const pid_t waitpid_retval = ::waitpid(child_pid, &status, WNOHANG);
      timedout = false;

      switch (waitpid_retval) {
      case 0: // Not exited yet; Sleep & retry
        timedout = true;
        waitpid_time_usec -= 500;
        ::usleep(500);
        continue;

      case -1: // Error
        retval = -1;
        break;

      default:
        if (waitpid_retval == child_pid) {
          // Child exited
          retval = WEXITSTATUS(status);
          waitpid_time_usec = 0;
          continue;
        }
        else {
          // Unexpected error
        }
      }
    }

    if (timedout) {
      // Try to be nice first
      ::kill(child_pid, SIGTERM);
      ::usleep(1000*500);

      // Send SIGKILL if the process is still running
      if (::waitpid(child_pid, &status, WNOHANG) != child_pid) {
        ::kill(child_pid, SIGKILL);
      }

      retval = -1;
    }

    return retval;
  }

  std::string filenameFromPath(const std::string& filepath, const bool include_extension)
  {
    const std::string directory_separator = "/";
    std::vector<std::string> path_tokens;
    tokenizeString(filepath, path_tokens, directory_separator);

    if (path_tokens.size() == 0) {
      return std::string();
    }

    const std::string& filename = path_tokens.back();

    if (include_extension) {
      return filename;
    }

    const size_t substr_to = filename.find_last_of('.');
    return filename.substr(0, substr_to);
  }

  std::string parentPath(const std::string& path)
  {
    const std::string directory_separator = "/";
    std::string parent_path(path);
    // find first not '/' (from end)
    // find first '/' (from end)
    // find first not '/' (from end)
    auto reverse_start_pos = \
      parent_path.find_last_not_of(directory_separator);

    /*
     * Whole path consists only of '/'.
     */
    if (reverse_start_pos == std::string::npos) {
      return std::string();
    }

    reverse_start_pos = \
      parent_path.find_last_of(directory_separator, reverse_start_pos);

    /*
     * No directory separator in the rest of the path.
     */
    if (reverse_start_pos == std::string::npos) {
      return std::string();
    }

    reverse_start_pos = \
      parent_path.find_last_not_of(directory_separator, reverse_start_pos);

    /*
     *
     * /foo/bar   => /foo
     * /foo/bar/  => /foo
     * /foo/bar// => /foo
     * /foo       => std::string()
     * /foo/      => std::string()
     * /          => std::string()
     * //foo      => std::string()
     *
     */
    if (reverse_start_pos == std::string::npos) {
      return std::string();
    }

    return path.substr(0, reverse_start_pos + 1);
  }

  std::string trimRight(const std::string& s, const std::string& delimiters)
  {
    const size_t substr_to = s.find_last_not_of(delimiters);

    if (substr_to != std::string::npos) {
      return s.substr(0, substr_to + 1);
    }
    else {
      return std::string();
    }
  }

  std::string trimLeft(const std::string& s, const std::string& delimiters)
  {
    const size_t substr_from = s.find_first_not_of(delimiters);

    if (substr_from == std::string::npos) {
      return s;
    }
    else {
      return s.substr(substr_from);
    }
  }

  std::string trim(const std::string& s, const std::string& delimiters)
  {
    return trimRight(trimLeft(s, delimiters), delimiters);
  }

  /*
   * The ostringstream class used for the implementation of numberToString
   * treats (u)int8_t as a char. We want to treat it as a number, so this
   * explicit specialization handles the uint8_t case by recasting it to
   * an unsigned int.
   */
  template<>
  std::string numberToString(const uint8_t number, const std::string& prefix, const int base, const int align,
    const char align_char)
  {
    const uint16_t n = static_cast<uint16_t>(number);
    return numberToString(n, prefix, base, align, align_char);
  }

  template<>
  uint8_t stringToNumber(const std::string& s, const int base)
  {
    const unsigned int num = stringToNumber<unsigned int>(s, base);
    return (uint8_t)num;
  }

  bool isNumericString(const std::string& s)
  {
    for (int c : s) {
      if (!isdigit(c)) {
        return false;
      }
    }

    return true;
  }

  int loadFiles(const std::string& directory,
    std::function<std::string(const std::string&, const struct dirent*)> filter,
    std::function<int(const std::string&, const std::string&)> loader,
    std::function<bool(const std::pair<std::string, std::string>&, const std::pair<std::string, std::string>&)> sorter,
    bool directory_required)
  {
    DIR* dirobj = opendir(directory.c_str());

    if (dirobj == nullptr) {
      if (!directory_required && errno == ENOENT) {
        return 0;
      }
      else {
        throw ErrnoException("loadFiles", directory, errno);
      }
    }

    int retval = 0;

    try {
      std::vector<std::pair<std::string, std::string>> loadpaths;
      struct dirent* entry_ptr = nullptr;

      /*
       * readdir usage note: We rely on the fact that readdir should be thread-safe
       * when used on a different directory stream. Since we create our own stream,
       * we should be fine with readdir here. The thread-safe version of readdir,
       * readdir_r, is deprecated in newer versions of glibc.
       */
      while ((entry_ptr = readdir(dirobj)) != nullptr) {
        const std::string filename(entry_ptr->d_name);

        if (filename == "." || filename == "..") {
          continue;
        }

        std::string fullpath = directory + "/" + filename;
        std::string loadpath = filter(fullpath, entry_ptr);

        if (!loadpath.empty()) {
          loadpaths.emplace_back(std::make_pair(std::move(loadpath), std::move(fullpath)));
        }
      }

      std::sort(loadpaths.begin(), loadpaths.end(), sorter);

      for (const auto& loadpath : loadpaths) {
        USBGUARD_LOG(Trace) << "L: " << loadpath.first << " : " << loadpath.second;
      }

      for (const auto& loadpath : loadpaths) {
        retval += loader(loadpath.first, loadpath.second);
      }
    }
    catch (...) {
      closedir(dirobj);
      throw;
    }

    closedir(dirobj);
    return retval;
  }

  std::string removePrefix(const std::string& prefix, const std::string& value)
  {
    if (value.compare(0, prefix.size(), prefix) == 0) {
      return value.substr(prefix.size());
    }
    else {
      return value;
    }
  }

  bool hasSuffix(const std::string& value, const std::string& suffix)
  {
    if (suffix.size() > value.size()) {
      return false;
    }

    const auto pos = value.size() - suffix.size();
    const auto cmp = value.compare(pos, suffix.size(), suffix);
    return cmp == 0;
  }

  bool hasPrefix(const std::string& value, const std::string& prefix)
  {
    if (prefix.size() > value.size()) {
      return false;
    }

    const auto cmp = value.compare(0, prefix.size(), prefix);
    return cmp == 0;
  }

  std::string symlinkPath(const std::string& linkpath, struct stat* st_user)
  {
    struct stat st = { };
    struct stat* st_ptr = nullptr;

    if (st_user == nullptr) {
      USBGUARD_SYSCALL_THROW("symlinkPath",
        lstat(linkpath.c_str(), &st) != 0);
      st_ptr = &st;
    }
    else {
      st_ptr = st_user;
    }

    if (!S_ISLNK(st_ptr->st_mode)) {
      throw Exception("symlinkPath", linkpath, "not a symlink");
    }

    if (st_ptr->st_size < 1) {
      st_ptr->st_size = 4096;
    }

    /*
     * Check sanity of st_size. min: 1 byte, max: 1 MiB (because 1 MiB should be enough :)
     */
    if (st_ptr->st_size < 1 || st_ptr->st_size > (1024 * 1024)) {
      USBGUARD_LOG(Debug) << "st_size=" << st_ptr->st_size;
      throw Exception("symlinkPath", linkpath, "symlink value size out of range");
    }

    std::string buffer(st_ptr->st_size, 0);
    const ssize_t link_size = readlink(linkpath.c_str(), &buffer[0], buffer.capacity());

    if (link_size <= 0 || link_size > st_ptr->st_size) {
      USBGUARD_LOG(Debug) << "link_size=" << link_size
        << " st_size=" << st_ptr->st_size;
      throw Exception("symlinkPath", linkpath, "symlink value size changed before read");
    }

    buffer.resize(link_size);

    if (buffer[0] == '/') {
      /* Absolute path */
      return buffer;
    }
    else {
      /* Relative path */
      return parentPath(linkpath) + "/" + buffer;
    }
  }

  std::size_t countPathComponents(const std::string& path)
  {
    bool was_component = false;
    std::size_t count = 0;

    for (std::size_t i = 0; i < path.size(); ++i) {
      const char c = path[i];

      if (c == '/') {
        if (was_component) {
          ++count;
          was_component = false;
        }
      }
      else {
        was_component = true;
      }
    }

    return count;
  }

  std::string normalizePath(const std::string& path)
  {
    std::vector<std::string> components;
    const bool is_absolute = (path[0] == '/');
    tokenizeString(path, components, "/", /*trim_empty=*/true);

    for (auto it = components.begin(); it != components.end();) {
      if (*it == ".") {
        /* remove this component */
        it = components.erase(it);
      }
      else if (*it == "..") {
        /* remove this and previous component (if any) */
        if (it != components.begin()) {
          it = components.erase(it - 1);
        }

        it = components.erase(it);
      }
      else {
        ++it;
      }
    }

    std::string normalized_path(is_absolute ? "/" : "");

    for (auto it = components.cbegin(); it != components.cend(); ++it) {
      normalized_path.append(*it);

      if ((it+1) != components.cend()) {
        normalized_path.append("/");
      }
    }

    return normalized_path;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
