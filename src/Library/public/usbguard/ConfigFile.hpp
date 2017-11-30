//
// Copyright (C) 2017 Red Hat, Inc.
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
#include "usbguard/Logger.hpp"

#include <string>
#include <vector>
#include <memory>

namespace usbguard
{
  class ConfigFilePrivate;
  class DLL_PUBLIC ConfigFile
  {
  public:
    ConfigFile(const std::vector<std::string>& known_names = std::vector<std::string>());
    ~ConfigFile();

    void open(const std::string& path, bool readonly = false);
    void write();
    void close();

    void setSettingValue(const std::string& name, std::string& value);
    bool hasSettingValue(const std::string& name) const;
    const std::string& getSettingValue(const std::string& name) const;

  private:
    std::unique_ptr<ConfigFilePrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
