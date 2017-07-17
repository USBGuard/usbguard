//
// Copyright (C) 2017 Red Hat, Inc.
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

#include "../Policy.hpp"

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "Logger.hpp"
#include "Rule.hpp"
#include "Typedefs.hpp"

#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <unistd.h>
#include <sys/types.h>

namespace usbguard
{
  class DLL_PUBLIC AuditIdentity
  {
  public:
    AuditIdentity();
    AuditIdentity(uid_t uid, pid_t pid);

    uid_t uid() const;
    pid_t pid() const;

    std::string toString() const;

  private:
    uid_t _uid;
    pid_t _pid;
  };

  class AuditBackend;

  class DLL_PUBLIC AuditEvent
  {
    AuditEvent(const AuditIdentity& identity, std::shared_ptr<AuditBackend>& backend);
  public:
    using Keys = std::unordered_map<std::string, std::string>;

    AuditEvent(AuditEvent&& event);
    AuditEvent(const AuditEvent& event) = delete;
    ~AuditEvent();

    void success();
    void failure();

    const AuditIdentity& identity() const;
    const Keys& keys() const;

  private:
    void commit(const std::string& result);
    void setCommited(bool state);
    void setKey(const std::string& key, const std::string& value);

    bool _commited;

    AuditIdentity _identity;
    std::shared_ptr<AuditBackend> _backend;
    Keys _keys;

    friend class Audit;
  };

  class DLL_PUBLIC AuditBackend
  {
  public:
    AuditBackend();
    virtual ~AuditBackend();

    virtual void write(const AuditEvent& event) = 0;
    void commit(const AuditEvent& event);

  private:
    std::mutex _mutex;
  };

  class DLL_PUBLIC Audit
  {
  public:
    Audit(const AuditIdentity& identity);

    void setBackend(std::unique_ptr<AuditBackend> backend);

    AuditEvent policyEvent(std::shared_ptr<Rule> rule, Policy::EventType event);
    AuditEvent policyEvent(std::shared_ptr<Rule> new_rule, std::shared_ptr<Rule> old_rule);
    AuditEvent policyEvent(std::shared_ptr<Device> device, Policy::EventType event);
    AuditEvent policyEvent(std::shared_ptr<Device> device, Rule::Target old_target, Rule::Target new_target);

    AuditEvent deviceEvent(std::shared_ptr<Device> device, DeviceManager::EventType event);
    AuditEvent deviceEvent(std::shared_ptr<Device> new_device, std::shared_ptr<Device> old_device);

    /*
     * Audit policy changes:
     *   - rule append
     *   - rule remove
     *   - rule update
     *   - policy parameter change
     *
     * Audit data:
     *   - who: uid + pid
     *   - when: time
     *   - what: append, remove, update
     *   - update: old, new
     */
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Rule> rule, Policy::EventType event);
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Rule> new_rule, std::shared_ptr<Rule> old_rule);
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, Policy::EventType event);
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, Rule::Target old_target,
      Rule::Target new_target);

    /*
     * Audit device changes:
     *   - device insertion
     *   - device removal
     *   - device authorization target change
     *
     * Audit data:
     *   - who: uid + pid
     *   - when: time
     *   - what: insert, remove, authorization target
     *   - change: old, new
     */
    AuditEvent deviceEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, DeviceManager::EventType event);
    AuditEvent deviceEvent(const AuditIdentity& identity, std::shared_ptr<Device> new_device, std::shared_ptr<Device> old_device);

  private:
    AuditIdentity _identity;
    std::shared_ptr<AuditBackend> _backend;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
