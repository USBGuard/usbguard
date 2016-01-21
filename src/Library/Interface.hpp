//
// Copyright (C) 2015 Red Hat, Inc.
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

#include <Typedefs.hpp>
#include <USB.hpp>
#include <Rule.hpp>
#include <Device.hpp>
#include <string>
#include <map>
#include <vector>
#include <cstdint>

namespace usbguard
{
  class DLL_PUBLIC Interface
  {
  public:
    /* Methods */
    virtual uint32_t appendRule(const std::string& rule_spec,
				uint32_t parent_seqn,
				uint32_t timeout_sec) = 0;

    virtual void removeRule(uint32_t seqn) = 0;

    virtual const std::map<std::string, std::string> listRules() = 0;

    virtual void allowDevice(uint32_t seqn,
			     bool append,
			     uint32_t timeout_sec) = 0;

    virtual void blockDevice(uint32_t seqn,
			     bool append,
			     uint32_t timeout_sec) = 0;

    virtual void rejectDevice(uint32_t seqn,
			      bool append,
			      uint32_t timeout_sec) = 0;

    virtual const std::map<std::string, std::string> listDevices(const std::string& query) = 0;

    /* Signals */
    virtual void DeviceInserted(uint32_t seqn,
				const std::map<std::string,std::string>& attributes,
				const std::vector<USBInterfaceType>& interfaces,
				bool rule_match,
				uint32_t rule_seqn) = 0;

    virtual void DevicePresent(uint32_t seqn,
			       const std::map<std::string,std::string>& attributes,
			       const std::vector<USBInterfaceType>& interfaces,
			       Rule::Target target) = 0;

    virtual void DeviceRemoved(uint32_t seqn,
			       const std::map<std::string,std::string>& attributes) = 0;

    virtual void DeviceAllowed(uint32_t seqn,
			       const std::map<std::string,std::string>& attributes,
			       bool rule_match,
			       uint32_t rule_seqn) = 0;

    virtual void DeviceBlocked(uint32_t seqn,
			       const std::map<std::string,std::string>& attributes,
			       bool rule_match,
			       uint32_t rule_seqn) = 0;

    virtual void DeviceRejected(uint32_t seqn,
				const std::map<std::string,std::string>& attributes,
				bool rule_match,
				uint32_t rule_seqn) = 0;
  };
} /* namespace usbguard */
