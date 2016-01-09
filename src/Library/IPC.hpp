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

#include <Interface.hpp>
#include <string>
#include <stdexcept>
#include <atomic>
#include <cstdint>

namespace usbguard
{
  class IPCException : public std::exception
  {
  public:
    enum ReasonCode {
      InvalidArgument, /**< Incorrect value provided (out-of-range, invalid type, ...) */
      NotFound, /**< No such object found (rule or device) */
      SyscallFailure, /**< An underlying system call failed */
      PermissionDenied, /**< The caller is not allowed to perform the requested action */
      TransientError, /**< The requested action cannot be executed at the moment, try again */
      ProtocolError, /**< An IPC protocol error (missing attribute, return value, incorrect type */
      ConnectionError,
    };

    IPCException(ReasonCode code, const std::string& message)
      : _message(message),
        _code(code)
    {
    }

    const char *what() const noexcept
    {
      return _message.c_str();
    }

    ReasonCode code() const
    {
      return _code;
    }

    const char * const codeAsString() const
    {
      switch(_code) {
      case InvalidArgument:
        return "Invalid Argument";
      case NotFound:
        return "Not Found";
      case SyscallFailure:
        return "Syscall Failure";
      case PermissionDenied:
        return "Permission Denied";
      case TransientError:
        return "Transient Error";
      case ProtocolError:
        return "Protocol Error";
      case ConnectionError:
        return "Connection Error";
      }
      return "<unknown error code>";
    }

  private:
    const std::string _message;
    ReasonCode _code;
  };

  class IPC : public Interface
  {
  public:
    static uint64_t uniqueID(void)
    {
      static std::atomic<uint64_t> id(0);
      return id++;
    }
  };

} /* namespace usbguard */
