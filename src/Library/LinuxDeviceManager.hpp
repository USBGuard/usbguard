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

#include "DeviceManager.hpp"
#include <Typedefs.hpp>
#include <Device.hpp>
#include <Rule.hpp>
#include "LinuxSysIO.hpp"
#include "Common/Thread.hpp"
#include <libudev.h>
#include <istream>

namespace usbguard {
  class LinuxDevice : public Device
  {
  public:
    LinuxDevice(struct udev_device* dev);
    const String& getSysPath() const;
    bool isController() const;

  protected:
    void readDescriptors(std::istream& stream);
    void readConfiguration(int c_num, std::istream& stream);
    void readInterfaceDescriptor(int c_num, int i_num, std::istream& stream);
    void readEndpointDescriptor(int c_num, int i_num, int e_num, std::istream& stream);

  private:
    String _syspath;
  };

  class LinuxDeviceManager : public DeviceManager
  {
  public:
    LinuxDeviceManager(DeviceManagerHooks& hooks);
    ~LinuxDeviceManager();

    void setDefaultBlockedState(bool state);
    void start();
    void stop();
    void scan();
    Pointer<Device> allowDevice(uint32_t id);
    Pointer<Device> blockDevice(uint32_t id);
    Pointer<Device> rejectDevice(uint32_t id);
    void insertDevice(Pointer<Device> device);
    Pointer<Device> removeDevice(const String& syspath);

  protected:
    Pointer<Device> applyDevicePolicy(uint32_t id, Rule::Target target);
    void sysioApplyTarget(const String& sys_path, Rule::Target target);
    void thread();
    void udevReceiveDevice();
    void udevEnumerateDevices();
    void processDevicePresence(struct udev_device *dev);
    void processDeviceInsertion(struct udev_device *dev);
    void processDeviceRemoval(struct udev_device *dev);

  private:
    struct udev *_udev;
    struct udev_monitor *_umon;
    int _event_fd;
    Thread<LinuxDeviceManager> _thread;
    StringKeyMap<uint32_t> _syspath_map;
  };

} /* namespace usbguard */
