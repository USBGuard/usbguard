//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "Device.hpp"
#include "Rule.hpp"
#include "../RuleSet.hpp"
#include "Typedefs.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <cstdint>

namespace usbguard
{
  class DeviceManagerHooks;
  class DeviceManagerPrivate;
  class DLL_PUBLIC DeviceManager
  {
  public:
    enum class EventType {
      Present = 0,
      Insert = 1,
      Update = 2, /* use case: writable attribute changed externally */
      Remove = 3,
    };

    static uint32_t eventTypeToInteger(EventType event);
    static EventType eventTypeFromInteger(uint32_t event_integer);
    static std::string eventTypeToString(EventType event);

    DeviceManager(DeviceManagerHooks& hooks);
    DeviceManager(const DeviceManager& rhs);
    const DeviceManager& operator=(const DeviceManager& rhs);

    virtual ~DeviceManager();

    virtual void setDefaultBlockedState(bool state) = 0;
    virtual void setEnumerationOnlyMode(bool state) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void scan() = 0;

    void setRestoreControllerDeviceState(bool enabled);
    bool getRestoreControllerDeviceState() const;

    virtual std::shared_ptr<Device> applyDevicePolicy(uint32_t id, Rule::Target target) = 0;

    virtual void insertDevice(std::shared_ptr<Device> device);
    std::shared_ptr<Device> removeDevice(uint32_t id);

    /* Returns a copy of the list of active USB devices */
    std::vector<std::shared_ptr<Device>> getDeviceList();
    std::vector<std::shared_ptr<Device>> getDeviceList(const Rule& query);

    std::shared_ptr<Device> getDevice(uint32_t id);
    std::mutex& refDeviceMapMutex();

    /* Call Daemon instance hooks */
    void DeviceEvent(EventType event, std::shared_ptr<Device> device);
    void DeviceException(const std::string& message);

    static std::shared_ptr<DeviceManager> create(DeviceManagerHooks& hooks, const std::string& backend);

  private:
    std::unique_ptr<DeviceManagerPrivate> d_pointer;
  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
