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
      InternalError,
    };

    IPCException(ReasonCode code, const std::string& message, uint64_t request_id = 0)
      : _message(message),
        _code(code),
        _request_id(request_id)
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

    uint64_t requestID() const
    {
      return _request_id;
    }

    void setRequestID(uint64_t id)
    {
      _request_id = id;
    }

    const char * const codeAsString() const
    {
      switch(_code) {
      case InvalidArgument:
        return "InvalidArgument";
      case NotFound:
        return "NotFound";
      case SyscallFailure:
        return "SyscallFailure";
      case PermissionDenied:
        return "PermissionDenied";
      case TransientError:
        return "TransientError";
      case ProtocolError:
        return "ProtocolError";
      case ConnectionError:
        return "ConnectionError";
      case InternalError:
        return "InternalError";
      }
      return "<unknown error code>";
    }

    static ReasonCode codeFromString(const std::string& code_string)
    {
      if (code_string == "InvalidArgument") {
        return InvalidArgument;
      }
      else if(code_string == "NotFound") {
        return NotFound;
      }
      else if(code_string == "SyscallFailure") {
        return PermissionDenied;
      }
      else if(code_string == "TransientError") {
        return TransientError;
      }
      else if(code_string == "ProtocolError") {
        return ProtocolError;
      }
      else if(code_string == "ConnectionError") {
        return ConnectionError;
      }
      else if(code_string == "InternalError") {
        return InternalError;
      }
      throw std::runtime_error("Invalid IPCException::ReasonCode string");
    }

  private:
    const std::string _message;
    ReasonCode _code;
    mutable uint64_t _request_id;
  };

  class IPC : public Interface
  {
  public:
    static uint64_t uniqueID(void)
    {
      static std::atomic<uint64_t> id(1);
      return id++;
    }
  };

} /* namespace usbguard */
