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

#if defined(HAVE_UEVENT)
#include "UEventDeviceManager.hpp"
#include "UEventParser.hpp"
#include "SysFSDevice.hpp"
#include "Base64.hpp"
#include "Common/FDInputStream.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"

#include <stdexcept>
#include <fstream>
#include <chrono>

#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <linux/netlink.h>
#include <limits.h>
#include <stdlib.h>

namespace usbguard
{

  UEventDeviceManager::UEventDeviceManager(DeviceManagerHooks& hooks)
    : DeviceManagerBase(hooks),
      _thread(this, &UEventDeviceManager::thread)
  {
  }

  UEventDeviceManager::~UEventDeviceManager()
  {
    if (getRestoreControllerDeviceState()) {
      setAuthorizedDefault(AuthorizedDefaultType::All); // FIXME: Set to previous state
    }

    stop();

    if (_uevent_fd >= 0) {
      (void)close(_uevent_fd);
    }

    if (_wakeup_fd >= 0) {
      (void)close(_wakeup_fd);
    }
  }

  void UEventDeviceManager::start()
  {
    // Lazy initialization is used for the sockets to allow scanning a devpath
    // without needed to open the socket or start the thread.
    USBGUARD_SYSCALL_THROW("UEvent device manager",
      (_wakeup_fd = eventfd(0, 0)) < 0);
    _uevent_fd = ueventOpen();
    _thread.start();
  }

  void UEventDeviceManager::stop()
  {
    if (_thread.running()) {
      // stop monitor
      _thread.stop(/*do_wait=*/false);
      { /* Wakeup the device manager thread */
        const uint64_t one = 1;
        USBGUARD_SYSCALL_THROW("Linux device manager",
          write(_wakeup_fd, &one, sizeof one) != sizeof one);
      }
      _thread.wait();
    }
  }

  void UEventDeviceManager::scan()
  {
    USBGUARD_LOG(Trace);
    Restorer<std::atomic<bool>, bool> \
    restorer(_enumeration, /*transient=*/true, /*restored=*/false);
    auto const enumeration_count = ueventEnumerateDevices();
    USBGUARD_LOG(Debug) << "enumeration_count=" << enumeration_count;

    if (enumeration_count == 0) {
      return;
    }

    if (enumeration_count < 0) {
      throw Exception("UEventDeviceManager", "present devices", "failed to enumerate");
    }

    // We keep track of the uevents received during the scanning and process
    // them ad-hoc at the end. This is to avoid any inconsistent state while
    // enumerating the devices.
    _enumeration = false;
    processBacklog();
  }

  void UEventDeviceManager::scan(const std::string& devpath)
  {
    std::vector<std::string> components;
    tokenizeString(devpath, components, "/", /*trim_empty=*/true);
    auto base = std::find_if(components.begin(), components.end(),
    [](const std::string& component) -> bool {
      return hasPrefix(component, "usb");
    });
    std::string path = "";

    for (auto itr = components.begin(); itr < components.end(); ++itr) {
      path += "/" + *itr;

      if (itr >= base) {
        ueventProcessAction("add", path);
      }
    }

    ueventProcessAction("add", path);
  }

