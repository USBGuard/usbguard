#pragma once

#include "IPC.hpp"
#include "Thread.hpp"
#include "JSON.hpp"

#include <map>
#include <future>

#include <qb/qbipcc.h>
#include <qb/qbloop.h>

namespace usbguard
{

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
    void denyDevice(uint32_t seqn, bool append, uint32_t timeout_sec);
    void rejectDevice(uint32_t seqn, bool append, uint32_t timeout_sec);

    virtual void IPCConnected() {}
    virtual void IPCDisconnected() {}

  protected:
    void thread();
    void stop();
    json qbIPCSendRecvJSON(const json& jval);
    bool isException(const json& jval) const;
    const json receiveOne();
    void processOne(const json& jobj);
    void processExceptionJSON(const json& jobj);
    void processSignalJSON(const json& jobj);
    void processMethodReturnJSON(const json& jobj);
    void processMethodCallJSON(const json& jobj);
    void processReturnValue(const json& jobj);

  private:
    qb_loop_t *_qb_loop;
    qb_ipcc_connection_t *_qb_conn;
    int _qb_conn_fd;
    int _eventfd;
    Thread<IPCClient> _thread;
    std::mutex _rv_map_mutex;
    std::map<uint64_t, std::promise<json> > _rv_map;
  };

} /* namespace usbguard */
