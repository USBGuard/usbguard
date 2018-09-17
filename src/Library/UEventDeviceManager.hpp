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

#if defined(HAVE_UEVENT)

#include "Common/Thread.hpp"
#include "SysFSDevice.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/DeviceManager.hpp"
#include "usbguard/Device.hpp"
#include "usbguard/Rule.hpp"
#include "usbguard/USB.hpp"

#include <condition_variable>
#include <istream>

#include <sys/stat.h>
#include <dirent.h>

namespace usbguard
{
  class UEventDeviceManager;

  class UEventDevice : public Device, public USBDescriptorParserHooks
  {
  public:
    UEventDevice(UEventDeviceManager& device_manager, SysFSDevice& sysfs_device);

    SysFSDevice& sysfsDevice();
    const std::string& getSysPath() const;
    bool isController() const override;
    std::string getSystemName() const override;

  private:
    void parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
      USBDescriptor* descriptor_out) override;
    void loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor) override;
    bool isLinuxRootHubDeviceDescriptor(const USBDescriptor* descriptor);
    void updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* descriptor);

    SysFSDevice _sysfs_device;
  };

  class UEventDeviceManager : public DeviceManager
  {
    using DeviceManager::insertDevice;

  public:
    UEventDeviceManager(DeviceManagerHooks& hooks);
    ~UEventDeviceManager();

    void setDefaultBlockedState(bool state) override;
    void setEnumerationOnlyMode(bool state) override;

    void start() override;
    void stop() override;
    void scan() override;
    void scan(const std::string& devpath) override;

    std::shared_ptr<Device> applyDevicePolicy(uint32_t id, Rule::Target target) override;
    void insertDevice(std::shared_ptr<UEventDevice> device);
    std::shared_ptr<Device> removeDevice(const std::string& syspath);

    uint32_t getIDFromSysfsPath(const std::string& syspath) const;

  private:
    static bool ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
      const std::pair<std::string, std::string>& b);
    static std::string ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry);

    void sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target);
    void thread();

    int ueventOpen();
    void ueventProcessRead();
    void ueventProcessUEvent(const UEvent& uevent);
    void ueventProcessAction(const std::string& action, const std::string& sysfs_devpath);
    int ueventEnumerateDevices();
    int ueventEnumerateTriggerDevice(const std::string& devpath, const std::string& buspath);

    void processDevicePresence(SysFSDevice& sysfs_device);
    void processDeviceInsertion(SysFSDevice& sysfs_device, bool signal_present);
    void processDevicePresence(uint32_t id);
    void processDeviceRemoval(const std::string& sysfs_devpath);

    Thread<UEventDeviceManager> _thread;
    int _uevent_fd;
    int _wakeup_fd;

    bool isPresentSysfsPath(const std::string& sysfs_path) const;
    bool knownSysfsPath(const std::string& sysfs_path, uint32_t* id = nullptr) const;
    void learnSysfsPath(const std::string& sysfs_path, uint32_t id = 0);
    void forgetSysfsPath(const std::string& sysfs_path);

    std::map<std::string, uint32_t> _sysfs_path_to_id_map;

    bool _default_blocked_state;
    bool _enumeration_only_mode;
    std::atomic<bool> _enumeration;
    std::condition_variable _enumeration_complete;
    std::mutex _enumeration_mutex;
  };
} /* namespace usbguard */
#endif /* HAVE_UEVENT */

/* vim: set ts=2 sw=2 et */
