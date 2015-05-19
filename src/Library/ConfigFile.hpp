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
#include <Typedefs.hpp>

namespace usbguard {
  class ConfigFilePrivate;
  class DLL_PUBLIC ConfigFile
  {
  public:
    ConfigFile(const StringVector& known_names);
    ~ConfigFile();

    void open(const String& path);
    void write();
    void close();

    void setSettingValue(const String& name, String& value);
    bool hasSettingValue(const String& name) const;
    const String& getSettingValue(const String& name) const;

  private:
    ConfigFilePrivate* d_pointer;
  };
} /* namespace usbguard */
