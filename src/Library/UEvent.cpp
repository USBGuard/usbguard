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

#include "UEvent.hpp"
#include "UEventParser.hpp"

#include "usbguard/Logger.hpp"

namespace usbguard
{
  UEvent::UEvent()
  {
  }

  UEvent::UEvent(UEvent&& rhs)
    : _attributes(std::move(rhs._attributes))
  {
  }

  UEvent& UEvent::operator=(UEvent&& rhs)
  {
    _attributes = std::move(rhs._attributes);
    return *this;
  }

  UEvent UEvent::fromString(const std::string& uevent_string, bool attributes_only, bool trace)
  {
    UEvent uevent;
    parseUEventFromString(uevent_string, uevent, attributes_only, trace);
    return uevent;
  }

  void UEvent::clear()
  {
    _attributes.clear();
  }

  void UEvent::setAttribute(const std::string& name, const std::string& value)
  {
    USBGUARD_LOG(Trace) << "Setting attribute: " << name << "=" << value;
    _attributes[name] = value;
  }

  std::string UEvent::getAttribute(const std::string& name) const
  {
    auto it = _attributes.find(name);

    if (it == _attributes.end()) {
      return std::string();
    }
    else {
      return it->second;
    }
  }

  bool UEvent::hasAttribute(const std::string& name) const
  {
    return _attributes.count(name) == 1;
  }

  std::string UEvent::getHeaderLine() const
  {
    if (!hasAttribute("ACTION") ||
      !hasAttribute("DEVPATH")) {
      throw std::runtime_error("uevent: missing required header line values");
    }

    std::string header_line;
    header_line.append(getAttribute("ACTION"));
    header_line.append(1, '@');
    header_line.append(getAttribute("DEVPATH"));
    return header_line;
  }

  std::string UEvent::toString(char separator) const
  {
    std::string uevent_string = getHeaderLine();
    uevent_string.append(1, separator);

    for (auto const& kv_pair : _attributes) {
      uevent_string.append(kv_pair.first);
      uevent_string.append(1, '=');
      uevent_string.append(kv_pair.second);
      uevent_string.append(1, separator);
    }

    return uevent_string;
  }

  bool UEvent::hasRequiredAttributes() const
  {
    for (const std::string name : {
        "ACTION", "DEVPATH", "SUBSYSTEM"
      }) {
      if (!hasAttribute(name)) {
        return false;
      }
    }
    return true;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
