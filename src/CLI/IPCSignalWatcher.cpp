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
//
#include <iostream>
#include "IPCSignalWatcher.hpp"

namespace usbguard
{
  void IPCSignalWatcher::IPCConnected()
  {
    std::cout << "[IPC] Connected" << std::endl;
  }

  void IPCSignalWatcher::IPCDisconnected(bool exception_initiated, const IPCException& exception)
  {
    std::cout << "[IPC] Disconnected: exception_initiated=" << exception_initiated;
    if (exception_initiated) {
      std::cout << " reason=" << exception.codeAsString();
      std::cout << " message=" << exception.message();
    }
    std::cout << std::endl;
  }

  void IPCSignalWatcher::DeviceInserted(uint32_t id, const std::map< std::string, std::string >& attributes, const std::vector< USBInterfaceType >& interfaces, bool rule_match, uint32_t rule_id)
  {
    std::cout << "[device] Inserted: id=" << id;
    for (auto attribute : attributes) {
      std::cout << " " << attribute.first << "=" << attribute.second;
    }

    std::cout << " interface={";
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
