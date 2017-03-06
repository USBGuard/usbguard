//
// Copyright (C) 2016 Red Hat, Inc.
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
#include "IPCServerPrivate.hpp"
#include "IPCPrivate.hpp"
#include "Logger.hpp"
#include "Exception.hpp"
#include "Common/Utility.hpp"

#include <sys/poll.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

namespace usbguard
{
  static qb_loop *G_qb_loop = nullptr;

  IPCServerPrivate::IPCServerPrivate(IPCServer& p_instance)
    : _p_instance(p_instance),
      _thread(this, &IPCServerPrivate::thread)
  {
    if (G_qb_loop != nullptr) {
      throw USBGUARD_BUG("Only one instance of IPCServer per process allowed");
    }

    G_qb_loop = _qb_loop = qb_loop_create();

    if (_qb_loop == nullptr) {
      throw Exception("IPC server initialization", "main loop", "Failed to create qb_loop object");
    }

    _wakeup_fd = -1;

    try {
      initIPC();
      USBGUARD_SYSCALL_THROW("IPC server initialization",
          (_wakeup_fd = eventfd(0, 0)) < 0);
      qb_loop_poll_add(_qb_loop, QB_LOOP_HIGH, _wakeup_fd,
                       POLLIN, nullptr,
                       &IPCServerPrivate::qbPollWakeupFn);
    }
    catch(...) {
      qb_loop_destroy(_qb_loop);
      G_qb_loop = nullptr;
      throw;
    }

    registerHandler<IPC::appendRule>(&IPCServerPrivate::handleAppendRule);
    registerHandler<IPC::removeRule>(&IPCServerPrivate::handleRemoveRule);
    registerHandler<IPC::listRules>(&IPCServerPrivate::handleListRules);
    registerHandler<IPC::applyDevicePolicy>(&IPCServerPrivate::handleApplyDevicePolicy);
    registerHandler<IPC::listDevices>(&IPCServerPrivate::handleListDevices);
    registerHandler<IPC::setParameter>(&IPCServerPrivate::handleSetParameter);
    registerHandler<IPC::getParameter>(&IPCServerPrivate::handleGetParameter);
  }

  void IPCServerPrivate::initIPC()
  {
    static struct qb_ipcs_service_handlers service_handlers = {
      IPCServerPrivate::qbIPCConnectionAcceptFn,
      IPCServerPrivate::qbIPCConnectionCreatedFn,
      IPCServerPrivate::qbIPCMessageProcessFn,
      IPCServerPrivate::qbIPCConnectionClosedFn,
      IPCServerPrivate::qbIPCConnectionDestroyedFn
    };

    _qb_service = qb_ipcs_create("usbguard", 0,
				 QB_IPC_NATIVE, &service_handlers);

    if (_qb_service == nullptr) {
      throw Exception("IPC server initialization", "service", "Failed to create qb_service object");
    }

    qb_ipcs_service_context_set(_qb_service, this);

    static struct qb_ipcs_poll_handlers poll_handlers = {
      IPCServerPrivate::qbIPCJobAdd,
      IPCServerPrivate::qbIPCDispatchAdd,
      IPCServerPrivate::qbIPCDispatchMod,
      IPCServerPrivate::qbIPCDispatchDel
    };

    qb_ipcs_poll_handlers_set(_qb_service, &poll_handlers);

    const auto rc = qb_ipcs_run(_qb_service);

    if (rc != 0) {
      throw ErrnoException("IPC server initialization", "service", (int)-rc);
    }
  }

  void IPCServerPrivate::finiIPC()
  {
    qb_ipcs_destroy(_qb_service);
  }

  IPCServerPrivate::~IPCServerPrivate()
  {
    destruct();
  }

  void IPCServerPrivate::thread()
  {
    qb_loop_run(_qb_loop);
  }

  void IPCServerPrivate::wakeup()
  {
    const uint64_t one = 1;
    USBGUARD_SYSCALL_THROW("IPC server",
                           write(_wakeup_fd, &one, sizeof one) != sizeof one);
  }

  void IPCServerPrivate::start()
  {
    if (!_thread.running()) {
      _thread.start();
    }
  }

  void IPCServerPrivate::stop()
  {
    _thread.stop(/*do_wait=*/false);
    qb_loop_stop(_qb_loop);
    wakeup();
    _thread.wait();
  }

