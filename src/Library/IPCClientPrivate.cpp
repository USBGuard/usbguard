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
//          Radovan Sroka <rsroka@redhat.com>
//
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "IPCClientPrivate.hpp"
#include "IPCPrivate.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/RuleSet.hpp"
#include "usbguard/MemoryRuleSet.hpp"

#include <stdint.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/eventfd.h>
#include <sys/types.h>

namespace usbguard
{
  int32_t IPCClientPrivate::qbPollWakeupFn(int32_t fd, int32_t revents, void* data)
  {
    USBGUARD_LOG(Trace) << "fd=" << fd
      << " revents=" << revents
      << " data=" << data;
    uint64_t one = 0;

    if (read(fd, &one, sizeof one) != sizeof one) {
      USBGUARD_LOG(Warning) << "IPC client: "
        << "Failed to read wakeup event: "
        << "errno=" << errno;
      return -1;
    }
    else {
      return 0;
    }
  }

  int32_t IPCClientPrivate::qbIPCMessageProcessFn(int32_t fd, int32_t revents, void* data)
  {
    USBGUARD_LOG(Trace) << "fd=" << fd
      << " revents=" << revents
      << " data=" << data;
    IPCClientPrivate* client = static_cast<IPCClientPrivate*>(data);
    client->processReceiveEvent();
    return 0;
  }

  IPCClientPrivate::IPCClientPrivate(IPCClient& p_instance, bool connected)
    : _p_instance(p_instance),
      _thread(this, &IPCClientPrivate::thread)
  {
    _qb_conn = nullptr;
    _qb_fd = -1;
    USBGUARD_SYSCALL_THROW("IPC client initialization",
      (_wakeup_fd = eventfd(0, 0)) < 0);
    _qb_loop = qb_loop_create();
    qb_loop_poll_add(_qb_loop, QB_LOOP_HIGH, _wakeup_fd, POLLIN, NULL, qbPollWakeupFn);
    registerHandler<IPC::getParameter>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::setParameter>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::listRules>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::appendRule>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::removeRule>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::applyDevicePolicy>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::listDevices>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::checkIPCPermissions>(&IPCClientPrivate::handleMethodResponse);
    registerHandler<IPC::Exception>(&IPCClientPrivate::handleException);
    registerHandler<IPC::DevicePresenceChangedSignal>(&IPCClientPrivate::handleDevicePresenceChangedSignal);
    registerHandler<IPC::DevicePolicyChangedSignal>(&IPCClientPrivate::handleDevicePolicyChangedSignal);
    registerHandler<IPC::DevicePolicyAppliedSignal>(&IPCClientPrivate::handleDevicePolicyAppliedSignal);
    registerHandler<IPC::PropertyParameterChangedSignal>(&IPCClientPrivate::handlePropertyParameterChangedSignal);

    if (connected) {
      try {
        connect();
      }
      catch (...) {
        destruct();
        throw;
      }
    }
  }

  void IPCClientPrivate::destruct()
  {
    USBGUARD_LOG(Trace);
    qb_loop_poll_del(_qb_loop, _wakeup_fd);
    close(_wakeup_fd);
    qb_loop_destroy(_qb_loop);
  }

  IPCClientPrivate::~IPCClientPrivate()
  {
    disconnect(/*do_wait=*/_thread.running());
    destruct();
  }

  void IPCClientPrivate::connect()
  {
    USBGUARD_LOG(Trace);
    _qb_conn = qb_ipcc_connect("usbguard", 1<<20);

    if (_qb_conn == nullptr) {
      throw ErrnoException("IPC connect", "service=usbguard", errno);
    }

    qb_ipcc_fd_get(_qb_conn, &_qb_fd);

    if (_qb_fd < 0) {
      qb_ipcc_disconnect(_qb_conn);
      _qb_conn = nullptr;
      _qb_fd = -1;
      throw Exception("IPC connect", "qb connection", "Bad file descriptor");
    }

    qb_loop_poll_add(_qb_loop, QB_LOOP_HIGH, _qb_fd, POLLIN, this, qbIPCMessageProcessFn);
    USBGUARD_LOG(Trace) << "Starting IPC client thread";
    _thread.start();
    _p_instance.IPCConnected();
  }

