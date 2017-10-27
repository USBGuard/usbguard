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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "USBGuard.hpp"
#include "ConfigFile.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <string>
#include <cstdlib>

#ifndef USBGUARD_DAEMON_CONF_PATH
  # warning "Using hard-coded USBGUARD_DAEMON_CONF_PATH value"
  #define USBGUARD_DAEMON_CONF_PATH "/etc/usbguard/usbguard-daemon.conf"
#endif

namespace usbguard
{
  std::string getDaemonConfigPath()
  {
    USBGUARD_LOG(Trace);
    const char* const  envval = getenv("USBGUARD_DAEMON_CONF");

    if (envval != nullptr) {
      USBGUARD_LOG(Debug) << "Returning environment variable path: " << envval;
      return std::string(envval);
    }
    else {
      USBGUARD_LOG(Debug) << "Returning build-time path: " << USBGUARD_DAEMON_CONF_PATH;
      return std::string(USBGUARD_DAEMON_CONF_PATH);
    }
  }

  std::string getIPCAccessControlFilesPath()
  {
    USBGUARD_LOG(Trace);
    const std::string daemon_conf_path = getDaemonConfigPath();
    ConfigFile daemon_conf;
    daemon_conf.open(daemon_conf_path);

    if (daemon_conf.hasSettingValue("IPCAccessControlFiles")) {
      return daemon_conf.getSettingValue("IPCAccessControlFiles");
    }

    throw Exception("getIPCAccessControlFilesPath", daemon_conf_path, "IPCAccessControlFiles not set");
  }

  std::string getIPCAccessControlFileBasename(const std::string& name, bool is_group)
  {
    USBGUARD_LOG(Trace) << "name=" << name << " is_group=" << is_group;
    std::string basename;

    if (is_group) {
      basename.append(":");
    }

    basename.append(name);
    return basename;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
