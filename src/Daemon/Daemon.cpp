#include <build-config.h>

#include "Daemon.hpp"

#include "Common/Logging.hpp"
#include "Common/Utility.hpp"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace usbguard
{
  qb_loop_t *G_qb_loop = nullptr;

  Daemon::Daemon()
  {
    G_qb_loop = _qb_loop = qb_loop_create();

    if (!_qb_loop) {
      throw std::runtime_error("qb init error");
    }

    _udev = udev_new();

    if (!_udev) {
      log->debug("Cannot initialize the udev subsystem. udev_new() failed.");
      log->trace("Returning from udev thread");
      throw std::runtime_error("udev init error");
    }

    _umon = udev_monitor_new_from_netlink(_udev, "udev");

    if (!_umon) {
      log->debug("Cannot initialize the udev monitor.");
      log->trace("Returning from udev thread");
      throw std::runtime_error("udev/umon init error");
    }

    udev_monitor_filter_add_match_subsystem_devtype(_umon, "usb", "usb_device");
    udev_monitor_enable_receiving(_umon);
    int umon_fd = udev_monitor_get_fd(_umon);

    if (qb_loop_poll_add(_qb_loop, QB_LOOP_MED, umon_fd, POLLIN, this, Daemon::qbUDevEventFn) != 0) {
      log->debug("Cannot register UDev event fd poll callback");
      throw std::runtime_error("udev/qb init error");
    }

    if (qb_loop_signal_add(_qb_loop, QB_LOOP_HIGH, SIGINT, _qb_loop, Daemon::qbSignalHandlerFn, NULL) != 0 ||
	qb_loop_signal_add(_qb_loop, QB_LOOP_HIGH, SIGTERM, _qb_loop, Daemon::qbSignalHandlerFn, NULL) != 0) {
      log->debug("Cannot register signal handlers.");
      throw std::runtime_error("signal init error");
    }

    initIPC();
    sysioSetAuthorizedDefault(false);

    return;
  }

  int32_t Daemon::qbSignalHandlerFn(int32_t signal, void *arg)
  {
    qb_loop_t *qb_loop = (qb_loop_t *)arg;
    log->debug("Stopping main loop from signal handler");
    qb_loop_stop(qb_loop);
    return QB_FALSE;
  }

  int32_t Daemon::qbUDevEventFn(int32_t fd, int32_t revents, void *arg)
  {
    Daemon *instance = (Daemon *)arg;
    if (revents & POLLIN) {
      instance->udevHandleEvent();
    }
    return QB_TRUE;
  }

  Daemon::~Daemon()
  {
    sysioSetAuthorizedDefault(true); /* FIXME: Set to pre-start state, not "true" */
    finiIPC();
    udev_monitor_unref(_umon);
    udev_unref(_udev);
    _config.close();
    qb_loop_destroy(_qb_loop);
    G_qb_loop = nullptr;
  }

  void Daemon::loadConfiguration(const String& path)
  {
    log->debug("Loading configuration from {}", path);
    _config.open(path);

    if (_config.hasSettingValue("RuleFile")) {
      log->debug("Setting rules file path from configuration file");
      const String& rule_file = _config.getSettingValue("RuleFile");
      loadRules(rule_file);
    } else {
      log->debug("No rules file path specified.");
    }

    log->debug("Configuration loaded successfully");
    return;
  }

  void Daemon::loadRules(const String& path)
  {
    _ruleset.load(path);
    return;
  }

  void Daemon::run()
  {
    qb_loop_run(_qb_loop);
  }

  void Daemon::quit()
  {
    qb_loop_stop(_qb_loop);
  }

  void Daemon::udevHandleEvent()
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

    const std::string action(action_cstr);

    if (action == "add") {
      processDeviceInsertion(dev);
    }
    else if (action == "remove") {
      processDeviceRemoval(dev);
    }
    else {
      log->warn("BUG? Unknown device action value \"{}\"", action);
    }
    /* Unref the udev device */
    udev_device_unref(dev);

    return;
  }

  Pointer<const Rule> Daemon::syncDeviceRule(Pointer<Rule> device_rule)
  {
    auto matching_rule = _ruleset.getFirstMatchingRule(device_rule);

    if (matching_rule->getTarget() != device_rule->getTarget()) {
      /*
       * Sync the device_rule
       */
      device_rule->setTarget(matching_rule->getTarget());
      device_rule->setAction(matching_rule->getAction());
      return matching_rule;
    } else {
      return nullptr;
    }
  }

  void Daemon::evalDeviceRule(Pointer<Rule> device_rule,
			      Pointer<const Rule> matching_rule)
  {
    /* Sync /sys device state */
    sysioSyncState(device_rule);

    /* TODO: execute action */

    /*
     * Send an IPC signal announcing which target was selected
     * for the device
     */
    switch(device_rule->getTarget())
      {
      case Rule::Target::Allow:
	DeviceAllowed(device_rule->getSeqn(),
		      device_rule->getDeviceName(),
		      device_rule->getDeviceName(), /* FIXME */
		      device_rule->getVendorID(),
		      device_rule->getProductID(),
		      (matching_rule->getSeqn() != Rule::SeqnDefault),
		      matching_rule->getSeqn());
	break;
      case Rule::Target::Block:
	DeviceBlocked(device_rule->getSeqn(),
		      device_rule->getDeviceName(),
		      device_rule->getDeviceName(), /* FIXME */
		      device_rule->getVendorID(),
		      device_rule->getProductID(),
		      (matching_rule->getSeqn() != Rule::SeqnDefault),
		      matching_rule->getSeqn());
	break;
      case Rule::Target::Reject:
	DeviceRejected(device_rule->getSeqn(),
		       device_rule->getDeviceName(),
		       device_rule->getDeviceName(), /* FIXME */
		       device_rule->getVendorID(),
		       device_rule->getProductID(),
		       (matching_rule->getSeqn() != Rule::SeqnDefault),
		       matching_rule->getSeqn());
	break;
      default:
	throw std::runtime_error("BUG: Unknown target");
      }
    return;
  }

  void Daemon::processDeviceInsertion(struct udev_device *device)
  {
    /* First create a device specific rule. The sequence number for
     * this rule is not yet assigned. The target is set to Unknown. */
    auto device_rule = makePointer<Rule>(UDevDeviceToDeviceRule(device));

    /* Register the device in device map */
    dmAddDeviceRule(device_rule);

    /* Find a matching firewall rule (if no match is found, a default
       rule is returned with seqn set to DefaultSeqn */
    auto matching_rule = syncDeviceRule(device_rule);

    /* At the insertion phase, the syncDeviceRule method cannot return
       a nullptr because the state of the device will always change from
       the default Unknown to one of Allowed, Blocked or Rejected */
    if (matching_rule == nullptr) {
      throw std::runtime_error("BUG: matching_rule is NULL after sync on device insert");
    }

    /* Send an IPC signal announcing that a device was added */
    DeviceInserted(device_rule->getSeqn(),
		   device_rule->getDeviceName(),
		   device_rule->getDeviceName(), /* FIXME */
		   device_rule->getVendorID(),
		   device_rule->getProductID(),
		   (matching_rule->getSeqn() != Rule::SeqnDefault),
		   matching_rule->getSeqn());

    /* Now we can evaluate the target and action, which will set
       the target state of the device */
    evalDeviceRule(device_rule, matching_rule);

    return;
  }

  void Daemon::sysioSyncState(Pointer<Rule> device_rule)
  {
    const char *target_file = nullptr;
    int target_value = 0;

    switch (device_rule->getTarget())
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
	/* FIXME: potential deadlock */
      }

