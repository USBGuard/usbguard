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
