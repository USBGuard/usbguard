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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "DeviceManagerPrivate.hpp"
#include "Common/Utility.hpp"

#include "usbguard/DeviceManagerHooks.hpp"
#include "usbguard/Exception.hpp"

namespace usbguard
{
  uint32_t DeviceManager::eventTypeToInteger(DeviceManager::EventType event)
  {
    return static_cast<uint32_t>(event);
  }

  DeviceManager::EventType DeviceManager::eventTypeFromInteger(uint32_t event_integer)
  {
    switch (event_integer) {
    case static_cast<uint32_t>(EventType::Insert):
    case static_cast<uint32_t>(EventType::Update):
    case static_cast<uint32_t>(EventType::Remove):
    case static_cast<uint32_t>(EventType::Present):
      break;

    default:
      throw std::runtime_error("Invalid event type integer value");
    }

    return static_cast<EventType>(event_integer);
  }

  std::string DeviceManager::eventTypeToString(DeviceManager::EventType event)
  {
    switch (event) {
    case DeviceManager::EventType::Present:
      return "Present";

    case DeviceManager::EventType::Insert:
      return "Insert";

    case DeviceManager::EventType::Remove:
      return "Remove";

    case DeviceManager::EventType::Update:
      return "Update";

    default:
      throw USBGUARD_BUG("unknown event type");
    }
  }

  DeviceManager::DeviceManager(DeviceManagerHooks& hooks)
    : d_pointer(usbguard::make_unique<DeviceManagerPrivate>(*this, hooks))
  {
  }

  DeviceManager::DeviceManager(const DeviceManager& rhs)
    : d_pointer(usbguard::make_unique<DeviceManagerPrivate>(*this, *rhs.d_pointer))
  {
  }

  const DeviceManager& DeviceManager::operator=(const DeviceManager& rhs)
  {
    d_pointer.reset(new DeviceManagerPrivate(*this, *rhs.d_pointer));
    return *this;
  }

  DeviceManager::~DeviceManager() = default;

  void DeviceManager::setRestoreControllerDeviceState(bool enabled)
  {
    d_pointer->setRestoreControllerDeviceState(enabled);
  }

  bool DeviceManager::getRestoreControllerDeviceState() const
  {
    return d_pointer->getRestoreControllerDeviceState();
  }

  void DeviceManager::insertDevice(std::shared_ptr<Device> device)
  {
    d_pointer->insertDevice(device);
  }

  std::shared_ptr<Device> DeviceManager::removeDevice(uint32_t id)
  {
    return d_pointer->removeDevice(id);
  }

  std::vector<std::shared_ptr<Device>> DeviceManager::getDeviceList()
  {
    return d_pointer->getDeviceList();
  }

  std::vector<std::shared_ptr<Device>> DeviceManager::getDeviceList(const Rule& query)
  {
    std::vector<std::shared_ptr<Device>> matching_devices;

    for (auto const& device : getDeviceList()) {
      if (query.appliesTo(device->getDeviceRule())) {
        switch (query.getTarget()) {
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

        case Rule::Target::Reject:
        case Rule::Target::Unknown:
        case Rule::Target::Empty:
        case Rule::Target::Invalid:
        default:
          throw std::runtime_error("Invalid device query target");
        }
      }
    }

    return matching_devices;
  }

  std::shared_ptr<Device> DeviceManager::getDevice(uint32_t id)
  {
    return d_pointer->getDevice(id);
  }

  void DeviceManager::DeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device)
  {
    d_pointer->DeviceEvent(event, device);
  }

  void DeviceManager::DeviceException(const std::string& message)
  {
    d_pointer->DeviceException(message);
  }
} /* namespace usbguard */

#if defined(HAVE_UEVENT)
  #include "UEventDeviceManager.hpp"
#endif

#if defined(HAVE_UMOCKDEV)
  #include "UMockdevDeviceManager.hpp"
#endif

std::shared_ptr<usbguard::DeviceManager> usbguard::DeviceManager::create(DeviceManagerHooks& hooks, const std::string& backend)
{
  if (backend == "udev") {
    USBGUARD_LOG(Warning) << "udev backend is OBSOLETE. Falling back to new default: uevent";
  }

#if defined(HAVE_UEVENT)

  if (backend == "uevent" || /* transition udev => uevent */backend == "udev") {
    return std::make_shared<usbguard::UEventDeviceManager>(hooks);
  }

#endif
#if defined(HAVE_UMOCKDEV)

  if (backend == "umockdev") {
    return std::make_shared<usbguard::UMockdevDeviceManager>(hooks);
  }

#endif
  throw Exception("DeviceManager", "backend", "requested backend is not available");
}

/* vim: set ts=2 sw=2 et */
