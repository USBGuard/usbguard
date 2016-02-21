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
              rule->getSeqn(),
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
      uint32_t parent_seqn = 0;
      uint32_t timeout_sec = 0;

      g_variant_get(parameters, "(&su)", &rule_spec_cstr, &parent_seqn);
      std::string rule_spec(rule_spec_cstr);

      const uint32_t rule_id = appendRule(rule_spec, parent_seqn, timeout_sec);
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
} /* namespace usbguard */

