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
//          Radovan Sroka <rsroka@redhat.com>
//
#pragma once

#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/DeviceManager.hpp"
#include "usbguard/DeviceManagerHooks.hpp"
#include "usbguard/Rule.hpp"
#include "usbguard/RuleSet.hpp"
#include "usbguard/MemoryRuleSet.hpp"

namespace usbguard
{
  class PolicyGenerator : public DeviceManagerHooks
  {
  public:
    PolicyGenerator();

    void setWithHashAttribute(bool state);
    void setHashOnly(bool state);
    void setPortSpecificRules(bool state);
    void setPortSpecificNoSerialRules(bool state);
    void setDevpath(const std::string& devpath);
    void setExplicitCatchAllRule(bool state, Rule::Target target = Rule::Target::Block);

    void generate();
    const std::vector<std::shared_ptr<RuleSet>> refRuleSet() const;

    void dmHookDeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device) override;
    uint32_t dmHookAssignID() override;
    void dmHookDeviceException(const std::string& message) override;

  private:
    std::vector<std::shared_ptr<RuleSet>> _ruleset;
    std::shared_ptr<DeviceManager> _dm;

    bool _with_hash;
    bool _hash_only;
    bool _port_specific;
    bool _port_specific_noserial;
    std::string _devpath;
    bool _with_catchall;
    Rule::Target _catchall_target;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
