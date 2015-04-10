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

#include <grp.h>
#include <pwd.h>

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

    _ipc_dac_acl = false;
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

    /* RuleFile */
    if (_config.hasSettingValue("RuleFile")) {
      log->debug("Setting rules file path from configuration file");
      const String& rule_file = _config.getSettingValue("RuleFile");
      try {
	loadRules(rule_file);
      } catch(const std::exception& ex) {
	log->warn("The configured rule file doesn't yet exists. Starting with an empty rule set.");
      }
    } else {
      log->debug("No rules file path specified.");
    }

    /* IPCAllowedUsers */
    if (_config.hasSettingValue("IPCAllowedUsers")) {
      log->debug("Setting allowed IPC users");
      StringVector usernames;
      tokenizeString(_config.getSettingValue("IPCAllowedUsers"),
		     usernames, " ", /*trim_empty=*/true);
      for (auto const& username : usernames) {
	log->debug("Allowed IPC user: {}", username);
	DACAddAllowedUID(username);
      }
      _ipc_dac_acl = true;
    }

    /* IPCAllowedGroups */
    if (_config.hasSettingValue("IPCAllowedGroups")) {
      log->debug("Setting allowed IPC groups");
      StringVector groupnames;
      tokenizeString(_config.getSettingValue("IPCAllowedGroups"),
		     groupnames, " ", /*trim_empty=*/true);
      for (auto const& groupname : groupnames) {
	log->debug("Allowed IPC group: {}", groupname);
	DACAddAllowedGID(groupname);
      }
      _ipc_dac_acl = true;
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
    const uint32_t seqn = _ruleset.appendRule(rule, parent_seqn);
    if (_config.hasSettingValue("RuleFile")) {
      _ruleset.save(_config.getSettingValue("RuleFile"));
    }
    return seqn;
  }

  void Daemon::removeRule(uint32_t seqn)
  {
    log->debug("Removing rule: seqn={}", seqn);
    _ruleset.removeRule(seqn);
    if (_config.hasSettingValue("RuleFile")) {
      _ruleset.save(_config.getSettingValue("RuleFile"));
    }
    return;
  }
  
  void Daemon::allowDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    log->debug("Allowing device: {}", seqn);
    Pointer<const Rule> rule;
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
    Pointer<const Rule> rule;
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
    Pointer<const Rule> rule;
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
			      const std::map<std::string,std::string>& attributes,
			      const std::vector<USBInterfaceType>& interfaces,
			      bool rule_match,
			      uint32_t rule_seqn)
  {
    log->debug("DeviceInserted: seqn={}, rule_match={}, rule_seqn={}",
	       seqn, rule_match, rule_seqn);

    json interfaces_json;
    for (auto const& type : interfaces) {
      interfaces_json.push_back(type.typeString());
    }

    const json j = {
      {         "_s", "DeviceInserted" },
      {       "seqn", seqn },
      { "attributes", attributes },
      { "interfaces", interfaces_json },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DevicePresent(uint32_t seqn,
			     const std::map<std::string,std::string>& attributes,
			     const std::vector<USBInterfaceType>& interfaces,
			     Rule::Target target)
  {
    log->debug("DeviceInserted: seqn={}, target={}", seqn, Rule::targetToString(target));

    json interfaces_json;
    for (auto const& type : interfaces) {
      interfaces_json.push_back(type.typeString());
    }

    const json j = {
      {         "_s", "DeviceInserted" },
      {       "seqn", seqn },
      { "attributes", attributes },
      { "interfaces", interfaces_json },
      {     "target", Rule::targetToString(target) },
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DeviceRemoved(uint32_t seqn,
			     const std::map<std::string,std::string>& attributes)

  {
    log->debug("DeviceRemoved: seqn={}", seqn);

    const json j = {
      {         "_s", "DeviceRemoved" },
      {       "seqn", seqn },
      { "attributes", attributes }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;    
  }

  void Daemon::DeviceAllowed(uint32_t seqn,
			     const std::map<std::string,std::string>& attributes,
			     bool rule_match,
			     uint32_t rule_seqn)
  {
    log->debug("DeviceAllowed: seqn={}, rule_match={}, rule_seqn={}",
	       seqn, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceAllowed" },
      {       "seqn", seqn },
      { "attributes", attributes },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DeviceBlocked(uint32_t seqn,
			     const std::map<std::string,std::string>& attributes,
			     bool rule_match,
			     uint32_t rule_seqn)
  {
    log->debug("DeviceBlocked: seqn={}, rule_match={}, rule_seqn={}",
	       seqn, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceBlocked" },
      {       "seqn", seqn },
      { "attributes", attributes },
      { "rule_match", rule_match },
      {  "rule_seqn", rule_seqn }
    };

    const std::string json_string = j.dump();
    qbIPCBroadcastString(json_string);
    return;
  }

  void Daemon::DeviceRejected(uint32_t seqn,
			      const std::map<std::string,std::string>& attributes,
			      bool rule_match,
			      uint32_t rule_seqn)
  {
    log->debug("DeviceRejected: seqn={}, rule_match={}, rule_seqn={}",
	       seqn, rule_match, rule_seqn);

    const json j = {
      {         "_s", "DeviceRejected" },
      {       "seqn", seqn },
      { "attributes", attributes },
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

    std::map<std::string,std::string> attributes;
    
    attributes["name"] = device_rule->getDeviceName();
    attributes["vendor_id"] = device_rule->getVendorID();
    attributes["product_id"] = device_rule->getProductID();
    attributes["serial"] = device_rule->getSerialNumber();
    attributes["hash"] = device_rule->getDeviceHash();

    DeviceInserted(device_rule->getSeqn(),
		   attributes,
		   device_rule->refInterfaceTypes(),
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

    std::map<std::string,std::string> attributes;
    
    attributes["name"] = device_rule->getDeviceName();
    attributes["vendor_id"] = device_rule->getVendorID();
    attributes["product_id"] = device_rule->getProductID();
    attributes["serial"] = device_rule->getSerialNumber();
    attributes["hash"] = device_rule->getDeviceHash();

    DeviceRemoved(device_rule->getSeqn(), attributes);
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
    Daemon* daemon = \
      static_cast<Daemon*>(qb_ipcs_connection_service_context_get(conn));

    const bool auth = daemon->qbIPCConnectionAllowed(uid, gid);

    if (auth) {
      log->debug("IPC Connection accepted. "
		 "Setting SHM permissions to uid={} gid={} mode=0660", uid, 0);
      qb_ipcs_connection_auth_set(conn, uid, 0, 0660);
      return 0;
    }
    else {
      log->debug("IPC Connection rejected");
      return -1;
    }
  }

  bool Daemon::qbIPCConnectionAllowed(uid_t uid, gid_t gid)
  {
    if (_ipc_dac_acl) {
      log->debug("Using DAC IPC ACL");
      log->debug("Connection request from uid={} gid={}", uid, gid);
      return DACAuthenticateIPCConnection(uid, gid);
    }
    else {
      log->debug("IPC authentication is turned off.");
      return true;
    }
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
    } catch(const std::exception& ex) {
      log->error("Exception: {}", ex.what());
      throw IPCException(IPCException::ProtocolError, "Invalid IPC method call");
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
    } catch(const std::exception& ex) {
      log->error("Exception: {}", ex.what());
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

    std::map<std::string,std::string> attributes;
    
    attributes["name"] = device_rule->getDeviceName();
    attributes["vendor_id"] = device_rule->getVendorID();
    attributes["product_id"] = device_rule->getProductID();

    DeviceAllowed(device_rule->getSeqn(),
		  attributes,
		  (matched_rule->getSeqn() != Rule::SeqnDefault),
		  matched_rule->getSeqn());
    return;
  }

  void Daemon::blockDevice(uint32_t seqn, Pointer<const Rule> matched_rule)
  {
    Pointer<Device> device = _dm->blockDevice(seqn);
    Pointer<Rule> device_rule = device->getDeviceRule();

    std::map<std::string,std::string> attributes;
    
    attributes["name"] = device_rule->getDeviceName();
    attributes["vendor_id"] = device_rule->getVendorID();
    attributes["product_id"] = device_rule->getProductID();

    DeviceBlocked(device_rule->getSeqn(),
		  attributes,
		  (matched_rule->getSeqn() != Rule::SeqnDefault),
		  matched_rule->getSeqn());
    return;
  }

  void Daemon::rejectDevice(uint32_t seqn, Pointer<const Rule> matched_rule)
  {
    Pointer<Device> device = _dm->rejectDevice(seqn);
    Pointer<Rule> device_rule = device->getDeviceRule();

    std::map<std::string,std::string> attributes;
    
    attributes["name"] = device_rule->getDeviceName();
    attributes["vendor_id"] = device_rule->getVendorID();
    attributes["product_id"] = device_rule->getProductID();

    DeviceRejected(device_rule->getSeqn(),
		   attributes,
		   (matched_rule->getSeqn() != Rule::SeqnDefault),
		   matched_rule->getSeqn());
    return;
  }

  Pointer<const Rule> Daemon::appendDeviceRule(uint32_t seqn, Rule::Target target, uint32_t timeout_sec)
  {
    Pointer<Device> device = _dm->getDevice(seqn);
    Pointer<Rule> device_rule = device->getDeviceRule();
    device_rule->setTarget(target);
    const String rule_string = device_rule->toString();
    log->debug("Appending rule: {}", rule_string);
    const uint32_t rule_seqn = appendRule(rule_string, Rule::SeqnLast, timeout_sec);
    log->debug("Rule seqn is: {}", rule_seqn);
    return _ruleset.getRule(rule_seqn);
  }

  bool Daemon::DACAuthenticateIPCConnection(uid_t uid, gid_t gid)
  {
    /* Check for UID match */
    for (auto allowed_uid : _ipc_allowed_uids) {
      if (allowed_uid == uid) {
	log->debug("uid {} is an allowed uid", uid);
	return true;
      }
    }

    /* Translate uid to username for group member matching */
    char pw_string_buffer[1024];
    struct passwd pw, *pwptr = nullptr;
    bool check_group_membership = true;

    if (getpwuid_r(uid, &pw,
		   pw_string_buffer, sizeof pw_string_buffer, &pwptr) != 0) {
      log->warn("Cannot lookup username for uid {}. Won't check group membership.", uid);
      check_group_membership = false;
    }

    /* Check for GID match or group member match */
    for (auto allowed_gid : _ipc_allowed_gids) {
      if (allowed_gid == gid) {
	log->debug("gid {} is an allowed gid", gid);
	return true;
      }
      else if (check_group_membership) {
	char gr_string_buffer[3072];
	struct group gr, *grptr = nullptr;

	/* Fetch list of current group members of group with a gid == allowed_gid */
	if (getgrgid_r(allowed_gid, &gr,
		       gr_string_buffer, sizeof gr_string_buffer, &grptr) != 0) {
	  log->warn("Cannot lookup groupname for gid {}. "
		    "Won't check group membership of uid {}", allowed_gid, uid);
	  continue;
	}

	/* Check for username match among group members */
	for (size_t i = 0; gr.gr_mem[i] != nullptr; ++i) {
	  if (strcmp(pw.pw_name, gr.gr_mem[i]) == 0) {
	    log->debug("uid {} ({}) is a member of an allowed group with gid {} ({})",
		       uid, pw.pw_name, allowed_gid, gr.gr_name);
	    return true;
	  }
	}
      }
    } /* allowed gid loop */

    return false;
  }

  void Daemon::DACAddAllowedUID(uid_t uid)
  {
    _ipc_allowed_uids.push_back(uid);
    return;
  }

  void Daemon::DACAddAllowedGID(gid_t gid)
  {
    _ipc_allowed_gids.push_back(gid);
    return;
  }

  void Daemon::DACAddAllowedUID(const String& username)
  {
    char string_buffer[4096];
    struct passwd pw, *pwptr = nullptr;

    if (getpwnam_r(username.c_str(), &pw,
		   string_buffer, sizeof string_buffer, &pwptr) != 0) {
      throw std::runtime_error("cannot lookup username");
    }

    DACAddAllowedUID(pw.pw_uid);
    return;
  }

  void Daemon::DACAddAllowedGID(const String& groupname)
  {
    char string_buffer[4096];
    struct group gr, *grptr = nullptr;

    if (getgrnam_r(groupname.c_str(), &gr,
		   string_buffer, sizeof string_buffer, &grptr) != 0) {
      throw std::runtime_error("cannot lookup groupname");
    }

    DACAddAllowedGID(gr.gr_gid);
    return;
  }

} /* namespace usbguard */
