#pragma once
#include <IPCClient.hpp>

namespace usbguard
{
  class IPCSignalWatcher : public IPCClient
  {
  public:
    void IPCConnected();
    void IPCDisconnected();

    void DeviceInserted(uint32_t seqn,
                        const std::map<std::string,std::string>& attributes,
                        const std::vector<USBInterfaceType>& interfaces,
                        bool rule_match,
                        uint32_t rule_seqn);

    void DevicePresent(uint32_t seqn,
                       const std::map<std::string,std::string>& attributes,
                       const std::vector<USBInterfaceType>& interfaces,
                       Rule::Target target);

    void DeviceRemoved(uint32_t seqn,
                       const std::map<std::string,std::string>& attributes);

    void DeviceAllowed(uint32_t seqn,
                       const std::map<std::string,std::string>& attributes,
                       bool rule_match,
                       uint32_t rule_seqn);

    void DeviceBlocked(uint32_t seqn,
                       const std::map<std::string,std::string>& attributes,
                       bool rule_match,
                       uint32_t rule_seqn);

    void DeviceRejected(uint32_t seqn,
                        const std::map<std::string,std::string>& attributes,
                        bool rule_match,
                        uint32_t rule_seqn);
  };
} /* namespace usbguard */