  void IPCServerPrivate::destruct()
  {
    if (_thread.running()) {
      stop();
    }
    finiIPC();
    qb_loop_poll_del(_qb_loop, _wakeup_fd);
    qb_loop_destroy(_qb_loop);
    USBGUARD_SYSCALL_THROW("IPC server", close(_wakeup_fd) != 0);
  }

  int32_t IPCServerPrivate::qbPollWakeupFn(int32_t fd, int32_t revents, void * data)
  {
    USBGUARD_LOG(Trace) << "fd=" << fd
                        << " revents=" << revents
                        << " data=" << data;
    uint64_t one = 0;
    if (read(fd, &one, sizeof one) != sizeof one) {
      USBGUARD_LOG(Warning) << "IPC server: "
                            << "Failed to read wakeup event: "
                            << "errno=" << errno;
      return -1;
    }
    else {
      return 0;
    }
  }

  void IPCServerPrivate::qbIPCConnectionCreatedFn(qb_ipcs_connection_t *conn)
  {
    USBGUARD_LOG(Trace) << "conn=" << conn;
    USBGUARD_LOG(Info) << "New IPC connection from PID " << qbIPCConnectionClientPID(conn);
  }

  void IPCServerPrivate::qbIPCConnectionDestroyedFn(qb_ipcs_connection_t *conn)
  {
    USBGUARD_LOG(Trace) << "conn=" << conn;
  }

  int32_t IPCServerPrivate::qbIPCConnectionClosedFn(qb_ipcs_connection_t *conn)
  {
    USBGUARD_LOG(Trace) << "conn=" << conn;
    USBGUARD_LOG(Info) << "Closed IPC connection to PID " << qbIPCConnectionClientPID(conn);
    return 0;
  }

  int32_t IPCServerPrivate::qbIPCJobAdd(enum qb_loop_priority p, void *data, qb_loop_job_dispatch_fn fn)
  {
    return qb_loop_job_add(G_qb_loop, p, data, fn);
  }

