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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Common/Utility.hpp"
#include "DeviceBase.hpp"
#include "DeviceManagerBase.hpp"
#include "SysFSDevice.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"

#include <stdexcept>
#include <fstream>
#include <chrono>

#include <limits.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

namespace usbguard
{

  DeviceManagerBase::DeviceManagerBase(DeviceManagerHooks& hooks)
    : DeviceManager(hooks),
      _uevent_fd(-1),
      _wakeup_fd(-1),
      _enumeration(false)
  {
    setEnumerationOnlyMode(/*state=*/false);
  }

  DeviceManagerBase::~DeviceManagerBase() = default;

  void DeviceManagerBase::setEnumerationOnlyMode(bool state)
  {
    _enumeration_only_mode = state;
  }

  std::shared_ptr<Device> DeviceManagerBase::applyDevicePolicy(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "id=" << id << " target=" << Rule::targetToString(target);
    std::shared_ptr<DeviceBase> device = std::static_pointer_cast<DeviceBase>(getDevice(id));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    sysfsApplyTarget(device->sysfsDevice(), target);
    device->setTarget(target);
    return device;
  }

  void DeviceManagerBase::insertDevice(std::shared_ptr<DeviceBase> device)
  {
    DeviceManager::insertDevice(std::static_pointer_cast<Device>(device));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    learnSysfsPath(device->getSysPath(), device->getID());
  }

  std::shared_ptr<Device> DeviceManagerBase::removeDevice(const std::string& syspath)
  {
    /*
     * FIXME: device map locking
     */
    if (!knownSysfsPath(syspath)) {
      throw Exception("removeDevice", syspath, "unknown syspath, cannot remove device");
    }

    std::shared_ptr<Device> device = DeviceManager::removeDevice(getIDFromSysfsPath(syspath));
    forgetSysfsPath(syspath);
    return device;
  }

  uint32_t DeviceManagerBase::getIDFromSysfsPath(const std::string& sysfs_path) const
  {
    uint32_t id = 0;

    if (knownSysfsPath(sysfs_path, &id)) {
      return id;
    }

    throw Exception("DeviceManagerBase", sysfs_path, "unknown sysfs path");
  }

