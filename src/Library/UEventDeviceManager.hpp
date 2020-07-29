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
#include "DeviceManagerBase.hpp"

namespace usbguard
{
  class UEvent;

  class UEventDeviceManager : public DeviceManagerBase
  {
  public:
    UEventDeviceManager(DeviceManagerHooks& hooks);
    ~UEventDeviceManager();

    void start() override;
    void stop() override;
    void scan() override;
    void scan(const std::string& devpath) override;

  private:
    static bool ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
      const std::pair<std::string, std::string>& b);

    void thread();
    void ueventProcessRead();
    void ueventProcessUEvent(UEvent uevent);
    void ueventProcessAction(const std::string& action, const std::string& sysfs_devpath);
    int ueventEnumerateDevices();
    int ueventEnumerateTriggerDevice(const std::string& devpath, const std::string& buspath);

    void processBacklog();

    Thread<UEventDeviceManager> _thread;
    std::vector<UEvent> _backlog;
  };
} /* namespace usbguard */
#endif /* HAVE_UEVENT */

/* vim: set ts=2 sw=2 et */
