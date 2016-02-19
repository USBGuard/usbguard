#include "DBusBridge.hpp"

namespace usbguard
{
  DBusBridge::DBusBridge(GDBusConnection * const gdbus_connection)
    : p_gdbus_connection(gdbus_connection)
  {
  }

  DBusBridge::~DBusBridge()
  {
  }

  void DBusBridge::handleMethodCall(const std::string interface, const std::string method_name, GVariant * parameters, GDBusMethodInvocation * invocation)
  {
  }
} /* namespace usbguard */

