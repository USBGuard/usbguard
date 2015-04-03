//
// Copyright (C) 2014 Red Hat, Inc.
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
#include <build-config.h>
#include "Common/Utility.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <alloca.h>
#include <fstream>

namespace usbguard
{

  void daemonize()
  {
    ::pid_t pid;
    ::pid_t sid;

    switch(pid = ::fork()) {
    case  0: /* child */
      break;
    case -1: /* error */
      ::exit(EXIT_FAILURE);
    default: /* parent */
      ::exit(EXIT_SUCCESS);
    }
    //
    // Decouple from parent environment
    // - chdir to /
    // - create new process session
    // - reset umask
    // - cleanup file descriptors
    // - ???
    // - consider using libdaemon
    //
    if (::chdir("/") != 0) {
      ::exit(EXIT_FAILURE);
    }
    if ((sid = ::setsid()) != 0) {
      ::exit(EXIT_FAILURE);
    }
    ::umask(::umask(077)|022);
    struct rlimit rlim;
    if (::getrlimit(RLIMIT_NOFILE, &rlim) != 0) {
      ::exit(EXIT_FAILURE);
    }
    int maxfd = (rlim.rlim_max == RLIM_INFINITY ? 1024 : rlim.rlim_max);
    for (int fd = 0; fd < maxfd; ++fd) {
      ::close(fd);
    }

    return;
  }

  bool writePID(const String& filepath)
  {
    std::ofstream pidstream(filepath, std::ios_base::trunc);
    if (!pidstream) {
      return false;
    }
    pidstream << numberToString(getpid()) << std::endl;
    return true;
  }

  static void runCommandExecChild(const String& path, const std::vector<String>& args)
  {
    int maxfd = 0;
    int nullfd;
    struct rlimit rlim;

    // Find out what the maxfd value could be
    if (::getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
      return;
    }
    maxfd = (rlim.rlim_max == RLIM_INFINITY ? 4096 : rlim.rlim_max);

    nullfd = ::open("/dev/null", O_RDWR);
    if (nullfd < 0) {
      return;
    }

    for (int fd = 0; fd < maxfd; ++fd) {
      if (fd == nullfd) {
	// Don't close our /dev/null fd
	continue;
      }
      switch(fd) {
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
    char **args_cstr = (char **)(::alloca(sizeof(const char *) * (args.size() + 2)));
    unsigned int i;

    args_cstr[0] = const_cast<char *>(path.c_str());
    for (i = 0; i < args.size(); ++i) {
      args_cstr[1+i] = const_cast<char *>(args[i].c_str());
    }
    args_cstr[1+i] = nullptr;

    // TODO: Reset environment?
    (void)::execv(path.c_str(), args_cstr);
  }

  int runCommand(const char *path, const char *arg1, int timeout_secs)
  {
    std::vector<String> args;
    args.push_back(arg1);
    return runCommand(path, args, timeout_secs);
  }

  int runCommand(const char *path, const char *arg1, const char *arg2, int timeout_secs)
  {
    std::vector<String> args;
    args.push_back(arg1);
    args.push_back(arg2);
    return runCommand(path, args, timeout_secs);
  }

  int runCommand(const String& path, const std::vector<String>& args, int timeout_secs)
  {
    int retval = 0, status = 0, waitpid_time_usec;
    pid_t child_pid, waitpid_retval = 0;
    bool timedout = false;

    switch (child_pid = ::fork()) {
    case 0:
      // Child
      runCommandExecChild(path, args);
      ::_exit(EXIT_FAILURE);
    case -1:
      break;
    }
    // Parent - wait for the child to exit (up to timeout seconds)
    waitpid_time_usec = timeout_secs * 1000 * 1000;

    while (waitpid_time_usec > 0) {
      timedout = false;
      switch(waitpid_retval = ::waitpid(child_pid, &status, WNOHANG)) {
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
	} else {
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

  String filenameFromPath(const String& filepath, bool include_extension)
  {
    const String directory_separator = "/";
    StringVector path_tokens;

    tokenizeString(filepath, path_tokens, directory_separator);

    if (path_tokens.size() == 0) {
      return String();
    }

    String& filename = path_tokens.back();

    if (include_extension) {
      return filename;
    }

    size_t substr_to = filename.find_last_of('.');

    return filename.substr(0, substr_to);
  }

  /*
   * The ostringstream class used for the implementation of numberToString
   * treats (u)int8_t as a char. We want to treat it as a number, so this
   * explicit specialization handles the uint8_t case by recasting it to
   * an unsigned int.
   */
  template<>
  String numberToString(uint8_t number, const String& prefix, int base, int align, char align_char)
  {
    return numberToString((unsigned int)number, prefix, base, align, align_char);
  }

  template<>
  uint8_t stringToNumber(const String& s, int base)
  {
    const unsigned int num = stringToNumber<unsigned int>(s, base);
    return (uint8_t)num;
  }

} /* namespace usbguard */