  void IPCClientPrivate::disconnect(bool exception_initiated, const IPCException& exception, bool do_wait)
  {
    USBGUARD_LOG(Trace) << "exception_initiated=" << exception_initiated
      << " exception=" << exception.message()
      << " do_wait=" << do_wait;
    USBGUARD_LOG(Trace) << "_qb_conn=" << _qb_conn
      << " _qb_fd=" << _qb_fd;
    std::unique_lock<std::mutex> disconnect_lock(_disconnect_mutex);

    if (_qb_conn != nullptr && _qb_fd >= 0) {
      qb_loop_poll_del(_qb_loop, _qb_fd);
      qb_ipcc_disconnect(_qb_conn);
      _qb_conn = nullptr;
      _qb_fd = -1;
      disconnect_lock.unlock();
      stop(do_wait);
      USBGUARD_LOG(Trace) << "Signaling IPCDisconnected";
      _p_instance.IPCDisconnected(/*exception_initiated=*/true, exception);
    }
    else if (_thread.running()) {
      disconnect_lock.unlock();
      stop(do_wait);
    }
  }

  void IPCClientPrivate::disconnect(bool do_wait)
  {
    USBGUARD_LOG(Trace) << "do_wait=" << do_wait;
    const IPCException empty_exception;
    disconnect(/*exception_initiated=*/false, empty_exception, do_wait);
  }

  bool IPCClientPrivate::isConnected() const
  {
    return _qb_fd != -1;
  }

  void IPCClientPrivate::wait()
  {
    USBGUARD_LOG(Trace);
    _thread.wait();
  }

  void IPCClientPrivate::thread()
  {
    USBGUARD_LOG(Trace) << "Entering IPC client main loop";
    qb_loop_run(_qb_loop);
    USBGUARD_LOG(Trace) << "Leaving IPC client main loop";
  }

  void IPCClientPrivate::wakeup()
  {
    const uint64_t one = 1;
    USBGUARD_SYSCALL_THROW("IPC client",
      write(_wakeup_fd, &one, sizeof one) != sizeof one);
  }

  void IPCClientPrivate::stop(bool do_wait)
  {
    USBGUARD_LOG(Trace) << "do_wait=" << do_wait;
    _thread.stop(/*do_wait=*/false);
    qb_loop_stop(_qb_loop);
    wakeup();

    if (do_wait) {
      wait();
    }
  }

  uint64_t IPCClientPrivate::generateMessageID()
  {
    static std::atomic<uint64_t> id(1);
    return id++;
  }

  IPC::MessagePointer IPCClientPrivate::qbIPCSendRecvMessage(IPC::MessageType& message)
  {
    USBGUARD_LOG(Trace) << "message=" << &message;

    if (!isConnected()) {
      throw Exception("IPC send/recv", "connection", "Not connected");
    }

    const uint64_t id = generateMessageID();
    IPC::setMessageHeaderID(message, id);
    std::string payload;
    message.SerializeToString(&payload);
    struct qb_ipc_request_header hdr;
    hdr.id = QB_IPC_MSG_USER_START + IPC::messageTypeNameToNumber(message.GetTypeName());
    hdr.size = sizeof hdr + payload.size();
    struct iovec iov[2];
    iov[0].iov_base = &hdr;
    iov[0].iov_len = sizeof hdr;
    iov[1].iov_base = (void*)payload.data();
    iov[1].iov_len = payload.size();
    /* Lock the return value slot map */
    std::unique_lock<std::mutex> return_map_lock(_return_mutex);
    /*
     * Create the promise and future objects.
     * The promise will be fullfiled by the message
     * processing handlers after they process
     * a reply from the server.
     */
    auto& promise = _return_map[id];
    auto future = promise.get_future();
    qb_ipcc_sendv(_qb_conn, iov, 2);
    /*
     * Unlock the return value map so that the message
     * processing handler aren't blocked.
     */
    return_map_lock.unlock();
    /* Wait for some time for the reply to be received */
    const std::chrono::milliseconds timeout_ms(15*1000); /* TODO: make this configurable */
    const bool timed_out = \
      future.wait_for(timeout_ms) == std::future_status::timeout;
    IPC::MessagePointer response;

    if (!timed_out) {
      response = future.get();
    }

    /* Remove the slot from the return value slot map */
    return_map_lock.lock();
    _return_map.erase(id);
    return_map_lock.unlock();

    if (timed_out) {
      throw Exception("IPC send/recv", "connection", "Timed out while waiting for IPC reply");
    }

    /*
     * We might have caused an exception. Check whether
     * that's the case and if true, throw it here.
     */
    if (IPC::isExceptionMessage(response)) {
      throw IPC::IPCExceptionFromMessage(response);
    }

    return response;
  }

