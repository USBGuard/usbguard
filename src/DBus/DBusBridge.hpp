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

    void handlePolicyMethodCall(const std::string& method_name, GVariant * parameters, GDBusMethodInvocation * invocation);
    void handleDevicesMethodCall(const std::string& method_name, GVariant * parameters, GDBusMethodInvocation * invocation);

    void DevicePresent(uint32_t id,
        const std::map<std::string,std::string>& attributes,
        const std::vector<usbguard::USBInterfaceType>& interfaces,
        usbguard::Rule::Target target);

    void DeviceInserted(uint32_t id,
        const std::map<std::string,std::string>& attributes,
        const std::vector<USBInterfaceType>& interfaces,
        bool rule_match,
        uint32_t rule_id);

    void DeviceRemoved(uint32_t id,
        const std::map<std::string,std::string>& attributes);

    void DeviceAllowed(uint32_t id,
        const std::map<std::string,std::string>& attributes,
        bool rule_match,
        uint32_t rule_id);

    void DeviceBlocked(uint32_t id,
        const std::map<std::string,std::string>& attributes,
        bool rule_match,
        uint32_t rule_id);

    void DeviceRejected(uint32_t id,
        const std::map<std::string,std::string>& attributes,
        bool rule_match,
        uint32_t rule_id);

    void emitDevicePolicyDecision(const char *policy_signal,
        uint32_t id,
        const std::map<std::string,std::string>& attributes,
        bool rule_match,
        uint32_t rule_id);

  private:
    GDBusConnection * const p_gdbus_connection;
    void(*p_ipc_callback)(bool);
  };
} /* namespace usbguard */

