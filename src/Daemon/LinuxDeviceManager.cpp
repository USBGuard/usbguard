#include "LinuxDeviceManager.hpp"
#include "Daemon.hpp"
#include "SysIO.hpp"
#include "Common/Logging.hpp"
#include <sys/eventfd.h>
#include <sys/select.h>
#include <stdexcept>

namespace usbguard {

  LinuxDevice::LinuxDevice(struct udev_device* dev)
  {
    log->debug("Creating a new LinuxDevice instance");
    const char *name = udev_device_get_sysattr_value(dev, "product");
    if (name) {
      log->debug("DeviceName={}", name);
      setDeviceName(name);
    }
    
    const char *id_vendor = udev_device_get_sysattr_value(dev, "idVendor");
    if (id_vendor) {
      log->debug("VendorID={}", id_vendor);
      setVendorID(id_vendor);
    }

    const char *id_product = udev_device_get_sysattr_value(dev, "idProduct");
    if (id_product) {
      log->debug("ProductID={}", id_product);
      setProductID(id_product);
    }

    const char *serial = udev_device_get_sysattr_value(dev, "serial");
    if (serial) {
      log->debug("Serial={}", serial);
      setSerialNumber(serial);
    }

    const char *syspath = udev_device_get_syspath(dev);
    if (syspath) {
      log->debug("Syspath={}", syspath);
      _syspath = syspath;
    } else {
      throw std::runtime_error("device wihtout syspath");
    }

    log->debug("DeviceHash={}", getDeviceHash());

    setTarget(Rule::Target::Unknown);

    return;
  }

  const String& LinuxDevice::getSysPath() const
  {
    return _syspath;
  }

  /*
   * Manager
   */
  LinuxDeviceManager::LinuxDeviceManager(Daemon& daemon)
    : DeviceManager(daemon),
      _thread(this, &LinuxDeviceManager::thread)
  {
    setDefaultBlockedState(/*state=*/true);

    if ((_event_fd = eventfd(0, 0)) < 0) {
      throw std::runtime_error("eventfd init error");
    }
    
    if ((_udev = udev_new()) == nullptr) {
      throw std::runtime_error("udev init error");
    }

    if ((_umon = udev_monitor_new_from_netlink(_udev, "udev")) == nullptr) {
      udev_unref(_udev);
      throw std::runtime_error("udev_monitor init error");
    }

    udev_monitor_filter_add_match_subsystem_devtype(_umon, "usb", "usb_device");
    return;
  }

  LinuxDeviceManager::~LinuxDeviceManager()
  {
    setDefaultBlockedState(/*state=*/false); // FIXME: Set to previous state
    return;
  }

  void LinuxDeviceManager::setDefaultBlockedState(bool state)
  {
    sysioSetAuthorizedDefault(!state);
    return;
  }

  void LinuxDeviceManager::start()
  {
    // enumerate devices
    // broadcast present devices
    // start monitor thread
    _thread.start();
    return;
  }

  void LinuxDeviceManager::stop()
  {
    // stop monitor
    _thread.stop(/*do_wait=*/false);
    { /* Wakeup the device manager thread */
      const uint64_t one = 1;
      write(_event_fd, &one, sizeof one);
    }
    _thread.wait();
    return;
  }

  Pointer<Device> LinuxDeviceManager::allowDevice(uint32_t seqn)
  {
    log->debug("Allowing device {}", seqn);
    Pointer<Device> device = applyDevicePolicy(seqn, Rule::Target::Allow);
    log->debug("Calling DeviceAllowed hook");
    DeviceAllowed(device);
    return device;
  }

  Pointer<Device> LinuxDeviceManager::blockDevice(uint32_t seqn)
  {
    log->debug("Blocking device {}", seqn);
    Pointer<Device> device = applyDevicePolicy(seqn, Rule::Target::Block);
    log->debug("Calling DeviceBlocked hook");
    DeviceBlocked(device);
    return device;
  }

  Pointer<Device> LinuxDeviceManager::rejectDevice(uint32_t seqn)
  {
    log->debug("Rejecting device {}", seqn);
    Pointer<Device> device = applyDevicePolicy(seqn, Rule::Target::Reject);
    log->debug("Calling DeviceRejected hook");
    DeviceRejected(device);
    return device;
  }

  Pointer<Device> LinuxDeviceManager::applyDevicePolicy(uint32_t seqn, Rule::Target target)
  {
    log->debug("Applying device policy {} to device {}", target, seqn);
    Pointer<LinuxDevice> device = std::static_pointer_cast<LinuxDevice>(getDevice(seqn));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());