  void IPCClientPrivate::processReceiveEvent()
  {
    USBGUARD_LOG(Trace);

    try {
      const std::string buffer = receive();
      process(buffer);
    }
    catch (const IPCException& exception) {
      disconnect(/*exception_initiated=*/true, exception);
    }
    catch (const Exception& exception) {
      const IPCException ipc_exception(exception);
      disconnect(/*exception_initiated=*/true, ipc_exception);
    }
    catch (const std::exception& exception) {
      const IPCException ipc_exception("IPC receive event", "", exception.what());
      disconnect(/*exception_initiated=*/true, ipc_exception);
    }
    catch (...) {
      const IPCException ipc_exception("IPC receive event", "BUG", "Unknown exception");
      disconnect(/*exception_initiated=*/true, ipc_exception);
    }
  }

  std::string IPCClientPrivate::receive()
  {
    USBGUARD_LOG(Trace);
    const size_t buffer_size_max = 1<<20;
    std::string buffer(buffer_size_max, 0);
    const ssize_t recv_size = \
      qb_ipcc_event_recv(_qb_conn, &buffer[0], /*msg_len=*/buffer_size_max, /*ms_timeout=*/500);

    if (recv_size < 0) {
      disconnect();
      throw Exception("IPC receive", "connection", "Receive error");
    }

    if (recv_size < (ssize_t)sizeof(struct qb_ipc_response_header)) {
      disconnect();
      throw Exception("IPC receive", "message", "Message too small");
    }

    USBGUARD_LOG(Debug) << "Received " << recv_size << " bytes";
    buffer.resize((size_t)recv_size);
    return buffer;
  }

  void IPCClientPrivate::process(const std::string& buffer)
  {
    USBGUARD_LOG(Trace) << "buffer=" << &buffer;
    const struct qb_ipc_response_header* hdr = \
        reinterpret_cast<const struct qb_ipc_response_header*>(buffer.data());

    if ((size_t)hdr->size != buffer.size()) {
      disconnect();
      throw Exception("IPC receive", "message header", "Invalid size value");
    }

    if (hdr->id < QB_IPC_MSG_USER_START) {
      disconnect();
      throw Exception("IPC receive", "message header", "Invalid payload type value");
    }

    const uint32_t payload_type = hdr->id - QB_IPC_MSG_USER_START;
    const std::string payload = buffer.substr(sizeof(struct qb_ipc_response_header));
    handleIPCPayload(payload_type, payload);
  }

  void IPCClientPrivate::handleIPCPayload(const uint32_t payload_type, const std::string& payload)
  {
    USBGUARD_LOG(Trace) << "payload_type=" << payload_type
      << " payload=" << &payload;

    try {
      auto& handler = _handlers.at(payload_type);
      auto message = handler.payloadToMessage(payload);
      USBGUARD_LOG(Debug) << "Message: " << message->DebugString();
      handler.run(message);
    }
    catch (const IPCException& exception) {
      throw;
    }
    catch (const std::exception& exception) {
      throw Exception("IPC connection", "message", exception.what());
    }
    catch (...) {
      throw Exception("IPC connection", "message", "Unknown payload type");
    }
  }

  std::string IPCClientPrivate::setParameter(const std::string& name, const std::string& value)
  {
    IPC::setParameter message_out;
    message_out.mutable_request()->set_name(name);
    message_out.mutable_request()->set_value(value);
    auto message_in = qbIPCSendRecvMessage(message_out);
    return message_in->response().value();
  }

  std::string IPCClientPrivate::getParameter(const std::string& name)
  {
    IPC::getParameter message_out;
    message_out.mutable_request()->set_name(name);
    auto message_in = qbIPCSendRecvMessage(message_out);
    return message_in->response().value();
  }

  uint32_t IPCClientPrivate::appendRule(const std::string& rule_spec, uint32_t parent_id, bool permanent)
  {
    IPC::appendRule message_out;
    message_out.mutable_request()->set_rule(rule_spec);
    message_out.mutable_request()->set_parent_id(parent_id);
    message_out.mutable_request()->set_permanent(permanent);
    auto message_in = qbIPCSendRecvMessage(message_out);
    return message_in->response().id();
  }

  void IPCClientPrivate::removeRule(uint32_t id)
  {
    IPC::removeRule message_out;
    message_out.mutable_request()->set_id(id);
    auto message_in = qbIPCSendRecvMessage(message_out);
  }

  const std::vector<Rule> IPCClientPrivate::listRules(const std::string& label)
  {
    IPC::listRules message_out;
    std::vector<Rule> rules;
    message_out.mutable_request()->set_label(label);
    auto message_in = qbIPCSendRecvMessage(message_out);

    for (auto rule_message : message_in->response().rules()) {
      Rule rule = Rule::fromString(rule_message.rule());
      rule.setRuleID(rule_message.id());
      rules.push_back(rule);
    }

    return rules;
  }

