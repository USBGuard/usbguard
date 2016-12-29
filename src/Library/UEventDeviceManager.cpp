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
#include <build-config.h>

#if defined(HAVE_UEVENT)
#include "UEventDeviceManager.hpp"
#include "UEventParser.hpp"
#include "SysFSDevice.hpp"
#include "Logger.hpp"
#include "Exception.hpp"
#include "USB.hpp"
#include "Base64.hpp"
#include "Common/FDInputStream.hpp"
#include "Common/Utility.hpp"

#include <stdexcept>
#include <fstream>

#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <linux/netlink.h>

namespace usbguard {
  UEventDevice::UEventDevice(UEventDeviceManager& device_manager, SysFSDevice& sysfs_device)
    : Device(device_manager)
  {
    /*
     * Look for the parent USB device and set the parent id
     * if we find one.
     */
    const String sysfs_parent_path(sysfs_device.getParentPath());
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
    setName(sysfs_device.readAttribute("product", /*strip_last_null=*/true));
    /*
     * Set USB ID
     */
    const String id_vendor(sysfs_device.readAttribute("idVendor", /*strip_last_null=*/true));
    const String id_product(sysfs_device.readAttribute("idProduct", /*strip_last_null=*/true));
    const USBDeviceID device_id(id_vendor, id_product);
    setDeviceID(device_id);
    /*
     * Set serial number
     */
    setSerial(sysfs_device.readAttribute("serial", /*strip_last_null=*/true));
    /*
     * Set USB port
     */
    setPort(sysfs_device.getName());
    /*
     * Sync authorization target
     */
    const String authorized_value(sysfs_device.readAttribute("authorized", /*strip_last_null=*/true));

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
      throw std::runtime_error("Cannot load USB descriptors: failed to open the descriptor data stream");
    }
    else {
      using namespace std::placeholders;
      USBDescriptorParser parser;

      auto load_device_descriptor = std::bind(&UEventDevice::loadDeviceDescriptor, this, _1, _2);
      auto load_configuration_descriptor = std::bind(&UEventDevice::loadConfigurationDescriptor, this, _1, _2);
      auto load_interface_descriptor = std::bind(&UEventDevice::loadInterfaceDescriptor, this, _1, _2);
      auto load_endpoint_descriptor = std::bind(&UEventDevice::loadEndpointDescriptor, this, _1, _2);

      parser.setHandler(USB_DESCRIPTOR_TYPE_DEVICE, sizeof (USBDeviceDescriptor),
                        USBParseDeviceDescriptor, load_device_descriptor);
      parser.setHandler(USB_DESCRIPTOR_TYPE_CONFIGURATION, sizeof (USBConfigurationDescriptor),
                        USBParseConfigurationDescriptor, load_configuration_descriptor);
      parser.setHandler(USB_DESCRIPTOR_TYPE_INTERFACE, sizeof (USBInterfaceDescriptor),
                        USBParseInterfaceDescriptor, load_interface_descriptor);
      parser.setHandler(USB_DESCRIPTOR_TYPE_ENDPOINT, sizeof (USBEndpointDescriptor),
                        USBParseEndpointDescriptor, load_endpoint_descriptor);
      parser.setHandler(USB_DESCRIPTOR_TYPE_ENDPOINT, sizeof (USBAudioEndpointDescriptor),
                        USBParseAudioEndpointDescriptor, load_endpoint_descriptor);

      if ((descriptor_expected_size = parser.parse(descriptor_stream)) < sizeof(USBDeviceDescriptor)) {
        throw std::runtime_error("Descriptor data parsing failed: parser processed less data than the size of a USB device descriptor");
      }
    }
    /*
     * Reset descriptor stream before before computing
     * the device hash.
     *
     * Because the eofbit is set, clear() has to be called
     * before seekg().
     */
    descriptor_stream.clear();
    descriptor_stream.seekg(0);

    /*
     * Compute and set the device hash.
     */
    updateHash(descriptor_stream, descriptor_expected_size);

