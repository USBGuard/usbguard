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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/ConfigFile.hpp"
#include "usbguard/KeyValueParser.hpp"


#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace usbguard
{
  class ConfigFilePrivate
  {
  public:
    ConfigFilePrivate(ConfigFile& p_instance, const std::vector<std::string>& known_names);
    ~ConfigFilePrivate();

    void open(const std::string& path, bool readonly);
    void write();
    void close();

    void setSettingValue(const std::string& name, std::string& value);
    bool hasSettingValue(const std::string& name) const;
    const std::string& getSettingValue(const std::string& name) const;

  protected:
    void parse();
    bool checkNVPair(const std::string& name, const std::string& value) const;

  private:
    struct NVPair {
      std::string name;
      std::string value;
      size_t line_number;
    };

    ConfigFile& _p_instance;
    std::string _path;
    std::fstream _stream;
    std::vector<std::string> _lines;
    std::map<std::string, NVPair> _settings;
    bool _dirty;
    bool _readonly;
    std::vector<std::string> _known_names;
  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