  uint32_t IPCClientPrivate::applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent)
  {
    IPC::applyDevicePolicy message_out;
    message_out.mutable_request()->set_id(id);
    message_out.mutable_request()->set_target(Rule::targetToInteger(target));
    message_out.mutable_request()->set_permanent(permanent);
    auto message_in = qbIPCSendRecvMessage(message_out);
    return message_in->response().rule_id();
  }

  const std::vector<Rule> IPCClientPrivate::listDevices(const std::string& query)
  {
    IPC::listDevices message_out;
    message_out.mutable_request()->set_query(query);
    auto message_in = qbIPCSendRecvMessage(message_out);
    std::vector<Rule> devices;

    for (auto rule_message : message_in->response().devices()) {
      Rule device_rule = Rule::fromString(rule_message.rule());
      device_rule.setRuleID(rule_message.id());
      devices.push_back(device_rule);
    }

    return devices;
  }

  bool IPCClientPrivate::checkIPCPermissions(const IPCServer::AccessControl::Section& section,
    const IPCServer::AccessControl::Privilege& privilege)
  {
    IPC::checkIPCPermissions message_out;
    message_out.mutable_request()->set_uid(getuid());
    message_out.mutable_request()->set_gid(getgid());
    message_out.mutable_request()->set_section(
      IPCServer::AccessControl::sectionToString(section)
    );
    message_out.mutable_request()->set_privilege(
      IPCServer::AccessControl::privilegeToString(privilege)
    );
    auto message_in = qbIPCSendRecvMessage(message_out);
    return message_in->response().permit();
  }

  void IPCClientPrivate::handleMethodResponse(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out)
  {
    (void)message_out;
    const uint64_t id = IPC::getMessageHeaderID(*message_in);

    try {
      auto& return_promise = _return_map.at(id);
      return_promise.set_value(std::move(message_in));
    }
    catch (...) {
      throw IPCException("IPC method response", "message", "Unexpected method call response", id);
    }
  }

  void IPCClientPrivate::handleException(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out)
  {
    (void)message_out;
    const IPC::Exception* const exception = \
      reinterpret_cast<const IPC::Exception*>(message_in.get());

    if (exception->has_request_id()) {
      const uint64_t id = exception->request_id();

      if (id > 0) {
        try {
          auto& return_promise = _return_map.at(id);
          return_promise.set_value(std::move(message_in));
        }
        catch (...) {
          throw IPCException("IPC exception response", "message", "Unexpected method response exception", id);
        }
      }
    }

    _p_instance.ExceptionMessage(exception->context(),
      exception->object(),
      exception->reason());
  }

  void IPCClientPrivate::handleDevicePresenceChangedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out)
  {
    (void)message_out;
    const IPC::DevicePresenceChangedSignal* const signal =\
      reinterpret_cast<const IPC::DevicePresenceChangedSignal*>(message_in.get());
    _p_instance.DevicePresenceChanged(signal->id(),
      DeviceManager::eventTypeFromInteger(signal->event()),
      Rule::targetFromInteger(signal->target()),
      signal->device_rule());
  }

  void IPCClientPrivate::handleDevicePolicyChangedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out)
  {
    (void)message_out;
    const IPC::DevicePolicyChangedSignal* const signal = \
      reinterpret_cast<const IPC::DevicePolicyChangedSignal*>(message_in.get());
    _p_instance.DevicePolicyChanged(signal->id(),
      Rule::targetFromInteger(signal->target_old()),
      Rule::targetFromInteger(signal->target_new()),
      signal->device_rule(),
      signal->rule_id());
  }

  void IPCClientPrivate::handleDevicePolicyAppliedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out)
  {
    (void)message_out;
    const IPC::DevicePolicyAppliedSignal* const signal = \
      reinterpret_cast<const IPC::DevicePolicyAppliedSignal*>(message_in.get());
    _p_instance.DevicePolicyApplied(signal->id(),
      Rule::targetFromInteger(signal->target_new()),
      signal->device_rule(),
      signal->rule_id());
  }

  void IPCClientPrivate::handlePropertyParameterChangedSignal(IPC::MessagePointer& message_in, IPC::MessagePointer& message_out)
  {
    (void)message_out;
    const IPC::PropertyParameterChangedSignal* const signal = \
      reinterpret_cast<const IPC::PropertyParameterChangedSignal*>(message_in.get());
    _p_instance.PropertyParameterChanged(signal->name(),
      signal->value_old(),
      signal->value_new());
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
