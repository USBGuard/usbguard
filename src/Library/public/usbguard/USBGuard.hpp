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

#include <string>

namespace usbguard
{
  /**
   * @brief Retrieves daemon config path from environment variable
   * \p USBGUARD_DAEMON_CONF\. If it fails, then it retrieves daemon config
   * path from build-time path \p USBGUARD_DAEMON_CONF_PATH.
   *
   * @return Daemon config path.
   */
  DLL_PUBLIC std::string getDaemonConfigPath();

  /**
   * @brief Reads daemon config file and retrieves setting value of
   * \p IPCAccessControlFiles.
   *
   * @return IPCAccessControlFiles path.
   * @throw Exception If setting \p IPCAccessControlFiles is not set in
   * daemon config file.
   */
  DLL_PUBLIC std::string getIPCAccessControlFilesPath();

  /**
   * Returns string \":<name>\" if name is group name, \"<name>\" otherwise.
   *
   * @param name User name or group name.
   * @param is_group Determines if name is a group name.
   * @return \p IPCAccessControlFile basename.
   */
  DLL_PUBLIC std::string getIPCAccessControlFileBasename(const std::string& name, bool is_group);
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
