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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "IPCClientPrivate.hpp"
#include "Common/Utility.hpp"

namespace usbguard
{
  IPCClient::IPCClient(bool connected)
    : d_pointer(usbguard::make_unique<IPCClientPrivate>(*this, connected))
  {
  }

  IPCClient::~IPCClient() = default;

  void IPCClient::connect()
  {
    d_pointer->connect();
  }

  void IPCClient::disconnect()
  {
    d_pointer->disconnect(/*do_wait*/true);
  }

  bool IPCClient::isConnected() const
  {
    return d_pointer->isConnected();
  }

  void IPCClient::wait()
  {
    d_pointer->wait();
  }

  std::string IPCClient::setParameter(const std::string& name, const std::string& value)
  {
    return d_pointer->setParameter(name, value);
  }

  std::string IPCClient::getParameter(const std::string& name)
  {
    return d_pointer->getParameter(name);
  }

  uint32_t IPCClient::appendRule(const std::string& rule_spec, uint32_t parent_id, bool permanent)
  {
    return d_pointer->appendRule(rule_spec, parent_id, permanent);
  }

  void IPCClient::removeRule(uint32_t id)
  {
    d_pointer->removeRule(id);
  }

  const std::vector<Rule> IPCClient::listRules(const std::string& label)
  {
    return d_pointer->listRules(label);
  }

  uint32_t IPCClient::applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent)
  {
    return d_pointer->applyDevicePolicy(id, target, permanent);
  }

  const std::vector<Rule> IPCClient::listDevices(const std::string& query)
  {
    return d_pointer->listDevices(query);
  }

  bool IPCClient::checkIPCPermissions(const IPCServer::AccessControl::Section& section,
    const IPCServer::AccessControl::Privilege& privilege)
  {
    return d_pointer->checkIPCPermissions(section, privilege);
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
