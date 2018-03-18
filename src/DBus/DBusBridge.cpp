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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "DBusBridge.hpp"

namespace usbguard
{
  DBusBridge::DBusBridge(GDBusConnection* const gdbus_connection,
    void(*ipc_callback)(bool))
    : p_gdbus_connection(gdbus_connection),
      p_ipc_callback(ipc_callback)
  {
  }

  DBusBridge::~DBusBridge()
  {
  }

  void DBusBridge::handleMethodCall(const std::string interface, const std::string method_name, GVariant* parameters,
    GDBusMethodInvocation* invocation)
  {
    if (!isConnected()) {
      g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_NO_SERVER, "USBGuard DBus service is not connected to the daemon.");
      return;
    }

    try {
      if (interface == "org.usbguard.Policy") {
        handlePolicyMethodCall(method_name, parameters, invocation);
      }
      else if (interface == "org.usbguard.Devices") {
        handleDevicesMethodCall(method_name, parameters, invocation);
      }
      else if (interface == "org.usbguard") {
        handleRootMethodCall(method_name, parameters, invocation);
      }
      else {
        g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
          G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
      }
    }
    catch (const Exception& ex) {
      g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_FAILED, "%s", ex.message().c_str());
    }
    catch (const std::exception& ex) {
      g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_FAILED, "%s", ex.what());
    }
    catch (...) {
      g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_FAILED, "BUG: Unknown exception");
    }

    return;
  }

  void DBusBridge::handleRootMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation)
  {
    if (method_name == "getParameter") {
      const char* name_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &name_cstr);
      std::string name(name_cstr);
      auto value = getParameter(name);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(s)", value.c_str()));
      return;
    }

    if (method_name == "setParameter") {
      const char* name_cstr = nullptr;
      const char* value_cstr = nullptr;
      g_variant_get(parameters, "(&s&s)", &name_cstr, &value_cstr);
      const std::string name(name_cstr);
      const std::string value(value_cstr);
      auto previous_value = setParameter(name, value);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(s)", previous_value.c_str()));
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
    return;
  }

  void DBusBridge::handlePolicyMethodCall(const std::string& method_name, GVariant* parameters, GDBusMethodInvocation* invocation)
  {
    if (method_name == "listRules") {
      const char* query_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &query_cstr);
      std::string query(query_cstr);
      auto rule_set = listRules(query);
      auto rules = rule_set->getRules();

      if (rules.size() > 0) {
        auto gvbuilder = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);

        try {
          for (auto rule : rules) {
            g_variant_builder_add(gvbuilder, "(us)",
              rule->getRuleID(),
              rule->toString().c_str());
          }

          g_dbus_method_invocation_return_value(invocation, g_variant_new("(a(us))", gvbuilder));
        }
        catch (...) {
          g_variant_builder_unref(gvbuilder);
          throw;
        }

        g_variant_builder_unref(gvbuilder);
      }
      else {
        g_dbus_method_invocation_return_value(invocation, g_variant_new("(a(us))", nullptr));
      }

      return;
    }

    if (method_name == "appendRule") {
      const char* rule_spec_cstr = nullptr;
      uint32_t parent_id = 0;
      g_variant_get(parameters, "(&su)", &rule_spec_cstr, &parent_id);
      std::string rule_spec(rule_spec_cstr);
      const uint32_t rule_id = appendRule(rule_spec, parent_id);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(u)", rule_id));
      return;
    }

    if (method_name == "removeRule") {
      uint32_t rule_id = 0;
      g_variant_get(parameters, "(u)", &rule_id);
      removeRule(rule_id);
      g_dbus_method_invocation_return_value(invocation, nullptr);
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
    return;
  }

  void DBusBridge::handleDevicesMethodCall(const std::string& method_name, GVariant* parameters,
    GDBusMethodInvocation* invocation)
  {
    if (method_name == "listDevices") {
      const char* query_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &query_cstr);
      std::string query(query_cstr);
      auto devices = listDevices(query);

      if (devices.size() > 0) {
        auto gvbuilder = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);

        try {
          for (auto device_rule : devices) {
            g_variant_builder_add(gvbuilder, "(us)",
              device_rule.getRuleID(),
              device_rule.toString().c_str());
          }

          g_dbus_method_invocation_return_value(invocation, g_variant_new("(a(us))", gvbuilder));
        }
        catch (...) {
          g_variant_builder_unref(gvbuilder);
          throw;
        }

        g_variant_builder_unref(gvbuilder);
      }
      else {
        g_dbus_method_invocation_return_value(invocation, g_variant_new("(a(us))", nullptr));
      }

      return;
    }

    if (method_name == "applyDevicePolicy") {
      uint32_t device_id = 0;
      uint32_t target_integer = 0;
      gboolean permanent = false;
      g_variant_get(parameters, "(uub)", &device_id, &target_integer, &permanent);
      const Rule::Target target = Rule::targetFromInteger(target_integer);
      const uint32_t rule_id = applyDevicePolicy(device_id, target, permanent);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(u)", rule_id));
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method ");
  }

  void DBusBridge::IPCConnected()
  {
    if (p_ipc_callback != nullptr) {
      p_ipc_callback(/*connected=*/true);
    }
  }

  void DBusBridge::IPCDisconnected(bool exception_initiated, const IPCException& exception)
  {
    (void)exception_initiated;
    (void)exception;

    if (p_ipc_callback != nullptr) {
      p_ipc_callback(/*connected=*/false);
    }
  }

  void DBusBridge::DevicePresenceChanged(uint32_t id,
    DeviceManager::EventType event,
    Rule::Target target,
    const std::string& device_rule)
  {
    GVariantBuilder* gv_builder_attributes = deviceRuleToAttributes(device_rule);
    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
      "/org/usbguard/Devices", "org.usbguard.Devices", "DevicePresenceChanged",
      g_variant_new("(uuusa{ss})",
        id,
        DeviceManager::eventTypeToInteger(event),
        Rule::targetToInteger(target),
        device_rule.c_str(),
        gv_builder_attributes),
      nullptr);

    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
  }

  void DBusBridge::DevicePolicyChanged(uint32_t id,
    Rule::Target target_old,
    Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
  {
    GVariantBuilder* gv_builder_attributes = deviceRuleToAttributes(device_rule);
    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
      "/org/usbguard/Devices", "org.usbguard.Devices", "DevicePolicyChanged",
      g_variant_new("(uuusua{ss})",
        id,
        Rule::targetToInteger(target_old),
        Rule::targetToInteger(target_new),
        device_rule.c_str(),
        rule_id,
        gv_builder_attributes),
      nullptr);

    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
  }

  void DBusBridge::ExceptionMessage(const std::string& context,
    const std::string& object,
    const std::string& reason)
  {
    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
      "/org/usbguard", "org.usbguard", "ExceptionMessage",
      g_variant_new("(sss)",
        context.c_str(),
        object.c_str(),
        reason.c_str()),
      nullptr);
  }

  GVariantBuilder* DBusBridge::deviceRuleToAttributes(const std::string& device_spec)
  {
    Rule device_rule = Rule::fromString(device_spec);
    GVariantBuilder* builder = g_variant_builder_new(G_VARIANT_TYPE_DICTIONARY);

    if (builder == nullptr) {
      return nullptr;
    }

    g_variant_builder_add(builder, "{ss}",
      "hash",
      device_rule.getHash().c_str());
    g_variant_builder_add(builder, "{ss}",
      "id",
      device_rule.getDeviceID().toString().c_str());
    g_variant_builder_add(builder, "{ss}",
      "name",
      device_rule.getName().c_str());
    g_variant_builder_add(builder, "{ss}",
      "parent-hash",
      device_rule.getParentHash().c_str());
    g_variant_builder_add(builder, "{ss}",
      "serial",
      device_rule.getSerial().c_str());
    g_variant_builder_add(builder, "{ss}",
      "via-port",
      device_rule.getViaPort().c_str());
    std::string with_interface_string;
    auto const& with_interface_vector = device_rule.attributeWithInterface().values();

    for (size_t i = 0; i < with_interface_vector.size(); ++i) {
      with_interface_string.append(with_interface_vector[i].toRuleString());

      if (i < (with_interface_vector.size() - 1)) {
        with_interface_string.append(" ");
      }
    }

    g_variant_builder_add(builder, "{ss}",
      "with-interface",
      with_interface_string.c_str());
    return builder;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