  bool UEventDeviceManager::ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
    const std::pair<std::string, std::string>& b)
  {
    const std::string base_a = filenameFromPath(a.second, /*include_extension=*/true);
    const std::string base_b = filenameFromPath(b.second, /*include_extension=*/true);
    const bool a_has_usb_prefix = (0 == base_a.compare(0, 3, "usb"));
    const bool b_has_usb_prefix = (0 == base_b.compare(0, 3, "usb"));

    if (a_has_usb_prefix) {
      if (!b_has_usb_prefix) {
        return true;
      }
    }
    else {
      if (b_has_usb_prefix) {
        return false;
      }
    }

    if (base_a.size() < base_b.size()) {
      return true;
    }
    else if (base_a.size() > base_b.size()) {
      return false;
    }

    return base_a < base_b;
  }

  void UEventDeviceManager::thread()
  {
    USBGUARD_LOG(Trace) << "Entering main loop.";

    try {
      const int max_fd = std::max(_uevent_fd, _wakeup_fd);
      fd_set readset;

      while (!_thread.stopRequested()) {
        struct timeval tv_timeout = { 5, 0 };
        FD_ZERO(&readset);
        FD_SET(_uevent_fd, &readset);
        FD_SET(_wakeup_fd, &readset);

        switch (select(max_fd + 1, &readset, NULL, NULL, &tv_timeout)) {
        case 1: /* uevent or wakeup */
        case 2: /* uevent and wakeup */
          if (FD_ISSET(_wakeup_fd, &readset)) {
            USBGUARD_LOG(Debug) << "Wakeup event.";
            continue;
          }

          if (FD_ISSET(_uevent_fd, &readset)) {
            USBGUARD_LOG(Debug) << "UEvent read event.";
            ueventProcessRead();
          }

          break;

        case 0: /* Timeout */
          continue;

        case -1: /* Error */
        default:
          USBGUARD_LOG(Error) << "UEventDeviceManager thread: select failed: errno=" << errno;
          _thread.stop();
        }
      } /* Thread main loop */
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Error) << "UEventDeviceManager thread: " << ex.message();
    }

    USBGUARD_LOG(Trace) << "Leaving main loop.";
  }

  void UEventDeviceManager::ueventProcessRead()
  {
    std::string buffer(4096, 0);
    struct iovec iov[1];
    iov[0].iov_base = (void*)&buffer[0];
    iov[0].iov_len = buffer.capacity();
    struct sockaddr_nl peer_sockaddr = { };
    union {
      struct cmsghdr header;
      uint8_t ucred[CMSG_SPACE(sizeof(struct ucred))];
    } cmsg_un;
    cmsg_un.header.cmsg_len = CMSG_LEN(sizeof(struct ucred));
    cmsg_un.header.cmsg_level = SOL_SOCKET;
    cmsg_un.header.cmsg_type = SCM_CREDENTIALS;
    struct msghdr msg_header = { };
    msg_header.msg_control = cmsg_un.ucred;
    msg_header.msg_controllen = sizeof cmsg_un.ucred;
    msg_header.msg_iov = iov;
    msg_header.msg_iovlen = 1;
    msg_header.msg_name = &peer_sockaddr;
    msg_header.msg_namelen = sizeof peer_sockaddr;
    const int rc = recvmsg(_uevent_fd, &msg_header, MSG_DONTWAIT);

    if (rc <= 0) {
      const int saved_errno = errno;

      if (saved_errno == EAGAIN || saved_errno == EWOULDBLOCK) {
        USBGUARD_LOG(Warning) << "ueventProcessRead: "
          << "reading from uevent source would block thread execution";
        return;
      }
      else if (saved_errno == ENOBUFS) {
        USBGUARD_LOG(Error) << "ueventProcessRead: "
          << "failed to read pending uevent (returning): "
          << "rc=" << rc << " errno=" << saved_errno;
        usleep(1000);
        return;
      }
      else {
        USBGUARD_LOG(Error) << "ueventProcessRead: "
          << "failed to read pending uevent: "
          << "rc=" << rc << " errno=" << saved_errno;
        throw ErrnoException("UEvent device manager", "recvmsg", saved_errno);
      }
    }

    buffer.resize((size_t)rc);
    const struct cmsghdr* const cmsg_header = CMSG_FIRSTHDR(&msg_header);

    if (cmsg_header == nullptr) {
      /* ignore */
      USBGUARD_LOG(Warning) << "ueventProcessRead: "
        << "received uevent without required control message: ignoring.";
      return;
    }

    if (cmsg_header->cmsg_len != CMSG_LEN(sizeof(struct ucred))
      || cmsg_header->cmsg_level != SOL_SOCKET
      || cmsg_header->cmsg_type != SCM_CREDENTIALS) {
      /* unexpected control message -- ignore */
      USBGUARD_LOG(Warning) << "ueventProcessRead: "
        << "received uevent with an invalid control message: ignoring.";
      return;
    }

    const struct ucred* const cmsg_ucred = \
        reinterpret_cast<const struct ucred*>(CMSG_DATA(cmsg_header));

    if (cmsg_ucred == nullptr) {
      /* missing ucred -- ignore */
      USBGUARD_LOG(Warning) << "ueventProcessRead: "
        << "received uevent without required SCM_CREDENTIALS data: ignoring.";
      return;
    }

    if (cmsg_ucred->pid != 0 ||
      cmsg_ucred->uid != 0 ||
      cmsg_ucred->gid != 0) {
      /* unknown origin -- ignore */
      USBGUARD_LOG(Debug) << "received uevent of unknown origin: ignoring.";
      return;
    }

    /*
     * Try to parse uevent from the buffer and process it.
     */
    try {
      UEvent uevent = UEvent::fromString(buffer, /*attributes_only=*/false, /*trace=*/false);
      ueventProcessUEvent(std::move(uevent));
    }
    catch (...) {
      USBGUARD_LOG(Warning) << "ueventProcessRead: received invalid uevent data";
      USBGUARD_LOG(Debug) << "ueventProcessRead: uevent_data=" << base64Encode(buffer);
    }
  }

  void UEventDeviceManager::ueventProcessUEvent(UEvent uevent)
  {
    const std::string subsystem = uevent.getAttribute("SUBSYSTEM");
    const std::string devtype = uevent.getAttribute("DEVTYPE");
    const std::string action = uevent.getAttribute("ACTION");

    /*
     * We don't care about the event if it's not from the "usb" subsystem.
     * The device type attribute value is checked later based on the data
     * read from the sysfs uevent file in the device directory.
     */
    if (subsystem != "usb") {
      USBGUARD_LOG(Debug) << "Ignoring non-USB device:"
        << " subsystem=" << subsystem
        << " devtype=" << devtype
        << " action=" << action;
      return;
    }

    const std::string sysfs_devpath = uevent.getAttribute("DEVPATH");

    if (_enumeration) {
      _backlog.emplace_back(std::move(uevent));
    }
    else {
      ueventProcessAction(action, sysfs_devpath);
    }
  }

  void UEventDeviceManager::ueventProcessAction(const std::string& action, const std::string& sysfs_devpath)
  {
    try {
      uint32_t id = 0;
      const bool known_path = knownSysfsPath(sysfs_devpath, &id);

      if (action == "add" /*|| action == "change"*/) {
        if (known_path && id > 0) {
          processDevicePresence(id);
        }
        else {
          SysFSDevice sysfs_device(sysfs_devpath);

          /*
           * Do some additional sanity checking.
           */
          if (sysfs_device.getUEvent().hasAttribute("DEVTYPE")) {
            const std::string devtype = sysfs_device.getUEvent().getAttribute("DEVTYPE");

            if (devtype != "usb_device") {
              USBGUARD_LOG(Debug) << sysfs_devpath << ": UEvent DEVTYPE != usb_device. Ignoring event.";
              return;
            }
          }
          else {
            if (!sysfs_device.hasAttribute("descriptors")) {
              USBGUARD_LOG(Debug) << sysfs_devpath << ": UEvent doesn't refer to a device with a descriptors file. Ignoring event.";
              return;
            }
          }

          processDeviceInsertion(sysfs_device, /*signal_present=*/known_path);
          USBGUARD_LOG(Debug) << "Enumeration notify: sysfs_devpath=" << sysfs_devpath
            << " _enumeration=" << _enumeration
            << " known_path=" << known_path;
        }
      }
      else if (action == "remove") {
        processDeviceRemoval(sysfs_devpath);
      }
      else if (action == "bind" || action == "unbind") {
        USBGUARD_LOG(Debug) << action << "=" << sysfs_devpath;
      }
      else {
        USBGUARD_LOG(Warning) << "Ignoring unknown UEvent action: sysfs_devpath=" << sysfs_devpath
          << " action=" << action;
      }
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Warning) << "USB Device Exception: "
        << ex.message();
      DeviceException(ex.message());
    }
    catch (...) {
      USBGUARD_LOG(Warning) << "USB Device Exception: unknown exception";
      DeviceException("unknown exception");
    }
  }

  int UEventDeviceManager::ueventEnumerateDevices()
  {
    USBGUARD_LOG(Trace);
    return loadFiles(SysFSDevice::getSysfsRoot() + "/bus/usb/devices",
        UEventDeviceManager::ueventEnumerateFilterDevice,
    [this](const std::string& devpath, const std::string& buspath) {
      return ueventEnumerateTriggerDevice(devpath, buspath);
    },
    UEventDeviceManager::ueventEnumerateComparePath);
  }

  int UEventDeviceManager::ueventEnumerateTriggerDevice(const std::string& devpath, const std::string& buspath)
  {
    USBGUARD_LOG(Trace) << "devpath=" << devpath << " buspath=" << buspath;

    try {
      std::string sysfs_absolute_path(PATH_MAX, 0);

      if (::realpath(devpath.c_str(), &sysfs_absolute_path[0]) == nullptr) {
        USBGUARD_LOG(Warning) << "Cannot resolve realpath for " << devpath;
        return 0;
      }

      sysfs_absolute_path.resize(::strlen(&sysfs_absolute_path[0]));

      if (!hasPrefix(sysfs_absolute_path, SysFSDevice::getSysfsRoot())) {
        USBGUARD_LOG(Warning) << "Device isn't rooted inside " << SysFSDevice::getSysfsRoot() << ". Skipping.";
        return 0;
      }

      const std::string sysfs_relative_path = removePrefix(SysFSDevice::getSysfsRoot(), sysfs_absolute_path);
      USBGUARD_LOG(Debug) << "sysfs_relative_path=" << sysfs_relative_path;
      SysFSDevice device(sysfs_relative_path);

      if (device.getUEvent().getAttribute("DEVTYPE") == "usb_device") {
        learnSysfsPath(sysfs_relative_path);
        ueventProcessAction("add", sysfs_relative_path);
        return 1;
      }
      else {
        USBGUARD_LOG(Debug) << "DEVTYPE != usb_device. Skipping.";
      }
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Warning) << "device enumeration exception: " << buspath << ": " << ex.message();
    }
    catch (const std::exception& ex) {
      USBGUARD_LOG(Warning) << "device enumeration exception: " << buspath << ": " << ex.what();
    }

    return 0;
  }

  void UEventDeviceManager::processBacklog()
  {
    USBGUARD_LOG(Debug) << "Processing backlog: _backlog.size() = " << _backlog.size();

    try {
      for (auto& it : _backlog) {
        ueventProcessUEvent(std::move(it));
      }
    }
    catch (...) {
      USBGUARD_LOG(Warning) << "ueventProcessBacklog: error processing uevent data";
    }
  }
} /* namespace usbguard */
#endif /* HAVE_UDEV */

/* vim: set ts=2 sw=2 et */
