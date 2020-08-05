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
#include "RuleSet.hpp"
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

  /**
   * @brief Manages and keeps track of active USB devices.
   *
   * Devices are stored in the device map. When an event occurs
   * on a device, the device manager aknowledges the hooks about the event.
   */
  class DLL_PUBLIC DeviceManager
  {
  public:

    /**
     * @brief Type of event that took place on the device.
     */
    enum class EventType {
      Present = 0, /**< USB device has been already present. */
      Insert = 1, /**< USB device has been inserted. */
      Update = 2, /**< Some writable attribute has been changed externally. */
      Remove = 3, /**< USB device has been ejected. */
    };

    /**
     * @brief Defines which devices are authorized by default.
     */
    enum class AuthorizedDefaultType {
      Keep = -128, /**< Do not change the authorization state. */
      None = 0, /**< Every new device starts out deauthorized. */
      All = 1, /**< Every new device starts out authorized. */
      Internal = 2, /**< Internal devices start out authorized,
                      external devices start out deauthorized
                      (this requires the ACPI tables to properly
                      label internal devices, and kernel support). */
    };

    /**
     * @brief Casts \link EventType event\endlink to it's integer
     * representation.
     *
     * @param event Event to cast to integer.
     * @return Integer representation of given event.
     */
    static uint32_t eventTypeToInteger(EventType event);

    /**
     * @brief Converts integer to it's \link EventType event\endlink
     * representation.
     *
     * @param event_integer Integer that should be converted.
     * @return \link EventType event\endlink for given integer.
     * @throw runtime_error If given integer is not a valid event.
     */
    static EventType eventTypeFromInteger(uint32_t event_integer);

    /**
     * @brief Converts \link EventType event\endlink to it's string
     * representation.
     *
     * @param event Event that should be converted.
     * @return String representation of given event.
     * @throw USBGUARD_BUG If given event is invalid.
     */
    static std::string eventTypeToString(EventType event);

    /**
     * @brief Casts \link AuthorizedDefaultType authorized default\endlink
     * to it's integer representation.
     *
     * @param authorized_default \link AuthorizedDefaultType Authorized
     * default\endlink to cast to integer.
     * @return Integer representation of given \link AuthorizedDefaultType
     * authorized default\endlink.
     */
    static int32_t authorizedDefaultTypeToInteger(AuthorizedDefaultType authorized_default);

    /**
     * @brief Converts integer to it's \link AuthorizedDefaultType authorized
     * default\endlink representation.
     *
     * @param authorized_default_integer Integer that should be converted.
     * @return \link AuthorizedDefaultType Authorized default\endlink
     * for given integer.
     * @throw runtime_error If given integer is not a valid
     * \link AuthorizedDefaultType authorized default\endlink.
     */
    static AuthorizedDefaultType authorizedDefaultTypeFromInteger(int32_t authorized_default_integer);

    /**
     * @brief Converts string to it's \link AuthorizedDefaultType authorized
     * default\endlink representation.
     *
     * @param authorized_default_string String that should be converted.
     * @return \link AuthorizedDefaultType Authorized default\endlink
     * for given string.
     * @throw Exception If given string does not contain valid
     * \link AuthorizedDefaultType authorized default\endlink.
     */
    static AuthorizedDefaultType authorizedDefaultTypeFromString(const std::string& authorized_default_string);

    /**
     * @brief Converts \link AuthorizedDefaultType authorized default\endlink
     * to it's string representation.
     *
     * @param authorized_default \link AuthorizedDefaultType Authorized
     * default\endlink that should be converted.
     * @return String representation of given authorized default.
     * @throw USBGUARD_BUG If given \link AuthorizedDefaultType authorized
     * default\endlink is invalid.
     */
    static const std::string authorizedDefaultTypeToString(AuthorizedDefaultType authorized_default);

    /**
     * @brief Constructs new device manager object with given hooks.
     *
     * @param hooks \link DeviceManagerHooks Device manager hook\endlink that
     * will get notified when an event or an exception occurs.
     */
    DeviceManager(DeviceManagerHooks& hooks);

    /**
     * @brief Constructs new device manager from a given device manager.
     *
     * @param rhs Device manager to copy.
     */
    DeviceManager(const DeviceManager& rhs);

    /**
     * @brief Constructs new device manager based on given device manager
     * and assignes it to this.
     *
     * @param rhs Device manager to copy.
     */
    const DeviceManager& operator=(const DeviceManager& rhs);

    /**
     * @brief Default destructor.
     */
    virtual ~DeviceManager();

    /**
     * @brief When a device is inserted into a system it's
     * \link AuthorizedDefaultType authorized default\endlink
     * will stay unchanged.
     *
     * @param state If true, enables enumeration only mode.
     */
    virtual void setEnumerationOnlyMode(bool state) = 0;

    /**
     * @brief Starts the deamon for monitoring USB device events.
     */
    virtual void start() = 0;

    /**
     * @brief Stops the daemon that is monitoring USB device events.
     */
    virtual void stop() = 0;

    /**
     * @brief Scans the system for USB devices.
     */
    virtual void scan() = 0;

    /**
     * @brief Scan for USB devices on given path.
     *
     * @param devpath Path to scan.
     */
    virtual void scan(const std::string& devpath) = 0;

    /**
     * @brief Enables you to set the default authorization of USB devices.
     *
     * @param authorized \link AuthorizedDefaultType authorized default\endlink
     * to be set.
     * @see \link AuthorizedDefaultType AuthorizedDefaultType\endlink
     */
    void setAuthorizedDefault(AuthorizedDefaultType authorized);

    /**
     * @brief Returns current \link AuthorizedDefaultType
     * authorized default\endlink setting.
     *
     * @return Current \link AuthorizedDefaultType authorized default\endlink
     * setting.
     * @see \link AuthorizedDefaultType AuthorizedDefaultType\endlink
     */
    AuthorizedDefaultType getAuthorizedDefault() const;

    /**
     * @brief Controls whether USBGuard daemon tries to restore the device
     * attribute values to the state before modification on shutdown.
     *
     * The USBGuard daemon modifies some attributes of controller devices
     * like the default authorization state of new child device instances.
     * Using this setting, you can control whether the daemon will try to
     * restore the attribute values to the state before modification on
     * shutdown.
     *
     * @warning If set to true, the USB authorization policy could be bypassed
     * by performing some sort of attack on the daemon (via local exploit
     * or via a USB device) to make it shutdown and restore to the
     * operating-system default state (known to be permissive).
     *
     * @param enabled Value to be set.
     */
    void setRestoreControllerDeviceState(bool enabled);

    /**
     * @brief Returns true if restore controller device state is enabled,
     * false otherwise.
     *
     * @return True if restore controller device state is enabled,
     * false otherwise.
     */
    bool getRestoreControllerDeviceState() const;

    /**
     * @brief Applies given policy to device with given ID.
     *
     * @param id ID of the device to which policy should be applied.
     * @param target Policy to apply.
     * @return Device with given ID and applied policy.
     */
    virtual std::shared_ptr<Device> applyDevicePolicy(uint32_t id, Rule::Target target) = 0;

    /**
     * @brief Adds new device into a device map.
     *
     * Device ID will be assigned by the device manager hooks.
     *
     * @param device Device to be added into map of devices.
     */
    virtual void insertDevice(std::shared_ptr<Device> device);

    /**
     * @brief Removes device with given ID from device map.
     *
     * @param id ID of the device that shall be removed.
     * @return Device that has been removed.
     * @throw Exception If device with such ID does not exist in device map.
     */
    std::shared_ptr<Device> removeDevice(uint32_t id);

    /**
     * @brief Returns a copy of the list of active USB devices.
     *
     * @return Copy of the list of active USB devices.
     */
    std::vector<std::shared_ptr<Device>> getDeviceList();

    /**
     * @brief Returns a copy of the list of active USB devices
     * with rule that applies to given query.
     *
     * @param query Rule to apply to.
     * @return Copy of the list of active USB devices
     * with rule that applies to given query.
     * @see \link Rule::appliesTo() appliesTo()\endlink
     */
    std::vector<std::shared_ptr<Device>> getDeviceList(const Rule& query);

    /**
     * @brief Returns device from a device map with a given ID.
     *
     * @param id ID of the device.
     * @return Device from a device map with a given ID.
     * @throw Exception If there is no device with such ID in device map.
     */
    std::shared_ptr<Device> getDevice(uint32_t id);

    /**
     * @brief Returns reference to device map mutex.
     *
     * @return Reference to device map mutex.
     */
    std::mutex& refDeviceMapMutex();

    /**
     * @brief Acknowledges daemon instance hooks about given event.
     *
     * Calls dmHookDeviceEvent() on object registered as hook.
     *
     * @param event Event that arose.
     * @param device Device that is part of the event.
     * @see \link DeviceManagerHooks::dmHookDeviceEvent()
     * dmHookDeviceEvent()\endlink
     */
    void DeviceEvent(EventType event, std::shared_ptr<Device> device);

    /**
     * @brief Acknowledges daemon instance hooks about given exception.
     *
     * Calls \link DeviceManagerHooks::dmHookDeviceException()
     * dmHookDeviceException()\endlink on object registered as hook.
     *
     * @param message Message of the exception.
     * @see \link DeviceManagerHooks::dmHookDeviceException()
     * dmHookDeviceException()\endlink
     */
    void DeviceException(const std::string& message);

    /**
     * @brief Creates \p UEventDeviceManager if backend is \"uevent\"
     * or creates \p UMockDevDeviceManager if backend is \"umockdev\".
     *
     * - uevent   - Netlink based implementation which uses sysfs to scan
     *            for present devices and an uevent netlink socket for
     *            receiving USB device related events.
     * - umockdev - Umockdev based device manager capable of simulating
     *            devices based on umockdev-record files. Useful for testing.
     *
     * @deprecated Backend \"udev\" is obsolete and \"uevent\" should be used instead.
     *
     * @param hooks \link DeviceManagerHooks Device manager hooks\endlink.
     * @param backend Type of device manager to create.
     * @return Object of type \p UEventDeviceManager or
     * \p UMockDevDeviceManager based on chosen backend.
     * @throw Exception If given backend is invalid.
     */
    static std::shared_ptr<DeviceManager> create(DeviceManagerHooks& hooks, const std::string& backend);

  private:
    std::unique_ptr<DeviceManagerPrivate> d_pointer;
  };

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