  int32_t IPCServerPrivate::qbIPCDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts,
				   void *data, qb_ipcs_dispatch_fn_t fn)
  {
    return qb_loop_poll_add(G_qb_loop, p, fd, evts, data, fn);
  }

  int32_t IPCServerPrivate::qbIPCDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts,
				   void *data, qb_ipcs_dispatch_fn_t fn)
  {
    return qb_loop_poll_mod(G_qb_loop, p, fd, evts, data, fn);
  }

  int32_t IPCServerPrivate::qbIPCDispatchDel(int32_t fd)
  {
    return qb_loop_poll_del(G_qb_loop, fd);
  }

  int32_t IPCServerPrivate::qbIPCConnectionClientPID(qb_ipcs_connection_t *connection)
  {
    std::unique_ptr<qb_ipcs_connection_stats_2, FreeDeleter> \
      stats(qb_ipcs_connection_stats_get_2(connection, /*clear_after_read=*/0));

    if (stats == nullptr) {
      throw std::runtime_error("Cannot retrieve qb connection statistics");
    }

    return stats->client_pid;
  }

  int32_t IPCServerPrivate::qbIPCConnectionAcceptFn(qb_ipcs_connection_t *conn, uid_t uid, gid_t gid)
  {
    try {
      IPCServerPrivate* server = \
        static_cast<IPCServerPrivate*>(qb_ipcs_connection_service_context_get(conn));

      const bool auth = server->qbIPCConnectionAllowed(uid, gid);

      if (auth) {
        USBGUARD_LOG(Info) << "IPC connection accepted: uid=" << uid
                           << " gid=" << gid
                           << " pid=" << qbIPCConnectionClientPID(conn);
        USBGUARD_LOG(Debug) << "Setting SHM permissions to uid=" << uid
                            << " gid=" << 0
                            << " mode=0660";
        qb_ipcs_connection_auth_set(conn, uid, 0, 0660);
        return 0;
      }
      else {
        USBGUARD_LOG(Warning) << "IPC connection denied: uid=" << uid
                              << " gid=" << gid
                              << " pid=" << qbIPCConnectionClientPID(conn);
        return -1;
      }
    }
    catch(const std::exception& exception) {
      USBGUARD_LOG(Error) << "IPC connection denied: Exception: " << exception.what();
    }
    catch(...) {
      USBGUARD_LOG(Error) << "IPC connection denied: BUG: unknown exception caught.";
    }

    return -1;
  }

  bool IPCServerPrivate::qbIPCConnectionAllowed(uid_t uid, gid_t gid)
  {
    if (!_allowed_uids.empty() || !_allowed_gids.empty()) {
      return authenticateIPCConnectionDAC(uid, gid);
    }
    else {
      USBGUARD_LOG(Debug) << "IPC ACL is empty."
                          << " Allowing connection for uid=" << uid
                          << " gid=" << gid;
      return true;
    }
  }

  void IPCServerPrivate::qbIPCSendMessage(qb_ipcs_connection_t *qb_conn, const IPC::MessagePointer& message)
  {
    if (qb_conn == nullptr || message == nullptr) {
      throw USBGUARD_BUG("NULL argument(s)");
    }

    std::string payload;
    message->SerializeToString(&payload);

    struct qb_ipc_response_header hdr;
    struct iovec iov[2];
 
    hdr.id = QB_IPC_MSG_USER_START + IPC::messageTypeNameToNumber(message->GetTypeName());
    hdr.size = sizeof hdr + payload.size();
    hdr.error = 0;

    iov[0].iov_base = &hdr;
    iov[0].iov_len = sizeof hdr;
    iov[1].iov_base = (void *)payload.data();
    iov[1].iov_len = payload.size();

    const size_t total_size = hdr.size;
    const ssize_t rc = qb_ipcs_event_sendv(qb_conn, iov, 2);

    if (rc < 0 || (size_t)rc != total_size) {
      std::unique_ptr<qb_ipcs_connection_stats_2, FreeDeleter> \
        stats(qb_ipcs_connection_stats_get_2(qb_conn, /*clear_after_read=*/0));

      if (stats == nullptr) {
        throw std::runtime_error("Cannot retrieve qb connection statistics");
      }

      if (rc < 0) {
        USBGUARD_LOG(Error) << "An error ocured while sending IPC message to pid=" << qbIPCConnectionClientPID(qb_conn);
        /* FALLTHROUGH */
      }
      else if ((size_t)rc != total_size) {
        USBGUARD_LOG(Error) << "Unable to sent complete IPC message to pid=" << qbIPCConnectionClientPID(qb_conn)
                            << " sent=" << (size_t)rc
                            << " expected=" << total_size;
        /* FALLTHROUGH */
      }
    }

    iov[0].iov_base = nullptr;
    iov[1].iov_base = nullptr;
  }

  int32_t IPCServerPrivate::qbIPCMessageProcessFn(qb_ipcs_connection_t *conn, void *data, size_t size)
  {
    if (conn == nullptr) {
      return -1;
    }

    qb_ipcs_connection_ref(conn);

    if (size <= sizeof (struct qb_ipc_request_header)) {
      USBGUARD_LOG(Debug) << "IPC message too short";
      qb_ipcs_disconnect(conn);
      return -1;
    }
    if (size > 1<<20) {
      USBGUARD_LOG(Debug) << "IPC message too large";
      qb_ipcs_disconnect(conn);
      return -1;
    }

    const struct qb_ipc_request_header * const hdr = \
      reinterpret_cast<const struct qb_ipc_request_header *>(data);

    if (size != (size_t)hdr->size) {
      USBGUARD_LOG(Debug) << "Invalid IPC header size";
      qb_ipcs_disconnect(conn);
      return -1;
    }
    if (hdr->id < QB_IPC_MSG_USER_START) {
      USBGUARD_LOG(Debug) << "Invalid IPC header id";
      qb_ipcs_disconnect(conn);
      return -1;
    }

    int32_t client_pid = -1;
    bool client_disconnect = false;
 
    try {
      client_pid = qbIPCConnectionClientPID(conn);
    }
    catch(...) {
      USBGUARD_LOG(Error) << "Unable to get client PID. Disconnecting client.";
      qb_ipcs_disconnect(conn);
      return -1;
    }

    try {
      IPCServerPrivate * const server = \
        reinterpret_cast<IPCServerPrivate*>(qb_ipcs_connection_service_context_get(conn));

      const uint32_t payload_type = hdr->id - QB_IPC_MSG_USER_START;
      const char * const payload_data = reinterpret_cast<const char*>(data) + sizeof(struct qb_ipc_request_header);
      const size_t payload_size = size - sizeof(struct qb_ipc_request_header);
      const std::string payload(payload_data, payload_size);

      USBGUARD_LOG(Debug) << "Handling IPC payload of type=" << payload_type
                          << " size=" << payload_size;

      auto response = server->handleIPCPayload(payload_type, payload);

      if (response) {
        USBGUARD_LOG(Debug) << "Sending response to client_pid=" << client_pid;
        qbIPCSendMessage(conn, response);
      }
    }
    catch(const IPCException& ex) {
      USBGUARD_LOG(Warning) << "IPC: client_pid=" << client_pid
                            << ": IPC exception: " << ex.message();
      qbIPCSendMessage(conn, IPC::IPCExceptionToMessage(ex));
      /* FALLTHROUGH */
    }
    catch(const Exception& ex) {
      USBGUARD_LOG(Warning) << "IPC: client_pid=" << client_pid
                            << ": Exception: " << ex.message();
      client_disconnect = true;
      /* FALLTHROUGH */
    }
    catch(const std::exception& ex) {
      USBGUARD_LOG(Warning) << "IPC: client_pid=" << client_pid
                            << ": Exception: " << ex.what();
      client_disconnect = true;
      /* FALLTHROUGH */
    }
    catch(...) {
      USBGUARD_LOG(Warning) << "IPC: client_pid=" << client_pid
                            << ": Unknown exception.";
      client_disconnect = true;
      /* FALLTHROUGH */
    }

    if (client_disconnect) {
      USBGUARD_LOG(Warning) << "IPC: client_pid=" << client_pid
                            << ": Disconnecting client.";
      qb_ipcs_disconnect(conn);
      return -1;
    }
    else {
      qb_ipcs_connection_unref(conn);
    }

    return 0;
  }

  void IPCServerPrivate::qbIPCBroadcastData(const struct iovec * const iov, const size_t iov_len)
  {
    auto qb_conn = qb_ipcs_connection_first_get(_qb_service);
    size_t total_size = 0;

    for (size_t i = 0; i < iov_len; ++i) {
      total_size += iov[i].iov_len;
    }

    while (qb_conn != nullptr) {
      /* Send the data */
      const ssize_t rc = qb_ipcs_event_sendv(qb_conn, iov, iov_len);

      if (rc < 0 || (size_t)rc != total_size) {
        std::unique_ptr<qb_ipcs_connection_stats_2, FreeDeleter> \
          stats(qb_ipcs_connection_stats_get_2(qb_conn, /*clear_after_read=*/0));

        if (stats == nullptr) {
          throw std::runtime_error("Cannot retrieve qb connection statistics");
        }

        if (rc < 0) {
          USBGUARD_LOG(Error) << "An error ocured while sending IPC message to pid=" << qbIPCConnectionClientPID(qb_conn);
        }
        else if ((size_t)rc != total_size) {
          USBGUARD_LOG(Error) << "Unable to sent complete IPC message to pid=" << qbIPCConnectionClientPID(qb_conn)
            << " sent=" << (size_t)rc
            << " expected=" << total_size;
        }
      }

      /* Get the next connection */
      auto qb_conn_next = qb_ipcs_connection_next_get(_qb_service, qb_conn);
      qb_ipcs_connection_unref(qb_conn);
      qb_conn = qb_conn_next;
    }
  }

  void IPCServerPrivate::qbIPCBroadcastMessage(const IPC::MessagePointer& message)
  {
    qbIPCBroadcastMessage(message.get());
  }

  void IPCServerPrivate::qbIPCBroadcastMessage(const IPC::MessageType * const message)
  {
    std::string payload;
    message->SerializeToString(&payload);

    struct qb_ipc_response_header hdr;
    hdr.id = QB_IPC_MSG_USER_START + IPC::messageTypeNameToNumber(message->GetTypeName());
    hdr.size = sizeof hdr + payload.size();
    hdr.error = 0;

    struct iovec iov[2];
    iov[0].iov_base = &hdr;
    iov[0].iov_len = sizeof hdr;
    iov[1].iov_base = (void *)payload.data();
    iov[1].iov_len = payload.size();

    qbIPCBroadcastData(iov, 2);

    iov[0].iov_base = nullptr;
    iov[1].iov_base = nullptr;
  }

  bool IPCServerPrivate::authenticateIPCConnectionDAC(uid_t uid, gid_t gid)
  {
    /* Check for UID match */
    for (auto allowed_uid : _allowed_uids) {
      if (allowed_uid == uid) {
	
	return true;
      }
    }

    /* Translate uid to username for group member matching */
    char pw_string_buffer[1024]; /* TODO: adjust size to max user/group name length */
    struct passwd pw, *pwptr = nullptr;
    bool check_group_membership = true;

    if (getpwuid_r(uid, &pw,
		   pw_string_buffer, sizeof pw_string_buffer, &pwptr) != 0) {
      USBGUARD_LOG(Warning) << "IPC ACL: Unable to lookup username for uid=" << uid
                            << ". Disabling group membership check for this uid.";
      check_group_membership = false;
    }

    /* Check for GID match or group member match */
    for (auto allowed_gid : _allowed_gids) {
      if (allowed_gid == gid) {
        USBGUARD_LOG(Debug) << "gid=" << gid << " is an allowed gid.";        
	return true;
      }
      else if (check_group_membership) {
	char gr_string_buffer[3072];
	struct group gr, *grptr = nullptr;

	/* Fetch list of current group members of group with a gid == allowed_gid */
	if (getgrgid_r(allowed_gid, &gr,
		       gr_string_buffer, sizeof gr_string_buffer, &grptr) != 0) {
	  USBGUARD_LOG(Warning) << "IPC ACL: Unable to lookup groupname for gid=" << allowed_gid
                                << ". Skipping group membership check for uid=" << uid;
	  continue;
	}

	/* Check for username match among group members */
	for (size_t i = 0; gr.gr_mem[i] != nullptr; ++i) {
	  if (strcmp(pw.pw_name, gr.gr_mem[i]) == 0) {
            USBGUARD_LOG(Debug) << "uid=" << uid << "(" << pw.pw_name << ")"
                                << " is a member of an allowed group with"
                                << " gid=" << allowed_gid << " (" << gr.gr_name << ")";
	    return true;
	  }
	}
      }
    } /* allowed gid loop */

    return false;
  }

  IPC::MessagePointer IPCServerPrivate::handleIPCPayload(const uint32_t payload_type, const std::string& payload)
  {
    const auto& handler_it = _handlers.find(payload_type);

    if (handler_it == _handlers.end()) {
      throw Exception("IPC connection", "IPC payload data", "Unknown payload type");
    }

    auto& handler = handler_it->second;

    /*
     * Try to parse the IPC payload as a message of the
     * specified type. If the parsing fails, the client will
     * be disconnected.
     */
    IPC::MessagePointer message_in;
    uint64_t request_id = 0;

    try {
      message_in = handler.payloadToMessage(payload);
      request_id = IPC::getMessageHeaderID(*message_in);
    }
    catch(...) {
      throw Exception("IPC connection", "IPC payload data", "Payload data parsing failed");
    }

    /*
     * Try to run the handler. Exception thrown from inside the handler
     * will be sent to the client.
     */
    try {
      IPC::MessagePointer response;
      handler.run(message_in, response);
      return response;
    }
    catch(IPCException& exception) {
      exception.setMessageID(request_id);
      throw exception;
    }
    catch(const Exception& exception) {
      throw IPCException(exception, request_id);
    }
    catch(const std::exception& exception) {
      throw IPCException("IPC method",
                         IPC::messageTypeNameFromNumber(payload_type),
                         exception.what(),
                         request_id);
    }
    catch(...) {
      throw IPCException("IPC method",
                         IPC::messageTypeNameFromNumber(payload_type),
                         "BUG: Unexpected exception",
                         request_id);
    }
  }

  void IPCServerPrivate::handleAppendRule(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    /*
     * Get request field values.
     */
    const IPC::appendRule * const message_in = static_cast<const IPC::appendRule *>(request.get());
    const std::string rule = message_in->request().rule();
    const uint32_t parent_id = message_in->request().parent_id();

    /*
     * Execute the method.
     */
    const uint32_t id = _p_instance.appendRule(rule, parent_id);

    /*
     * Construct the response.
     */
    IPC::appendRule * const message_out = message_in->New();
    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->set_id(id);

    response.reset(message_out);
  }

  void IPCServerPrivate::handleRemoveRule(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    /*
     * Get request field values.
     */
    const IPC::removeRule * const message_in = static_cast<const IPC::removeRule *>(request.get());
    const uint32_t id = message_in->request().id();

    /*
     * Execute the method.
     */
    _p_instance.removeRule(id);

    /*
     * Construct the response.
     */
    IPC::removeRule * const message_out = message_in->New();
    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->set_id(id);

    response.reset(message_out);
  }

  void IPCServerPrivate::handleListRules(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    /*
     * Get request field values.
     */
    const IPC::listRules * const message_in = static_cast<const IPC::listRules *>(request.get());
    const std::string query = message_in->request().query();

    /*
     * Execute the method.
     */
    auto rule_set = _p_instance.listRules(query);
    const uint32_t default_target = Rule::targetToInteger(rule_set.getDefaultTarget());

    /*
     * Construct the response.
     */
    IPC::listRules * const message_out = message_in->New();
    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->set_default_target(default_target);

    for (const auto& rule : rule_set.getRules()) {
      auto message_rule = message_out->mutable_response()->add_rules();
      message_rule->set_id(rule->getRuleID());
      message_rule->set_rule(rule->toString());
    }

    response.reset(message_out);
  }

  void IPCServerPrivate::handleApplyDevicePolicy(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    /*
     * Get request field values.
     */
    const IPC::applyDevicePolicy * const message_in = static_cast<const IPC::applyDevicePolicy*>(request.get());
    const uint32_t id = message_in->request().id();
    const Rule::Target target = Rule::targetFromInteger(message_in->request().target());
    const bool permanent = message_in->request().permanent();

    /*
     * Execute the method.
     */
    const uint32_t rule_id = _p_instance.applyDevicePolicy(id, target, permanent);

    /*
     * Construct the response.
     */
    IPC::applyDevicePolicy * const message_out = message_in->New();
    IPC::MessagePointer response_local(message_out);

    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->set_rule_id(rule_id);

    response = std::move(response_local);
  }

  void IPCServerPrivate::handleListDevices(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    /*
     * Get request field values.
     */
    const IPC::listDevices * const message_in = static_cast<const IPC::listDevices *>(request.get());
    const std::string query = message_in->request().query();

    /*
     * Execute the method.
     */
    auto device_rules = _p_instance.listDevices(query);

    /*
     * Construct the response.
     */
    IPC::listDevices * const message_out = message_in->New();
    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->Clear();

    for (const auto& device_rule : device_rules) {
      auto message_rule = message_out->mutable_response()->add_devices();
      message_rule->set_id(device_rule.getRuleID());
      message_rule->set_rule(device_rule.toString());
    }

    response.reset(message_out);
  }

  void IPCServerPrivate::handleSetParameter(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    /*
     * Get request field values.
     */
    const IPC::setParameter * const message_in = static_cast<const IPC::setParameter*>(request.get());
    const std::string name = message_in->request().name();
    const std::string value = message_in->request().value();

    /*
     * Execute the method.
     */
    auto previous_value = _p_instance.setParameter(name, value);

    /*
     * Construct the response.
     */
    IPC::setParameter * const message_out = message_in->New();
    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->set_value(previous_value);

    response.reset(message_out);
  }

  void IPCServerPrivate::handleGetParameter(IPC::MessagePointer& request, IPC::MessagePointer& response)
  {
    const IPC::getParameter * const message_in = static_cast<const IPC::getParameter*>(request.get());
    const std::string name = message_in->request().name();

    auto value = _p_instance.getParameter(name);

    IPC::getParameter * const message_out = message_in->New();
    message_out->MergeFrom(*message_in);
    message_out->mutable_response()->set_value(value);

    response.reset(message_out);
  }

  void IPCServerPrivate::DevicePresenceChanged(uint32_t id,
                                               DeviceManager::EventType event,
                                               Rule::Target target,
                                               const std::string& device_rule)
  {
    IPC::DevicePresenceChangedSignal signal;

    signal.set_id(id);
    signal.set_event(DeviceManager::eventTypeToInteger(event));
    signal.set_target(Rule::targetToInteger(target));
    signal.set_device_rule(device_rule);

    qbIPCBroadcastMessage(&signal);
  }

  void IPCServerPrivate::DevicePolicyChanged(uint32_t id,
                                             Rule::Target target_old,
                                             Rule::Target target_new,
                                             const std::string& device_rule,
                                             uint32_t rule_id)
  {
    IPC::DevicePolicyChangedSignal signal;

    signal.set_id(id);
    signal.set_target_old(Rule::targetToInteger(target_old));
    signal.set_target_new(Rule::targetToInteger(target_new));
    signal.set_device_rule(device_rule);
    signal.set_rule_id(rule_id);

    qbIPCBroadcastMessage(&signal);
  }

  void IPCServerPrivate::ExceptionMessage(const std::string& context,
                                          const std::string& object,
                                          const std::string& reason,
                                          uint64_t request_id)
  {
    IPC::Exception exception;

    exception.set_context(context);
    exception.set_object(object);
    exception.set_reason(reason);

    if (request_id > 0) {
      exception.set_request_id(request_id);
    }

    qbIPCBroadcastMessage(&exception);
  }

  void IPCServerPrivate::addAllowedUID(uid_t uid)
  {
    _allowed_uids.push_back(uid);
  }

  void IPCServerPrivate::addAllowedGID(gid_t gid)
  {
    _allowed_gids.push_back(gid);
  }
} /* namespace usbguard */
