#include <build-config.h>

#include "Daemon.hpp"
#include "LinuxDeviceManager.hpp"

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

    try {
      _dm = new LinuxDeviceManager(*this);
      initIPC();
    } catch(...) {
      qb_loop_destroy(_qb_loop);
      G_qb_loop = nullptr;
      throw;
    }

    if (qb_loop_signal_add(_qb_loop, QB_LOOP_HIGH, SIGINT, _qb_loop, Daemon::qbSignalHandlerFn, NULL) != 0 ||
	qb_loop_signal_add(_qb_loop, QB_LOOP_HIGH, SIGTERM, _qb_loop, Daemon::qbSignalHandlerFn, NULL) != 0) {
      log->debug("Cannot register signal handlers.");
      throw std::runtime_error("signal init error");
    }

    return;
  }

  Daemon::~Daemon()
  {
    finiIPC();
    delete _dm;
    _config.close();
    qb_loop_destroy(_qb_loop);
    G_qb_loop = nullptr;
    return;
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
    _dm->start();
    qb_loop_run(_qb_loop);
    return;
  }

  void Daemon::quit()
  {
    qb_loop_stop(_qb_loop);
    return;
  }

  uint32_t Daemon::assignSeqn()
  {
    return _ruleset.assignSeqn();
  }

  /*
   * IPC service methods
   */
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
  
  void Daemon::allowDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Allowing device: {}", seqn);
    Pointer<Rule> rule;
    if (append) {
      rule = appendDeviceRule(seqn, Rule::Target::Allow, timeout_sec);
    }
    else {
      rule = makePointer<Rule>();
    }
    allowDevice(seqn, rule);
    return;
  }

  void Daemon::blockDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Blocking device: {}", seqn);
    Pointer<Rule> rule;
    if (append) {
      rule = appendDeviceRule(seqn, Rule::Target::Block, timeout_sec);
    }
    else {
      rule = makePointer<Rule>();
    }
    blockDevice(seqn, rule);
    return;
  }

  void Daemon::rejectDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Rejecting device: {}", seqn);
    Pointer<Rule> rule;
    if (append) {
      rule = appendDeviceRule(seqn, Rule::Target::Reject, timeout_sec);
    }
    else {
      rule = makePointer<Rule>();
    }
    rejectDevice(seqn, rule);
    return;
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

  void Daemon::dmDeviceInserted(Pointer<Device> device)
  {
    Pointer<Rule> device_rule = device->getDeviceRule();
    Pointer<const Rule> matched_rule = _ruleset.getFirstMatchingRule(device_rule);

    DeviceInserted(device_rule->getSeqn(),
		   device_rule->getDeviceName(),
		   device_rule->getDeviceName(), /* FIXME */
		   device_rule->getVendorID(),
		   device_rule->getProductID(),
		   matched_rule->isImplicit() ? false : true,
		   matched_rule->getSeqn());

    switch(matched_rule->getTarget()) {
    case Rule::Target::Allow:
      allowDevice(device_rule->getSeqn(), matched_rule);
      break;
    case Rule::Target::Block:
      blockDevice(device_rule->getSeqn(), matched_rule);
      break;
    case Rule::Target::Reject:
      rejectDevice(device_rule->getSeqn(), matched_rule);
      break;
    default:
      throw std::runtime_error("BUG: Wrong matched_rule target");
    }

    return;
  }

  void Daemon::dmDeviceRemoved(Pointer<Device> device)
  {
    Pointer<Rule> device_rule = device->getDeviceRule();
    DeviceRemoved(device_rule->getSeqn(),
		  device_rule->getDeviceName(),
		  device_rule->getDeviceName(), /* FIXME */
		  device_rule->getVendorID(),
		  device_rule->getProductID());
    return;
  }

  void Daemon::dmDeviceAllowed(Pointer<Device> device)
  {
    return;
  }

  void Daemon::dmDeviceBlocked(Pointer<Device> device)
  {
    return;
  }

  void Daemon::dmDeviceRejected(Pointer<Device> device)
  {
    return;
  }

  int32_t Daemon::qbSignalHandlerFn(int32_t signal, void *arg)
  {
    qb_loop_t *qb_loop = (qb_loop_t *)arg;
    log->debug("Stopping main loop from signal handler");
    qb_loop_stop(qb_loop);
    return QB_FALSE;
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

  void Daemon::allowDevice(uint32_t seqn, Pointer<const Rule> matched_rule)
  {
    Pointer<Device> device = _dm->allowDevice(seqn);
    Pointer<Rule> device_rule = device->getDeviceRule();
    DeviceAllowed(device_rule->getSeqn(),
		  device_rule->getDeviceName(),
		  device_rule->getDeviceName(), /* FIXME */
		  device_rule->getVendorID(),
		  device_rule->getProductID(),
		  (matched_rule->getSeqn() != Rule::SeqnDefault),
		  matched_rule->getSeqn());
    return;
  }

  void Daemon::blockDevice(uint32_t seqn, Pointer<const Rule> matched_rule)
  {
    Pointer<Device> device = _dm->blockDevice(seqn);
    Pointer<Rule> device_rule = device->getDeviceRule();
    DeviceBlocked(device_rule->getSeqn(),
		  device_rule->getDeviceName(),
		  device_rule->getDeviceName(), /* FIXME */
		  device_rule->getVendorID(),
		  device_rule->getProductID(),
		  (matched_rule->getSeqn() != Rule::SeqnDefault),
		  matched_rule->getSeqn());
    return;
  }

  void Daemon::rejectDevice(uint32_t seqn, Pointer<const Rule> matched_rule)
  {
    Pointer<Device> device = _dm->rejectDevice(seqn);
    Pointer<Rule> device_rule = device->getDeviceRule();
    DeviceRejected(device_rule->getSeqn(),
		   device_rule->getDeviceName(),
		   device_rule->getDeviceName(), /* FIXME */
		   device_rule->getVendorID(),
		   device_rule->getProductID(),
		   (matched_rule->getSeqn() != Rule::SeqnDefault),
		   matched_rule->getSeqn());
    return;
  }

  Pointer<Rule> Daemon::appendDeviceRule(uint32_t seqn, Rule::Target taget, uint32_t timeout_sec)
  {
    return nullptr; /* FIXME */
  }

} /* namespace usbguard */
