//
// Copyright (C) 2016 Red Hat, Inc.
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

#include "usbguard/IPCClient.hpp"

namespace usbguard
{
  class IPCSignalWatcher : public IPCClient
  {
  public:
    ~IPCSignalWatcher();

    void setExecutable(const std::string& path);

    void IPCConnected() override;
    void IPCDisconnected(bool exception_initiated, const IPCException& exception) override;

    void DevicePresenceChanged(uint32_t id,
      DeviceManager::EventType event,
      Rule::Target target,
      const std::string& device_rule) override;

    void DevicePolicyChanged(uint32_t id,
      Rule::Target target_old,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) override;

    void DevicePolicyApplied(uint32_t id,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) override;

    void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new) override;
  private:
    void openExecutable(const std::string& path);
    void closeExecutable();
    bool hasOpenExecutable() const;

    void runExecutable(const std::map<std::string, std::string>& environment);
    static char** createExecutableEnvironment(const std::map<std::string, std::string>& environment);
    static void destroyExecutableEnvironment(char** const envp);
    static char* cstrCopy(const char* c_str);

    std::string _exec_path;
    int _exec_path_fd {-1};
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