#if 0
    char sysio_path[SYSIO_PATH_MAX];
    snprintf(sysio_path, SYSIO_PATH_MAX, "%s/%s",
	     device_rule->ref_syspath.c_str(), target_file);
    /* FIXME: check that snprintf wrote the whole path */
    sysIOWrite(sysio_path, target_value);
    // FIXME: sysio
#endif

    return;
  }

  void Daemon::processDeviceRemoval(struct udev_device *device)
  {
    const char *syspath = udev_device_get_syspath(device);

    if (!syspath) {
      log->error("The device to be removed doesn't have a syspath assigned!");
      return;
    }

    auto device_rule = dmGetDeviceRuleByPath(syspath);

    if (device_rule == nullptr) {
      log->warn("Ignoring removal of an unknown device: sysdev={}", udev_device_get_syspath(device));
      return;
    }

    dmRemoveDeviceRule(device_rule);

    DeviceRemoved(device_rule->getSeqn(),
		  device_rule->getDeviceName(),
		  device_rule->getDeviceName(), /* FIXME */
		  device_rule->getVendorID(),
		  device_rule->getProductID());
    return;
  }

  unsigned int Daemon::dmAddDeviceRule(Pointer<Rule> rule)
  {
    const uint32_t seqn = _ruleset.assignSeqn(rule);
    _device_map[seqn] = rule;
    return seqn;
  }

  void Daemon::dmRemoveDeviceRule(Pointer<const Rule> rule)
  {
    _device_map.erase(rule->getSeqn());
    return;
  }

  static ssize_t sysioWriteFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen)
  {
    const int fd = openat(dirfd(dirfp), relpath.c_str(), O_WRONLY);

    if (fd < 0) {
      log->debug("Cannot open {}: {}", relpath, errno);
      return -1;
    }

    errno = 0;
    const ssize_t bytes_written = write(fd, buffer, buflen);
    log->debug("Wrote {} bytes out of {}: errno {}", bytes_written, buflen, errno);
    close(fd);

    return bytes_written;
  }

  static ssize_t sysioReadFileAt(DIR* dirfp, const std::string& relpath, char *buffer, size_t buflen)
  {
    const int fd = openat(dirfd(dirfp), relpath.c_str(), O_RDONLY);

    if (fd < 0) {
      log->debug("Cannot open {}: {}", relpath, errno);
      return -1;
    }

    errno = 0;
    const ssize_t bytes_read = read(fd, buffer, buflen);
    log->debug("Read {} bytes out of {}: errno: {}", bytes_read, buflen, errno);
    close(fd);

    return bytes_read;
  }

  void Daemon::sysioSetAuthorizedDefault(bool state)
  {
    log->debug("Setting default authorized flag values on all USB controllers to {}", state);
    const char * const dir = "/sys/bus/usb/devices/";
    DIR *dirfp = opendir(dir);
    struct dirent *dent;

    if (!dirfp) {
      throw std::runtime_error("Cannot open USB devices /sys directory");
    }

    for (dent = readdir(dirfp); dent != nullptr; dent = readdir(dirfp)) {
      char buffer[3] = { 0, 0, 0 };
      const std::string devpath(dent->d_name);

      if (devpath.compare(0, 3, "usb") != 0) {
	log->debug("Skipping device {}: does not start with 'usb'", devpath);
	continue;
      }

      /* bDeviceClass */
      if (sysioReadFileAt(dirfp, devpath + "/bDeviceClass", buffer, sizeof buffer) < 2) {
	log->debug("Skipping device {}: cannot read device class", devpath);
	continue;
      }
      if (!(buffer[0] == '0' && buffer[1] == '9')) {
	log->debug("Skipping device {}: wrong class value: {}{}", devpath, buffer[0], buffer[1]);
	continue;
      }
      /* bDeviceSubClass */
      if (sysioReadFileAt(dirfp, devpath + "/bDeviceSubClass", buffer, sizeof buffer) < 2) {
	log->debug("Skipping device {}: cannot read device subclass", devpath);
	continue;
      }
      if (!(buffer[0] == '0' && buffer[1] == '0')) {
	log->debug("Skipping device {}: wrong subclass value: {}{}", devpath, buffer[0], buffer[1]);
	continue;
      }

      buffer[0] = state ? '1' : '0';

      if (sysioWriteFileAt(dirfp, devpath + "/authorized_default",
			   buffer, 1) != 1) {
	log->debug("Cannot set default authorized state for device {}", devpath);
	continue;
      }
    }

    closedir(dirfp);
    return;
  }

  Pointer<const Rule> Daemon::dmGetDeviceRuleBySeqn(unsigned int seqn) const
  {
    auto it = _device_map.find(seqn);
    if (it == _device_map.end()) {
      return nullptr;
    }
    else {
      return it->second;
    }
  }

  Pointer<Rule> Daemon::dmGetDeviceRuleBySeqnMutable(unsigned int seqn)
  {
    auto it = _device_map.find(seqn);
    if (it == _device_map.end()) {
      return nullptr;
    }
    else {
      return it->second;
    }    
  }

  Pointer<const Rule> Daemon::dmGetDeviceRuleByPath(const String& syspath)
  {
    for (auto it = _device_map.begin(); it != _device_map.end(); ++it) {
      //if (it->second->ref_syspath == syspath) {
      //return it->second;
      //} FIXME
    }
    return nullptr;
  }

  void Daemon::initIPC()
  {
    static struct qb_ipcs_service_handlers service_handlers = {
      Daemon::qbIPCConnectionAcceptFn,
      Daemon::qbIPCConnectionCreatedFn,
      Daemon::qbIPCMessageProcessFn,
      Daemon::qbIPCConnectionClosedFn,
      Daemon::qbIPCConnectionDestroyedFn
    };

    _qb_service = qb_ipcs_create("usbguard", 0,
				 QB_IPC_NATIVE, &service_handlers);

    if (_qb_service == nullptr) {
      throw std::runtime_error("Cannot create qb_service object");
    }

    qb_ipcs_service_context_set(_qb_service, this);

    static struct qb_ipcs_poll_handlers poll_handlers = {
      Daemon::qbIPCJobAdd,
      Daemon::qbIPCDispatchAdd,
      Daemon::qbIPCDispatchMod,
      Daemon::qbIPCDispatchDel
    };

    qb_ipcs_poll_handlers_set(_qb_service, &poll_handlers);

    auto rc = qb_ipcs_run(_qb_service);
    if (rc != 0) {
      log->error("Cannot start the IPC server: qb_ipcs_run failed: {}", strerror((int)-rc));
      throw std::runtime_error("IPC server error");
    }

    return;
  }

  void Daemon::finiIPC()
  {
    qb_ipcs_destroy(_qb_service);
    _qb_service = nullptr;
    return;
  }

  int32_t Daemon::qbIPCConnectionAcceptFn(qb_ipcs_connection_t *conn, uid_t uid, gid_t gid)
  {
    log->debug("Connection accept");
    return 0;
  }

  void Daemon::qbIPCConnectionCreatedFn(qb_ipcs_connection_t *conn)
  {
    log->debug("Connection created");
  }

  void Daemon::qbIPCConnectionDestroyedFn(qb_ipcs_connection_t *conn)
  {
    log->debug("Connection destroyed");
  }

  int32_t Daemon::qbIPCConnectionClosedFn(qb_ipcs_connection_t *conn)
  {
    log->debug("Connection closed");
    return 0;
  }

  json Daemon::processJSON(const json& jobj)
  {
    log->debug("Processing JSON object: {}", jobj);

    if (!jobj["_e"].is_null()) {
      //processExceptionJSON(jobj);
    }
    else if (!jobj["_s"].is_null()) {
      //processSignalJSON(jobj);
    }
    else if (!jobj["_r"].is_null()) {
      //processMethodReturnJSON(jobj);
    }
    else if (!jobj["_m"].is_null()) {
      return processMethodCallJSON(jobj);
    }
    else {
      throw IPCException(IPCException::ProtocolError, "Unknown message");
    }
    return std::move(json());
  }

  json Daemon::processMethodCallJSON(const json& jobj)
  {
    log->debug("Processing method call");
    json retval = {{ "_i", jobj["_i"]}};

    try {
      const std::string name = jobj["_m"];
      log->debug("Method name = {}", name);

      if (name == "appendRule") {
	uint32_t val = appendRule(jobj["rule_spec"], jobj["parent_seqn"], jobj["timeout_sec"]);
	retval["retval"] = val;
      }
      else if (name == "removeRule") {
	removeRule(jobj["seqn"]);
      }
      else if (name == "allowDevice") {
	allowDevice(jobj["seqn"], jobj["append"], jobj["timeout_sec"]);
      }
      else if (name == "blockDevice") {
	blockDevice(jobj["seqn"], jobj["append"], jobj["timeout_sec"]);
      }
      else if (name == "rejectDevice") {
	rejectDevice(jobj["seqn"], jobj["append"], jobj["timeout_sec"]);
      }
      else {
	throw 0;
      }
      retval["_r"] = name;
    } catch(...) {
      throw IPCException(IPCException::ProtocolError, "Invalid IPC signal data");
    }

    log->debug("Returning JSON object: {}", retval);
    return std::move(retval);
  }

  int32_t Daemon::qbIPCMessageProcessFn(qb_ipcs_connection_t *conn, void *data, size_t size)
  {
    if (size <= sizeof (struct qb_ipc_request_header)) {
      log->error("Received invalid IPC data. Disconnecting from the client.");
      qb_ipcs_disconnect(conn);
      return 0;
    }

    const struct qb_ipc_request_header *hdr = \
      (const struct qb_ipc_request_header *)data;

    if (size != (size_t)hdr->size) {
      log->error("Invalid size in IPC header. Disconnecting from the client.");
      qb_ipcs_disconnect(conn);
      return 0;
    }
    if (size > 1<<20) {
      log->error("Message too large. Disconnecting from the client.");
      qb_ipcs_disconnect(conn);
      return 0;
    }

    try {
      const char *jdata = (char *)data + sizeof(struct qb_ipc_request_header);
      const size_t jsize = size - sizeof(struct qb_ipc_request_header);
      const std::string json_string((const char *)jdata, jsize);
      const json jobj = json::parse(json_string);

      log->debug("Received JSON object: {}", jobj.dump());

      Daemon* daemon = \
	static_cast<Daemon*>(qb_ipcs_connection_service_context_get(conn));

      const json retval = daemon->processJSON(jobj);

      if (!retval.is_null()) {
	qbIPCSendJSON(conn, retval);
      }
    } catch(...) {
      log->error("Invalid JSON object received. Disconnecting from the client.");
      qb_ipcs_disconnect(conn);
      return 0;
    }

    return 0;
  }

  void Daemon::qbIPCSendJSON(qb_ipcs_connection_t *qb_conn, const json& jobj)
  {
    const std::string s = jobj.dump();
    struct qb_ipc_response_header hdr;
    struct iovec iov[2];

    hdr.id = 0;
    hdr.size = sizeof hdr + s.size();
    hdr.error = 0;

    iov[0].iov_base = &hdr;
    iov[0].iov_len = sizeof hdr;
    iov[1].iov_base = (void *)s.c_str();
    iov[1].iov_len = s.size();

    const size_t total_size = hdr.size;
    const ssize_t rc = qb_ipcs_event_sendv(qb_conn, iov, 2);

    if (rc < 0) {
      /* FIXME: There's no client identification value in the message */
      log->warn("Failed to send data: {}", strerror((int)-rc));
    }
    else if ((size_t)rc != total_size) {
      /* FIXME: There's no client identification value in the message */
      log->warn("Sent less data than expected. Expected {}, send {}.",
		total_size, rc);
    }

    return;
  }

  int32_t Daemon::qbIPCJobAdd(enum qb_loop_priority p, void *data, qb_loop_job_dispatch_fn fn)
  {
    return qb_loop_job_add(G_qb_loop, p, data, fn);
  }

  int32_t Daemon::qbIPCDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts,
				   void *data, qb_ipcs_dispatch_fn_t fn)
  {
    return qb_loop_poll_add(G_qb_loop, p, fd, evts, data, fn);
  }

  int32_t Daemon::qbIPCDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts,
				   void *data, qb_ipcs_dispatch_fn_t fn)
  {
    return qb_loop_poll_mod(G_qb_loop, p, fd, evts, data, fn);
  }

  int32_t Daemon::qbIPCDispatchDel(int32_t fd)
  {
    return qb_loop_poll_del(G_qb_loop, fd);
  }

  void Daemon::DeviceInserted(uint32_t seqn,
			      const std::string& name,
			      const std::string& usb_class,
			      const std::string& vendor_id,
			      const std::string& product_id,
			      bool rule_match,
			      uint32_t rule_seqn)
  {
    log->debug("DeviceInserted: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceInserted" },
      {       "seqn", seqn },
      {       "name", name },
      {  "usb_class", usb_class },
      {  "vendor_id", vendor_id },
      { "product_id", product_id },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DeviceRemoved(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id)
  {
    log->debug("DeviceRemoved: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}",
	       seqn, name, usb_class, vendor_id, product_id);

    const json j = {
      {         "_s", "DeviceRemoved" },
      {       "seqn", seqn },
      {       "name", name },
      {  "usb_class", usb_class },
      {  "vendor_id", vendor_id },
      { "product_id", product_id }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;    
  }

  void Daemon::DeviceAllowed(uint32_t seqn,
				const std::string& name,
				const std::string& usb_class,
				const std::string& vendor_id,
				const std::string& product_id,
				bool rule_match,
				uint32_t rule_seqn)
  {
    log->debug("DeviceAllowed: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceAllowed" },
      {       "seqn", seqn },
      {       "name", name },
      {  "usb_class", usb_class },
      {  "vendor_id", vendor_id },
      { "product_id", product_id },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DeviceBlocked(uint32_t seqn,
			     const std::string& name,
			     const std::string& usb_class,
			     const std::string& vendor_id,
			     const std::string& product_id,
			     bool rule_match,
			     uint32_t rule_seqn)
  {
    log->debug("DeviceBlocked: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceBlocked" },
      {       "seqn", seqn },
      {       "name", name },
      {  "usb_class", usb_class },
      {  "vendor_id", vendor_id },
      { "product_id", product_id },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DeviceRejected(uint32_t seqn,
				 const std::string& name,
				 const std::string& usb_class,
				 const std::string& vendor_id,
				 const std::string& product_id,
				 bool rule_match,
				 uint32_t rule_seqn)
  {
    log->debug("DeviceRejected: seqn={}, name={}, usb_class={}, "
	       "vendor_id={}, product_id={}, rule_match={}, rule_seqn={}",
	       seqn, name, usb_class, vendor_id, product_id, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceRejected" },
      {       "seqn", seqn },
      {       "name", name },
      {  "usb_class", usb_class },
      {  "vendor_id", vendor_id },
      { "product_id", product_id },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::qbIPCBroadcastData(const struct iovec *iov, size_t iov_len)
  {
    auto qb_conn = qb_ipcs_connection_first_get(_qb_service);
    size_t total_size = 0;

    for (size_t i = 0; i < iov_len; ++i) {
      total_size += iov[i].iov_len;
    }

    log->debug("Sending data of total size {}.", total_size);

    while (qb_conn != nullptr) {
      /* Send the data */
      ssize_t rc = qb_ipcs_event_sendv(qb_conn, iov, iov_len);

      if (rc < 0) {
	/* FIXME: There's no client identification value in the message */
	log->warn("Failed to send broadcast data to: {}", strerror((int)-rc));
      }
      else if ((size_t)rc != total_size) {
	/* FIXME: There's no client identification value in the message */
	log->warn("Sent less data than expected to. Expected {}, send {}.",
		  total_size, rc);
      }
      
      /* Get the next connection */
      auto qb_conn_next = qb_ipcs_connection_next_get(_qb_service, qb_conn);
      qb_ipcs_connection_unref(qb_conn);
      qb_conn = qb_conn_next;
    }

    return;
  }

  void Daemon::qbIPCBroadcastString(const std::string& s)
  {
    struct qb_ipc_response_header hdr;
    struct iovec iov[2];

    hdr.id = 0;
    hdr.size = sizeof hdr + s.size();
    hdr.error = 0;

    iov[0].iov_base = &hdr;
    iov[0].iov_len = sizeof hdr;
    iov[1].iov_base = (void *)s.c_str();
    iov[1].iov_len = s.size();

    qbIPCBroadcastData(iov, 2);
    return;
  }

  uint32_t Daemon::appendRule(const std::string& rule_spec,
			      uint32_t parent_seqn,
			      uint32_t timeout_sec)
  {
    (void)timeout_sec; /* TODO */
    const Rule rule = Rule::fromString(rule_spec);
    /* TODO: reevaluate the firewall rules for all active devices */
    log->debug("Appending rule: {}", rule_spec);
    return _ruleset.appendRule(rule, parent_seqn);
  }

  void Daemon::removeRule(uint32_t seqn)
  {
    log->debug("Removing rule: seqn={}", seqn);
    _ruleset.removeRule(seqn);
  }

  void Daemon::applyDevicePolicy(uint32_t seqn, Rule::Target target, bool append, uint32_t timeout_sec)
  {
    auto device_rule = dmGetDeviceRuleBySeqnMutable(seqn);

    if (device_rule == nullptr) {
      return;
    }

    if (append) {
      /* Make a copy of the device rule */
      Rule rule = *device_rule;
      /* Set target and reset seqn */
      rule.setTarget(target);
      rule.setSeqn(Rule::SeqnDefault);
      /* Append the rule, seqn will be assigned */
      _ruleset.appendRule(rule, Rule::SeqnLast);
      /* Sync the state of the device rule with firewall state */
      auto matching_rule = syncDeviceRule(device_rule);
    }
    else {
      device_rule->setTarget(target);
      /* 
       * NOTE: What to do with the seqn & matching rule fields here?
       *       They might be set from a previous evaluation...
       */
    }

    /* Evaluate the rule; This will sync the /sys state and send signals */
    evalDeviceRule(device_rule);
    return;
  }

  void Daemon::allowDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Allowing device: {}", seqn);
    applyDevicePolicy(seqn, Rule::Target::Allow, append, timeout_sec);
  }

  void Daemon::blockDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Blocking device: {}", seqn);
    applyDevicePolicy(seqn, Rule::Target::Block, append, timeout_sec);
  }

  void Daemon::rejectDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Rejecting device: {}", seqn);
    applyDevicePolicy(seqn, Rule::Target::Reject, append, timeout_sec);
  }

} /* namespace usbguard */
