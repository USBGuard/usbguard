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
#include <Rule.hpp>
#include <RuleSet.hpp>
#include <DeviceManager.hpp>
#include <DeviceManagerHooks.hpp>

namespace usbguard
{
  class PolicyGenerator : public DeviceManagerHooks
  {
  public:
    PolicyGenerator();

    void setWithHashAttribute(bool state);
    void setPortSpecificRules(bool state);
    void setPortSpeficicRuleNoSerial(bool state);
    void setExplicitCatchAllRule(bool state, Rule::Target target = Rule::Target::Block);

    void generate();
    const RuleSet& refRuleSet() const;

    void dmHookDeviceInserted(Pointer<Device> device);
    void dmHookDevicePresent(Pointer<Device> device);
    void dmHookDeviceRemoved(Pointer<Device> device);
    void dmHookDeviceAllowed(Pointer<Device> device);
    void dmHookDeviceBlocked(Pointer<Device> device);
    void dmHookDeviceRejected(Pointer<Device> device);
    uint32_t dmHookAssignSeqn();

  private:
    RuleSet _ruleset;
    Pointer<DeviceManager> _dm;

    bool _with_hash;
    bool _port_specific;
    bool _port_specific_noserial;
    bool _with_catchall;
    Rule::Target _catchall_target;
  };
} /* namespace usbguard */
