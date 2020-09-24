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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "IPCPrivate.hpp"
#include "Common/Thread.hpp"

#include "Devices.pb.h"
#include "Policy.pb.h"
#include "Exception.pb.h"
#include "Parameter.pb.h"

#include "usbguard/Typedefs.hpp"
#include "usbguard/IPCServer.hpp"

#include <map>
#include <mutex>
#include <future>

#include <qb/qbipcs.h>
#include <qb/qbloop.h>

namespace usbguard
{
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

    void DevicePolicyApplied(uint32_t id,
      Rule::Target target_new,
      const std::string& device_rule,
      uint32_t rule_id);

    void PropertyParameterChanged(const std::string& name,
      const std::string& value_old,
      const std::string& value_new);

    void ExceptionMessage(const std::string& context,
      const std::string& object,
      const std::string& reason,
      uint64_t request_id = 0);

    void addAllowedUID(uid_t uid, const IPCServer::AccessControl& ac);
    void addAllowedGID(gid_t gid, const IPCServer::AccessControl& ac);
    void addAllowedUsername(const std::string& username, const IPCServer::AccessControl& ac);
    void addAllowedGroupname(const std::string& groupname, const IPCServer::AccessControl& ac);

  private:
    struct ClientContext {
      IPCServer::AccessControl access_control;
      std::mutex mutex;
    };

    void initIPC();
    void finiIPC();

    void thread();
    void wakeup();
    void destruct();

    static int32_t qbPollWakeupFn(int32_t fd, int32_t revents, void* data);
    static int32_t qbIPCConnectionAcceptFn(qb_ipcs_connection_t*, uid_t, gid_t);
    static void qbIPCConnectionCreatedFn(qb_ipcs_connection_t*);
    static void qbIPCConnectionDestroyedFn(qb_ipcs_connection_t*);
    static int32_t qbIPCConnectionClosedFn(qb_ipcs_connection_t*);
    static int32_t qbIPCMessageProcessFn(qb_ipcs_connection_t*, void*, size_t);

    static int32_t qbIPCJobAdd(enum qb_loop_priority p, void* data, qb_loop_job_dispatch_fn fn);
    static int32_t qbIPCDispatchAdd(enum qb_loop_priority p, int32_t fd, int32_t evts, void* data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbIPCDispatchMod(enum qb_loop_priority p, int32_t fd, int32_t evts, void* data, qb_ipcs_dispatch_fn_t fn);
    static int32_t qbIPCDispatchDel(int32_t fd);
    static int32_t qbIPCConnectionClientPID(qb_ipcs_connection_t* connection);

    bool hasACLEntries() const;
    bool qbIPCConnectionAllowed(uid_t uid, gid_t gid, IPCServer::AccessControl* const ac_ptr) const;
    bool authenticateIPCConnectionDAC(uid_t uid, gid_t gid, IPCServer::AccessControl* const ac_ptr = nullptr) const;

    bool matchACLByUID(uid_t uid, IPCServer::AccessControl* const ac_ptr) const;
    bool matchACLByGID(gid_t gid, IPCServer::AccessControl* const ac_ptr) const;
    bool matchACLByName(uid_t uid, gid_t gid, IPCServer::AccessControl* const ac_ptr) const;

    static std::string getNameFromUID(uid_t uid);
    static std::string getNameFromGID(gid_t gid);
    static std::vector<std::string> getGroupMemberNames(gid_t gid);
    static std::vector<std::string> getGroupMemberNames(const std::string& groupname);

    static void qbIPCSendMessage(qb_ipcs_connection_t* qb_conn, const IPC::MessagePointer& message);
    static IPCServer::AccessControl::Section messageTypeNameToAccessControlSection(const std::string& name);
    void qbIPCBroadcastData(const struct iovec* iov, size_t iov_len, IPCServer::AccessControl::Section section);
    void qbIPCBroadcastMessage(const IPC::MessagePointer& message);
    void qbIPCBroadcastMessage(const IPC::MessageType* message);

    IPC::MessagePointer handleIPCPayload(const uint32_t payload_type, const std::string& payload,
      const IPCServer::AccessControl* const access_control);

    template<class T>
    void registerHandler(MessageHandler::HandlerType method, IPCServer::AccessControl::Section section,
      IPCServer::AccessControl::Privilege privilege)
    {
      const uint32_t type_number = IPC::messageTypeNameToNumber(T::default_instance().GetTypeName());
      _handlers.emplace(type_number, MessageHandler::create<T>(*this, method, section, privilege));
    }

    void handleAppendRule(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleRemoveRule(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleListRules(IPC::MessagePointer& request, IPC::MessagePointer& response);

    void handleApplyDevicePolicy(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleListDevices(IPC::MessagePointer& request, IPC::MessagePointer& response);

    void handleSetParameter(IPC::MessagePointer& request, IPC::MessagePointer& response);
    void handleGetParameter(IPC::MessagePointer& request, IPC::MessagePointer& response);

    void handleCheckIPCPermissions(IPC::MessagePointer& request, IPC::MessagePointer& response);

    IPCServer& _p_instance;

    qb_loop_t* _qb_loop;
    qb_ipcs_service_t* _qb_service;
    int _wakeup_fd;

    std::unordered_map<uid_t, IPCServer::AccessControl> _allowed_uids;
    std::unordered_map<gid_t, IPCServer::AccessControl> _allowed_gids;
    std::unordered_map<std::string, IPCServer::AccessControl> _allowed_usernames;
    std::unordered_map<std::string, IPCServer::AccessControl> _allowed_groupnames;

    Thread<IPCServerPrivate> _thread;

    std::unordered_map<uint32_t, MessageHandler> _handlers;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
