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
#ifndef USBFW_EXCEPTIONS_HPP
#define USBFW_EXCEPTIONS_HPP
#include <build-config.h>

#include <stdexcept>

namespace usbguard
{
  /**
   * Exception class representing an error
   * in configuration. FirewallD has several
   * sources of configuration data.
   *  1) the main configuration file
   *  2) the XML definition files for zones, services, icmptypes
   */
  class ConfigurationError : public std::runtime_error
  {
  public:
    ConfigurationError(const std::string& source, const std::string& message, size_t line = 0)
      : std::runtime_error("Configuration error"),
	_source(source),
	_message(message),
	_line(line)
    {}

    const std::string source() const { return _source; }
    size_t line() const { return _line; }
    const std::string message() const { return _message; }

  private:
    const std::string _source; // configuration file specifier
    const std::string _message;
    size_t _line; // error line or 0 if unknown/unavailable
  };

  /**
   * Exception class representin an error
   * in the firewall abstraction layer.
   */
  class FirewallError : public std::runtime_error
  {
  public:
    FirewallError(const String& name, const String& message = String())
      : std::runtime_error("Firewall error"),
	_name(name),
	_message(message)
    {}

    const String& name() const { return _name; }
    const String& message() const { return _message; }

  private:
    const String _name;
    const String _message;
  };

} /* namespace usbguard */

#endif /* USBFW_EXCEPTIONS_HPP */
