#pragma once
#include "Typedefs.hpp"
#include "RuleSet.hpp"
#include "Device.hpp"
#include <mutex>

namespace usbguard {
  class Daemon;
  class DeviceManager
  {
  public:
    DeviceManager(Daemon& daemon);
    virtual ~DeviceManager();

    virtual void setDefaultBlockedState(bool state) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual Pointer<Device> allowDevice(uint32_t seqn) = 0;
    virtual Pointer<Device> blockDevice(uint32_t seqn) = 0;
    virtual Pointer<Device> rejectDevice(uint32_t seqn) = 0;

    virtual void insertDevice(Pointer<Device> device);
    virtual Pointer<Device> removeDevice(uint32_t seqn);

    /* Returns a copy of the list of active USB devices */
    PointerVector<Device> getDeviceList();
    Pointer<Device> getDevice(uint32_t seqn);
    std::mutex& refDeviceMapMutex();

    /* Call Daemon instance hooks */
    void DeviceInserted(Pointer<Device> device);
    void DevicePresent(Pointer<Device> device);
    void DeviceRemoved(Pointer<Device> device);
    void DeviceAllowed(Pointer<Device> device);
    void DeviceBlocked(Pointer<Device> device);
    void DeviceRejected(Pointer<Device> device);

  private:
    Daemon& _daemon;
    std::mutex _device_map_mutex;
    PointerMap<uint32_t, Device> _device_map;
  };

} /* namespace usbguard */
