//
// Copyright (C) 2018 Red Hat, Inc.
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

#if defined(HAVE_LINUX)

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

#include <umockdev.h>

namespace usbguard
{
  class LinuxDeviceManager;

  class LinuxDevice : public Device, public USBDescriptorParserHooks
  {
  public:
    LinuxDevice(LinuxDeviceManager& device_manager, SysFSDevice& sysfs_device);

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

  /*
   * TODO: Create a base class template that provides
   *       a shared implementation for Linux based device manager.
   */
  class LinuxDeviceManager : public DeviceManager
  {
    using DeviceManager::insertDevice;

  public:
    LinuxDeviceManager(DeviceManagerHooks& hooks);
    ~LinuxDeviceManager();

    void setDefaultBlockedState(bool state) override;
    void setEnumerationOnlyMode(bool state) override;

    void setProcessLibudevUEvents(bool state);
    void setProcessKernelUEvents(bool state);
    void setRestrictUEventOrigin(bool state);

    void start() override;
    void stop() override;
    void scan() override;

    std::shared_ptr<Device> applyDevicePolicy(uint32_t id, Rule::Target target) override;
    void insertDevice(std::shared_ptr<LinuxDevice> device);
    std::shared_ptr<Device> removeDevice(const std::string& syspath);

    uint32_t getIDFromSysfsPath(const std::string& sysfs_path) const;

    int ueventOpen();
    void sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target);

    void thread();
    void ueventProcessRead();
    void ueventProcessUEvent(const UEvent& uevent);
    static bool ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
      const std::pair<std::string, std::string>& b);
    int ueventEnumerateDevices();

    static std::string ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry);
    int ueventEnumerateTriggerAndWaitForDevice(const std::string& devpath, const std::string& buspath);

    void processDevicePresence(SysFSDevice& sysfs_device);
    void processDeviceInsertion(SysFSDevice& sysfs_device, bool signal_present);
    void processDevicePresence(uint32_t id);
    void processDeviceRemoval(const std::string& sysfs_devpath);

    struct GObjectDeleter {
      void operator()(void* gobject)
      {
        if (gobject != nullptr) {
          g_object_unref(gobject);
        }
      }
    };

    Thread<LinuxDeviceManager> _thread;

    int _uevent_fd{-1};
    int _wakeup_fd{-1};

    /*
     * The following maps sysfs devices paths to their IDs.
     * The key must not contain the sysfs (/sys) directory
     * root prefix in the path and must be normalized to not
     * contain ./ and ../ path components.
     */
    std::map<std::string, uint32_t> _sysfs_path_to_id_map;

    bool isPresentSysfsPath(const std::string& sysfs_path) const;
    bool knownSysfsPath(const std::string& sysfs_path, uint32_t* id = nullptr) const;
    void learnSysfsPath(const std::string& sysfs_path, uint32_t id = 0);
    void forgetSysfsPath(const std::string& sysfs_path);

    bool _default_blocked_state{true};
    bool _process_libudev_uevents{false};
    bool _process_kernel_uevents{true};
    bool _restrict_uevent_origin{true};
    bool _enumeration_only_mode{false};

    std::atomic<bool> _enumeration{false};
    std::condition_variable _enumeration_complete;
    std::mutex _enumeration_mutex;
  };
} /* namespace usbguard */
#endif /* HAVE_LINUX */

/* vim: set ts=2 sw=2 et */
