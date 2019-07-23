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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "DBusIPCServerPrivate.hpp"
#include "DBusConstants.hpp"

#include "usbguard/Exception.hpp"

static const gchar introspection_xml[] =
#include "../../DBus/DBusInterface.xml.cstr"
  ;

static const char DBUS_INIT_CONTEXT[] = "D-Bus IPC server initialization";
static const char DBUS_EXCEPTION_OBJECT[] = "D-Bus thread";

namespace usbguard
{
  namespace
  {
    void BusAcquiredCallback(GDBusConnection* connection, const gchar* name, gpointer user_data)
    {
      DBusIPCServerPrivate* p_instance = reinterpret_cast<DBusIPCServerPrivate*>(user_data);

      if (p_instance != nullptr) {
        p_instance->OnBusAcquired(connection, name);
      }
      else {
        throw Exception(DBUS_INIT_CONTEXT, DBUS_EXCEPTION_OBJECT, "Failed to register D-Bus: bus acquired");
      }
    }

    void NameAcquiredCallback(GDBusConnection* connection, const gchar* name, gpointer user_data)
    {
      DBusIPCServerPrivate* p_instance = reinterpret_cast<DBusIPCServerPrivate*>(user_data);

      if (p_instance != nullptr) {
        p_instance->OnNameAcquired(connection, name);
      }
      else {
        throw Exception(DBUS_INIT_CONTEXT, DBUS_EXCEPTION_OBJECT, "Failed to register D-Bus: name acquired");
      }
    }

    void NameLostCallback(GDBusConnection* connection, const gchar* name, gpointer user_data)
    {
      DBusIPCServerPrivate* p_instance = reinterpret_cast<DBusIPCServerPrivate*>(user_data);

      if (p_instance != nullptr) {
        p_instance->OnNameLost(connection, name);
      }
    }

    void MethodCallCallback(GDBusConnection* connection,
      const gchar* sender,
      const gchar* object_path,
      const gchar* interface_name,
      const gchar* method_name,
      GVariant* parameters,
      GDBusMethodInvocation* invocation,
      gpointer user_data)
    {
      DBusIPCServerPrivate* p_instance = reinterpret_cast<DBusIPCServerPrivate*>(user_data);

      if (p_instance != nullptr) {
        p_instance->HandleMethodCall(connection, sender, object_path, interface_name, method_name, parameters, invocation);
      }
    }
  } /* namespace */

  DBusIPCServerPrivate::DBusIPCServerPrivate(IPCServer& p_instance)
    : _p_instance(p_instance), _dbus_interface_vtable{
    MethodCallCallback,
    nullptr,
    nullptr,
    {}
  },
  _thread(this, &DBusIPCServerPrivate::thread) {}

  DBusIPCServerPrivate::~DBusIPCServerPrivate()
  {
    if (_thread.running()) {
      stop();
    }

    if (_dbus_introspection_data != nullptr) {
      g_dbus_node_info_unref(_dbus_introspection_data);
    }

    if (_dbus_main_loop != nullptr) {
      g_main_loop_unref(_dbus_main_loop);
    }
  }

  void DBusIPCServerPrivate::start()
  {
    if (!_thread.running()) {
      _thread.start();
    }
  }

  void DBusIPCServerPrivate::stopWithoutJoin()
  {
    _thread.stop(/*do_wait=*/false);

    if (_dbus_main_loop != nullptr) {
      g_main_loop_quit(_dbus_main_loop);
    }
  }

  void DBusIPCServerPrivate::stop()
  {
    if (_dbus_owner_id > 0) {
      g_bus_unown_name(_dbus_owner_id);
      _dbus_owner_id = 0;
    }

    stopWithoutJoin();
    _thread.wait();
  }

  void DBusIPCServerPrivate::thread()
  {
    if (_dbus_main_loop != nullptr) {
      throw USBGUARD_BUG("Tried to start the same thread more than once");
    }

    if ((_dbus_introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, nullptr)) == nullptr) {
      throw USBGUARD_BUG("Failed to parse dbus interface data");
    }

    _dbus_owner_id = g_bus_own_name (_use_system_bus ?
        G_BUS_TYPE_SYSTEM : G_BUS_TYPE_SESSION,
        DBUS_SERVICE_NAME,
        G_BUS_NAME_OWNER_FLAGS_NONE,
        BusAcquiredCallback,
        NameAcquiredCallback,
        NameLostCallback,
        /*user_data=*/this,
        /*user_data_free_func=*/nullptr);

