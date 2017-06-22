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
#include <build-config.h>

#if defined(HAVE_UEVENT)

#include "Typedefs.hpp"
#include "Common/Thread.hpp"

#include "DeviceManager.hpp"
#include "Device.hpp"
#include "Rule.hpp"
#include "SysFSDevice.hpp"
#include "USB.hpp"

#include <condition_variable>

#include <istream>
#include <sys/stat.h>
#include <dirent.h>

namespace usbguard {
  class UEventDeviceManager;

  class UEventDevice : public Device, public USBDescriptorParserHooks
  {
  public:
    UEventDevice(UEventDeviceManager& device_manager, SysFSDevice& sysfs_device);

    SysFSDevice& sysfsDevice();
    const std::string& getSysPath() const;
    bool isController() const override;

  private:
    void parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out) override;
    void loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor) override;
    bool isLinuxRootHubDeviceDescriptor(const USBDescriptor* descriptor);
    void updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* descriptor);

    SysFSDevice _sysfs_device;
  };

#if !defined(USBGUARD_SYSFS_ROOT)
#define USBGUARD_SYSFS_ROOT "/sys"
#endif

  class UEventDeviceManager : public DeviceManager
  {
    using DeviceManager::insertDevice;

  public:
    UEventDeviceManager(DeviceManagerHooks& hooks, const std::string& sysfs_root = USBGUARD_SYSFS_ROOT, bool dummy_mode = false);
    ~UEventDeviceManager();

    void setDefaultBlockedState(bool state) override;
    void setEnumerationOnlyMode(bool state) override;

    void start() override;
    void stop() override;
    void scan() override;

    Pointer<Device> applyDevicePolicy(uint32_t id, Rule::Target target) override;
    void insertDevice(Pointer<UEventDevice> device);
    Pointer<Device> removeDevice(const std::string& syspath);

    uint32_t getIDFromSysPath(const std::string& syspath) const;

  protected:
    int ueventOpen();
    int ueventDummyOpen();
    void sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target);

    bool knownSysPath(const std::string& syspath, uint32_t * id = nullptr) const;
    void learnSysPath(const std::string& syspath, uint32_t id = 0);
    void forgetSysPath(const std::string& syspath);

    void thread();
    void ueventProcessRead();
    void ueventProcessUEvent(const UEvent& uevent);
    static bool ueventEnumerateComparePath(const std::pair<std::string,std::string>& a, const std::pair<std::string,std::string>& b);
    int ueventEnumerateDevices();
    int ueventEnumerateDummyDevices();

    static std::string ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry);
    int ueventEnumerateTriggerDevice(const std::string& devpath, const std::string& buspath);

    void processDevicePresence(SysFSDevice& sysfs_device);

    void processDeviceInsertion(SysFSDevice& sysfs_device, bool signal_present);
    void processDevicePresence(uint32_t id);
    void processDeviceRemoval(const std::string& sysfs_devpath);

  private:
    Thread<UEventDeviceManager> _thread;
    int _uevent_fd;
    int _wakeup_fd;
    std::map<std::string,uint32_t> _syspath_map;
    std::string _sysfs_root;
    bool _default_blocked_state;
    bool _enumeration_only_mode;
    bool _dummy_mode;
    std::atomic<bool> _enumeration;
    std::atomic<int> _enumeration_count;
    std::condition_variable _enumeration_complete;
    std::mutex _enumeration_mutex;
  };
} /* namespace usbguard */
#endif /* HAVE_UEVENT */
