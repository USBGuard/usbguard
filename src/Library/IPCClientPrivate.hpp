#pragma once
#include <build-config.h>
#include "IPCClient.hpp"
#include "Common/Thread.hpp"
#include "Typedefs.hpp"
#include "Common/JSON.hpp"

#include <map>
#include <mutex>
#include <future>

#include <qb/qbipcc.h>
#include <qb/qbloop.h>

namespace usbguard {
  class IPCClientPrivate
  {
  public:
    IPCClientPrivate(IPCClient& p_instance);
    ~IPCClientPrivate();

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

  protected:
    void thread();
    void stop();

    json qbIPCSendRecvJSON(const json& jval);
    bool isExceptionJSON(const json& jval) const;

    const json receiveOne();
    void processOne(const json& jobj);
    void processExceptionJSON(const json& jobj);
    void processSignalJSON(const json& jobj);
    void processMethodReturnJSON(const json& jobj);
    void processMethodCallJSON(const json& jobj);
    void processReturnValue(const json& jobj);

  private:
    IPCClient& _p_instance;
    qb_loop_t *_qb_loop;
    qb_ipcc_connection_t *_qb_conn;
    int _qb_conn_fd;
    int _eventfd;
    Thread<IPCClientPrivate> _thread;
    std::mutex _rv_map_mutex;
    std::map<uint64_t, std::promise<json> > _rv_map;
  };

} /* namespace usbguard */