    if (_dbus_owner_id <= 0) {
      throw Exception(DBUS_INIT_CONTEXT, DBUS_EXCEPTION_OBJECT, "Failed to request D-Bus name");
    }

    _dbus_main_loop = g_main_loop_new(NULL, FALSE);

    if (_dbus_main_loop == nullptr) {
      throw Exception(DBUS_INIT_CONTEXT, DBUS_EXCEPTION_OBJECT, "Failed to allocate glib main loop");
    }

    g_main_loop_run(_dbus_main_loop);
  }

  void DBusIPCServerPrivate::HandleMethodCall(GDBusConnection* connection,
    const gchar*           sender,
    const gchar*           object_path,
    const gchar*           interface_name,
    const gchar*           method_name,
    GVariant*              parameters,
    GDBusMethodInvocation* invocation)
  {
    (void)connection;
    (void)sender;
    (void)object_path;

    try {
      if (strcmp(interface_name, DBUS_POLICY_INTERFACE) == 0) {
        handlePolicyMethodCall(method_name, parameters, invocation);
      }
      else if (strcmp(interface_name, DBUS_DEVICES_INTERFACE) == 0) {
        handleDevicesMethodCall(method_name, parameters, invocation);
      }
      else if (strcmp(interface_name, DBUS_ROOT_INTERFACE) == 0) {
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
  }


  void DBusIPCServerPrivate::handleRootMethodCall(const std::string& method_name, GVariant* parameters,
    GDBusMethodInvocation* invocation)
  {
    if (method_name == "getParameter") {
      const char* name_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &name_cstr);
      std::string name(name_cstr);
      auto value = _p_instance.getParameter(name);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(s)", value.c_str()));
      return;
    }

    if (method_name == "setParameter") {
      const char* name_cstr = nullptr;
      const char* value_cstr = nullptr;
      g_variant_get(parameters, "(&s&s)", &name_cstr, &value_cstr);
      const std::string name(name_cstr);
      const std::string value(value_cstr);
      auto previous_value = _p_instance.setParameter(name, value);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(s)", previous_value.c_str()));
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
    return;
  }

  void DBusIPCServerPrivate::handlePolicyMethodCall(const std::string& method_name, GVariant* parameters,
    GDBusMethodInvocation* invocation)
  {
    if (method_name == "listRules") {
      const char* label_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &label_cstr);
      std::string label(label_cstr);
      auto rules = _p_instance.listRules(label);

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
      const char* rule_spec_cstr = nullptr;
      uint32_t parent_id = 0;
      bool temporary = false;
      g_variant_get(parameters, "(&sub)", &rule_spec_cstr, &parent_id, &temporary);
      std::string rule_spec(rule_spec_cstr);
      const uint32_t rule_id = _p_instance.appendRule(rule_spec, parent_id, !temporary);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(u)", rule_id));
      return;
    }

    if (method_name == "removeRule") {
      uint32_t rule_id = 0;
      g_variant_get(parameters, "(u)", &rule_id);
      _p_instance.removeRule(rule_id);
      g_dbus_method_invocation_return_value(invocation, nullptr);
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
    return;
  }

  void DBusIPCServerPrivate::handleDevicesMethodCall(const std::string& method_name, GVariant* parameters,
    GDBusMethodInvocation* invocation)
  {
    if (method_name == "listDevices") {
      const char* query_cstr = nullptr;
      g_variant_get(parameters, "(&s)", &query_cstr);
      std::string query(query_cstr);
      auto devices = _p_instance.listDevices(query);

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
      const uint32_t rule_id = _p_instance.applyDevicePolicy(device_id, target, permanent);
      g_dbus_method_invocation_return_value(invocation, g_variant_new("(u)", rule_id));
      return;
    }

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
      G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method ");
  }

  void DBusIPCServerPrivate::OnBusAcquired(GDBusConnection* connection, const gchar* name)
  {
    (void)name;
    USBGUARD_LOG(Info) << "DBusIPCServerPrivate::OnBusAcquired()";
    auto usbguard_rid = g_dbus_connection_register_object(connection,
        DBUS_ROOT_PATH,
        _dbus_introspection_data->interfaces[0],
        &_dbus_interface_vtable,
        /*user_data=*/this,
        /*user_data_free_func=*/nullptr,
        /*GError=*/nullptr);
    auto policy_rid = g_dbus_connection_register_object(connection,
        DBUS_POLICY_PATH,
        _dbus_introspection_data->interfaces[1],
        &_dbus_interface_vtable,
        /*user_data=*/this,
        /*user_data_free_func=*/nullptr,
        /*GError=*/nullptr);
    auto devices_rid = g_dbus_connection_register_object(connection,
        DBUS_DEVICES_PATH,
        _dbus_introspection_data->interfaces[2],
        &_dbus_interface_vtable,
        /*user_data=*/this,
        /*user_data_free_func=*/nullptr,
        /*GError=*/nullptr);

    if (policy_rid <= 0 || devices_rid <= 0 || usbguard_rid <= 0) {
      stopWithoutJoin();
      throw Exception(DBUS_INIT_CONTEXT, DBUS_EXCEPTION_OBJECT, "Unable to register required objects on the bus");
    }
  }

  void DBusIPCServerPrivate::OnNameAcquired(GDBusConnection* connection, const gchar* name)
  {
    (void)name;
    USBGUARD_LOG(Info) << "DBusIPCServerPrivate::OnNameAcquired()";
    _gdbus_connection = connection;
  }

  void DBusIPCServerPrivate::OnNameLost(GDBusConnection* connection, const gchar* name)
  {
    (void)connection;
    (void)name;
    USBGUARD_LOG(Info) << "DBusIPCServerPrivate::OnNameLost()";
    _gdbus_connection = nullptr;
    if (_thread.running()) {
      stopWithoutJoin();
      throw Exception(DBUS_INIT_CONTEXT, DBUS_EXCEPTION_OBJECT, "D-Bus name lost");
    }
  }

  void DBusIPCServerPrivate::DevicePresenceChanged(uint32_t id,
    DeviceManager::EventType event,
    Rule::Target target,
    const std::string& device_rule)
  {
    if (_gdbus_connection == nullptr) {
      return;
    }

    GVariantBuilder* gv_builder_attributes = deviceRuleToAttributes(device_rule);
    g_dbus_connection_emit_signal(_gdbus_connection, nullptr,
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

  void DBusIPCServerPrivate::DevicePolicyChanged(uint32_t id,
    Rule::Target target_old,
    Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
  {
    if (_gdbus_connection == nullptr) {
      return;
    }

    GVariantBuilder* gv_builder_attributes = deviceRuleToAttributes(device_rule);
    g_dbus_connection_emit_signal(_gdbus_connection, nullptr,
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

  void DBusIPCServerPrivate::PropertyParameterChanged(const std::string& name,
    const std::string& value_old,
    const std::string& value_new)
  {
    if (_gdbus_connection == nullptr) {
      return;
    }

    g_dbus_connection_emit_signal(_gdbus_connection, nullptr,
      DBUS_ROOT_PATH, DBUS_ROOT_INTERFACE, "PropertyParameterChanged",
      g_variant_new("(sss)",
        name.c_str(),
        value_old.c_str(),
        value_new.c_str()),
      nullptr);
  }

  void DBusIPCServerPrivate::ExceptionMessage(const std::string& context,
    const std::string& object,
    const std::string& reason,
    uint64_t request_id)
  {
    (void)request_id;

    if (_gdbus_connection == nullptr) {
      return;
    }

    g_dbus_connection_emit_signal(_gdbus_connection, nullptr,
      DBUS_ROOT_PATH, DBUS_ROOT_INTERFACE, "ExceptionMessage",
      g_variant_new("(sss)",
        context.c_str(),
        object.c_str(),
        reason.c_str()),
      nullptr);
  }

  GVariantBuilder* DBusIPCServerPrivate::deviceRuleToAttributes(const std::string& device_spec)
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

  // These functions are intentionally left empty because enforcement is
  // performed either through D-Bus itself or PolicyKit/PolKit. The D-Bus IPC
  // mechanism will effectively ignore policy defined through these functions.
  // To put limits on access to the USBGuard D-Bus interface, either change the
  // Polkit policy or the permissions in src/DBus/org.usguard1.conf if PolKit
  // is disabled.
  void DBusIPCServerPrivate::addAllowedUID(uid_t uid, const IPCServer::AccessControl& ac)
  {
    (void)uid;
    (void)ac;
  }

  void DBusIPCServerPrivate::addAllowedGID(gid_t gid, const IPCServer::AccessControl& ac)
  {
    (void)gid;
    (void)ac;
  }

  void DBusIPCServerPrivate::addAllowedUsername(const std::string& username, const IPCServer::AccessControl& ac)
  {
    (void)username;
    (void)ac;
  }

  void DBusIPCServerPrivate::addAllowedGroupname(const std::string& groupname, const IPCServer::AccessControl& ac)
  {
    (void)groupname;
    (void)ac;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */