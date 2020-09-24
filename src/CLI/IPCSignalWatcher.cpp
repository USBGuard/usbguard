//
// Copyright (C) 2016 Red Hat, Inc.
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

#include "IPCSignalWatcher.hpp"
#include "usbguard/Exception.hpp"

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

namespace usbguard
{
  IPCSignalWatcher::~IPCSignalWatcher()
  {
    if (hasOpenExecutable()) {
      closeExecutable();
    }
  }

  void IPCSignalWatcher::setExecutable(const std::string& path)
  {
    if (hasOpenExecutable()) {
      closeExecutable();
    }

    openExecutable(path);
  }

  void IPCSignalWatcher::IPCConnected()
  {
    std::cout << "[IPC] Connected" << std::endl;

    if (hasOpenExecutable()) {
      const std::map<std::string, std::string> env = {
        { "USBGUARD_IPC_SIGNAL", "IPC.Connected" }
      };
      runExecutable(env);
    }
  }

  void IPCSignalWatcher::IPCDisconnected(bool exception_initiated, const IPCException& exception)
  {
    std::cout << "[IPC] Disconnected: exception_initiated=" << exception_initiated;

    if (exception_initiated) {
      std::cout << " message=" << exception.message();
    }

    std::cout << std::endl;

    if (hasOpenExecutable()) {
      const std::map<std::string, std::string> env = {
        { "USBGUARD_IPC_SIGNAL", "IPC.Disconnected" },
        { "USBGUARD_MESSAGE", exception_initiated ? exception.message() : "" }
      };
      runExecutable(env);
    }
  }

  void IPCSignalWatcher::DevicePresenceChanged(uint32_t id,
    DeviceManager::EventType event,
    Rule::Target target,
    const std::string& device_rule)
  {
    std::cout << "[device] PresenceChanged: id=" << id << std::endl;
    std::cout << " event=" << DeviceManager::eventTypeToString(event) << std::endl;
    std::cout << " target=" << Rule::targetToString(target) << std::endl;
    std::cout << " device_rule=" << device_rule << std::endl;

    if (hasOpenExecutable()) {
      const std::map<std::string, std::string> env = {
        { "USBGUARD_IPC_SIGNAL", "Device.PresenceChanged" },
        { "USBGUARD_DEVICE_ID", std::to_string(id) },
        { "USBGUARD_DEVICE_EVENT", DeviceManager::eventTypeToString(event) },
        { "USBGUARD_DEVICE_TARGET", Rule::targetToString(target) },
        { "USBGUARD_DEVICE_RULE", device_rule }
      };
      runExecutable(env);
    }
  }

