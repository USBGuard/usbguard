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

#include "IPCPrivate.hpp"

#include "Devices.pb.h"
#include "Policy.pb.h"
#include "Exception.pb.h"
#include "Parameter.pb.h"

#include "usbguard/Typedefs.hpp"
#include "usbguard/IPCServer.hpp"

namespace usbguard
{
  class IPCServerPrivate
  {
  public:
    virtual ~IPCServerPrivate() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void DevicePresenceChanged(uint32_t id,
      DeviceManager::EventType event,
      Rule::Target target,
      const std::string& device_rule) = 0;

    virtual void DevicePolicyChanged(uint32_t id,
      Rule::Target target_old,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) = 0;

    virtual void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new) = 0;

    virtual void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason,
      uint64_t request_id = 0) = 0;

    virtual void addAllowedUID(uid_t uid, const IPCServer::AccessControl& ac) = 0;
    virtual void addAllowedGID(gid_t gid, const IPCServer::AccessControl& ac) = 0;
    virtual void addAllowedUsername(const std::string& username, const IPCServer::AccessControl& ac) = 0;
    virtual void addAllowedGroupname(const std::string& groupname, const IPCServer::AccessControl& ac) = 0;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
