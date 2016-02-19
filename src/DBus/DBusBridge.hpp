#pragma once

#include <gio/gio.h>
#include "IPCClient.hpp"

namespace usbguard
{
  class DBusBridge : public IPCClient
  {
  public:
    DBusBridge(GDBusConnection * const gdbus_connection);
    ~DBusBridge();

    void handleMethodCall(const std::string interface, const std::string method_name,
        GVariant * parameters, GDBusMethodInvocation * invocation);

  private:
    GDBusConnection * const p_gdbus_connection;
  };
} /* namespace usbguard */

