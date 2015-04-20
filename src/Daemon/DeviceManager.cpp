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
#include "DeviceManager.hpp"
#include "Daemon.hpp"

namespace usbguard {
  DeviceManager::DeviceManager(Daemon& daemon)
    : _daemon(daemon)
  {
  }

  DeviceManager::~DeviceManager()
  {
  }

  void DeviceManager::insertDevice(Pointer<Device> device)
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    const uint32_t seqn = _daemon.assignSeqn();
    device->setSeqn(seqn);
    _device_map[seqn] = device;
    return;
  }

  Pointer<Device> DeviceManager::removeDevice(uint32_t seqn)
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

  PointerVector<Device> DeviceManager::getDeviceList()
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    PointerVector<Device> devices;

    for (auto& map_entry : _device_map) {
      devices.push_back(map_entry.second);
    }

    return std::move(devices);
  }

  Pointer<Device> DeviceManager::getDevice(uint32_t seqn)
  {
    std::unique_lock<std::mutex> device_map_lock(_device_map_mutex);
    return _device_map.at(seqn);
  }

  void DeviceManager::DeviceInserted(Pointer<Device> device)
  {
    _daemon.dmDeviceInserted(device);
    return;
  }

  void DeviceManager::DevicePresent(Pointer<Device> device)
  {
    _daemon.dmDevicePresent(device);
    return;
  }

  void DeviceManager::DeviceRemoved(Pointer<Device> device)
  {
    _daemon.dmDeviceRemoved(device);
    return;
  }

  void DeviceManager::DeviceAllowed(Pointer<Device> device)
  {
    _daemon.dmDeviceAllowed(device);
    return;
  }

  void DeviceManager::DeviceBlocked(Pointer<Device> device)
  {
    _daemon.dmDeviceBlocked(device);
    return;
  }

  void DeviceManager::DeviceRejected(Pointer<Device> device)
  {
    _daemon.dmDeviceRejected(device);
    return;
  }

} /* namespace usbguard */

#if defined(__linux__)
# include "LinuxDeviceManager.hpp"
#endif

usbguard::Pointer<usbguard::DeviceManager> usbguard::DeviceManager::create(Daemon& daemon)
{
#if defined(__linux__)
  auto dm = usbguard::makePointer<usbguard::LinuxDeviceManager>(daemon);
#else
# error "No DeviceManager implementation available"
#endif
  return std::move(dm);
}
