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
#include "DeviceManager.hpp"
#include "Typedefs.hpp"

#include <memory>
#include <string>

#include <cstdint>

namespace usbguard
{
  /**
   * @brief Allows reacting to device events.
   */
  class DLL_PUBLIC DeviceManagerHooks
  {
  public:
    /**
     * @brief Device manager hook can react to USB device event
     * through this method.
     *
     * @param event USB device event.
     * @param device USB device that caused the event.
     */
    virtual void dmHookDeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device);

    /**
     * @brief Device manager hook can assign ID to the USB device through
     * this method.
     *
     * @return New ID of USB device.
     */
    virtual uint32_t dmHookAssignID() = 0;

    /**
     * @brief Device manager hook can react to USB device exceptions through
     * this method.
     *
     * @param message Exception message.
     */
    virtual void dmHookDeviceException(const std::string& message) = 0;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
