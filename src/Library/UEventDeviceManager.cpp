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
#include <linux/netlink.h>
#include <limits.h>
#include <stdlib.h>

namespace usbguard {
  UEventDevice::UEventDevice(UEventDeviceManager& device_manager, SysFSDevice& sysfs_device)
    : Device(device_manager)
  {
    /*
     * Look for the parent USB device and set the parent id
     * if we find one.
     */
    const std::string sysfs_parent_path(sysfs_device.getParentPath());
    const SysFSDevice sysfs_parent_device(sysfs_parent_path);

    if (sysfs_parent_device.getUEvent().getAttribute("DEVTYPE") == "usb_device") {
      setParentID(device_manager.getIDFromSysPath(sysfs_parent_path));
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
     * Process USB descriptor data
     */
    FDInputStream descriptor_stream(sysfs_device.openAttribute("descriptors"));

    /* Find out the descriptor data stream size */
    size_t descriptor_expected_size = 0;

    if (!descriptor_stream.good()) {
      throw ErrnoException("UEventDevice", sysfs_device.getPath(), errno);
    }

    initializeHash();

    USBDescriptorParser parser(*this);
    if ((descriptor_expected_size = parser.parse(descriptor_stream)) < sizeof(USBDeviceDescriptor)) {
      throw Exception("UEventDevice", sysfs_device.getPath(), "USB descriptor parser processed less data than the size of a USB device descriptor");
    }

    finalizeHash();

    /*
     * From now own we take ownership of the SysFSDevice instance.
     */
    _sysfs_device = std::move(sysfs_device);
  }

  SysFSDevice& UEventDevice::sysfsDevice()
  {
    return _sysfs_device;
  }

  const std::string& UEventDevice::getSysPath() const
  {
    return _sysfs_device.getPath();
  }

  bool UEventDevice::isController() const
  {
    if (getPort().substr(0, 3) != "usb" || getInterfaceTypes().size() != 1) {
      return false;
    }

    const USBInterfaceType hub_interface("09:00:*");

    return hub_interface.appliesTo(getInterfaceTypes()[0]);
  }

  void UEventDevice::parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw, USBDescriptor* descriptor_out)
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

