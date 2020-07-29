//
// Copyright (C) 2016 Red Hat, Inc.
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

#include "SysFSDevice.hpp"

#include "usbguard/DeviceManager.hpp"
#include "usbguard/Device.hpp"
#include "usbguard/Rule.hpp"

#include <dirent.h>

namespace usbguard
{
  class DeviceBase;

  class DeviceManagerBase : public DeviceManager
  {
    using DeviceManager::insertDevice;

  public:
    DeviceManagerBase(DeviceManagerHooks& hooks);
    virtual ~DeviceManagerBase();

    void setEnumerationOnlyMode(bool state) override;

    std::shared_ptr<Device> applyDevicePolicy(uint32_t id, Rule::Target target) override;
    void insertDevice(std::shared_ptr<DeviceBase> device);
    std::shared_ptr<Device> removeDevice(const std::string& syspath);

    uint32_t getIDFromSysfsPath(const std::string& syspath) const;

  protected:
    static std::string ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry);

    int ueventOpen();
    void setDeviceAuthorizedDefault(SysFSDevice* device, DeviceManager::AuthorizedDefaultType auth_default);

    virtual void sysfsAuthorizeDevice(SysFSDevice& sysfs_device);
    virtual void sysfsDeauthorizeDevice(SysFSDevice& sysfs_device);
    virtual void sysfsRemoveDevice(SysFSDevice& sysfs_device);
    void sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target);

    void processDevicePresence(uint32_t id);
    void processDeviceInsertion(SysFSDevice& sysfs_device, bool signal_present);
    void processDeviceRemoval(const std::string& sysfs_devpath);

    bool isPresentSysfsPath(const std::string& sysfs_path) const;
    bool knownSysfsPath(const std::string& sysfs_path, uint32_t* id = nullptr) const;
    void learnSysfsPath(const std::string& sysfs_path, uint32_t id = 0);
    void forgetSysfsPath(const std::string& sysfs_path);

    int _uevent_fd;
    int _wakeup_fd;
    std::map<std::string, uint32_t> _sysfs_path_to_id_map;
    bool _enumeration_only_mode;
    std::atomic<bool> _enumeration;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
