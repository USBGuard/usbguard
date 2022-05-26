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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <gio/gio.h>
#pragma clang diagnostic pop

#define DBUS_SERVICE_NAME      "org.usbguard1"
#define DBUS_ROOT_INTERFACE    "org.usbguard1"
#define DBUS_ROOT_PATH         "/org/usbguard1"
#define DBUS_POLICY_INTERFACE  "org.usbguard.Policy1"
#define DBUS_POLICY_PATH       "/org/usbguard1/Policy"
#define DBUS_DEVICES_INTERFACE "org.usbguard.Devices1"
#define DBUS_DEVICES_PATH      "/org/usbguard1/Devices"

namespace usbguard
{
  class DBusBridge : public IPCClient
  {
  public:
    DBusBridge(GDBusConnection* const gdbus_connection,
      void(*ipc_callback)(bool) = nullptr);
    ~DBusBridge();

    void handleMethodCall(const std::string interface, const std::string method_name,
      GVariant* parameters, GDBusMethodInvocation* invocation);

  private:
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

    void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason) override;

    static GVariantBuilder* deviceRuleToAttributes(const std::string& device_spec);

    void handleRootMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation);
    void handlePolicyMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation);
    void handleDevicesMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation);

    void emitDevicePolicyDecision(const char* policy_signal,
      uint32_t id,
      const std::map<std::string, std::string>& attributes,
      bool rule_match,
      uint32_t rule_id);

    static std::string formatGError(GError* error);
    static bool isAuthorizedByPolkit(GDBusMethodInvocation* invocation, GDBusError* authError, const gchar** authErrorMessage);

    GDBusConnection* const p_gdbus_connection;
    void(*p_ipc_callback)(bool);
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
