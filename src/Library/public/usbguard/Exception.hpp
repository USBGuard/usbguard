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
#pragma once

#include "Typedefs.hpp"

#include <stdexcept>
#include <string>

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

#include <cerrno>
#include <cstdint>
#include <cstring>

namespace usbguard
{
  /**
   * @brief USBGuard exception.
   */
  class DLL_PUBLIC Exception : public std::exception
  {
  public:
    Exception(const std::string& context,
      const std::string& object,
      const std::string& reason_val)
      : _context(context),
        _object(object),
        _reason(reason_val)
    {
    }

    Exception(const Exception& rhs) = default;
    Exception& operator=(const Exception& rhs) = default;

    Exception(Exception&& rhs) = default;
    Exception& operator=(Exception&& rhs) = default;

    const std::string& context() const noexcept
    {
      return _context;
    }

    const std::string& object() const noexcept
    {
      return _object;
    }

    const std::string& reason() const noexcept
    {
      return _reason;
    }

    void setContext(const std::string& context)
    {
      _context = context;
    }

    void setObject(const std::string& object)
    {
      _object = object;
    }

    void setReason(const std::string& reason_val)
    {
      _reason = reason_val;
    }

    /**
     * @brief Returns exception message.
     *
     * Format: \<context\>: [\<object\>: ]\<reason\>
     *
     * @return Exception message.
     */
    virtual std::string message() const noexcept
    {
      try {
        return _context + ": " + (!_object.empty() ? _object + ": " : "") + _reason;
      }
      catch (...) {
        return "usbguard::Exception: exception^2";
      }
    }

    virtual const char* what() const noexcept
    {
      return "usbguard::Exception";
    }

  private:
    std::string _context;
    std::string _object;
    std::string _reason;
  };

#define USBGUARD_BUG(m) \
  ::usbguard::Exception(__PRETTY_FUNCTION__, "BUG", m)

  class ErrnoException : public Exception
  {
  public:
    ErrnoException(const std::string& context, const std::string& object, const int errno_value)
      : Exception(context, object, ErrnoException::reasonFromErrno(errno_value))
    {
    }

    static std::string reasonFromErrno(const int errno_value)
    {
      char buffer[1024] = "Unknown error";
      const char* error_message = buffer;
      // See "man strerror_r" for why we need these two cases:
#ifndef HAVE_GNU_STRERROR_R
      // We have the XSI-compliant version of strerror_r with int return
      strerror_r(errno_value, buffer, sizeof buffer);
#else
      // We have the GNU-specific version of strerror_r with char* return
      error_message = strerror_r(errno_value, buffer, sizeof buffer);
#endif
      return std::string(error_message);
    }
  };

#define USBGUARD_SYSCALL_THROW(context, syscall_bool_expression) \
  do { \
    if (syscall_bool_expression) { \
      throw usbguard::ErrnoException(context, #syscall_bool_expression, errno); \
    } \
  } while(0)

  class IPCException : public Exception
  {
  public:
    IPCException()
      : Exception("", "", "")
    {
    }

    IPCException(const Exception& exception,
      uint64_t message_id = 0)
      : Exception(exception),
        _message_id(message_id)
    {
    }

    IPCException(const std::string& context,
      const std::string& object,
      const std::string& reason,
      uint64_t message_id = 0)
      : Exception(context, object, reason),
        _message_id(message_id)
    {
    }

    IPCException(const IPCException& rhs)
      : Exception(rhs),
        _message_id(rhs._message_id)
    {
    }

    bool hasMessageID() const noexcept
    {
      return _message_id != 0;
    }

    uint64_t messageID() const noexcept
    {
      return _message_id;
    }

    void setMessageID(uint64_t message_id)
    {
      _message_id = message_id;
    }

  private:
    uint64_t _message_id{0};
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
