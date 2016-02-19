#pragma once

#include <gio/gio.h>
#include "IPCClient.hpp"

namespace usbguard
{
  class DBusBridge : public IPCClient
  {
  public:
    DBusBridge(GDBusConnection * const gdbus_connection,
        void(*ipc_callback)(bool) = nullptr);
    ~DBusBridge();

    void handleMethodCall(const std::string interface, const std::string method_name,
        GVariant * parameters, GDBusMethodInvocation * invocation);

  protected:
    void IPCConnected();
    void IPCDisconnected();

  private:
    GDBusConnection * const p_gdbus_connection;
    void(*p_ipc_callback)(bool);
  };
} /* namespace usbguard */

