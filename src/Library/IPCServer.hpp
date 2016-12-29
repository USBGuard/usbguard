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
#include <Typedefs.hpp>
#include <Interface.hpp>
#include <Rule.hpp>
#include <DeviceManager.hpp>

namespace usbguard
{
  class IPCServerPrivate;
  class DLL_PUBLIC IPCServer : public Interface
  {
  public:
    IPCServer();
    virtual ~IPCServer();

    void start();
    void stop();

#if 0
    virtual uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id) = 0;
    virtual void removeRule(uint32_t id) = 0;
    virtual const RuleSet listRules(const std::string& query) = 0;

    virtual uint32_t applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent) = 0;
    virtual const std::vector<Rule> listDevices(const std::string& query) = 0;
#endif

    void DevicePresenceChanged(uint32_t id,
                               DeviceManager::EventType event,
                               Rule::Target target,
                               const std::string& device_rule);

    void DevicePolicyChanged(uint32_t id,
                             Rule::Target target_old,
                             Rule::Target target_new,
                             const std::string& device_rule,
                             uint32_t rule_id);

    void ExceptionMessage(const std::string& context,
                          const std::string& object,
                          const std::string& reason);

    void addAllowedUID(uid_t uid);
    void addAllowedGID(gid_t gid);

  private:
    IPCServerPrivate* d_pointer;
  };
} /* namespace usbguard */
