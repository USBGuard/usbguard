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
    IPCClientPrivate(IPCClient& p_instance, bool connected);
    ~IPCClientPrivate();

    void connect();
    void disconnect(bool exception_initiated, const IPCException& exception);
    void disconnect();
    bool isConnected() const;
    void wait();
    void processEvent();

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id, uint32_t timeout_sec);
    void removeRule(uint32_t id);
    const RuleSet listRules();

    void allowDevice(uint32_t id, bool append, uint32_t timeout_sec);
    void blockDevice(uint32_t id, bool append, uint32_t timeout_sec);
    void rejectDevice(uint32_t id, bool append, uint32_t timeout_sec);
    const std::vector<Rule> listDevices(const std::string& query);

  protected:
    void destruct();
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
