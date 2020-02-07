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
#include "usbguard/KeyValueParser.hpp"

#include "Common/Utility.hpp"

#include "usbguard/Exception.hpp"
#include "usbguard/Logger.hpp"

#include <stdexcept>
#include <vector>
#include <string>

#include <cstddef>


namespace usbguard
{
  ConfigFilePrivate::ConfigFilePrivate(ConfigFile& p_instance, const std::vector<std::string>& known_names)
    : _p_instance(p_instance),
      _known_names(known_names)
  {
    (void)_p_instance;
    _dirty = false;
    _readonly = false;
  }

  ConfigFilePrivate::~ConfigFilePrivate()
  {
    close();
  }

  void ConfigFilePrivate::open(const std::string& path, bool readonly)
  {
    _readonly = readonly;

    if (_readonly) {
      _stream.open(path, std::ios::in);
    }
    else {
      _stream.open(path, std::ios::in|std::ios::out);
    }

    if (!_stream.is_open()) {
      throw Exception("Configuration", path, "unable to open the configuration file");
    }

    parse();
  }

  void ConfigFilePrivate::write()
  {
    if (!_stream.is_open()) {
      throw USBGUARD_BUG("ConfigFilePrivate::write: write() before open()");
    }

    if (_readonly) {
      throw USBGUARD_BUG("ConfigFilePrivate::write: not applicable in read-only mode");
    }

    if (_dirty) {
      /* Update _lines */
      for (auto const& map_entry : _settings) {
        const NVPair& setting = map_entry.second;
        _lines[setting.line_number - 1] = setting.name + "=" + setting.value;
      }
    }

    _stream.clear();
    _stream.seekp(0);

    for (auto const& line : _lines) {
      _stream << line << std::endl;

      if (!_stream.good()) {
        throw Exception("Configuration", "write", "failed to write configuration to disk");
      }
    }

    _stream.flush();
    _dirty = false;
  }

  void ConfigFilePrivate::close()
  {
    if (_stream.is_open()) {
      if (_dirty && !_readonly) {
        write();
      }

      _stream.close();
    }
  }

  const std::string& ConfigFilePrivate::getSettingValue(const std::string& name) const
  {
    const NVPair& setting = _settings.at(name);
    return setting.value;
  }

  void ConfigFilePrivate::setSettingValue(const std::string& name, std::string& value)
  {
    NVPair& setting = _settings.at(name);
    setting.value = value;
    _dirty = true;
  }

  bool ConfigFilePrivate::hasSettingValue(const std::string& name) const
  {
    return (_settings.count(name) != 0);
  }

  void ConfigFilePrivate::parse()
  {
    std::string config_line;
    size_t config_line_number = 0;
    KeyValueParser kvparser(_known_names, "=",
      /*case_sensitive=*/true,
      /*validate_keys=*/!_known_names.empty());

    while (std::getline(_stream, config_line)) {
      ++config_line_number;
      _lines.push_back(config_line);

      if ((config_line.size() < 1) || (config_line[0] == '#')) {
        continue;
      }

      auto p = kvparser.parseLine(config_line);
      NVPair& setting = _settings[p.first];
      setting.name = p.first;
      setting.value = p.second;
      setting.line_number = config_line_number;
      USBGUARD_LOG(Debug) << "Parsed: " << p.first << "=" << p.second;
    }
  }

  bool ConfigFilePrivate::checkNVPair(const std::string& name, const std::string& value) const
  {
    (void)value; /* TODO */

    /*
     * If the known_names array is empty, then consider
     * any name-value pair as known.
     */
    if (_known_names.empty()) {
      return true;
    }

    for (auto const& known_name : _known_names) {
      if (name == known_name) {
        return true;
      }
    }

    return false;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
