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

    // call interface handlers

    g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR,
        G_DBUS_ERROR_UNKNOWN_METHOD, "Unknown method interface");
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

