#pragma once
#include <Typedefs.hpp>
#include <IPC.hpp>

namespace usbguard
{
  class IPCClientPrivate;
  class IPCClient : public IPC
  {
  public:
    IPCClient();
    ~IPCClient();

    void connect();
    void disconnect();
    bool isConnected() const;
    void wait();
    void processEvent();

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_seqn, uint32_t timeout_sec);
    void removeRule(uint32_t seqn);
    void allowDevice(uint32_t seqn, bool append, uint32_t timeout_sec);
    void blockDevice(uint32_t seqn, bool append, uint32_t timeout_sec);
    void rejectDevice(uint32_t seqn, bool append, uint32_t timeout_sec);

    virtual void IPCConnected() {}
    virtual void IPCDisconnected() {}

  private:
    IPCClientPrivate* d_pointer;
  };

} /* namespace usbguard */
