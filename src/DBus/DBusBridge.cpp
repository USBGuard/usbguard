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
// Authors: Sebastian Pipping <sebastian@pipping.org>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "DBusBridge.hpp"
#include <polkit/polkit.h>

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
      if (interface == DBUS_POLICY_INTERFACE) {
        handlePolicyMethodCall(method_name, parameters, invocation);
      }
      else if (interface == DBUS_DEVICES_INTERFACE) {
        handleDevicesMethodCall(method_name, parameters, invocation);
      }
      else if (interface == DBUS_ROOT_INTERFACE) {
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
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

      const char* name_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &name_cstr);
      std::string name(name_cstr);
      auto value = getParameter(name);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(s)", value.c_str()));
      return;
    }

    if (method_name == "setParameter") {
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

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
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

      const char* label_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &label_cstr);
      std::string label(label_cstr);
      auto rules = listRules(label);

      if (rules.size() > 0) {
        auto gvbuilder = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);

        try {
          for (auto rule : rules) {
            g_variant_builder_add(gvbuilder, "(us)",
              rule.getRuleID(),
              rule.toString().c_str());
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
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

      const char* rule_spec_cstr = nullptr;
      uint32_t parent_id = 0;
      gboolean temporary = false;
      g_variant_get(parameters, "(&sub)", &rule_spec_cstr, &parent_id, &temporary);
      std::string rule_spec(rule_spec_cstr);
      const uint32_t rule_id = appendRule(rule_spec, parent_id, !temporary);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(u)", rule_id));
      return;
    }

    if (method_name == "removeRule") {
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

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
    USBGUARD_LOG(Debug) << "dbus devices method call: " << method_name;

    if (method_name == "listDevices") {
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

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
      GDBusError authErrorCode = G_DBUS_ERROR_FAILED;
      const gchar* authErrorMessage = NULL;

      if (! isAuthorizedByPolkit(invocation, &authErrorCode, &authErrorMessage)) {
        g_dbus_method_invocation_return_error_literal(invocation, G_DBUS_ERROR, authErrorCode, authErrorMessage);
        return;
      }

      uint32_t device_id = 0;
      uint32_t target_integer = 0;
      gboolean permanent = false;
      g_variant_get(parameters, "(uub)", &device_id, &target_integer, &permanent);
      USBGUARD_LOG(Debug) << "DBus: applyDevicePolicy: Parsed device_id: " << device_id << " target_integer: " << target_integer <<
        " and permanent: " << permanent;
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
      DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE, "DevicePresenceChanged",
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
      DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE, "DevicePolicyChanged",
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

  void DBusBridge::DevicePolicyApplied(uint32_t id,
    Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
  {
    GVariantBuilder* gv_builder_attributes = deviceRuleToAttributes(device_rule);
    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
      DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE, "DevicePolicyApplied",
      g_variant_new("(uusua{ss})",
        id,
        Rule::targetToInteger(target_new),
        device_rule.c_str(),
        rule_id,
        gv_builder_attributes),
      nullptr);

    if (gv_builder_attributes != nullptr) {
      g_variant_builder_unref(gv_builder_attributes);
    }
  }

  void DBusBridge::PropertyParameterChanged(const std::string& name,
    const std::string& value_old,
    const std::string& value_new)
  {
    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
      DBUS_ROOT_PATH, DBUS_ROOT_INTERFACE, "PropertyParameterChanged",
      g_variant_new("(sss)",
        name.c_str(),
        value_old.c_str(),
        value_new.c_str()),
      nullptr);
  }

  void DBusBridge::ExceptionMessage(const std::string& context,
    const std::string& object,
    const std::string& reason)
  {
    g_dbus_connection_emit_signal(p_gdbus_connection, nullptr,
      DBUS_ROOT_PATH, DBUS_ROOT_INTERFACE, "ExceptionMessage",
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
    g_variant_builder_add(builder, "{ss}",
      "with-connect-type",
      device_rule.getWithConnectType().c_str());
    return builder;
  }

  std::string DBusBridge::formatGError(GError* error)
  {
    if (error) {
      std::stringstream formatGError;
      formatGError << error->message << " (code " << error->code << ")";
      return formatGError.str();
    }
    else {
      return "unknown error";
    }
  }

  bool DBusBridge::isAuthorizedByPolkit(GDBusMethodInvocation* invocation, GDBusError* authErrorCode,
    const gchar** authErrorMessage)
  {
    GError* error = NULL;
    USBGUARD_LOG(Trace) << "Extracting bus name...";
    const gchar* const /*no-free!*/ bus_name = g_dbus_method_invocation_get_sender (invocation);

    if (! bus_name) {
      USBGUARD_LOG(Trace) << "Failed to extract bus name.";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to extract bus name.";
      return false;
    }

    USBGUARD_LOG(Trace) << "Extracted bus name \"" << bus_name << "\".";
    USBGUARD_LOG(Trace) << "Extracting interface name...";
    const gchar* const /*no-free!*/ interfaceName = g_dbus_method_invocation_get_interface_name(invocation);

    if (! interfaceName) {
      USBGUARD_LOG(Trace) << "Failed to extract interface name.";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to extract interface name.";
      return false;
    }

    USBGUARD_LOG(Trace) << "Extracted interface name \"" << interfaceName << "\".";
    USBGUARD_LOG(Trace) << "Extracting method name...";
    const gchar* const /*no-free!*/ methodName = g_dbus_method_invocation_get_method_name(invocation);

    if (! methodName) {
      USBGUARD_LOG(Trace) << "Failed to extract method name.";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to extract method name.";
      return false;
    }

    std::stringstream action_id;
    action_id << interfaceName << "." << methodName;
    USBGUARD_LOG(Trace) << "Extracted method name \"" << methodName << "\".";
    USBGUARD_LOG(Trace) << "Creating a system bus Polkit subject...";
    PolkitSubject* const subject = polkit_system_bus_name_new(bus_name);

    if (! subject) {
      USBGUARD_LOG(Trace) << "Failed to create Polkit subject.";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to create Polkit subject.";
      return false;
    }

    USBGUARD_LOG(Trace) << "Created.";
    USBGUARD_LOG(Trace) << "Connecting with Polkit authority...";
    PolkitAuthority* const authority = polkit_authority_get_sync(/*cancellable=*/ NULL, &error);

    if (! authority) {
      USBGUARD_LOG(Trace) << "Failed to connect to Polkit authority: " << formatGError(error) << ".";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to connect to Polkit authority";
      g_error_free(error);
      g_object_unref(subject);
      return false;
    }

    USBGUARD_LOG(Trace) << "Connected.";
    USBGUARD_LOG(Trace) << "Customizing Polkit authentication dialog...";
    PolkitDetails* const details = polkit_details_new();

    if (! details) {
      USBGUARD_LOG(Trace) << "Failed to customize the Polkit authentication dialog.";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to customize the Polkit authentication dialog.";
      g_object_unref(authority);
      g_object_unref(subject);
      return false;
    }

    polkit_details_insert (details, "polkit.message", "This USBGuard action needs authorization");
    USBGUARD_LOG(Trace) << "Customized.";
    USBGUARD_LOG(Trace) << "Checking authorization of action \"" << action_id.str() << "\" with Polkit ...";
    GDBusMessage* const message = g_dbus_method_invocation_get_message (invocation);
    const PolkitCheckAuthorizationFlags flags = (g_dbus_message_get_flags (message) &
        G_DBUS_MESSAGE_FLAGS_ALLOW_INTERACTIVE_AUTHORIZATION)
      ? POLKIT_CHECK_AUTHORIZATION_FLAGS_ALLOW_USER_INTERACTION
      : POLKIT_CHECK_AUTHORIZATION_FLAGS_NONE;
    PolkitAuthorizationResult* const result = polkit_authority_check_authorization_sync
      (authority,
        subject,
        action_id.str().c_str(),
        details,
        flags,
        /*cancellable=*/ NULL,
        &error);

    if (! result) {
      USBGUARD_LOG(Trace) << "Failed to check back with Polkit for authoriation: " << formatGError(error) << ".";
      *authErrorCode = G_DBUS_ERROR_AUTH_FAILED;
      *authErrorMessage = "Failed to check back with Polkit for authoriation.";
      g_error_free(error);
      g_object_unref(details);
      g_object_unref(authority);
      g_object_unref(subject);
      return false;
    }

    gboolean isAuthorized = polkit_authorization_result_get_is_authorized(result);
    USBGUARD_LOG(Trace) << (isAuthorized ? "Authorized" : "Not authorized") << ".";

    if (! isAuthorized) {
      *authErrorCode = G_DBUS_ERROR_ACCESS_DENIED;
      *authErrorMessage = "Not authorized.";
    }

    g_object_unref(result);
    g_object_unref(details);
    g_object_unref(authority);
    g_object_unref(subject);
    return isAuthorized;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
