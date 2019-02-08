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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#if defined(HAVE_LINUX)
#include "LinuxDeviceManager.hpp"
#include "LinuxDeviceDefinition.hpp"

#include "SysFSDevice.hpp"
#include "Base64.hpp"

#include "Common/FDInputStream.hpp"
#include "Common/Utility.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/Exception.hpp"
#include "usbguard/USB.hpp"

#include <stdexcept>
#include <fstream>
#include <chrono>

#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/inotify.h>
#include <linux/netlink.h>
#include <limits.h>
#include <stdlib.h>

namespace usbguard
{
  LinuxDevice::LinuxDevice(LinuxDeviceManager& device_manager, SysFSDevice& sysfs_device)
    : Device(device_manager)
  {
    /*
     * Look for the parent USB device and set the parent id
     * if we find one.
     */
    const std::string sysfs_parent_path(sysfs_device.getParentPath());
    const SysFSDevice sysfs_parent_device(sysfs_parent_path);

    if (sysfs_parent_device.getUEvent().getAttribute("DEVTYPE") == "usb_device") {
      setParentID(device_manager.getIDFromSysfsPath(sysfs_parent_path));
    }
    else {
      setParentID(Rule::RootID);
      setParentHash(hashString(sysfs_parent_path));
    }

    /*
     * Set name
     */
    setName(sysfs_device.readAttribute("product", /*strip_last_null=*/true, /*optional=*/true));
    /*
     * Set USB ID
     */
    const std::string id_vendor(sysfs_device.readAttribute("idVendor", /*strip_last_null=*/true));
    const std::string id_product(sysfs_device.readAttribute("idProduct", /*strip_last_null=*/true));
    const USBDeviceID device_id(id_vendor, id_product);
    setDeviceID(device_id);
    /*
     * Set serial number
     */
    setSerial(sysfs_device.readAttribute("serial", /*strip_last_null=*/true, /*optional=*/true));
    /*
     * Set USB port
     */
    setPort(sysfs_device.getName());
    /*
     * Sync authorization target
     */
    const std::string authorized_value(sysfs_device.readAttribute("authorized", /*strip_last_null=*/true));

    if (authorized_value == "0") {
      setTarget(Rule::Target::Block);
    }
    else if (authorized_value == "1") {
      setTarget(Rule::Target::Allow);
    }
    else {
      /*
       * Block the device if we get an unexpected value
       */
      setTarget(Rule::Target::Block);
    }
    /*
     * Set connect type
     */
    setConnectType(sysfs_device.readAttribute("port/connect_type", /*strip_last_null=*/true, /*optional=*/true));

    /*
     * Process USB descriptor data.
     *
     * FDInputStream (stdio_filebuf) is responsible for closing the file
     * descriptor returned by sysfs_device.openAttribute().
     *
     */
    FDInputStream descriptor_stream(sysfs_device.openAttribute("descriptors"));
    /*
     * Find out the descriptor data stream size
     */
    size_t descriptor_expected_size = 0;

    if (!descriptor_stream.good()) {
      throw ErrnoException("LinuxDevice", sysfs_device.getPath(), errno);
    }

    initializeHash();
    USBDescriptorParser parser(*this);

    if ((descriptor_expected_size = parser.parse(descriptor_stream)) < sizeof(USBDeviceDescriptor)) {
      throw Exception("LinuxDevice", sysfs_device.getPath(),
        "USB descriptor parser processed less data than the size of a USB device descriptor");
    }

    finalizeHash();
    /*
     * From now own we take ownership of the SysFSDevice instance.
     */
    _sysfs_device = std::move(sysfs_device);
  }

  SysFSDevice& LinuxDevice::sysfsDevice()
  {
    return _sysfs_device;
  }

  const std::string& LinuxDevice::getSysPath() const
  {
    return _sysfs_device.getPath();
  }

  bool LinuxDevice::isController() const
  {
    /*
     * On Linux, we classify a USB device as USB controller device if and only if its
     * port indentifier starts with "usb" and the device has exactly one USB HUB interface.
     */
    if (!hasPrefix(getPort(), "usb") || getInterfaceTypes().size() != 1) {
      return false;
    }

    const USBInterfaceType hub_interface("09:00:*");
    return hub_interface.appliesTo(getInterfaceTypes()[0]);
  }

