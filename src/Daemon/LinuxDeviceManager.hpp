#pragma once

#include "Typedefs.hpp"
#include "Device.hpp"
#include "DeviceManager.hpp"
#include "Rule.hpp"
#include "SysIO.hpp"
#include "Common/Thread.hpp"
#include <libudev.h>

namespace usbguard {
  class LinuxDevice : public Device
  {
  public:
    LinuxDevice(struct udev_device* dev);
    const String& getSysPath() const;
  private:
    String _syspath;
  };

  class LinuxDeviceManager : public DeviceManager
  {
  public:
    LinuxDeviceManager(Daemon& daemon);
    ~LinuxDeviceManager();

    void setDefaultBlockedState(bool state);
    void start();
    void stop();
    Pointer<Device> allowDevice(uint32_t seqn);
    Pointer<Device> blockDevice(uint32_t seqn);
    Pointer<Device> rejectDevice(uint32_t seqn);
    void insertDevice(Pointer<Device> device);
    Pointer<Device> removeDevice(const String& syspath);

  protected:
    Pointer<Device> applyDevicePolicy(uint32_t seqn, Rule::Target target);
    void thread();
    void udevReceiveDevice();
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
