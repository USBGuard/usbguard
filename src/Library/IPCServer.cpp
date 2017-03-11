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
#include "IPCServerPrivate.hpp"

namespace usbguard
{
  IPCServer::AccessControl::AccessControl()
  {
    /* Empty: no privileges */
  }

  IPCServer::AccessControl::AccessControl(IPCServer::AccessControl::Section section, IPCServer::AccessControl::Privilege privilege)
  {
    setPrivilege(section, privilege);
  }

  bool IPCServer::AccessControl::hasPrivilege(IPCServer::AccessControl::Section section, IPCServer::AccessControl::Privilege privilege) const
  {
    (void)section;
    (void)privilege;
    /*
     * TODO: No fine-grained access control at this moment.
     */
    return true;
  }

  void IPCServer::AccessControl::setPrivilege(IPCServer::AccessControl::Section section, IPCServer::AccessControl::Privilege privilege)
  {
    /*
     * TODO: Setting fine-grained access control not implemented yet.
     */
    (void)section;
    (void)privilege;
  }

  IPCServer::IPCServer()
  {
    d_pointer = new IPCServerPrivate(*this);
  }

  IPCServer::~IPCServer()
  {
    delete d_pointer;
  }

  void IPCServer::start()
  {
    d_pointer->start();
  }

  void IPCServer::stop()
  {
    d_pointer->stop();
  }

  void IPCServer::DevicePresenceChanged(uint32_t id,
                                        DeviceManager::EventType event,
                                        Rule::Target target,
                                        const std::string& device_rule)
  {
    d_pointer->DevicePresenceChanged(id, event, target, device_rule);
  }

  void IPCServer::DevicePolicyChanged(uint32_t id,
                                      Rule::Target target_old,
                                      Rule::Target target_new,
                                      const std::string& device_rule,
                                      uint32_t rule_id)
  {
    d_pointer->DevicePolicyChanged(id, target_old, target_new, device_rule, rule_id);
  }

  void IPCServer::ExceptionMessage(const std::string& context,
                                   const std::string& object,
                                   const std::string& reason)
  {
    d_pointer->ExceptionMessage(context, object, reason);
  }

  void IPCServer::addAllowedUID(uid_t uid)
  {
    d_pointer->addAllowedUID(uid);
  }

  void IPCServer::addAllowedGID(gid_t gid)
  {
    d_pointer->addAllowedGID(gid);
  }

  void IPCServer::addAllowedUsername(const std::string& username)
  {
    d_pointer->addAllowedUsername(username);
  }

  void IPCServer::addAllowedGroupname(const std::string& groupname)
  {
    d_pointer->addAllowedGroupname(groupname);
  }
} /* namespace usbguard */
