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
#include "ConfigFilePrivate.hpp"

namespace usbguard
{
  ConfigFile::ConfigFile()
  {
    d_pointer = new ConfigFilePrivate(*this);
  }

  ConfigFile::~ConfigFile()
  {
    delete d_pointer;
  }

  void ConfigFile::open(const String& path)
  {
    d_pointer->open(path);
    return;
  }

  void ConfigFile::write()
  {
    d_pointer->write();
    return;
  }

  void ConfigFile::close()
  {
    d_pointer->close();
    return;
  }

  const String& ConfigFile::getSettingValue(const String& name) const
  {
    return d_pointer->getSettingValue(name);
  }

  void ConfigFile::setSettingValue(const String& name, String& value)
  {
    d_pointer->setSettingValue(name, value);
    return;
  }

  bool ConfigFile::hasSettingValue(const String& name) const
  {
    return d_pointer->hasSettingValue(name);
  }
} /* namespace usbguard */
