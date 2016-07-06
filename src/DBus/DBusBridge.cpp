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
#include "DBusBridge.hpp"

namespace usbguard
{
  DBusBridge::DBusBridge(GDBusConnection * const gdbus_connection,
      void(*ipc_callback)(bool))
    : p_gdbus_connection(gdbus_connection),
      p_ipc_callback(ipc_callback)
  {
  }

  DBusBridge::~DBusBridge()
  {
  }

  void DBusBridge::handleMethodCall(const std::string interface, const std::string method_name, GVariant * parameters, GDBusMethodInvocation * invocation)
  {
    if (!isConnected()) {
      g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
          G_DBUS_ERROR_NO_SERVER, "USBGuard DBus service is not connected to the daemon.");
      return;
    }

    if (interface == "org.usbguard.Policy") {
      handlePolicyMethodCall(method_name, parameters, invocation);
    }
    else if (interface == "org.usbguard.Devices") {
      handleDevicesMethodCall(method_name, parameters, invocation);
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
    return;
  }

  void DBusBridge::handlePolicyMethodCall(const std::string& method_name, GVariant * parameters, GDBusMethodInvocation * invocation)
  {
    if (method_name == "listRules") {
      auto rule_set = listRules();
      auto rules = rule_set.getRules();

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
        catch(...) {
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
      const char *rule_spec_cstr = nullptr;
      uint32_t parent_id = 0;
      uint32_t timeout_sec = 0;

      g_variant_get(parameters, "(&su)", &rule_spec_cstr, &parent_id);
      std::string rule_spec(rule_spec_cstr);

      const uint32_t rule_id = appendRule(rule_spec, parent_id, timeout_sec);
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

  void DBusBridge::handleDevicesMethodCall(const std::string& method_name, GVariant * parameters, GDBusMethodInvocation * invocation)
  {
    if (method_name == "listDevices") {
      const char *query_cstr = nullptr;
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
        catch(...) {
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

    if (method_name == "allowDevice" ||
        method_name == "blockDevice" ||
        method_name == "rejectDevice") {
      uint32_t device_id = 0;
      gboolean permanent = false;
      uint32_t timeout_sec = 0;

      g_variant_get(parameters, "(ub)", &device_id, &permanent);

      if (method_name == "allowDevice") {
        allowDevice(device_id, permanent, timeout_sec);
      }
      else if (method_name == "blockDevice") {
        blockDevice(device_id, permanent, timeout_sec);
      }
      else {
        rejectDevice(device_id, permanent, timeout_sec);
      }

      g_dbus_method_invocation_return_value(invocation, nullptr);
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method ");
    return;
  }

  void DBusBridge::IPCConnected()
  {
    if (p_ipc_callback != nullptr) {
      p_ipc_callback(/*connected=*/true);
    }
  }

  void DBusBridge::IPCDisconnected()
  {
    if (p_ipc_callback != nullptr) {
      p_ipc_callback(/*connected=*/false);
    }
  }

  void DBusBridge::DevicePresent(uint32_t id,
      const std::map<std::string,std::string>& attributes,
      const std::vector<usbguard::USBInterfaceType>& interfaces,
      usbguard::Rule::Target target)
  {
    GVariantBuilder *gv_builder_attributes = nullptr;
    if (!attributes.empty()) {
      gv_builder_attributes = g_variant_builder_new(G_VARIANT_TYPE_DICTIONARY);

      for (auto kv_pair : attributes) {
        g_variant_builder_add(gv_builder_attributes, "{ss}", kv_pair.first.c_str(), kv_pair.second.c_str());
      }
    }

    GVariantBuilder *gv_builder_interfaces = nullptr;
    if (!interfaces.empty()) {
      gv_builder_interfaces = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);
      for (auto interface : interfaces) {
        g_variant_builder_add(gv_builder_interfaces, "s", interface.typeString().c_str());
      }
    }

    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
        "/org/usbguard/Devices", "org.usbguard.Devices", "DevicePresent",
        g_variant_new("(ua{ss}ass)",
          id, gv_builder_attributes, gv_builder_interfaces, usbguard::Rule::targetToString(target).c_str()),
        nullptr);

    if (gv_builder_interfaces != nullptr) {
      g_variant_builder_unref(gv_builder_interfaces);
    }
    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
    return;
  }

  void DBusBridge::DeviceInserted(uint32_t id,
      const std::map<std::string,std::string>& attributes,
      const std::vector<USBInterfaceType>& interfaces,
      bool rule_match,
      uint32_t rule_id)
  {
    GVariantBuilder *gv_builder_attributes = nullptr;
    if (!attributes.empty()) {
      gv_builder_attributes = g_variant_builder_new(G_VARIANT_TYPE_DICTIONARY);

      for (auto kv_pair : attributes) {
        g_variant_builder_add(gv_builder_attributes, "{ss}", kv_pair.first.c_str(), kv_pair.second.c_str());
      }
    }

    GVariantBuilder *gv_builder_interfaces = nullptr;
    if (!interfaces.empty()) {
      gv_builder_interfaces = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);
      for (auto interface : interfaces) {
        g_variant_builder_add(gv_builder_interfaces, "s", interface.typeString().c_str());
      }
    }

    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
        "/org/usbguard/Devices", "org.usbguard.Devices", "DeviceInserted",
        g_variant_new("(ua{ss}asbu)",
          id, gv_builder_attributes, gv_builder_interfaces, rule_match, rule_id),
        nullptr);

    if (gv_builder_interfaces != nullptr) {
      g_variant_builder_unref(gv_builder_interfaces);
    }
    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
    return;
  }

  void DBusBridge::DeviceRemoved(uint32_t id,
      const std::map<std::string,std::string>& attributes)
  {
    GVariantBuilder *gv_builder_attributes = nullptr;
    if (!attributes.empty()) {
      gv_builder_attributes = g_variant_builder_new(G_VARIANT_TYPE_DICTIONARY);

      for (auto kv_pair : attributes) {
        g_variant_builder_add(gv_builder_attributes, "{ss}", kv_pair.first.c_str(), kv_pair.second.c_str());
      }
    }

    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
        "/org/usbguard/Devices", "org.usbguard.Devices", "DeviceRemoved",
        g_variant_new("(ua{ss})",
          id, gv_builder_attributes),
        nullptr);

    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
    return;
  }

  void DBusBridge::DeviceAllowed(uint32_t id,
      const std::map<std::string,std::string>& attributes,
      bool rule_match,
      uint32_t rule_id)
  {
    emitDevicePolicyDecision("DeviceAllowed", id, attributes, rule_match, rule_id);
  }


  void DBusBridge::DeviceBlocked(uint32_t id,
      const std::map<std::string,std::string>& attributes,
      bool rule_match,
      uint32_t rule_id)
  {
    emitDevicePolicyDecision("DeviceBlocked", id, attributes, rule_match, rule_id);
  }

  void DBusBridge::DeviceRejected(uint32_t id,
      const std::map<std::string,std::string>& attributes,
      bool rule_match,
      uint32_t rule_id)
  {
    emitDevicePolicyDecision("DeviceRejected", id, attributes, rule_match, rule_id);
  }

  void DBusBridge::emitDevicePolicyDecision(const char *policy_signal,
      uint32_t id,
      const std::map<std::string,std::string>& attributes,
      bool rule_match,
      uint32_t rule_id)
  {
    GVariantBuilder *gv_builder_attributes = nullptr;
    if (!attributes.empty()) {
      gv_builder_attributes = g_variant_builder_new(G_VARIANT_TYPE_DICTIONARY);

      for (auto kv_pair : attributes) {
        g_variant_builder_add(gv_builder_attributes, "{ss}", kv_pair.first.c_str(), kv_pair.second.c_str());
      }
    }

    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
        "/org/usbguard/Devices", "org.usbguard.Devices", policy_signal,
        g_variant_new("(ua{ss}bu)",
          id, gv_builder_attributes, rule_match, rule_id),
        nullptr);

    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
    return;
  }

} /* namespace usbguard */

