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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "usbguard/Device.hpp"
#include "usbguard/DeviceManager.hpp"
#include "usbguard/RuleSet.hpp"
#include "usbguard/Typedefs.hpp"

#include <map>
#include <memory>
#include <mutex>

namespace usbguard
{
  class DeviceManagerHooks;
  class DeviceManagerPrivate
  {
  public:
    DeviceManagerPrivate(DeviceManager& p_instance, DeviceManagerHooks& hooks);
    DeviceManagerPrivate(DeviceManager& p_instance, const DeviceManagerPrivate& rhs);
    const DeviceManagerPrivate& operator=(const DeviceManagerPrivate& rhs);

    void setAuthorizedDefault(DeviceManager::AuthorizedDefaultType authorized);
    DeviceManager::AuthorizedDefaultType getAuthorizedDefault() const;

    void setRestoreControllerDeviceState(bool enabled);
    bool getRestoreControllerDeviceState() const;

    void insertDevice(std::shared_ptr<Device> device);
    std::shared_ptr<Device> removeDevice(uint32_t id);

    /* Returns a copy of the list of active USB devices */
    std::vector<std::shared_ptr<Device>> getDeviceList();
    std::shared_ptr<Device> getDevice(uint32_t id);
    std::mutex& refDeviceMapMutex();

    /* Call Daemon instance hooks */
    void DeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device);
    void DeviceException(const std::string& message);

  private:
    DeviceManager& _p_instance;
    DeviceManagerHooks& _hooks;
    mutable std::mutex _device_map_mutex;
    std::map<uint32_t, std::shared_ptr<Device>> _device_map;
    DeviceManager::AuthorizedDefaultType _authorized_default{DeviceManager::AuthorizedDefaultType::None};
    bool _restore_controller_device_state{false};
  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
