#include "IPCClientPrivate.hpp"

namespace usbguard
{
  IPCClient::IPCClient()
  {
    d_pointer = new IPCClientPrivate(*this);
    return;
  }

  IPCClient::~IPCClient()
  {
    delete d_pointer;
  }

  void IPCClient::connect()
  {
    d_pointer->connect();
    return;
  }

  void IPCClient::disconnect()
  {
    d_pointer->disconnect();
    return;
  }

  bool IPCClient::isConnected() const
  {
    return d_pointer->isConnected();
  }

  void IPCClient::wait()
  {
    d_pointer->wait();
    return;
  }

  void IPCClient::processEvent()
  {
    d_pointer->processEvent();
    return;
  }

  uint32_t IPCClient::appendRule(const std::string& rule_spec, uint32_t parent_seqn, uint32_t timeout_sec)
  {
    return d_pointer->appendRule(rule_spec, parent_seqn, timeout_sec);
  }

  void IPCClient::removeRule(uint32_t seqn)
  {
    d_pointer->removeRule(seqn);
    return;
  }

  void IPCClient::allowDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    d_pointer->allowDevice(seqn, append, timeout_sec);
    return;
  }

  void IPCClient::blockDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    d_pointer->blockDevice(seqn, append, timeout_sec);
    return;
  }

  void IPCClient::rejectDevice(uint32_t seqn, bool append, uint32_t timeout_sec)
  {
    d_pointer->rejectDevice(seqn, append, timeout_sec);
    return;
  }
} /* namespace usbguard */
