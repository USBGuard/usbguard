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

#include <unordered_map>
#include <cstdint>

namespace usbguard
{
  class IPCServerPrivate;
  class DLL_PUBLIC IPCServer : public Interface
  {
  public:
    class AccessControl
    {
      public:
        enum class Section : uint8_t {
          NONE = 0,
          DEVICES = 1,
          POLICY = 2,
          PARAMETERS = 3,
          EXCEPTIONS = 4,
          ALL = 255
        };

        enum class Privilege : uint8_t {
          NONE = 0x00,
          LIST = 0x01,
          MODIFY = 0x02,
          DROP = 0x04,
          LISTEN = 0x08,
          ALL = 0xff
        };

        AccessControl();
        AccessControl(Section section, Privilege privilege);

        bool hasPrivilege(Section section, Privilege privilege) const;
        void setPrivilege(Section section, Privilege privilege);

      private:
        std::unordered_map<uint8_t,uint8_t> _access_control;
    };

    IPCServer();
    virtual ~IPCServer();

    void start();
    void stop();

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
    void addAllowedUsername(const std::string& username);
    void addAllowedGroupname(const std::string& groupname);

  private:
    IPCServerPrivate* d_pointer;
  };
} /* namespace usbguard */
