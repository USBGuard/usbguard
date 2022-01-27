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
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "IPCPrivate.hpp"
#include "Common/Thread.hpp"

#include "Policy.pb.h"
#include "Devices.pb.h"
#include "Exception.pb.h"
#include "Parameter.pb.h"

#include "usbguard/Typedefs.hpp"
#include "usbguard/IPCClient.hpp"

#include <map>
#include <mutex>
#include <future>

#include <qb/qbipcc.h>
#include <qb/qbloop.h>

namespace usbguard
{
  class IPCClientPrivate
  {
    using MessageHandler = IPC::MessageHandler<IPCClientPrivate>;

  public:
    IPCClientPrivate(IPCClient& p_instance, bool connected);
    ~IPCClientPrivate();

    void connect();
    void disconnect(bool exception_initiated, const IPCException& exception, bool do_wait = false);
    void disconnect(bool do_wait = false);
    bool isConnected() const;
    void wait();

    std::string setParameter(const std::string& name, const std::string& value);
    std::string getParameter(const std::string& name);

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id, bool permanent);
    void removeRule(uint32_t id);
    const std::vector<Rule> listRules(const std::string& label);

    uint32_t applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent);
    const std::vector<Rule> listDevices(const std::string& query);

    bool checkIPCPermissions(const IPCServer::AccessControl::Section& section,
      const IPCServer::AccessControl::Privilege& privilege);

    void processReceiveEvent();

  private:
    static int32_t qbPollWakeupFn(int32_t fd, int32_t revents, void* data);
    static int32_t qbIPCMessageProcessFn(int32_t fd, int32_t revents, void* data);

    void destruct();
    void thread();
    void wakeup();
    void stop(bool do_wait = false);

    static uint64_t generateMessageID(void);

    template<class T>
    void registerHandler(MessageHandler::HandlerType method)
    {
      const uint32_t type_number = IPC::messageTypeNameToNumber(T::default_instance().GetTypeName());
      _handlers.emplace(type_number, MessageHandler::create<T>(*this, method));
    }

    template<typename ProtobufType>
    std::unique_ptr<ProtobufType> qbIPCSendRecvMessage(ProtobufType& message_out)
    {
      IPC::MessagePointer message_in = qbIPCSendRecvMessage(reinterpret_cast<IPC::MessageType&>(message_out));

      if (message_in->GetTypeName() != ProtobufType::default_instance().GetTypeName()) {
        throw std::runtime_error("qbIPCSendRecvMessage: response type mismatch");
      }

      return std::unique_ptr<ProtobufType>(static_cast<ProtobufType*>(message_in.release()));
    }

    IPC::MessagePointer qbIPCSendRecvMessage(IPC::MessageType& message);

    std::string receive();
    void process(const std::string& buffer);
    void handleIPCPayload(uint32_t payload_type, const std::string& payload);

    void handleMethodResponse(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out);
    void handleException(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out);
    void handleDevicePresenceChangedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out);
    void handleDevicePolicyChangedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out);
    void handleDevicePolicyAppliedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out);
    void handlePropertyParameterChangedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out);

    IPCClient& _p_instance;

    qb_loop_t* _qb_loop;
    qb_ipcc_connection_t* _qb_conn;
    int _qb_fd;

    int _wakeup_fd;

    std::mutex _return_mutex, _disconnect_mutex;
    std::map<uint64_t, std::promise<IPC::MessagePointer>> _return_map;

    Thread<IPCClientPrivate> _thread;
    std::map<uint32_t, MessageHandler> _handlers;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
