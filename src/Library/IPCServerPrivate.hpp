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
#pragma once
#include <build-config.h>
#include "Typedefs.hpp"
#include "IPCServer.hpp"
#include "IPCPrivate.hpp"
#include "Common/Thread.hpp"

#include <map>
#include <mutex>
#include <future>

#include <qb/qbipcs.h>
#include <qb/qbloop.h>

#include "Devices.pb.h"
#include "Policy.pb.h"
#include "Exception.pb.h"
#include "Parameter.pb.h"

namespace usbguard {
  class IPCServerPrivate
  {
    using MessageHandler = IPC::MessageHandler<IPCServerPrivate>;

  public:
    IPCServerPrivate(IPCServer& p_instance);
    ~IPCServerPrivate();

    void start();
    void stop();
 
    void DevicePresenceChanged(uint32_t id,
                               DeviceManager::EventType event,
                               Rule::Target target,
                               const std::string& device_rule);

    void DevicePolicyChanged(uint32_t id,
                             Rule::Target target_old,
                             Rule::Target target_new,
                             const std::string& device_rule,
                             uint32_t rule_id);

    void ExceptionMessage(const std::string& context,
                          const std::string& object,
                          const std::string& reason,
                          uint64_t request_id = 0);

    void addAllowedUID(uid_t uid);
    void addAllowedGID(gid_t gid);

  private:
    void initIPC();
    void finiIPC();

    void thread();
    void wakeup();
    void destruct();

    static int32_t qbPollWakeupFn(int32_t fd, int32_t revents, void * data);
    static int32_t qbIPCConnectionAcceptFn(qb_ipcs_connection_t *, uid_t, gid_t);
    static void qbIPCConnectionCreatedFn(qb_ipcs_connection_t *);
    static void qbIPCConnectionDestroyedFn(qb_ipcs_connection_t *);
    static int32_t qbIPCConnectionClosedFn(qb_ipcs_connection_t *);
    static int32_t qbIPCMessageProcessFn(qb_ipcs_connection_t *, void *, size_t);

    static int32_t qbIPCJobAdd(enum qb_loop_priority p, void *data, qb_loop_job_dispatch_fn fn);
    static int32_t qbIPCDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts, void *data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbIPCDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts, void *data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbIPCDispatchDel(int32_t fd);
    static int32_t qbIPCConnectionClientPID(qb_ipcs_connection_t *connection);

    bool qbIPCConnectionAllowed(uid_t uid, gid_t gid);
    bool authenticateIPCConnectionDAC(uid_t uid, gid_t gid);

    static void qbIPCSendMessage(qb_ipcs_connection_t *qb_conn, const IPC::MessagePointer& message);
    void qbIPCBroadcastData(const struct iovec *iov, size_t iov_len);
    void qbIPCBroadcastMessage(const IPC::MessagePointer& message);
    void qbIPCBroadcastMessage(const IPC::MessageType* message);

    IPC::MessagePointer handleIPCPayload(const uint32_t payload_type, const std::string& payload);
 
    template<class T>
    void registerHandler(MessageHandler::HandlerType method)
    {
      const uint32_t type_number = IPC::messageTypeNameToNumber(T::default_instance().GetTypeName());
      _handlers.emplace(type_number, MessageHandler::create<T>(*this, method));
    }
    
    void handleAppendRule(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleRemoveRule(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleListRules(IPC::MessagePointer& request, IPC::MessagePointer& response);

    void handleApplyDevicePolicy(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleListDevices(IPC::MessagePointer& request, IPC::MessagePointer& response);

    void handleSetParameter(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleGetParameter(IPC::MessagePointer& request, IPC::MessagePointer& response);

    IPCServer& _p_instance;

    qb_loop_t *_qb_loop;
    qb_ipcs_service_t *_qb_service;
    int _wakeup_fd;

    std::vector<uid_t> _allowed_uids;
    std::vector<gid_t> _allowed_gids;

    Thread<IPCServerPrivate> _thread;

    std::unordered_map<uint32_t, MessageHandler> _handlers;
  };
} /* namespace usbguard */