    /*
     * From now own we take ownership of the SysFSDevice instance.
     */
    _sysfs_device = std::move(sysfs_device);
  }

  SysFSDevice& UEventDevice::sysfsDevice()
  {
    return _sysfs_device;
  }

  const String& UEventDevice::getSysPath() const
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

  /*
   * Manager
   */
  UEventDeviceManager::UEventDeviceManager(DeviceManagerHooks& hooks, const String& sysfs_root, bool dummy_mode)
    : DeviceManager(hooks),
      _thread(this, &UEventDeviceManager::thread),
      _uevent_fd(-1),
      _wakeup_fd(-1),
      _sysfs_root(sysfs_root),
      _enumeration_complete(false),
      _default_blocked_state(true),
      _dummy_mode(dummy_mode)
  {
    setDefaultBlockedState(/*state=*/true);

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
    _enumeration_complete = false;
    ueventEnumerateDevices();
    _enumeration_complete = true;
  }

  Pointer<Device> UEventDeviceManager::applyDevicePolicy(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "id=" << id
                        << " target=" << Rule::targetToString(target);

    Pointer<UEventDevice> device = std::static_pointer_cast<UEventDevice>(getDevice(id));
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

      struct sockaddr_nl sa = { 0 };
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

      struct sockaddr_un sa = { 0 };
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
    String name;
    String value("0");

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

      if (!_dummy_mode) {
      ueventEnumerateDevices(); /* scan() without thread state check */
      }
      else {
        ueventEnumerateDummyDevices();
      }

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
    String buffer(4096, 0);

    struct iovec iov[1];
    iov[0].iov_base = (void *)&buffer[0];
    iov[0].iov_len = buffer.capacity();

    struct sockaddr_nl peer_sockaddr = { 0 };

    union {
      struct cmsghdr header;
      uint8_t ucred[CMSG_SPACE(sizeof(struct ucred))];
    } cmsg_un;

    cmsg_un.header.cmsg_len = CMSG_LEN(sizeof(struct ucred));
    cmsg_un.header.cmsg_level = SOL_SOCKET;
    cmsg_un.header.cmsg_type = SCM_CREDENTIALS;

    struct msghdr msg_header = { 0 };
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
      (const struct ucred * const)CMSG_DATA(cmsg_header);

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
    const String subsystem = uevent.getAttribute("SUBSYSTEM");
    const String devtype = uevent.getAttribute("DEVTYPE");
    const String action = uevent.getAttribute("ACTION");

    if (subsystem != "usb" || devtype != "usb_device") {
      USBGUARD_LOG(Debug) << "Ignoring non-USB device:" 
                          << " subsystem=" << subsystem
                          << " devtype=" << devtype
                          << " action=" << action;
      return;
    }

    const String sysfs_devpath = _sysfs_root + uevent.getAttribute("DEVPATH");

    try {
      if (action == "add" || action == "change") {
        SysFSDevice sysfs_device(sysfs_devpath);

        /*
         * Do some additional sanity checking.
         */
        if (sysfs_device.getUEvent().hasAttribute("DEVTYPE")) {
          const String devtype = sysfs_device.getUEvent().getAttribute("DEVTYPE");
          if (devtype != "usb_device") {
            USBGUARD_LOG(Warning) << sysfs_devpath << ": UEvent DEVTYPE mismatch."
                                  << " Expected \"usb_device\", got \"" << devtype << "\"";
            return;
          }
        }

        processDeviceInsertion(sysfs_device);
      }
      else if (action == "remove") {
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
  }

  void UEventDeviceManager::ueventEnumerateDevices()
  {
    USBGUARD_LOG(Trace);
    loadFiles(_sysfs_root + "/bus/usb/devices",
      UEventDeviceManager::ueventEnumerateFilterDevice,
      UEventDeviceManager::ueventEnumerateTriggerDevice);
  }

  void UEventDeviceManager::ueventEnumerateDummyDevices()
  {
    USBGUARD_LOG(Trace);
    loadFiles(_sysfs_root + "/bus/usb/devices",
      UEventDeviceManager::ueventEnumerateFilterDevice,
      [&](const String& filepath)
      {
        UEvent uevent;
        uevent.setAttribute("SUBSYSTEM", "usb");
        uevent.setAttribute("DEVTYPE", "usb_device");
        uevent.setAttribute("ACTION", "add");
        uevent.setAttribute("DEVPATH", removePrefix(_sysfs_root, filepath));
        ueventProcessUEvent(uevent);
      });
  }

  String UEventDeviceManager::ueventEnumerateFilterDevice(const String& filepath, const struct dirent* direntry)
  {
#if defined(_DIRENT_HAVE_D_TYPE)
    if (direntry->d_type != DT_UNKNOWN) {
      switch(direntry->d_type) {
        case DT_LNK:
          return symlinkPath(filepath);
        case DT_DIR:
          return filepath;
        default:
          return String();
      }
    }
    else {
      /*
       * Unknown type. We have to call lstat.
       */
#endif
      struct stat st = { 0 };

      if (lstat(filepath.c_str(), &st) != 0) {
        /*
         * Cannot stat, skip this entry.
         */
        USBGUARD_LOG(Warning) << "lstat(" << filepath << "): errno=" << errno;
        return String();
      }

      if (S_ISLNK(st.st_mode)) {
        return symlinkPath(filepath, &st);
      }
      else if (S_ISDIR(st.st_mode)) {
        return filepath;
      }
      else {
        return String();
      }
#if defined(_DIRENT_HAVE_D_TYPE)
    }
#endif
    /* UNREACHABLE */
    return String();
  }

  void UEventDeviceManager::ueventEnumerateTriggerDevice(const String& filepath)
  {
    USBGUARD_LOG(Trace) << "filepath=" << filepath;
    try {
      std::ofstream uevent_stream(filepath + "/uevent");
      uevent_stream << "add";
    }
    catch(const Exception& ex) {
      USBGUARD_LOG(Warning) << "device enumeration exception: " << filepath << ": " << ex.message();
    }
    catch(const std::exception& ex) {
      USBGUARD_LOG(Warning) << "device enumeration exception: " << filepath << ": " << ex.what();
    }
  }

  void UEventDeviceManager::processDevicePresence(SysFSDevice& sysfs_device)
  {
    try {
      const uint32_t id = getIDFromSysPath(sysfs_device.getPath());
      Pointer<UEventDevice> device = \
        std::static_pointer_cast<UEventDevice>(DeviceManager::getDevice(id));
      device->sysfsDevice().reload();
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

  void UEventDeviceManager::processDeviceInsertion(SysFSDevice& sysfs_device)
  {
   /*
     * If we already know about a device at the sysfs path, redirect this
     * to device presence processing.
     */
    if (knownSysPath(sysfs_device.getPath())) {
      processDevicePresence(sysfs_device);
    }
    else {
      try {
        Pointer<UEventDevice> device = makePointer<UEventDevice>(*this, sysfs_device);
        insertDevice(device);
        /*
         * Signal insertions as presence if device enumeration hasn't
         * completed yet.
         */
        if (_enumeration_complete) {
          DeviceEvent(DeviceManager::EventType::Insert, device);
        }
        else {
          DeviceEvent(DeviceManager::EventType::Present, device);
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
       * Something went wrong and an exception was generated.
       * Either the device is malicious or the system lacks some
       * resources to successfully process the device. In either
       * case, we take the safe route and fallback to rejecting
       * the device.
       */
      USBGUARD_LOG(Warning) << "Rejecting device at syspath=" << sysfs_device.getPath();
      sysfsApplyTarget(sysfs_device, Rule::Target::Reject);
    }
  }

  void UEventDeviceManager::insertDevice(Pointer<UEventDevice> device)
  {
    DeviceManager::insertDevice(std::static_pointer_cast<Device>(device));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    _syspath_map.emplace(device->getSysPath(), device->getID());
  }

  void UEventDeviceManager::processDeviceRemoval(const String& sysfs_devpath)
  {
    USBGUARD_LOG(Trace) << "sysfs_devpath=" << sysfs_devpath;

    try {
      Pointer<Device> device = removeDevice(sysfs_devpath);
      DeviceEvent(DeviceManager::EventType::Remove, device);
    } catch(...) {
      /* Ignore for now */
      USBGUARD_LOG(Debug) << "Removal of an unknown device ignored.";
      return;
    }
  }

  Pointer<Device> UEventDeviceManager::removeDevice(const String& syspath)
  {
    /*
     * FIXME: device map locking
     */
    if (!knownSysPath(syspath)) {
      throw Exception("removeDevice", syspath, "unknown syspath, cannot remove device");
    }

    Pointer<Device> device = DeviceManager::removeDevice(getIDFromSysPath(syspath));
    _syspath_map.erase(syspath);

    return device;
  }

  uint32_t UEventDeviceManager::getIDFromSysPath(const String& syspath) const
  {
    auto it = _syspath_map.find(syspath);
    if (it == _syspath_map.end()) {
      throw Exception("UEventDeviceManager", syspath, "unknown syspath");
    }
    return it->second;
  }

  bool UEventDeviceManager::knownSysPath(const String& syspath) const
  {
    return _syspath_map.count(syspath) > 0;
  }
} /* namespace usbguard */
#endif /* HAVE_UDEV */