    const char *target_file = nullptr;
    int target_value = 0;

    switch (target)
      {
      case Rule::Target::Allow:
	target_file = "authorized";
	target_value = 1;
	break;
      case Rule::Target::Block:
	target_file = "authorized";
	target_value = 0;
	break;
      case Rule::Target::Reject:
	target_file = "remove";
	target_value = 1;
	break;
      default:
	log->critical("BUG: unknown rule target");
	throw std::runtime_error("Uknown rule target in applyDevicePolicy");
      }

    char sysio_path[SYSIO_PATH_MAX];
    snprintf(sysio_path, SYSIO_PATH_MAX, "%s/%s",
	     device->getSysPath().c_str(), target_file);
    /* FIXME: check that snprintf wrote the whole path */
    log->debug("SysIO: writing '{}' to {}", target_value, sysio_path);
    sysioWrite(sysio_path, target_value);

    return std::move(device);
  }

  void LinuxDeviceManager::thread()
  {
    log->debug("Entering LinuxDeviceManager thread");

    const int umon_fd = udev_monitor_get_fd(_umon);
    const int max_fd = umon_fd > _event_fd ? umon_fd : _event_fd;
    fd_set readset;

    udev_monitor_enable_receiving(_umon);

    while (!_thread.stopRequested()) {
      struct timeval tv_timeout = { 5, 0 };
      
      FD_ZERO(&readset);
      FD_SET(umon_fd, &readset);
      FD_SET(_event_fd, &readset);
      
      switch (select(max_fd + 1, &readset, NULL, NULL, &tv_timeout)) {
      case 1: /* Device or event */
      case 2: /* Device and event */
	if (FD_ISSET(_event_fd, &readset)) {
	  log->debug("Wakeup event received");
	  continue;
	}
	if (FD_ISSET(umon_fd, &readset)) {
	  log->debug("Handling UDev read event");
	  udevReceiveDevice();
	}
	break;
      case 0: /* Timeout */
	continue;
      case -1: /* Error */
      default:
	log->debug("Select failure: {}", errno);
	_thread.stop();
      }
    } /* Thread main loop */
    log->debug("Returning from LinuxDeviceManager thread");
    return;
  }

  void LinuxDeviceManager::udevReceiveDevice()
  {
    struct udev_device *dev = udev_monitor_receive_device(_umon);

    if (!dev) {
      return;
    }

    const char *action_cstr = udev_device_get_action(dev);

    if (!action_cstr) {
      log->warn("BUG? Device event witout action value.");
      udev_device_unref(dev);
      return;
    }

    if (strcmp(action_cstr, "add") == 0) {
      processDeviceInsertion(dev);
    }
    else if (strcmp(action_cstr, "remove") == 0) {
      processDeviceRemoval(dev);
    }
    else {
      log->warn("BUG? Unknown device action value \"{}\"", action_cstr);
    }

    udev_device_unref(dev);
    return;
  }

  void LinuxDeviceManager::processDeviceInsertion(struct udev_device *dev)
  {
    log->debug("Processing device insertion");
    Pointer<LinuxDevice> device = makePointer<LinuxDevice>(dev);
    insertDevice(device);
    DeviceInserted(device);
    return;
  }

  void LinuxDeviceManager::insertDevice(Pointer<Device> device)
  {
    DeviceManager::insertDevice(device);
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    _syspath_map[std::static_pointer_cast<LinuxDevice>(device)->getSysPath()] = device->getSeqn();
    return;
  }

  void LinuxDeviceManager::processDeviceRemoval(struct udev_device *dev)
  {
    log->debug("Processing device removal");
    const char *syspath_cstr = udev_device_get_syspath(dev);
    if (!syspath_cstr) {
      return;
    }
    const String syspath(syspath_cstr);
    Pointer<Device> device = removeDevice(syspath);
    DeviceRemoved(device);
    return;
  }

  Pointer<Device> LinuxDeviceManager::removeDevice(const String& syspath)
  {
    /* FIXME: device map locking */
    auto it = _syspath_map.find(syspath);
    if (it == _syspath_map.end()) {
      throw std::runtime_error("Unknown device, cannot remove from syspath map");
    }
    const uint32_t seqn = it->second;
    Pointer<Device> device = DeviceManager::removeDevice(seqn);
    _syspath_map.erase(it);
    return device;
  }

} /* namespace usbguard */
