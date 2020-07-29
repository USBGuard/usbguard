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
#include "DeviceManagerBase.hpp"
#include "UMockdevDeviceDefinition.hpp"

#include <condition_variable>
#include <umockdev.h>

namespace usbguard
{
  class UEvent;

  class UMockdevDeviceManager : public DeviceManagerBase
  {
  public:
    UMockdevDeviceManager(DeviceManagerHooks& hooks);
    ~UMockdevDeviceManager();

    void start() override;
    void stop() override;
    void scan() override;
    void scan(const std::string& devpath) override;

  private:
    struct GObjectDeleter {
      void operator()(void* gobject)
      {
        if (gobject != nullptr) {
          g_object_unref(gobject);
        }
      }
    };

    static bool ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
      const std::pair<std::string, std::string>& b);

    void umockdevInit();
    void umockdevAdd(const std::shared_ptr<UMockdevDeviceDefinition>& definition);
    void umockdevRemove(const std::shared_ptr<UMockdevDeviceDefinition>& definition);
    void umockdevRemove(const std::string& sysfs_path);
    void umockdevProcessInotify();
    std::vector<std::string> umockdevLoadFromFile(const std::string& definitions_path);
    std::vector<std::string> umockdevRemoveByFile(const std::string& definitions_path);
    std::vector<std::string> umockdevGetChildrenBySysfsPath(const std::string& sysfs_path);

    void sysfsAuthorizeDevice(SysFSDevice& sysfs_device) override;
    void sysfsDeauthorizeDevice(SysFSDevice& sysfs_device) override;

    void thread();
    void ueventProcessRead();
    void ueventProcessUEvent(const UEvent& uevent);
    int ueventEnumerateDevices();
    int ueventEnumerateTriggerAndWaitForDevice(const std::string& devpath, const std::string& buspath);

    Thread<UMockdevDeviceManager> _thread;
    std::unique_ptr<UMockdevTestbed, GObjectDeleter> _testbed{nullptr};
    std::string _umockdev_deviceroot;
    int _inotify_fd{-1};

    std::map<std::string, std::shared_ptr<UMockdevDeviceDefinition>> _sysfs_path_map;
    std::multimap<std::string, std::weak_ptr<UMockdevDeviceDefinition>> _umockdev_file_map;

    std::condition_variable _enumeration_complete;
    std::mutex _enumeration_mutex;
  };
} /* namespace usbguard */
#endif /* HAVE_UMOCKDEV */

/* vim: set ts=2 sw=2 et */
