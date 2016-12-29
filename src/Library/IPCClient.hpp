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
#include <Typedefs.hpp>
#include <Interface.hpp>
#include <DeviceManager.hpp>
#include <Exception.hpp>

namespace usbguard
{
  class IPCClientPrivate;
  class DLL_PUBLIC IPCClient : public Interface
  {
  public:
    IPCClient(bool connected = false);
    virtual ~IPCClient();

    void connect();
    void disconnect();
    bool isConnected() const;
    void wait();

    std::string setParameter(const std::string& name, const std::string& value) override;
    std::string getParameter(const std::string& name) override;

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id) override;
    void removeRule(uint32_t id) override;
    const RuleSet listRules(const std::string& query) override;
    const RuleSet listRules()
    {
      return listRules("match");
    }

    uint32_t applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent);
    const std::vector<Rule> listDevices(const std::string& query);
    const std::vector<Rule> listDevices() /* NOTE: left for compatibility */
    {
      return listDevices("match");
    }

    virtual void IPCConnected() {}
    virtual void IPCDisconnected(bool exception_initiated, const IPCException& exception) {}

    virtual void DevicePresenceChanged(uint32_t id,
                                       DeviceManager::EventType event,
                                       Rule::Target target,
                                       const std::string& device_rule) {}

    virtual void DevicePolicyChanged(uint32_t id,
                                     Rule::Target target_old,
                                     Rule::Target target_new,
                                     const std::string& device_rule,
                                     uint32_t rule_id) {}

    virtual void ExceptionMessage(const std::string& context,
                                  const std::string& object,
                                  const std::string& reason) {}

  private:
    IPCClientPrivate* d_pointer;
  };

} /* namespace usbguard */
