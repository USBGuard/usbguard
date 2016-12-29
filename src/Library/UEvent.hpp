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

namespace usbguard
{
  class UEvent
  {
  public:
    UEvent();
    UEvent(UEvent&& rhs);
    UEvent& operator=(UEvent&& rhs);

    static UEvent fromString(const String& uevent_string, bool attributes_only = false, bool trace = false);

    void clear();
    void setAttribute(const String& name, const String& value);
    String getAttribute(const String& name) const;
    bool hasAttribute(const String& name) const;
    bool hasRequiredAttributes() const;

    String getHeaderLine() const;
    String toString(char separator = '\0') const;

   private:
    StringKeyMap<String> _attributes;
  };
} /* namespace usbguard */
