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

#if defined(HAVE_UMOCKDEV)

#include "Common/Thread.hpp"
#include "SysFSDevice.hpp"
#include "UMockdevDeviceDefinition.hpp"

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
  class UMockdevDeviceManager;

  class UMockdevDevice : public Device, public USBDescriptorParserHooks
  {
  public:
    UMockdevDevice(UMockdevDeviceManager& device_manager, SysFSDevice& sysfs_device);

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
  class UMockdevDeviceManager : public DeviceManager
  {
    using DeviceManager::insertDevice;

  public:
    UMockdevDeviceManager(DeviceManagerHooks& hooks);
    ~UMockdevDeviceManager();

    void setDefaultBlockedState(bool state) override;
    void setEnumerationOnlyMode(bool state) override;

    void start() override;
    void stop() override;
    void scan() override;
    void scan(const std::string& devpath) override;

    std::shared_ptr<Device> applyDevicePolicy(uint32_t id, Rule::Target target) override;
    void insertDevice(std::shared_ptr<UMockdevDevice> device);
    std::shared_ptr<Device> removeDevice(const std::string& syspath);

    uint32_t getIDFromSysfsPath(const std::string& sysfs_path) const;

  protected:
    void umockdevInit();
    std::vector<std::string> umockdevLoadFromFile(const std::string& definitions_path);
    std::vector<std::string> umockdevRemoveByFile(const std::string& definitions_path);
    void umockdevRemoveBySysfsPath(const std::string& sysfs_path);
    void umockdevProcessInotify();
    std::vector<std::string> umockdevGetChildrenBySysfsPath(const std::string& sysfs_path);
    void umockdevAuthorizeBySysfsPath(const std::string& sysfs_path);
    void umockdevDeauthorizeBySysfsPath(const std::string& sysfs_path);
    void umockdevAdd(const std::shared_ptr<UMockdevDeviceDefinition>& definition);
    void umockdevRemove(const std::shared_ptr<UMockdevDeviceDefinition>& definition);
    void umockdevRemove(const std::string& sysfs_path);

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

  private:
    struct GObjectDeleter {
      void operator()(void* gobject)
      {
        if (gobject != nullptr) {
          g_object_unref(gobject);
        }
      }
    };

    Thread<UMockdevDeviceManager> _thread;
    std::unique_ptr<UMockdevTestbed, GObjectDeleter> _testbed{nullptr};
    std::string _umockdev_deviceroot;
    int _inotify_fd{-1};
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

    std::map<std::string, std::shared_ptr<UMockdevDeviceDefinition>> _sysfs_path_map;
    std::multimap<std::string, std::weak_ptr<UMockdevDeviceDefinition>> _umockdev_file_map;

    bool _default_blocked_state{true};

    bool _enumeration_only_mode{false};
    std::atomic<bool> _enumeration{false};
    std::condition_variable _enumeration_complete;
    std::mutex _enumeration_mutex;
  };
} /* namespace usbguard */
#endif /* HAVE_UMOCKDEV */

/* vim: set ts=2 sw=2 et */
