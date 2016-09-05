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
#include <DeviceManager.hpp>
#include <DeviceManagerHooks.hpp>
#include "DeviceManagerPrivate.hpp"

namespace usbguard {
  uint32_t DeviceManager::eventTypeToInteger(DeviceManager::EventType event)
  {
    return static_cast<uint32_t>(event);
  }

  DeviceManager::EventType DeviceManager::eventTypeFromInteger(uint32_t event_integer)
  {
    switch(event_integer) {
      case static_cast<uint32_t>(EventType::Insert):
      case static_cast<uint32_t>(EventType::Update):
      case static_cast<uint32_t>(EventType::Remove):
        break;
      default:
        throw std::runtime_error("Invalid event type integer value");
    }
    return static_cast<EventType>(event_integer);
  }

  std::string DeviceManager::eventTypeToString(DeviceManager::EventType event)
  {
    switch(event) {
      case DeviceManager::EventType::Present:
        return "Present";
      case DeviceManager::EventType::Insert:
        return "Insert";
      case DeviceManager::EventType::Remove:
        return "Remove";
      case DeviceManager::EventType::Update:
        return "Update";
    }
    throw std::runtime_error("BUG: Unknown event type");
  }

  DeviceManager::DeviceManager(DeviceManagerHooks& hooks)
  {
    d_pointer = new DeviceManagerPrivate(*this, hooks);
    return;
  }

  DeviceManager::DeviceManager(const DeviceManager& rhs)
  {
    d_pointer = new DeviceManagerPrivate(*this, *rhs.d_pointer);
    return;
  }

  const DeviceManager& DeviceManager::operator=(const DeviceManager& rhs)
  {
    DeviceManagerPrivate* n_pointer = new DeviceManagerPrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
  }

  DeviceManager::~DeviceManager()
  {
    delete d_pointer;
    d_pointer = nullptr;
    return;
  }

  void DeviceManager::insertDevice(Pointer<Device> device)
  {
    d_pointer->insertDevice(device);
    return;
  }

  Pointer<Device> DeviceManager::removeDevice(uint32_t id)
  {
    return d_pointer->removeDevice(id);
  }

  PointerVector<Device> DeviceManager::getDeviceList()
  {
    return d_pointer->getDeviceList();
  }

  PointerVector<Device> DeviceManager::getDeviceList(const Rule& query)
  {
    PointerVector<Device> matching_devices;

    for (auto const& device : getDeviceList()) {
      if (query.appliesTo(device->getDeviceRule())) {
        switch(query.getTarget()) {
          case Rule::Target::Allow:
          case Rule::Target::Block:
            if (device->getTarget() == query.getTarget()) {
              matching_devices.push_back(device);
            }
            break;
          case Rule::Target::Device:
          case Rule::Target::Match:
            matching_devices.push_back(device);
            break;
          default:
            throw std::runtime_error("Invalid device query target");
        }
      }
    }

    return matching_devices;
  }

  Pointer<Device> DeviceManager::getDevice(uint32_t id)
  {
    return d_pointer->getDevice(id);
  }

  void DeviceManager::DeviceEvent(DeviceManager::EventType event, Pointer<Device> device)
  {
    d_pointer->DeviceEvent(event, device);
  }
} /* namespace usbguard */

#if defined(__linux__)
# include "LinuxDeviceManager.hpp"
#endif

usbguard::Pointer<usbguard::DeviceManager> usbguard::DeviceManager::create(DeviceManagerHooks& hooks)
{
#if defined(__linux__)
  auto dm = usbguard::makePointer<usbguard::LinuxDeviceManager>(hooks);
#else
# error "No DeviceManager implementation available"
#endif
  return std::move(dm);
}
