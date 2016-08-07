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
#include "IPCClientPrivate.hpp"

namespace usbguard
{
  IPCClient::IPCClient(bool connected)
  {
    d_pointer = new IPCClientPrivate(*this, connected);
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

  uint32_t IPCClient::appendRule(const std::string& rule_spec, uint32_t parent_id, uint32_t timeout_sec)
  {
    return d_pointer->appendRule(rule_spec, parent_id, timeout_sec);
  }

  void IPCClient::removeRule(uint32_t id)
  {
    d_pointer->removeRule(id);
    return;
  }

  const RuleSet IPCClient::listRules()
  {
    return d_pointer->listRules();
  }

  void IPCClient::allowDevice(uint32_t id, bool permanent, uint32_t timeout_sec)
  {
    d_pointer->allowDevice(id, permanent, timeout_sec);
    return;
  }

  void IPCClient::blockDevice(uint32_t id, bool permanent, uint32_t timeout_sec)
  {
    d_pointer->blockDevice(id, permanent, timeout_sec);
    return;
  }

  void IPCClient::rejectDevice(uint32_t id, bool permanent, uint32_t timeout_sec)
  {
    d_pointer->rejectDevice(id, permanent, timeout_sec);
    return;
  }

  const std::vector<Rule> IPCClient::listDevices(const std::string& query)
  {
    return d_pointer->listDevices(query);
  }
} /* namespace usbguard */
