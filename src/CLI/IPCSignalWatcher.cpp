#include <iostream>
#include "IPCSignalWatcher.hpp"

namespace usbguard
{
  void IPCSignalWatcher::IPCConnected()
  {
    std::cout << "[IPC] Connected" << std::endl;
  }

  void IPCSignalWatcher::IPCDisconnected()
  {
    std::cout << "[IPC] Disconnected" << std::endl;
  }

  void IPCSignalWatcher::DeviceInserted(uint32_t id, const std::map< std::string, std::string >& attributes, const std::vector< USBInterfaceType >& interfaces, bool rule_match, uint32_t rule_id)
  {
    std::cout << "[device] Inserted: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }

    std::cout << " if={";
    for (auto interface : interfaces) {
      std::cout << " " << interface.typeString();
    }
    std::cout << "}";
    std::cout << " rule_match=" << rule_match;
    std::cout << " rule_id=" << rule_id << std::endl;
  }

  void IPCSignalWatcher::DevicePresent(uint32_t id, const std::map< std::string, std::string >& attributes, const std::vector< USBInterfaceType >& interfaces, Rule::Target target)
  {
    std::cout << "[device] Present: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }

    std::cout << " if={";
    for (auto interface : interfaces) {
      std::cout << " " << interface.typeString();
    }
    std::cout << "}";
    std::cout << " target=" << Rule::targetToString(target) << std::endl;
  }

  void IPCSignalWatcher::DeviceRemoved(uint32_t id, const std::map< std::string, std::string >& attributes)
  {
    std::cout << "[device] Removed: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }
    std::cout << std::endl;
  }

  void IPCSignalWatcher::DeviceAllowed(uint32_t id, const std::map< std::string, std::string >& attributes, bool rule_match, uint32_t rule_id)
  {
    std::cout << "[device] Allowed: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }

    std::cout << " rule_match=" << rule_match;
    std::cout << " rule_id=" << rule_id << std::endl;
  }

  void IPCSignalWatcher::DeviceBlocked(uint32_t id, const std::map< std::string, std::string >& attributes, bool rule_match, uint32_t rule_id)
  {
    std::cout << "[device] Blocked: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }

    std::cout << " rule_match=" << rule_match;
    std::cout << " rule_id=" << rule_id << std::endl;
  }

  void IPCSignalWatcher::DeviceRejected(uint32_t id, const std::map< std::string, std::string >& attributes, bool rule_match, uint32_t rule_id)
  {
    std::cout << "[device] Rejected: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }

    std::cout << " rule_match=" << rule_match;
    std::cout << " rule_id=" << rule_id << std::endl;
  }
} /* namespace usbguard */