  std::string LinuxDevice::getSystemName() const
  {
    return getSysPath();
  }

  void LinuxDevice::parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
    USBDescriptor* descriptor_out)
  {
    USBGUARD_LOG(Trace);
    USBDescriptorParserHooks::parseUSBDescriptor(parser, descriptor_raw, descriptor_out);

    if (isLinuxRootHubDeviceDescriptor(descriptor_out)) {
      updateHashLinuxRootHubDeviceDescriptor(descriptor_raw);
    }
    else {
      updateHash(descriptor_raw, static_cast<size_t>(descriptor_raw->bHeader.bLength));
    }
  }

  void LinuxDevice::loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor)
  {
    const auto type = static_cast<USBDescriptorType>(descriptor->bHeader.bDescriptorType);

    switch (type) {
    case USBDescriptorType::Device:
      loadDeviceDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::Configuration:
      loadConfigurationDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::Interface:
      loadInterfaceDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::Endpoint:
      loadEndpointDescriptor(parser, descriptor);
      break;

    case USBDescriptorType::AssociationInterface:
    case USBDescriptorType::Unknown:
    case USBDescriptorType::String:
    default:
      USBGUARD_LOG(Debug) << "Ignoring descriptor: type=" << (int)type
        << " size=" << (int)descriptor->bHeader.bLength;
    }
  }

  bool LinuxDevice::isLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);

    if (descriptor->bHeader.bDescriptorType != USB_DESCRIPTOR_TYPE_DEVICE) {
      return false;
    }

    const USBDeviceDescriptor* const device_descriptor = \
      reinterpret_cast<const USBDeviceDescriptor* const>(descriptor);

    if (device_descriptor->idVendor == 0x1d6b /* Linux Foundation */) {
      switch (device_descriptor->idProduct) {
      case 0x0001: /* 1.1 root hub */
      case 0x0002: /* 2.0 root hub */
      case 0x0003: /* 3.0 root hub */
        return true;

      default:
        return false;
      }
    }

    return false;
  }

  void LinuxDevice::updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);
    USBDeviceDescriptor descriptor_modified = *reinterpret_cast<const USBDeviceDescriptor* const>(descriptor);
    descriptor_modified.bcdDevice = 0;
    updateHash(&descriptor_modified, sizeof descriptor_modified);
  }

  /*
   * Manager
   */
  LinuxDeviceManager::LinuxDeviceManager(DeviceManagerHooks& hooks)
    : DeviceManager(hooks),
      _thread(this, &LinuxDeviceManager::thread),
      _enumeration(false)
  {
    setDefaultBlockedState(/*state=*/true);
    setEnumerationOnlyMode(/*state=*/false);
    USBGUARD_SYSCALL_THROW("LinuxDeviceManager", (_wakeup_fd = eventfd(0, 0)) < 0);
    _uevent_fd = ueventOpen();
  }

  LinuxDeviceManager::~LinuxDeviceManager()
  {
    if (getRestoreControllerDeviceState()) {
      setDefaultBlockedState(/*state=*/false); // FIXME: Set to previous state
    }

    stop();

    if (_uevent_fd >= 0) {
      (void)close(_uevent_fd);
    }

    if (_wakeup_fd >= 0) {
      (void)close(_wakeup_fd);
    }
  }

  void LinuxDeviceManager::setDefaultBlockedState(bool state)
  {
    _default_blocked_state = state;
  }

  void LinuxDeviceManager::setEnumerationOnlyMode(bool state)
  {
    _enumeration_only_mode = state;
  }

  void LinuxDeviceManager::start()
  {
    _thread.start();
  }

  void LinuxDeviceManager::stop()
  {
    // stop monitor
    _thread.stop(/*do_wait=*/false);
    { /* Wakeup the device manager thread */
      const uint64_t one = 1ULL;
      USBGUARD_SYSCALL_THROW("LinuxDeviceManager",
        write(_wakeup_fd, &one, sizeof one) != sizeof one);
    }
    _thread.wait();
  }

  void LinuxDeviceManager::scan()
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
      throw Exception("LinuxDeviceManager", "present devices", "failed to enumerate");
    }
  }

  std::shared_ptr<Device> LinuxDeviceManager::applyDevicePolicy(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "id=" << id
      << " target=" << Rule::targetToString(target);
    std::shared_ptr<LinuxDevice> device = std::static_pointer_cast<LinuxDevice>(getDevice(id));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    sysfsApplyTarget(device->sysfsDevice(), target);
    device->setTarget(target);
    return std::move(device);
  }

  int LinuxDeviceManager::ueventOpen()
  {
    int socket_fd = -1;
    USBGUARD_SYSCALL_THROW("LinuxDeviceManager",
      (socket_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)) < 0);

    try {
      const int optval = 1;
      USBGUARD_SYSCALL_THROW("LinuxDeviceManager",
        setsockopt(socket_fd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof optval) != 0);
      /*
       * Set a 1MiB receive buffer on the netlink socket to avoid ENOBUFS error
       * in recvmsg.
       */
      const size_t rcvbuf_max = 1024 * 1024;
      USBGUARD_SYSCALL_THROW("LinuxDeviceManager",
        setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_max, sizeof rcvbuf_max) != 0);
      struct sockaddr_nl sa = { };
      sa.nl_family = AF_NETLINK;
      sa.nl_pid = getpid();
      sa.nl_groups = -1;
      USBGUARD_SYSCALL_THROW("LinuxDeviceManager",
        bind(socket_fd, reinterpret_cast<const sockaddr*>(&sa), sizeof sa) != 0);
    }
    catch (...) {
      (void)close(socket_fd);
      throw;
    }

    return socket_fd;
  }

  void LinuxDeviceManager::sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target)
  {
    std::string name;
    std::string value("0");

    switch (target) {
    case Rule::Target::Allow:
      umockdevAuthorizeBySysfsPath(sysfs_device.getPath());
      name = "authorized";
      value = "1";
      break;

    case Rule::Target::Block:
      umockdevDeauthorizeBySysfsPath(sysfs_device.getPath());
      name = "authorized";
      value = "0";
      break;

    case Rule::Target::Reject:
      name = "remove";
      value = "1";
      break;

    case Rule::Target::Match:
    case Rule::Target::Device:
    case Rule::Target::Unknown:
    case Rule::Target::Invalid:
    default:
      throw std::runtime_error("Unknown rule target in applyDevicePolicy");
    }

    sysfs_device.setAttribute(name, value);
  }

  void LinuxDeviceManager::thread()
  {
    USBGUARD_LOG(Trace) << "Entering main loop.";

    try {
      const int max_fd = std::max(_uevent_fd, std::max(_wakeup_fd, _inotify_fd));
      fd_set readset;

      while (!_thread.stopRequested()) {
        struct timeval tv_timeout = { 5, 0 };
        FD_ZERO(&readset);
        FD_SET(_uevent_fd, &readset);
        FD_SET(_wakeup_fd, &readset);
        FD_SET(_inotify_fd, &readset);
        const int count = ::select(max_fd + 1, &readset, NULL, NULL, &tv_timeout);

        if (count >= 1 || count <= 3) {
          if (FD_ISSET(_wakeup_fd, &readset)) {
            USBGUARD_LOG(Debug) << "Wakeup event.";
            continue;
          }

          if (FD_ISSET(_uevent_fd, &readset)) {
            USBGUARD_LOG(Debug) << "UEvent read event.";
            ueventProcessRead();
          }

          if (FD_ISSET(_inotify_fd, &readset)) {
            USBGUARD_LOG(Debug) << "Inotify event.";
            umockdevProcessInotify();
          }
        }
        else if (count == 0) {
          /* Timeout */
          continue;
        }
        else {
          /* Error */
          USBGUARD_LOG(Error) << "LinuxDeviceManager: select failed: errno=" << errno;
          _thread.stop();
        }
      } /* Thread main loop */
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Error) << "LinuxDeviceManager: " << ex.message();
    }

    USBGUARD_LOG(Trace) << "Leaving main loop.";
  }

  void LinuxDeviceManager::ueventProcessRead()
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
        reinterpret_cast<const struct ucred* const>(CMSG_DATA(cmsg_header));

    if (cmsg_ucred == nullptr) {
      /* missing ucred -- ignore */
      USBGUARD_LOG(Warning) << "ueventProcessRead: "
        << "received uevent without required SCM_CREDENTIALS data: ignoring.";
      return;
    }

    USBGUARD_LOG(Debug) << "ucred.pid=" << cmsg_ucred->pid;
    USBGUARD_LOG(Debug) << "ucred.gid=" << cmsg_ucred->gid;
    USBGUARD_LOG(Debug) << "ucred.uid=" << cmsg_ucred->uid;

    if (_restrict_uevent_origin) {
      if (/* root */!(cmsg_ucred->pid == 0 && cmsg_ucred->uid == 0 && cmsg_ucred->gid == 0) &&
        /* self */!(cmsg_ucred->pid == getpid())) {
        /* Unknown origin -- ignore */
        USBGUARD_LOG(Debug) << "Received an uevent of unknown origin:"
          << " uid=" << cmsg_ucred->uid << " gid=" << cmsg_ucred->gid << " pid=" << cmsg_ucred->pid;
        return;
      }
    }

    /**/
    try {
      if (hasPrefix(buffer, "libudev")) {
        if (!_process_libudev_uevents) {
          USBGUARD_LOG(Debug) << "libudev uevent processing disabled: ignoring.";
          return;
        }

        USBGUARD_LOG(Debug) << "Parsing uevent with libudev header";
        const struct libudev_netlink_header {
          /* "libudev" prefix to distinguish libudev and kernel messages */
          char prefix[8];
          /*
           * magic to protect against daemon <-> library message format mismatch
           * used in the kernel from socket filter rules; needs to be stored in network order
           */
          unsigned int magic;

#ifndef UDEV_MONITOR_MAGIC
#define UDEV_MONITOR_MAGIC 0xfeedcafe
#endif

          /* total length of header structure known to the sender */
          unsigned int header_size;
          /* properties string buffer */
          unsigned int properties_off;
          unsigned int properties_len;
          /*
           * hashes of primary device properties strings, to let libudev subscribers
           * use in-kernel socket filters; values need to be stored in network order
           */
          unsigned int filter_subsystem_hash;
          unsigned int filter_devtype_hash;
          unsigned int filter_tag_bloom_hi;
          unsigned int filter_tag_bloom_lo;
        } * const header = reinterpret_cast<const struct libudev_netlink_header*>(&buffer[0]);
        const std::string attributes_buffer = buffer.substr(header->properties_off, header->properties_len);
        USBGUARD_LOG(Debug) << "data:" << attributes_buffer;
        const UEvent uevent = UEvent::fromString(attributes_buffer, /*attributes_only=*/true, /*trace=*/false);
        ueventProcessUEvent(uevent);
      }
      else {
        if (!_process_kernel_uevents) {
          USBGUARD_LOG(Debug) << "kernel uevent processing disabled, ignoring.";
          return;
        }

        USBGUARD_LOG(Debug) << "Parsing uevent with kernel header";
        /*
         * Try to parse uevent from the buffer and process it.
         */
        const UEvent uevent = UEvent::fromString(buffer, /*attributes_only=*/false, /*trace=*/false);
        ueventProcessUEvent(uevent);
      }
    }
    catch (...) {
      USBGUARD_LOG(Warning) << "ueventProcessRead: received invalid uevent data";
      USBGUARD_LOG(Debug) << "ueventProcessRead: uevent_data=" << base64Encode(buffer);
    }
  }

  void LinuxDeviceManager::ueventProcessUEvent(const UEvent& uevent)
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
    bool enumeration_notify = false;

    try {
      std::unique_lock<std::mutex> lock(_enumeration_mutex);
      uint32_t id = 0;
      const bool known_path = knownSysfsPath(sysfs_devpath, &id);

      if (action == "add" || action == "change") {
        lock.unlock();
        USBGUARD_LOG(Debug) << "known_path=" << known_path << " id=" << id;

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

          if (known_path) {
            enumeration_notify = true;
          }
        }
      }
      else if (action == "remove") {
        lock.unlock();
        USBGUARD_LOG(Debug) << "remove=" << sysfs_devpath;
        processDeviceRemoval(sysfs_devpath);

        if (known_path) {
          enumeration_notify = true;
        }
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

    if (_enumeration && enumeration_notify) {
      _enumeration_complete.notify_one();
      USBGUARD_LOG(Debug) << "Notified enumeration routine after sysfs_path=" << sysfs_devpath;
    }
  }

  bool LinuxDeviceManager::ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
    const std::pair<std::string, std::string>& b)
  {
    USBGUARD_LOG(Trace) << "a.second=" << a.second << " b.second=" << b.second;
    const std::string full_a = a.second;
    const std::string full_b = b.second;
    const std::size_t component_count_a = countPathComponents(full_a);
    const std::size_t component_count_b = countPathComponents(full_b);

    if (component_count_a < component_count_b) {
      return true;
    }
    else if (component_count_a > component_count_b) {
      return false;
    }

    const std::string base_a = filenameFromPath(full_a, /*include_extension=*/true);
    const std::string base_b = filenameFromPath(full_b, /*include_extension=*/true);
    const bool a_has_usb_prefix = hasPrefix(base_a, "usb");
    const bool b_has_usb_prefix = hasPrefix(base_b, "usb");
    USBGUARD_LOG(Debug) << "a_prefix=" << a_has_usb_prefix << " b_prefix=" << b_has_usb_prefix;

    if (a_has_usb_prefix) {
      if (!b_has_usb_prefix) {
        return true;
      }
      else {
        return base_a < base_b;
      }
    }
    else {
      if (b_has_usb_prefix) {
        return false;
      }
    }

    if (full_a.size() < full_b.size()) {
      return true;
    }
    else if (full_a.size() > full_b.size()) {
      return false;
    }

    return full_a < full_b;
  }

  int LinuxDeviceManager::ueventEnumerateDevices()
  {
    USBGUARD_LOG(Trace);
    const auto lambdaEnumerateTriggerAndWaitForDevice = [this](const std::string& devpath, const std::string& buspath) {
      return ueventEnumerateTriggerAndWaitForDevice(devpath, buspath);
    };
    return loadFiles(SysFSDevice::getSysfsRoot() + "/bus/usb/devices",
        LinuxDeviceManager::ueventEnumerateFilterDevice,
        lambdaEnumerateTriggerAndWaitForDevice,
        LinuxDeviceManager::ueventEnumerateComparePath,
        /*rootdir_required=*/false);
  }

  std::string LinuxDeviceManager::ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry)
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
      struct stat st = { };

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
    /* UNREACHABLE */
    return std::string();
  }

  int LinuxDeviceManager::ueventEnumerateTriggerAndWaitForDevice(const std::string& devpath, const std::string& buspath)
  {
    USBGUARD_LOG(Trace) << "devpath=" << devpath << " buspath=" << buspath;

    try {
      std::string umockdev_realpath(PATH_MAX, 0);
      std::unique_ptr<gchar, FreeDeleter> umockdev_root_dir(umockdev_testbed_get_root_dir(_testbed.get()));
      std::string umockdev_devpath(std::string(umockdev_root_dir.get()) + devpath);

      if (access(umockdev_devpath.c_str(), F_OK) != 0) {
        USBGUARD_LOG(Info) << "Device disappeared during enumeration: " << umockdev_devpath;
        return 1;
      }

      if (::realpath(umockdev_devpath.c_str(), &umockdev_realpath[0]) == nullptr) {
        USBGUARD_LOG(Warning) << "Cannot resolve realpath for " << devpath;
        return 0;
      }

      umockdev_realpath.resize(::strlen(&umockdev_realpath[0]));
      const std::string sysfs_absolute_path(removePrefix(umockdev_root_dir.get(), umockdev_realpath));
      USBGUARD_LOG(Debug) << "umockdev_realpath=" << umockdev_realpath;
      USBGUARD_LOG(Debug) << "sysfs_absolute_path=" << sysfs_absolute_path;

      if (!hasPrefix(sysfs_absolute_path, SysFSDevice::getSysfsRoot())) {
        USBGUARD_LOG(Warning) << "Device isn't rooted inside " << SysFSDevice::getSysfsRoot() << ". Skipping.";
        return 0;
      }

      const std::string sysfs_relative_path = removePrefix(SysFSDevice::getSysfsRoot(), sysfs_absolute_path);
      USBGUARD_LOG(Debug) << "sysfs_relative_path=" << sysfs_relative_path;
      SysFSDevice device(sysfs_relative_path);

      if (device.getUEvent().getAttribute("DEVTYPE") == "usb_device") {
        std::unique_lock<std::mutex> lock(_enumeration_mutex);
        USBGUARD_LOG(Debug) << "DEVTYPE == usb_device";
        learnSysfsPath(sysfs_relative_path);
        /* TODO: Hide this call inside the Device instance by intercepting setAttribute("uevent", ...) calls */
        USBGUARD_LOG(Debug) << "Triggering: " << sysfs_absolute_path;
        umockdev_testbed_uevent(_testbed.get(), sysfs_absolute_path.c_str(), "add");
        USBGUARD_LOG(Debug) << "Waiting for: " << sysfs_absolute_path;
        const auto lambdaSysfsPathIDAssigned = [this, sysfs_relative_path] {
          uint32_t id = 0;
          const bool known = knownSysfsPath(sysfs_relative_path, &id);
          return id != 0;
        }

        if (!_enumeration_complete.wait_for(lock, std::chrono::seconds(2), lambdaSysfsPathIDAssigned)) {
          throw Exception("LinuxDeviceManager", sysfs_absolute_path, "enumeration timeout");
        }

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

  void LinuxDeviceManager::processDevicePresence(const uint32_t id)
  {
    USBGUARD_LOG(Trace) << "id=" << id;

    try {
      std::shared_ptr<LinuxDevice> device = \
        std::static_pointer_cast<LinuxDevice>(DeviceManager::getDevice(id));
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

  void LinuxDeviceManager::processDeviceInsertion(SysFSDevice& sysfs_device, const bool signal_present)
  {
    USBGUARD_LOG(Trace) << "sysfs_device=" << sysfs_device.getPath();

    try {
      std::shared_ptr<LinuxDevice> device = std::make_shared<LinuxDevice>(*this, sysfs_device);

      if (device->isController() && !_enumeration_only_mode) {
        USBGUARD_LOG(Debug) << "Setting default blocked state for controller device to " << _default_blocked_state;
        device->sysfsDevice().setAttribute("authorized_default", _default_blocked_state ? "0" : "1");
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

  void LinuxDeviceManager::insertDevice(std::shared_ptr<LinuxDevice> device)
  {
    DeviceManager::insertDevice(std::static_pointer_cast<Device>(device));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    learnSysfsPath(device->getSysPath(), device->getID());
  }

  void LinuxDeviceManager::processDeviceRemoval(const std::string& sysfs_devpath)
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

  std::shared_ptr<Device> LinuxDeviceManager::removeDevice(const std::string& sysfs_path)
  {
    /*
     * FIXME: device map locking
     */
    if (!knownSysfsPath(sysfs_path)) {
      throw Exception("LinuxDeviceManager", sysfs_path, "unknown syspath, cannot remove device");
    }

    std::shared_ptr<Device> device = DeviceManager::removeDevice(getIDFromSysfsPath(sysfs_path));
    forgetSysfsPath(sysfs_path);
    return device;
  }

  uint32_t LinuxDeviceManager::getIDFromSysfsPath(const std::string& sysfs_path) const
  {
    uint32_t id = 0;

    if (knownSysfsPath(sysfs_path, &id)) {
      return id;
    }

    throw Exception("LinuxDeviceManager", sysfs_path, "unknown sysfs path");
  }

  bool LinuxDeviceManager::isPresentSysfsPath(const std::string& sysfs_path) const
  {
    uint32_t id = 0;

    if (knownSysfsPath(sysfs_path, &id)) {
      return 0 == id;
    }

    return false;
  }

  bool LinuxDeviceManager::knownSysfsPath(const std::string& sysfs_path, uint32_t* id_ptr) const
  {
    USBGUARD_LOG(Trace) << "sysfs_path=" << sysfs_path << " id_ptr=" << (void*)id_ptr;
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

    return known;
  }

  void LinuxDeviceManager::learnSysfsPath(const std::string& sysfs_path, uint32_t id)
  {
    USBGUARD_LOG(Trace) << "Learn sysfs_path=" << sysfs_path << " size=" << sysfs_path.size() << " id=" << id;
    _sysfs_path_to_id_map[sysfs_path] = id;
  }

  void LinuxDeviceManager::forgetSysfsPath(const std::string& sysfs_path)
  {
    USBGUARD_LOG(Trace) << "Forget sysfs_path=" << sysfs_path;
    _sysfs_path_to_id_map.erase(sysfs_path);
  }
} /* namespace usbguard */
#endif /* HAVE_LINUX */
/* vim: set ts=2 sw=2 et */