  void UEventDevice::loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor)
  {
    const auto type = static_cast<USBDescriptorType>(descriptor->bHeader.bDescriptorType);
 
    switch(type) {
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
                            << " size=" << descriptor->bHeader.bLength;
    }
  }

  bool UEventDevice::isLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);

    if (descriptor->bHeader.bDescriptorType != USB_DESCRIPTOR_TYPE_DEVICE) {
      return false;
    }

    const USBDeviceDescriptor * const device_descriptor = \
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

  void UEventDevice::updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);

    USBDeviceDescriptor descriptor_modified = *reinterpret_cast<const USBDeviceDescriptor* const>(descriptor);
    descriptor_modified.bcdDevice = 0;
    updateHash(&descriptor_modified, sizeof descriptor_modified);
  }

  /*
   * Manager
   */
  UEventDeviceManager::UEventDeviceManager(DeviceManagerHooks& hooks, const std::string& sysfs_root, bool dummy_mode)
    : DeviceManager(hooks),
      _thread(this, &UEventDeviceManager::thread),
      _uevent_fd(-1),
      _wakeup_fd(-1),
      _sysfs_root(sysfs_root),
      _dummy_mode(dummy_mode),
      _enumeration(false),
      _enumeration_count(0)
  {
    setDefaultBlockedState(/*state=*/true);
    setEnumerationOnlyMode(/*state=*/false);

    USBGUARD_SYSCALL_THROW("UEvent device manager",
       (_wakeup_fd = eventfd(0, 0)) < 0);

    if (!_dummy_mode) {
      _uevent_fd = ueventOpen();
    }
    else {
      _uevent_fd = ueventDummyOpen();
    }
  }

  UEventDeviceManager::~UEventDeviceManager()
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

  void UEventDeviceManager::setDefaultBlockedState(bool state)
  {
    _default_blocked_state = state;
  }

  void UEventDeviceManager::setEnumerationOnlyMode(bool state)
  {
    _enumeration_only_mode = state;
  }

  void UEventDeviceManager::start()
  {
    _thread.start();
  }

  void UEventDeviceManager::stop()
  {
    // stop monitor
    _thread.stop(/*do_wait=*/false);
    { /* Wakeup the device manager thread */
      const uint64_t one = 1;
      USBGUARD_SYSCALL_THROW("Linux device manager",
        write(_wakeup_fd, &one, sizeof one) != sizeof one);
    }
    _thread.wait();
  }

  void UEventDeviceManager::scan()
  {
    USBGUARD_LOG(Trace);

    std::unique_lock<std::mutex> lock(_enumeration_mutex);
    Restorer<std::atomic<bool>, bool> \
      r(_enumeration, /*transient=*/true, /*restored=*/false);

    if (!_dummy_mode) {
      _enumeration_count = ueventEnumerateDevices();
    }
    else {
      _enumeration_count = ueventEnumerateDummyDevices();
    }

    USBGUARD_LOG(Debug) << "_enumeration_count=" << _enumeration_count;

    if (_enumeration_count == 0) {
      return;
    }

    if (_enumeration_count < 0) {
      throw Exception("UEventDeviceManager", "present devices", "failed to enumerate");
    }

    const auto timeout = std::chrono::seconds(2 * _enumeration_count);

    if (!_enumeration_complete.wait_for(lock, timeout,
          [&] { return (_enumeration_count > 0 ? false : true); })) {
      throw Exception("UEventDeviceManager", "present devices", "enumeration timeout");
    }
  }

  std::shared_ptr<Device> UEventDeviceManager::applyDevicePolicy(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "id=" << id
                        << " target=" << Rule::targetToString(target);

    std::shared_ptr<UEventDevice> device = std::static_pointer_cast<UEventDevice>(getDevice(id));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());

    sysfsApplyTarget(device->sysfsDevice(), target);
    device->setTarget(target);
 
    return std::move(device);
  }

  int UEventDeviceManager::ueventOpen()
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

      struct sockaddr_nl sa = { };
      sa.nl_family = AF_NETLINK;
      sa.nl_pid = getpid();
      sa.nl_groups = -1;

      USBGUARD_SYSCALL_THROW("UEvent device manager",
        bind(socket_fd, reinterpret_cast<const sockaddr*>(&sa), sizeof sa) != 0);
    }
    catch(...) {
      (void)close(socket_fd);
      throw;
    }

    return socket_fd;
  }

  int UEventDeviceManager::ueventDummyOpen()
  {
    int socket_fd = -1;

    USBGUARD_SYSCALL_THROW("UEvent device manager",
        (socket_fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0);

    try {
      const int optval = 1;

      USBGUARD_SYSCALL_THROW("UEvent device manager",
          setsockopt(socket_fd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof optval) != 0);

      struct sockaddr_un sa = { };
      sa.sun_family = AF_UNIX;
      strcpy(sa.sun_path, "/tmp/usbguard-dummy.sock");

      USBGUARD_SYSCALL_THROW("UEvent device manager",
          bind(socket_fd, reinterpret_cast<const sockaddr*>(&sa), sizeof sa) != 0);
    }
    catch(...) {
      (void)close(socket_fd);
      throw;
    }

    return socket_fd;
  }

  void UEventDeviceManager::sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target)
  {
    std::string name;
    std::string value("0");

    switch (target) {
      case Rule::Target::Allow:
        name = "authorized";
        value = "1";
        break;
      case Rule::Target::Block:
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

        switch (select(max_fd + 1, &readset, NULL, NULL, &tv_timeout))
        {
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
    catch(const Exception& ex) {
      USBGUARD_LOG(Error) << "UEventDeviceManager thread: " << ex.message();
    }

    USBGUARD_LOG(Trace) << "Leaving main loop.";
  }

  void UEventDeviceManager::ueventProcessRead()
  {
    std::string buffer(4096, 0);

    struct iovec iov[1];
    iov[0].iov_base = (void *)&buffer[0];
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
    
    const struct cmsghdr * const cmsg_header = CMSG_FIRSTHDR(&msg_header);

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

    const struct ucred * const cmsg_ucred = \
      reinterpret_cast<const struct ucred * const>(CMSG_DATA(cmsg_header));

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
      const UEvent uevent = UEvent::fromString(buffer, /*attributes_only=*/false, /*trace=*/false);
      ueventProcessUEvent(uevent);
    }
    catch(...) {
      USBGUARD_LOG(Warning) << "ueventProcessRead: received invalid uevent data";
      USBGUARD_LOG(Debug) << "ueventProcessRead: uevent_data=" << base64Encode(buffer);
    }
  }

  void UEventDeviceManager::ueventProcessUEvent(const UEvent& uevent)
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

    const std::string sysfs_devpath = _sysfs_root + uevent.getAttribute("DEVPATH");

    try {
      std::unique_lock<std::mutex> lock(_enumeration_mutex);

      if (action == "add" || action == "change") {
        uint32_t id = 0;
        const bool known_path = knownSysPath(sysfs_devpath, &id);

        lock.unlock();

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

          if (_enumeration && known_path) {
            --_enumeration_count;
            _enumeration_complete.notify_one();
          }
        }
      }
      else if (action == "remove") {
        lock.unlock();
        processDeviceRemoval(sysfs_devpath);
      }
      else {
        USBGUARD_LOG(Warning) << "Ignoring unknown UEvent action: sysfs_devpath=" << sysfs_devpath
          << " action=" << action;
      }
    }
    catch(const Exception& ex) {
      USBGUARD_LOG(Warning) << "USB Device Exception: "
                            << ex.message();
      DeviceException(ex.message());
    }
    catch(...) {
      USBGUARD_LOG(Warning) << "USB Device Exception: unknown exception";
      DeviceException("unknown exception");
    }
  }

  bool UEventDeviceManager::ueventEnumerateComparePath(const std::pair<std::string,std::string>& a, const std::pair<std::string,std::string>& b)
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

  int UEventDeviceManager::ueventEnumerateDevices()
  {
    USBGUARD_LOG(Trace);
    return loadFiles(_sysfs_root + "/bus/usb/devices",
      UEventDeviceManager::ueventEnumerateFilterDevice,
      [this](const std::string& devpath, const std::string& buspath)
      {
        return ueventEnumerateTriggerDevice(devpath, buspath);
      },
      UEventDeviceManager::ueventEnumerateComparePath);
  }

  int UEventDeviceManager::ueventEnumerateDummyDevices()
  {
    USBGUARD_LOG(Trace);
    return loadFiles(_sysfs_root + "/bus/usb/devices",
      UEventDeviceManager::ueventEnumerateFilterDevice,
      [this](const std::string& devpath, const std::string& buspath)
      {
        (void)buspath;
        UEvent uevent;
        uevent.setAttribute("SUBSYSTEM", "usb");
        uevent.setAttribute("DEVTYPE", "usb_device");
        uevent.setAttribute("ACTION", "add");
        uevent.setAttribute("DEVPATH", removePrefix(_sysfs_root, devpath));

        std::unique_ptr<char, FreeDeleter> realpath_cstr(::realpath(devpath.c_str(), nullptr));
        const std::string syspath(realpath_cstr.get());
        learnSysPath(syspath);

        ueventProcessUEvent(uevent);
        return 1;
      },
      UEventDeviceManager::ueventEnumerateComparePath);
  }

  std::string UEventDeviceManager::ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry)
  {
#if defined(_DIRENT_HAVE_D_TYPE)
    if (direntry->d_type != DT_UNKNOWN) {
      switch(direntry->d_type) {
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

  int UEventDeviceManager::ueventEnumerateTriggerDevice(const std::string& devpath, const std::string& buspath)
  {
    USBGUARD_LOG(Trace) << "devpath=" << devpath << " buspath=" << buspath;

    try {
      std::unique_ptr<char, FreeDeleter> realpath_cstr(::realpath(devpath.c_str(), nullptr));
      const std::string syspath(realpath_cstr.get());
      SysFSDevice device(syspath);

      if (device.getUEvent().getAttribute("DEVTYPE") == "usb_device") {
        learnSysPath(syspath);
        device.setAttribute("uevent", "add");
        return 1;
      }
    }
    catch(const Exception& ex) {
      USBGUARD_LOG(Warning) << "device enumeration exception: " << buspath << ": " << ex.message();
    }
    catch(const std::exception& ex) {
      USBGUARD_LOG(Warning) << "device enumeration exception: " << buspath << ": " << ex.what();
    }
    return 0;
  }

  void UEventDeviceManager::processDevicePresence(const uint32_t id)
  {
    USBGUARD_LOG(Trace) << "id=" << id;

    try {
      std::shared_ptr<UEventDevice> device = \
        std::static_pointer_cast<UEventDevice>(DeviceManager::getDevice(id));

      device->sysfsDevice().reload();

      /*
       * TODO: Check attribute state
       *  - authorized_default (in case of controller)
       */
      DeviceEvent(DeviceManager::EventType::Present, device);
      return;
    }
    catch(const Exception& ex) {
      USBGUARD_LOG(Error) << "Present device exception: " << ex.message();
      DeviceException(ex.message());
    }
    catch(const std::exception& ex) {
      USBGUARD_LOG(Error) << "Present device exception: " << ex.what();
      DeviceException(ex.what());
    }
    catch(...) {
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

  void UEventDeviceManager::processDeviceInsertion(SysFSDevice& sysfs_device, const bool signal_present)
  {
    try {
      std::shared_ptr<UEventDevice> device = std::make_shared<UEventDevice>(*this, sysfs_device);

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
    catch(const Exception& ex) {
      USBGUARD_LOG(Error) << "Device insert exception: " << ex.message();
      DeviceException(ex.message());
    }
    catch(const std::exception& ex) {
      USBGUARD_LOG(Error) << "Device insert exception: " << ex.what();
      DeviceException(ex.what());
    }
    catch(...) {
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

  void UEventDeviceManager::insertDevice(std::shared_ptr<UEventDevice> device)
  {
    DeviceManager::insertDevice(std::static_pointer_cast<Device>(device));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    learnSysPath(device->getSysPath(), device->getID());
  }

  void UEventDeviceManager::processDeviceRemoval(const std::string& sysfs_devpath)
  {
    USBGUARD_LOG(Trace) << "sysfs_devpath=" << sysfs_devpath;

    try {
      std::shared_ptr<Device> device = removeDevice(sysfs_devpath);
      DeviceEvent(DeviceManager::EventType::Remove, device);
    } catch(...) {
      /* Ignore for now */
      USBGUARD_LOG(Debug) << "Removal of an unknown device ignored.";
      return;
    }
  }

  std::shared_ptr<Device> UEventDeviceManager::removeDevice(const std::string& syspath)
  {
    /*
     * FIXME: device map locking
     */
    if (!knownSysPath(syspath)) {
      throw Exception("removeDevice", syspath, "unknown syspath, cannot remove device");
    }

    std::shared_ptr<Device> device = DeviceManager::removeDevice(getIDFromSysPath(syspath));
    _syspath_map.erase(syspath);

    return device;
  }

  uint32_t UEventDeviceManager::getIDFromSysPath(const std::string& syspath) const
  {
    uint32_t id = 0;
    const bool known = knownSysPath(syspath, &id);

    if (known && id > 0) {
      return id;
    }

    throw Exception("UEventDeviceManager", syspath, "unknown syspath");
  }

  bool UEventDeviceManager::knownSysPath(const std::string& syspath, uint32_t * id_ptr) const
  {
    auto it = _syspath_map.find(syspath);

    uint32_t known_id = 0;
    bool known = false;

    if (it != _syspath_map.end()) {
      known = true;
      known_id = it->second;
    }

    if (id_ptr != nullptr) {
      *id_ptr = known_id;
    }

    return known;
  }

  void UEventDeviceManager::learnSysPath(const std::string& syspath, uint32_t id)
  {
    USBGUARD_LOG(Trace) << "syspath=" << syspath << " id=" << id;
    _syspath_map[syspath] = id;
  }

  void UEventDeviceManager::forgetSysPath(const std::string& syspath)
  {
    _syspath_map.erase(syspath);
  }
} /* namespace usbguard */
#endif /* HAVE_UDEV */

/* vim: set ts=2 sw=2 et */
