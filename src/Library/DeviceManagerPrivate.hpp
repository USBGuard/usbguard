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
#include <DeviceManager.hpp>
#include <Typedefs.hpp>
#include <RuleSet.hpp>
#include <Device.hpp>
#include <mutex>

namespace usbguard {
  class DeviceManagerHooks;
  class DeviceManagerPrivate
  {
  public:
    DeviceManagerPrivate(DeviceManager& p_instance, DeviceManagerHooks& hooks);
    DeviceManagerPrivate(DeviceManager& p_instance, const DeviceManagerPrivate& rhs);
    const DeviceManagerPrivate& operator=(const DeviceManagerPrivate& rhs);
    
    void insertDevice(Pointer<Device> device);
    Pointer<Device> removeDevice(uint32_t id);

    /* Returns a copy of the list of active USB devices */
    PointerVector<Device> getDeviceList();
    Pointer<Device> getDevice(uint32_t id);
    std::mutex& refDeviceMapMutex();

    /* Call Daemon instance hooks */
    void DeviceEvent(DeviceManager::EventType event, Pointer<Device> device);

  private:
    DeviceManager& _p_instance;
    DeviceManagerHooks& _hooks;
    mutable std::mutex _device_map_mutex;
    PointerMap<uint32_t, Device> _device_map;
  };

} /* namespace usbguard */
