//
// Copyright (C) 2019 Red Hat, Inc.
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
//          Allen Webb <allenwebb@google.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "IPCPrivate.hpp"
#include "IPCServerPrivate.hpp"
#include "Common/Thread.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <gio/gio.h>
#pragma clang diagnostic pop

#include <functional>

namespace usbguard
{
  class DBusIPCServerPrivate : public IPCServerPrivate
  {
  public:
    DBusIPCServerPrivate(IPCServer& p_instance);
    virtual ~DBusIPCServerPrivate() final;

    virtual void start() final;
    virtual void stop() final;

    virtual void DevicePresenceChanged(uint32_t id,
      DeviceManager::EventType event,
      Rule::Target target,
      const std::string& device_rule) final;

    virtual void DevicePolicyChanged(uint32_t id,
      Rule::Target target_old,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id) final;

    virtual void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new) final;

    virtual void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason,
      uint64_t request_id = 0) final;

    virtual void addAllowedUID(uid_t uid, const IPCServer::AccessControl& ac) final;
    virtual void addAllowedGID(gid_t gid, const IPCServer::AccessControl& ac) final;
    virtual void addAllowedUsername(const std::string& username, const IPCServer::AccessControl& ac) final;
    virtual void addAllowedGroupname(const std::string& groupname, const IPCServer::AccessControl& ac) final;

    void HandleMethodCall(GDBusConnection* connection,
      const gchar*           sender,
      const gchar*           object_path,
      const gchar*           interface_name,
      const gchar*           method_name,
      GVariant*              parameters,
      GDBusMethodInvocation* invocation);

    void OnBusAcquired(GDBusConnection* connection, const gchar* name);
    void OnNameAcquired(GDBusConnection* connection, const gchar* name);
    void OnNameLost(GDBusConnection* connection, const gchar* name);
  private:
    static GVariantBuilder* deviceRuleToAttributes(const std::string& device_spec);

    void thread();
    void wakeup();
    void stopWithoutJoin();

    void handleRootMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation);
    void handlePolicyMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation);
    void handleDevicesMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation);

    bool _use_system_bus = true;

    IPCServer& _p_instance;

    const GDBusInterfaceVTable _dbus_interface_vtable;

    guint _dbus_owner_id = 0;
    GMainLoop* _dbus_main_loop = nullptr;
    GDBusNodeInfo* _dbus_introspection_data = nullptr;
    GDBusConnection* _gdbus_connection = nullptr;

    Thread<DBusIPCServerPrivate> _thread;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
