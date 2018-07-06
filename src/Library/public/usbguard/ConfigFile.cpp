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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "ConfigFilePrivate.hpp"
#include "Common/Utility.hpp"

namespace usbguard
{
  ConfigFile::ConfigFile(const std::vector<std::string>& known_names)
    : d_pointer(usbguard::make_unique<ConfigFilePrivate>(*this, known_names))
  {
  }

  ConfigFile::~ConfigFile() = default;

  void ConfigFile::open(const std::string& path, bool readonly)
  {
    d_pointer->open(path, readonly);
  }

  void ConfigFile::write()
  {
    d_pointer->write();
  }

  void ConfigFile::close()
  {
    d_pointer->close();
  }

  const std::string& ConfigFile::getSettingValue(const std::string& name) const
  {
    return d_pointer->getSettingValue(name);
  }

  void ConfigFile::setSettingValue(const std::string& name, std::string& value)
  {
    d_pointer->setSettingValue(name, value);
  }

  bool ConfigFile::hasSettingValue(const std::string& name) const
  {
    return d_pointer->hasSettingValue(name);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
