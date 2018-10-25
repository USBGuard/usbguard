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

#if defined(HAVE_UMOCKDEV)
#include "UMockdevDeviceManager.hpp"
#include "UMockdevDeviceDefinition.hpp"

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
  UMockdevDevice::UMockdevDevice(UMockdevDeviceManager& device_manager, SysFSDevice& sysfs_device)
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
      throw ErrnoException("UMockdevDevice", sysfs_device.getPath(), errno);
    }

    initializeHash();
    USBDescriptorParser parser(*this);

    if ((descriptor_expected_size = parser.parse(descriptor_stream)) < sizeof(USBDeviceDescriptor)) {
      throw Exception("UMockdevDevice", sysfs_device.getPath(),
        "USB descriptor parser processed less data than the size of a USB device descriptor");
    }

    finalizeHash();
    /*
     * From now own we take ownership of the SysFSDevice instance.
     */
    _sysfs_device = std::move(sysfs_device);
  }

  SysFSDevice& UMockdevDevice::sysfsDevice()
  {
    return _sysfs_device;
  }

  const std::string& UMockdevDevice::getSysPath() const
  {
    return _sysfs_device.getPath();
  }

  bool UMockdevDevice::isController() const
  {
    if (getPort().substr(0, 3) != "usb" || getInterfaceTypes().size() != 1) {
      return false;
    }

    const USBInterfaceType hub_interface("09:00:*");
    return hub_interface.appliesTo(getInterfaceTypes()[0]);
  }

  std::string UMockdevDevice::getSystemName() const
  {
    return getSysPath();
  }

  void UMockdevDevice::parseUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor_raw,
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

  void UMockdevDevice::loadUSBDescriptor(USBDescriptorParser* parser, const USBDescriptor* descriptor)
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

  bool UMockdevDevice::isLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
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

  void UMockdevDevice::updateHashLinuxRootHubDeviceDescriptor(const USBDescriptor* const descriptor)
  {
    USBGUARD_LOG(Trace);
    USBDeviceDescriptor descriptor_modified = *reinterpret_cast<const USBDeviceDescriptor* const>(descriptor);
    descriptor_modified.bcdDevice = 0;
    updateHash(&descriptor_modified, sizeof descriptor_modified);
  }

  /*
   * Manager
   */
  UMockdevDeviceManager::UMockdevDeviceManager(DeviceManagerHooks& hooks)
    : DeviceManager(hooks),
      _thread(this, &UMockdevDeviceManager::thread),
      _enumeration(false)
  {
    umockdevInit();
    setDefaultBlockedState(/*state=*/true);
    setEnumerationOnlyMode(/*state=*/false);
    USBGUARD_SYSCALL_THROW("UEventDeviceManager", (_wakeup_fd = eventfd(0, 0)) < 0);
    _uevent_fd = ueventOpen();
  }

  void UMockdevDeviceManager::umockdevInit()
  {
    const char* const envval = ::getenv("USBGUARD_UMOCKDEV_DEVICEDIR");

    if (envval == nullptr) {
      throw Exception("UMockdevDeviceManager", "USBGUARD_UMOCKDEV_DEVICEDIR", "Environment variable not set.");
    }

    _umockdev_deviceroot = std::string(envval);
    _testbed.reset(umockdev_testbed_new());
    USBGUARD_LOG(Info) << "umockdev device directory set to " << _umockdev_deviceroot;
    const auto lambdaUMockdevFilterEntry = [](const std::string& fullpath, const struct dirent* dirent) {
      (void)dirent;
      struct stat st = {};

      if (::stat(fullpath.c_str(), &st) != 0) {
        USBGUARD_LOG(Warning) << "stat() failed: " << fullpath << ": Skipping file!";
        return std::string();
      }

      if (S_ISREG(st.st_mode)) {
        return fullpath;
      }
      else {
        return std::string();
      }
    };
    const auto lambdaUMockdevAddFromFile = [this](const std::string& fullpath, const std::string& loadpath) {
      (void)fullpath;

      for (const auto& device_path : umockdevLoadFromFile(loadpath)) {
        umockdevAdd(_sysfs_path_map.at(device_path));
      }

      return 1;
    };
    loadFiles(_umockdev_deviceroot, lambdaUMockdevFilterEntry, lambdaUMockdevAddFromFile);
    USBGUARD_SYSCALL_THROW("UMockdevDeviceManager", (_inotify_fd = inotify_init1(IN_NONBLOCK)) < 0);
    USBGUARD_SYSCALL_THROW("UMockdevDeviceManager", (inotify_add_watch(_inotify_fd, _umockdev_deviceroot.c_str(),
          IN_CREATE|IN_DELETE)) < 0);
  }

  void UMockdevDeviceManager::umockdevAdd(const std::shared_ptr<UMockdevDeviceDefinition>& definition)
  {
    if (access((SysFSDevice::getSysfsRoot() + definition->getSysfsPath()).c_str(), F_OK) == 0) {
      USBGUARD_LOG(Debug) << "Device already defined: " << definition->getSysfsPath();
      return;
    }

    umockdev_testbed_add_from_string(_testbed.get(), definition->getUMockdevDefinition().c_str(), /*error=*/nullptr);
    umockdev_testbed_uevent(_testbed.get(), (SysFSDevice::getSysfsRoot() + definition->getSysfsPath()).c_str(), "add");
  }

  void UMockdevDeviceManager::umockdevRemove(const std::shared_ptr<UMockdevDeviceDefinition>& definition)
  {
    USBGUARD_LOG(Trace) << "definition=" << definition->getUMockdevName() << " sysfs_path=" << definition->getSysfsPath();
    umockdevRemove(definition->getSysfsPath());
  }

  void UMockdevDeviceManager::umockdevRemove(const std::string& sysfs_path)
  {
    USBGUARD_LOG(Trace) << "sysfs_path=" << sysfs_path;

    for (const auto& child_device_path : umockdevGetChildrenBySysfsPath(sysfs_path)) {
      USBGUARD_LOG(Debug) << "Removing child device " << child_device_path;
      umockdevRemove(child_device_path);
    }

    umockdev_testbed_uevent(_testbed.get(), (SysFSDevice::getSysfsRoot() + sysfs_path).c_str(), "remove");
    umockdev_testbed_remove_device(_testbed.get(), (SysFSDevice::getSysfsRoot() + sysfs_path).c_str());
    //_sysfs_path_map.erase(sysfs_path);
  }

  std::vector<std::string> UMockdevDeviceManager::umockdevLoadFromFile(const std::string& definitions_path)
  {
    USBGUARD_LOG(Debug) << "Loading device definitions from " << definitions_path;
    std::vector<std::unique_ptr<UMockdevDeviceDefinition>> definitions;
    std::vector<std::string> device_paths;

    try {
      definitions = UMockdevDeviceDefinition::parseFromFile(definitions_path, /*sort_by_hierarchy=*/true);
    }
    catch (...) {
      USBGUARD_LOG(Warning) << "UMockdevDeviceManager: failed to parse " << definitions_path;
      throw;
    }

    for (auto&& definition : definitions) {
      if (_sysfs_path_map.count(definition->getSysfsPath()) > 0) {
        USBGUARD_LOG(Warning) << "UMockdevDeviceManager: " << definition->getSysfsPath() << ": device already defined, skipping.";
        continue;
      }

      std::shared_ptr<UMockdevDeviceDefinition> sharedptr_definition(std::move(definition));
      std::weak_ptr<UMockdevDeviceDefinition> weakptr_definition(sharedptr_definition);

      if (sharedptr_definition->getDeviceType() == "usb_device") {
        if (_sysfs_path_map.count(parentPath(sharedptr_definition->getSysfsPath())) < 1) {
          USBGUARD_LOG(Warning) << "UMockdevDeviceManager: " << sharedptr_definition->getSysfsPath() <<
            ": parent device not defined, skipping.";
          continue;
        }

        USBGUARD_LOG(Debug) << "Adding new USB device definition: " << sharedptr_definition->getSysfsPath();
      }
      else {
        USBGUARD_LOG(Debug) << "Adding new non-USB device definition: " << sharedptr_definition->getSysfsPath();
      }

      _sysfs_path_map.emplace(sharedptr_definition->getSysfsPath(), sharedptr_definition);
      _umockdev_file_map.emplace(sharedptr_definition->getUMockdevName(), weakptr_definition);
      device_paths.push_back(sharedptr_definition->getSysfsPath());
    }

    return device_paths;
  }

  std::vector<std::string> UMockdevDeviceManager::umockdevGetChildrenBySysfsPath(const std::string& sysfs_path)
  {
    std::vector<std::string> children;
    auto it = _sysfs_path_map.find(sysfs_path);

    if (it == _sysfs_path_map.cend()) {
      throw Exception("UMockdevDeviceManager", sysfs_path, "cannot list children for undefined device");
    }

    const auto child_component_count = countPathComponents(sysfs_path) + 1;
    ++it;

    while (it != _sysfs_path_map.cend()) {
      if (!hasPrefix(it->first, sysfs_path)) {
        break;
      }
      else {
        if (countPathComponents(it->first) == child_component_count) {
          children.push_back(it->first);
        }
      }

      ++it;
    }

    return children;
  }

  std::vector<std::string> UMockdevDeviceManager::umockdevRemoveByFile(const std::string& definitions_path)
  {
    USBGUARD_LOG(Trace) << "definitions_path=" << definitions_path;
    std::vector<std::string> device_paths;
    const std::string umockdev_name = filenameFromPath(definitions_path, /*include_extension=*/true);
    auto it = _umockdev_file_map.find(umockdev_name);

    while (it != _umockdev_file_map.end()) {
      if (it->first != umockdev_name) {
        break;
      }

      std::shared_ptr<UMockdevDeviceDefinition> definition = it->second.lock();
      USBGUARD_LOG(Debug) << "device_paths <- " << definition->getSysfsPath();
      device_paths.push_back(definition->getSysfsPath());
      it = _umockdev_file_map.erase(it);
    }

    const auto lambdaSysfsPathHierarchyCompare = [](const std::string& full_b,
    const std::string& full_a) {
      const std::size_t component_count_a = countPathComponents(full_a);
      const std::size_t component_count_b = countPathComponents(full_b);
      USBGUARD_LOG(Debug) << "c_c_a=" << component_count_a << " c_c_b=" << component_count_b;

      if (component_count_a < component_count_b) {
        return true;
      }
      else if (component_count_a > component_count_b) {
        return false;
      }

      const std::string base_a = filenameFromPath(full_a, /*include_extension=*/true);
      const std::string base_b = filenameFromPath(full_b, /*include_extension=*/true);
      const bool a_has_usb_prefix = hasPrefix("usb", base_a);
      const bool b_has_usb_prefix = hasPrefix("usb", base_b);
      USBGUARD_LOG(Debug) << "a_p=" << a_has_usb_prefix << " b_p=" << b_has_usb_prefix;

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

      return base_a < base_b;
    };
    std::sort(device_paths.begin(), device_paths.end(), lambdaSysfsPathHierarchyCompare);
    return device_paths;
  }

  /*
   * Set authorized=1 and add child devices.
   */
  void UMockdevDeviceManager::umockdevAuthorizeBySysfsPath(const std::string& sysfs_path)
  {
    /* set_attribute requires full device path */
    umockdev_testbed_set_attribute(_testbed.get(), (SysFSDevice::getSysfsRoot() + sysfs_path).c_str(), "authorized", "1");

    for (const auto& child_device_path : umockdevGetChildrenBySysfsPath(sysfs_path)) {
      USBGUARD_LOG(Debug) << "(authorize) Adding " << child_device_path;
      umockdevAdd(_sysfs_path_map.at(child_device_path));
    }
  }

  /*
   * Set authorized=0 and remove child devices.
   */
  void UMockdevDeviceManager::umockdevDeauthorizeBySysfsPath(const std::string& sysfs_path)
  {
    /* set_attribute requires full device path */
    umockdev_testbed_set_attribute(_testbed.get(), (SysFSDevice::getSysfsRoot() + sysfs_path).c_str(), "authorized", "0");

    for (const auto& child_device_path : umockdevGetChildrenBySysfsPath(sysfs_path)) {
      USBGUARD_LOG(Debug) << "(deauthorize) Removing " << child_device_path;
      umockdevRemove(child_device_path);
    }
  }

  void UMockdevDeviceManager::umockdevProcessInotify()
  {
    char buffer[sizeof (inotify_event) + NAME_MAX + 1] __attribute__((aligned(__alignof__(inotify_event))));

    if (read (_inotify_fd, buffer, sizeof buffer) <= 0) {
      USBGUARD_LOG(Warning) << "Inotify event read size mismatch";
      return;
    }

    const struct inotify_event* const event = reinterpret_cast<inotify_event*>(buffer);

    if (event->len <= 0 || event->len > NAME_MAX) {
      USBGUARD_LOG(Warning) << "Inotify event pathname size is out-of-range.";
      return;
    }

    std::string definitions_path(event->name, event->len);
    USBGUARD_LOG(Debug) << "inotify: definitions_path= " << definitions_path << " event_mask=" << event->mask;

    if (event->mask & IN_CREATE) {
      USBGUARD_LOG(Debug) << "inotify: IN_CREATE";

      for (const auto& sysfs_path : umockdevLoadFromFile(_umockdev_deviceroot + "/" + definitions_path)) {
        umockdevAdd(_sysfs_path_map.at(sysfs_path));
        //umockdev_testbed_uevent(_testbed.get(), sysfs_path.c_str(), "add");
      }
    }
    else if (event->mask & IN_DELETE) {
      USBGUARD_LOG(Debug) << "inotify: IN_DELETE";

      for (const auto& sysfs_path : umockdevRemoveByFile(_umockdev_deviceroot + "/" + definitions_path)) {
        umockdevRemove(_sysfs_path_map.at(sysfs_path));
        //umockdev_testbed_uevent(_testbed.get(), sysfs_path.c_str(), "remove");
      }
    }
    else {
      USBGUARD_LOG(Debug) << "inotify: Ignoring event.";
    }
  }

  UMockdevDeviceManager::~UMockdevDeviceManager()
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

  void UMockdevDeviceManager::setDefaultBlockedState(bool state)
  {
    _default_blocked_state = state;
  }

  void UMockdevDeviceManager::setEnumerationOnlyMode(bool state)
  {
    _enumeration_only_mode = state;
  }

  void UMockdevDeviceManager::start()
  {
    _thread.start();
  }

  void UMockdevDeviceManager::stop()
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

  void UMockdevDeviceManager::scan()
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
      throw Exception("UMockdevDeviceManager", "present devices", "failed to enumerate");
    }
  }

  void UMockdevDeviceManager::scan(const std::string& devpath)
  {
    USBGUARD_LOG(Trace) << "devpath=" << devpath;
  }

  std::shared_ptr<Device> UMockdevDeviceManager::applyDevicePolicy(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "id=" << id
      << " target=" << Rule::targetToString(target);
    std::shared_ptr<UMockdevDevice> device = std::static_pointer_cast<UMockdevDevice>(getDevice(id));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    sysfsApplyTarget(device->sysfsDevice(), target);
    device->setTarget(target);
    return std::move(device);
  }

  int UMockdevDeviceManager::ueventOpen()
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
    catch (...) {
      (void)close(socket_fd);
      throw;
    }

    return socket_fd;
  }

  void UMockdevDeviceManager::sysfsApplyTarget(SysFSDevice& sysfs_device, Rule::Target target)
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
    case Rule::Target::Empty:
    case Rule::Target::Invalid:
    default:
      throw std::runtime_error("Unknown rule target in applyDevicePolicy");
    }

    sysfs_device.setAttribute(name, value);
  }

  void UMockdevDeviceManager::thread()
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

        switch (select(max_fd + 1, &readset, NULL, NULL, &tv_timeout)) {
        case 1:
        case 2:
        case 3:
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

          break;

        case 0: /* Timeout */
          continue;

        case -1: /* Error */
        default:
          USBGUARD_LOG(Error) << "UMockdevDeviceManager thread: select failed: errno=" << errno;
          _thread.stop();
        }
      } /* Thread main loop */
    }
    catch (const Exception& ex) {
      USBGUARD_LOG(Error) << "UMockdevDeviceManager thread: " << ex.message();
    }

    USBGUARD_LOG(Trace) << "Leaving main loop.";
  }

  void UMockdevDeviceManager::ueventProcessRead()
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
        USBGUARD_LOG(Warning) << "Read: "
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

    /*
     * FIXME: This check needs an on/off switch
     */
    if (/*root*/!(cmsg_ucred->pid == 0 && cmsg_ucred->uid == 0 && cmsg_ucred->gid == 0) &&
      /*self*/!(cmsg_ucred->pid == getpid())) {
      // unknown origin -- ignore
      USBGUARD_LOG(Debug) << "received uevent of unknown origin: ignoring.";
      return;
    }

    /**/
    try {
      if (buffer.substr(0, strlen("libudev")) == "libudev") {
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

  void UMockdevDeviceManager::ueventProcessUEvent(const UEvent& uevent)
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

  bool UMockdevDeviceManager::ueventEnumerateComparePath(const std::pair<std::string, std::string>& a,
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

  int UMockdevDeviceManager::ueventEnumerateDevices()
  {
    USBGUARD_LOG(Trace);
    const auto lambdaEnumerateTriggerAndWaitForDevice = [this](const std::string& devpath, const std::string& buspath) {
      return ueventEnumerateTriggerAndWaitForDevice(devpath, buspath);
    };
    return loadFiles(SysFSDevice::getSysfsRoot() + "/bus/usb/devices",
        UMockdevDeviceManager::ueventEnumerateFilterDevice,
        lambdaEnumerateTriggerAndWaitForDevice,
        UMockdevDeviceManager::ueventEnumerateComparePath,
        /*rootdir_required=*/false);
  }

  std::string UMockdevDeviceManager::ueventEnumerateFilterDevice(const std::string& filepath, const struct dirent* direntry)
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

  int UMockdevDeviceManager::ueventEnumerateTriggerAndWaitForDevice(const std::string& devpath, const std::string& buspath)
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
        /* uevent required full device path */
        umockdev_testbed_uevent(_testbed.get(), sysfs_absolute_path.c_str(), "add");
        USBGUARD_LOG(Debug) << "Waiting for: " << sysfs_absolute_path;

        if (!_enumeration_complete.wait_for(lock, std::chrono::seconds(2),
        [this, sysfs_relative_path]() {
        uint32_t id = 0;
        const bool known = knownSysfsPath(sysfs_relative_path, &id);
          USBGUARD_LOG(Debug) << "cv: known=" << known << " id=" << id;
          return id != 0;
        })) {
          throw Exception("UMockdevDeviceManager", sysfs_absolute_path, "enumeration timeout");
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

  void UMockdevDeviceManager::processDevicePresence(const uint32_t id)
  {
    USBGUARD_LOG(Trace) << "id=" << id;

    try {
      std::shared_ptr<UMockdevDevice> device = \
        std::static_pointer_cast<UMockdevDevice>(DeviceManager::getDevice(id));
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

  void UMockdevDeviceManager::processDeviceInsertion(SysFSDevice& sysfs_device, const bool signal_present)
  {
    USBGUARD_LOG(Trace) << "sysfs_device=" << sysfs_device.getPath();

    try {
      std::shared_ptr<UMockdevDevice> device = std::make_shared<UMockdevDevice>(*this, sysfs_device);

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

  void UMockdevDeviceManager::insertDevice(std::shared_ptr<UMockdevDevice> device)
  {
    DeviceManager::insertDevice(std::static_pointer_cast<Device>(device));
    std::unique_lock<std::mutex> device_lock(device->refDeviceMutex());
    learnSysfsPath(device->getSysPath(), device->getID());
  }

  void UMockdevDeviceManager::processDeviceRemoval(const std::string& sysfs_devpath)
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

  std::shared_ptr<Device> UMockdevDeviceManager::removeDevice(const std::string& sysfs_path)
  {
    /*
     * FIXME: device map locking
     */
    if (!knownSysfsPath(sysfs_path)) {
      throw Exception("removeDevice", sysfs_path, "unknown syspath, cannot remove device");
    }

    std::shared_ptr<Device> device = DeviceManager::removeDevice(getIDFromSysfsPath(sysfs_path));
    forgetSysfsPath(sysfs_path);
    return device;
  }

  uint32_t UMockdevDeviceManager::getIDFromSysfsPath(const std::string& sysfs_path) const
  {
    uint32_t id = 0;

    if (knownSysfsPath(sysfs_path, &id)) {
      return id;
    }

    throw Exception("UMockdevDeviceManager", sysfs_path, "unknown sysfs path");
  }

  bool UMockdevDeviceManager::isPresentSysfsPath(const std::string& sysfs_path) const
  {
    uint32_t id = 0;

    if (knownSysfsPath(sysfs_path, &id)) {
      return 0 == id;
    }

    return false;
  }

  bool UMockdevDeviceManager::knownSysfsPath(const std::string& sysfs_path, uint32_t* id_ptr) const
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

  void UMockdevDeviceManager::learnSysfsPath(const std::string& sysfs_path, uint32_t id)
  {
    USBGUARD_LOG(Trace) << "Learn sysfs_path=" << sysfs_path << " size=" << sysfs_path.size() << " id=" << id;
    _sysfs_path_to_id_map[sysfs_path] = id;
  }

  void UMockdevDeviceManager::forgetSysfsPath(const std::string& sysfs_path)
  {
    USBGUARD_LOG(Trace) << "Forget sysfs_path=" << sysfs_path;
    _sysfs_path_to_id_map.erase(sysfs_path);
  }
} /* namespace usbguard */
#endif /* HAVE_UMOCKDEV */

/* vim: set ts=2 sw=2 et */