  std::string DeviceManagerBase::ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry)
  {
#if defined(_DIRENT_HAVE_D_TYPE)

    if (direntry->d_type != DT_UNKNOWN) {
      switch (direntry->d_type) {
      case DT_LNK:
        return symlinkPath(filepath);

      case DT_DIR:
        return filepath;

      default:
        return std::string();
      }
    }
    else {
      /*
       * Unknown type. We have to call lstat.
       */
#endif
      struct stat st = {};

      if (lstat(filepath.c_str(), &st) != 0) {
        /*
         * Cannot stat, skip this entry.
         */
        USBGUARD_LOG(Warning) << "lstat(" << filepath << "): errno=" << errno;
        return std::string();
      }

      if (S_ISLNK(st.st_mode)) {
        return symlinkPath(filepath, &st);
      }
      else if (S_ISDIR(st.st_mode)) {
        return filepath;
      }
      else {
        return std::string();
      }

#if defined(_DIRENT_HAVE_D_TYPE)
    }

#endif
  }

  int DeviceManagerBase::ueventOpen()
  {
    int socket_fd = -1;
    USBGUARD_SYSCALL_THROW("UEvent device manager",
      (socket_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)) < 0);

    try {
      const int optval = 1;
      USBGUARD_SYSCALL_THROW("UEvent device manager",
        setsockopt(socket_fd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof optval) != 0);
      /*
       * Set a 1MiB receive buffer on the netlink socket to avoid ENOBUFS error
       * in recvmsg.
       */
      const size_t rcvbuf_max = 1024 * 1024;
      USBGUARD_SYSCALL_THROW("UEvent device manager",
        setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_max, sizeof rcvbuf_max) != 0);
      struct sockaddr_nl sa = {};
      sa.nl_family = AF_NETLINK;
      sa.nl_pid = getpid();
      sa.nl_groups = -1;
      USBGUARD_SYSCALL_THROW("UEvent device manager",
        bind(socket_fd, reinterpret_cast<const sockaddr*>(&sa), sizeof sa) != 0);
    }
    catch (...) {
      (void)close(socket_fd);
      throw;
    }

    return socket_fd;
  }

  void DeviceManagerBase::setDeviceAuthorizedDefault(SysFSDevice* device, DeviceManager::AuthorizedDefaultType auth_default)
  {
    if (auth_default == DeviceManager::AuthorizedDefaultType::Keep) {
      return;
    }

    std::string auth_default_str = std::to_string(DeviceManager::authorizedDefaultTypeToInteger(auth_default));
    device->setAttribute("authorized_default", auth_default_str);

    if (device->readAttribute("authorized_default", /*strip_last_null=*/true) != auth_default_str) {
      if (auth_default == DeviceManager::AuthorizedDefaultType::Internal) {
        USBGUARD_LOG(Warning) << "No kernel support for authorized_default = 2, falling back to 0";
        setDeviceAuthorizedDefault(device, DeviceManager::AuthorizedDefaultType::None);
      }
      else {
        throw Exception("DeviceBase", device->getPath(), "Failed to set authorized_default to \"" + auth_default_str + "\"");
      }
    }
  }

  void DeviceManagerBase::sysfsAuthorizeDevice(SysFSDevice& sysfs_device)
  {
    sysfs_device.setAttribute("authorized", "1");
  }

  void DeviceManagerBase::sysfsDeauthorizeDevice(SysFSDevice& sysfs_device)
  {
    sysfs_device.setAttribute("authorized", "0");
  }

  void DeviceManagerBase::sysfsRemoveDevice(SysFSDevice& sysfs_device)
  {
    sysfs_device.setAttribute("remove", "1");
  }

  void DeviceManagerBase::sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target)
  {
    switch (target) {
    case Rule::Target::Allow:
      sysfsAuthorizeDevice(sysfs_device);
      break;

    case Rule::Target::Block:
      sysfsDeauthorizeDevice(sysfs_device);
      break;

    case Rule::Target::Reject:
      sysfsRemoveDevice(sysfs_device);
      break;

    case Rule::Target::Match:
    case Rule::Target::Device:
    case Rule::Target::Unknown:
    case Rule::Target::Empty:
    case Rule::Target::Invalid:
    default:
      throw std::runtime_error("Unknown rule target in applyDevicePolicy");
    }
  }

  void DeviceManagerBase::processDevicePresence(const uint32_t id)
  {
    USBGUARD_LOG(Trace) << "id=" << id;

    try {
      std::shared_ptr<DeviceBase> device = \
        std::static_pointer_cast<DeviceBase>(DeviceManager::getDevice(id));
      device->sysfsDevice().reload();
      /*
       * TODO: Check attribute state
       *  - authorized_default (in case of controller)
       */
      DeviceEvent(DeviceManager::EventType::Present, device);
      return;
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Error) << "Present device exception: " << ex.message();
      DeviceException(ex.message());
    }
    catch (const std::exception& ex) {
      USBGUARD_LOG(Error) << "Present device exception: " << ex.what();
      DeviceException(ex.what());
    }
    catch (...) {
      USBGUARD_LOG(Error) << "BUG: Unknown device exception.";
      DeviceException("BUG: Unknown device exception.");
    }

    /*
     * We don't reject the device here (as is done in processDeviceInsertion)
     * because the device was already connected to the system when USBGuard
     * started. Therefore, if the device is malicious, it already had a chance
     * to interact with the system.
     */
  }

  void DeviceManagerBase::processDeviceInsertion(SysFSDevice& sysfs_device, const bool signal_present)
  {
    USBGUARD_LOG(Trace) << "sysfs_device=" << sysfs_device.getPath();

    try {
      std::shared_ptr<DeviceBase> device = std::make_shared<DeviceBase>(*this, sysfs_device);
      DeviceManager::AuthorizedDefaultType auth_default = getAuthorizedDefault();

      if (device->isController() && !_enumeration_only_mode) {
        USBGUARD_LOG(Debug) << "Setting default blocked state for controller device to " <<
          DeviceManager::authorizedDefaultTypeToString(auth_default);
        setDeviceAuthorizedDefault(&device->sysfsDevice(), auth_default);
      }

      insertDevice(device);

      /*
       * Signal insertions as presence if device enumeration hasn't
       * completed yet.
       */
      if (signal_present) {
        DeviceEvent(DeviceManager::EventType::Present, device);
      }
      else {
        DeviceEvent(DeviceManager::EventType::Insert, device);
      }

      return;
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Error) << "Device insert exception: " << ex.message();
      DeviceException(ex.message());
    }
    catch (const std::exception& ex) {
      USBGUARD_LOG(Error) << "Device insert exception: " << ex.what();
      DeviceException(ex.what());
    }
    catch (...) {
      USBGUARD_LOG(Error) << "BUG: Unknown device insert exception.";
      DeviceException("BUG: Unknown device insert exception.");
    }

    /*
     * Skip device reject when in enumeration only mode.
     */
    if (_enumeration_only_mode) {
      return;
    }

    /*
     * Something went wrong and an exception was generated.
     * Either the device is malicious or the system lacks some
     * resources to successfully process the device. In either
     * case, we take the safe route and fallback to rejecting
     * the device.
     */
    USBGUARD_LOG(Warning) << "Rejecting device at syspath=" << sysfs_device.getPath();
    sysfsApplyTarget(sysfs_device, Rule::Target::Reject);
  }

  void DeviceManagerBase::processDeviceRemoval(const std::string& sysfs_devpath)
  {
    USBGUARD_LOG(Trace) << "sysfs_devpath=" << sysfs_devpath;

    try {
      std::shared_ptr<Device> device = removeDevice(sysfs_devpath);
      DeviceEvent(DeviceManager::EventType::Remove, device);
    }
    catch (...) {
      /* Ignore for now */
      USBGUARD_LOG(Debug) << "Removal of an unknown device ignored.";
      return;
    }
  }

  bool DeviceManagerBase::isPresentSysfsPath(const std::string& sysfs_path) const
  {
    uint32_t id = 0;

    if (knownSysfsPath(sysfs_path, &id)) {
      return 0 == id;
    }

    return false;
  }

  bool DeviceManagerBase::knownSysfsPath(const std::string& sysfs_path, uint32_t* id_ptr) const
  {
    USBGUARD_LOG(Trace) << "Known? sysfs_path=" << sysfs_path << " size=" << sysfs_path.size() << " id_ptr=" << (void*)id_ptr;
    auto it = _sysfs_path_to_id_map.find(sysfs_path);
    uint32_t known_id = 0;
    bool known = false;

    if (it != _sysfs_path_to_id_map.end()) {
      known = true;
      known_id = it->second;
    }

    if (id_ptr != nullptr) {
      *id_ptr = known_id;
    }

    USBGUARD_LOG(Trace) << "Known? sysfs_path=" << sysfs_path << " id_ptr=" << (void*)id_ptr << " known=" << known << " known_id="
      << known_id;
    return known;
  }

  void DeviceManagerBase::learnSysfsPath(const std::string& sysfs_path, uint32_t id)
  {
    USBGUARD_LOG(Trace) << "Learn sysfs_path=" << sysfs_path << " size=" << sysfs_path.size() << " id=" << id;
    _sysfs_path_to_id_map[sysfs_path] = id;
  }

  void DeviceManagerBase::forgetSysfsPath(const std::string& sysfs_path)
  {
    USBGUARD_LOG(Trace) << "Forget sysfs_path=" << sysfs_path;
    _sysfs_path_to_id_map.erase(sysfs_path);
  }

} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
