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
#pragma once
#include <IPCClient.hpp>

namespace usbguard
{
  class IPCSignalWatcher : public IPCClient
  {
  public:
    void IPCConnected() override;
    void IPCDisconnected(bool exception_initiated, const IPCException& exception) override;

    void DeviceInserted(uint32_t id,
                        const std::map<std::string,std::string>& attributes,
                        const std::vector<USBInterfaceType>& interfaces,
                        bool rule_match,
                        uint32_t rule_id);

    void DevicePresent(uint32_t id,
                       const std::map<std::string,std::string>& attributes,
                       const std::vector<USBInterfaceType>& interfaces,
                       Rule::Target target);

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
  };
} /* namespace usbguard */
