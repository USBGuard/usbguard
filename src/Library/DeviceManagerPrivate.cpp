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
#include "DeviceManagerPrivate.hpp"
#include <DeviceManagerHooks.hpp>

namespace usbguard {
  DeviceManagerPrivate::DeviceManagerPrivate(DeviceManager& p_instance, DeviceManagerHooks& hooks)
    : _p_instance(p_instance),
      _hooks(hooks)
  {
  }

  DeviceManagerPrivate::DeviceManagerPrivate(DeviceManager& p_instance, const DeviceManagerPrivate& rhs)
    : _p_instance(p_instance),
      _hooks(rhs._hooks)
  {
    *this = rhs;
    return;
  }
  
  const DeviceManagerPrivate& DeviceManagerPrivate::operator=(const DeviceManagerPrivate& rhs)
  {
    std::unique_lock<std::mutex> local_device_map_lock(_device_map_mutex);
    std::unique_lock<std::mutex> remote_device_map_lock(rhs._device_map_mutex);
    _device_map = rhs._device_map;
    return *this;
  }

  void DeviceManagerPrivate::insertDevice(Pointer<Device> device)
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    const uint32_t seqn = _hooks.dmHookAssignSeqn();
    device->setSeqn(seqn);
    _device_map[seqn] = device;
    return;
  }

  Pointer<Device> DeviceManagerPrivate::removeDevice(uint32_t seqn)
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    auto it = _device_map.find(seqn);
    if (it == _device_map.end()) {
      throw std::runtime_error("Uknown device, cannot remove from device map");
    }
    Pointer<Device> device = it->second;
    _device_map.erase(it);
    return device;
  }

  PointerVector<Device> DeviceManagerPrivate::getDeviceList()
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    PointerVector<Device> devices;

    for (auto& map_entry : _device_map) {
      devices.push_back(map_entry.second);
    }

    return std::move(devices);
  }

  Pointer<Device> DeviceManagerPrivate::getDevice(uint32_t seqn)
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    return _device_map.at(seqn);
  }

  void DeviceManagerPrivate::DeviceInserted(Pointer<Device> device)
  {
    _hooks.dmHookDeviceInserted(device);
    return;
  }

  void DeviceManagerPrivate::DevicePresent(Pointer<Device> device)
  {
    _hooks.dmHookDevicePresent(device);
    return;
  }

  void DeviceManagerPrivate::DeviceRemoved(Pointer<Device> device)
  {
    _hooks.dmHookDeviceRemoved(device);
    return;
  }

  void DeviceManagerPrivate::DeviceAllowed(Pointer<Device> device)
  {
    _hooks.dmHookDeviceAllowed(device);
    return;
  }

  void DeviceManagerPrivate::DeviceBlocked(Pointer<Device> device)
  {
    _hooks.dmHookDeviceBlocked(device);
    return;
  }

  void DeviceManagerPrivate::DeviceRejected(Pointer<Device> device)
  {
    _hooks.dmHookDeviceRejected(device);
    return;
  }
} /* namespace usbguard */
