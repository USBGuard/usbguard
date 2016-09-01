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
      std::cout << " message=" << exception.message();
    }
    std::cout << std::endl;
  }

  void IPCSignalWatcher::DevicePresenceChanged(uint32_t id,
                                               DeviceManager::EventType event,
                                               Rule::Target target,
                                               const std::string& device_rule)
  {
    std::cout << "[device] PresenceChanged: id=" << id << std::endl;
    std::cout << " event=" << DeviceManager::eventTypeToString(event) << std::endl;
    std::cout << " target=" << Rule::targetToString(target) << std::endl;
    std::cout << " device_rule=" << device_rule << std::endl;
  }

  void IPCSignalWatcher::DevicePolicyChanged(uint32_t id,
                                             Rule::Target target_old,
                                             Rule::Target target_new,
                                             const std::string& device_rule,
                                             uint32_t rule_id)
  {
    std::cout << "[device] PolicyChanged: id=" << id << std::endl;
    std::cout << " target_old=" << Rule::targetToString(target_old) << std::endl;
    std::cout << " target_new=" << Rule::targetToString(target_new) << std::endl;
    std::cout << " device_rule=" << device_rule << std::endl;
    std::cout << " rule_id=" << rule_id << std::endl;
  }
} /* namespace usbguard */
