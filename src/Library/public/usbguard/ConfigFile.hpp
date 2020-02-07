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

  /**
   * @brief Enables manipulation with the config file
   * and its settings.
   */
  class DLL_PUBLIC ConfigFile
  {
  public:

    /**
     * @brief Constructs ConfigFile object.
     *
     * @param known_names Names present in this vector will be considered
     *  as known key names in the config file. If this vector is empty,
     *  then any key name will be considered as known.
     */
    ConfigFile(const std::vector<std::string>& known_names = std::vector<std::string>());

    /**
     * @brief Destructs ConfigFile object.
     *
     * Calls \link close() close()\endlink before destroying ConfigFile object.
     *
     * @see \link close() close()\endlink
     */
    ~ConfigFile();

    /**
     * @brief Opens the config file.
     *
     * Opens stream to config file and parses the config file.
     *
     * @param path Path to the config file.
     * @param readonly Determines if the config file will be opened
     *  in readonly mode.
     */
    void open(const std::string& path, bool readonly = false);

    /**
     * @brief Writes changes into config file.
     *
     * If the config file was not opened or was opened as readonly,
     * then this call will fail.
     */
    void write();

    /**
     * @brief Closes the config file.
     *
     * If any changes were made and config file was not opened as readonly,
     * changes are written to the config file before stream is closed.
     * If config file is not opened, nothing happens.
     */
    void close();

    /**
     * @brief Sets value of given key to a given value.
     *
     * @param name Name of the key to set.
     * @param value Value to set.
     */
    void setSettingValue(const std::string& name, std::string& value);

    /**
     * @brief Checks if the config file contains key with a given name.
     *
     * @param name Name of the key to search for.
     * @return True if config file contains key with a given name,
     *  false otherwise.
     */
    bool hasSettingValue(const std::string& name) const;

    /**
     * @brief Retreives value of the given key.
     *
     * @param name Name of the key to search for.
     * @return Value that is assigned to a given key.
     */
    const std::string& getSettingValue(const std::string& name) const;

  private:
    std::unique_ptr<ConfigFilePrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
