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

#include "ConfigFile.hpp"
#include "Common/Logging.hpp"
#include "Common/Utility.hpp"

namespace usbguard
{
  ConfigFile::ConfigFile()
  {
    _dirty = false;
  }

  ConfigFile::~ConfigFile()
  {
    if (_dirty && _stream) {
      write();
      _dirty = false;
    }
  }

  void ConfigFile::open(const String& path)
  {
    _stream.open(path, std::ios::in|std::ios::out);
    if (!_stream.is_open()) {
      throw std::runtime_error("Can't open " + path);
    }
    _dirty = false;
    parse();
  }

  void ConfigFile::write()
  {
    if (!_stream.is_open()) {
      throw std::runtime_error("BUG: ConfigFile::write: write() before open()");
    }

    if (_dirty) {
      /* Update _lines */
      for(auto const& map_entry : _settings) {
	const NVPair& setting = map_entry.second;
	_lines[setting.line_number - 1] = setting.name + "=" + setting.value;
      }
    }

    _stream.seekp(0);
    for (auto const& line : _lines) {
      _stream << line << std::endl;
    }
    _stream.flush();
    _dirty = false;
    return;
  }

  void ConfigFile::close()
  {
    if (_dirty && _stream) {
      write();
    }
    _dirty = false;
    _stream.close();
  }

  const String& ConfigFile::getSettingValue(const String& name) const
  {
    const NVPair& setting = _settings.at(name);
    return setting.value;
  }

  void ConfigFile::setSettingValue(const String& name, String& value)
  {
    NVPair& setting = _settings.at(name);
    setting.value = value;
    _dirty = true;
  }

  bool ConfigFile::hasSettingValue(const String& name) const
  {
    return (_settings.count(name) != 0);
  }

  void ConfigFile::parse()
  {
    String config_line;
    size_t config_line_number = 0;

    while(std::getline(_stream, config_line)) {
      ++config_line_number;
      _lines.push_back(config_line);

      const size_t nv_separator = config_line.find_first_of("=");
      if (nv_separator == String::npos) {
	continue;
      }

      String name = trim(config_line.substr(0, nv_separator));
      String value = config_line.substr(nv_separator + 1);

      if (name[0] == '#') {
	continue;
      }

      if (!checkNVPair(name, value)) {
	log->warn("Unknown name-value pair in configuration file: line {}: {} = {}",
		  config_line_number, name, value);
	continue;
      }

      NVPair& setting = _settings[name];

      setting.name = name;
      setting.value = value;
      setting.line_number = config_line_number;
    }

    return;
  }

  bool ConfigFile::checkNVPair(const String& name, const String& value) const
  {
    const StringVector known_names = { "RuleFile",
				       "LogToSylog",
				       "LogToConsole",
				       "LogToFile",
				       "WritePID",
				       "DebugMode" };
    for (auto const& known_name : known_names) {
      if (name == known_name) {
	return true;
      }
    }
    return false;
  }

} /* namespace usbguard */
