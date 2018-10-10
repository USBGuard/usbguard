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

#include "DeviceManager.hpp"
#include "Interface.hpp"
#include "Rule.hpp"
#include "Typedefs.hpp"

#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <memory>

#include <cstddef>
#include <cstdint>

#include <unistd.h>
#include <sys/types.h>

namespace usbguard
{
  class IPCServerPrivate;
  class DLL_PUBLIC IPCServer : public Interface
  {
  public:
    static void checkAccessControlName(const std::string& name);

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

      static Section sectionFromString(const std::string& section_string);
      static std::string sectionToString(const Section section);

      enum class Privilege : uint8_t {
        NONE = 0x00,
        LIST = 0x01,
        MODIFY = 0x02,
        LISTEN = 0x08,
        ALL = 0xff
      };

      static Privilege privilegeFromString(const std::string& privilege_string);
      static std::string privilegeToString(const Privilege privilege);

      AccessControl();
      AccessControl(const std::string& access_control_string);
      AccessControl(Section section, Privilege privilege);
      AccessControl(const AccessControl& rhs);
      AccessControl& operator=(const AccessControl& rhs);

      bool hasPrivilege(Section section, Privilege privilege) const;
      void setPrivilege(Section section, Privilege privilege);
      void clear();
      void load(std::istream& stream);
      void save(std::ostream& stream) const;
      void merge(const AccessControl& rhs);
      void merge(const std::string& access_control_string);

    private:
      struct SectionHash {
        std::size_t operator()(Section value) const
        {
          return static_cast<std::size_t>(value);
        }
      };

      std::unordered_map<Section, uint8_t, SectionHash> _access_control;
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

    void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new);

    void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason);

    void addAllowedUID(uid_t uid, const IPCServer::AccessControl& ac);
    void addAllowedGID(gid_t gid, const IPCServer::AccessControl& ac);
    void addAllowedUsername(const std::string& username, const IPCServer::AccessControl& ac);
    void addAllowedGroupname(const std::string& groupname, const IPCServer::AccessControl& ac);

  private:
    std::unique_ptr<IPCServerPrivate> d_pointer;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