  void IPCSignalWatcher::DevicePolicyChanged(uint32_t id,
    Rule::Target target_old,
    Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
  {
    std::cout << "[device] PolicyChanged: id=" << id << std::endl;
    std::cout << " target_old=" << Rule::targetToString(target_old) << std::endl;
    std::cout << " target_new=" << Rule::targetToString(target_new) << std::endl;
    std::cout << " device_rule=" << device_rule << std::endl;
    std::cout << " rule_id=" << rule_id << std::endl;

    if (hasOpenExecutable()) {
      const std::map<std::string, std::string> env = {
        { "USBGUARD_IPC_SIGNAL", "Device.PolicyChanged" },
        { "USBGUARD_DEVICE_ID", std::to_string(id) },
        { "USBGUARD_DEVICE_TARGET_OLD", Rule::targetToString(target_old) },
        { "USBGUARD_DEVICE_TARGET_NEW", Rule::targetToString(target_new) },
        { "USBGUARD_DEVICE_RULE", device_rule },
        { "USBGUARD_DEVICE_RULE_ID", std::to_string(rule_id) }
      };
      runExecutable(env);
    }
  }

  void IPCSignalWatcher::DevicePolicyApplied(uint32_t id,
    Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
  {
    std::cout << "[device] PolicyApplied: id=" << id << std::endl;
    std::cout << " target_new=" << Rule::targetToString(target_new) << std::endl;
    std::cout << " device_rule=" << device_rule << std::endl;
    std::cout << " rule_id=" << rule_id << std::endl;

    if (hasOpenExecutable()) {
      const std::map<std::string, std::string> env = {
        { "USBGUARD_IPC_SIGNAL", "Device.PolicyApplied" },
        { "USBGUARD_DEVICE_ID", std::to_string(id) },
        { "USBGUARD_DEVICE_TARGET_NEW", Rule::targetToString(target_new) },
        { "USBGUARD_DEVICE_RULE", device_rule },
        { "USBGUARD_DEVICE_RULE_ID", std::to_string(rule_id) }
      };
      runExecutable(env);
    }
  }

  void IPCSignalWatcher::PropertyParameterChanged(const std::string& name,
    const std::string& value_old,
    const std::string& value_new)
  {
    std::cout << "[property] ParameterChanged: name=" << name << std::endl;
    std::cout << " value_old=" << value_old << std::endl;
    std::cout << " value_new=" << value_new << std::endl;

    if (hasOpenExecutable()) {
      const std::map<std::string, std::string> env = {
        { "USBGUARD_IPC_SIGNAL", "Property.ParameterChanged" },
        { "USBGUARD_PROPERTY_NAME", name },
        { "USBGUARD_PROPERTY_VALUE_OLD", value_old },
        { "USBGUARD_PROPERTY_VALUE_NEW", value_new }
      };
      runExecutable(env);
    }
  }

  void IPCSignalWatcher::openExecutable(const std::string& path)
  {
    const int fd = ::open(path.c_str(), O_RDONLY);

    if (fd < 0) {
      throw ErrnoException("openExecutable", path, errno);
    }

    struct ::stat st = { };

    if (::fstat(fd, &st) != 0) {
      ::close(fd);
      throw ErrnoException("openExecutable", path, errno);
    }

    if (!S_ISREG(st.st_mode) || !(st.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH))) {
      ::close(fd);
      throw Exception("openExecutable", path, "not an executable file");
    }

    _exec_path = path;
    _exec_path_fd = fd;
  }

  void IPCSignalWatcher::closeExecutable()
  {
    _exec_path.clear();
    (void)::close(_exec_path_fd);
  }

  bool IPCSignalWatcher::hasOpenExecutable() const
  {
    return (_exec_path_fd >= 0);
  }

  void IPCSignalWatcher::runExecutable(const std::map<std::string, std::string>& environment)
  {
    /*
     * Fork child process, that will fork again and exec.
     */
    const ::pid_t pid_fork1 = fork();

    if (pid_fork1 == -1) {
      std::cerr << "ERROR: runExecutable: " << strerror(errno) << std::endl;
      return;
    }

    if (pid_fork1 > 0) {
      /*
       * Wait for the intermediary process to fork and exit. In case something
       * fails in the intermediary process, we can retrieve the error code from
       * the return status.
       */
      int fork1_status = -1;
      const ::pid_t pid_waited = ::waitpid(pid_fork1, &fork1_status, 0);

      if (pid_waited == -1) {
        std::cerr << "ERROR: runExecutable: "  << strerror(errno) << std::endl;
      }
      else if (WIFEXITED(fork1_status)) {
        const int fork1_errno = WEXITSTATUS(fork1_status);

        if (fork1_errno != 0) {
          std::cerr << "ERROR: runExecutable: " << strerror(errno) << std::endl;
        }
      }
      else if (WIFSIGNALED(fork1_status)) {
        const int fork1_signal = WTERMSIG(fork1_status);
        std::cerr << "ERROR: runExecutable: terminated by signal " << fork1_signal << std::endl;
      }

      return;
    }

    /*
     * Intermediary process.
     */
    const ::pid_t sid = ::setsid();

    if (sid == (::pid_t)-1) {
      ::_exit(errno);
    }

    const ::pid_t pid_fork2 = fork();

    if (pid_fork2 == (::pid_t)-1) {
      ::_exit(errno);
    }

    if (pid_fork2 > 0) {
      ::_exit(0);
    }

    char* const exec_argv[] = { &_exec_path[0], nullptr };
    char** const exec_envp = createExecutableEnvironment(environment);
    (void)::fexecve(_exec_path_fd, exec_argv, exec_envp);
    const int saved_errno = errno;
    destroyExecutableEnvironment(exec_envp);
    _exit(saved_errno);
  }

  char** IPCSignalWatcher::createExecutableEnvironment(const std::map<std::string, std::string>& environment)
  {
    size_t environ_size = 0;

    if (environ != nullptr) {
      for (char** global_envp = environ; global_envp[0] != nullptr; ++global_envp) {
        ++environ_size;
      }
    }

    char** envp = new char* [environ_size + environment.size() + 1];
    size_t i = 0;

    for (; i < environ_size; ++i) {
      envp[i] = cstrCopy(environ[i]);
    }

    for (const auto& map_entry : environment) {
      std::string environ_value;
      environ_value.append(map_entry.first);
      environ_value.append("=");
      environ_value.append(map_entry.second);
      envp[i++] = cstrCopy(environ_value.c_str());
    }

    envp[i] = nullptr;
    return envp;
  }

  void IPCSignalWatcher::destroyExecutableEnvironment(char** const envp)
  {
    if (envp != nullptr) {
      char** envp_current = envp;

      while (envp_current[0] != nullptr) {
        delete [] envp_current[0];
        ++envp_current;
      }

      delete [] envp;
    }
  }

  char* IPCSignalWatcher::cstrCopy(const char* c_str)
  {
    if (c_str == nullptr) {
      return nullptr;
    }

    const auto c_str_len = ::strlen(c_str);
    char* c_str_copy = new char [c_str_len + 1];
    /*
     * new throws std::bad_alloc on failure, no need
     * to check the pointer.
     */
    ::memcpy(c_str_copy, c_str, c_str_len);
    c_str_copy[c_str_len] = '\0';
    return c_str_copy;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
