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

#include "Policy.hpp"

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
  /**
   * @brief Represents the audit identity.
   */
  class DLL_PUBLIC AuditIdentity
  {
  public:
    /**
     * @brief Constructs audit identity with real user ID and proccess ID
     * of the calling proccess.
     */
    AuditIdentity();

    /**
     * @brief Constructs audit identity with given UID and PID.
     *
     * @param uid Real user ID.
     * @param pid Proccess ID.
     */
    AuditIdentity(uid_t uid, pid_t pid);

    /**
     * @brief Returns real user ID of this audit identity.
     *
     * @return Real user ID of this audit identity.
     */
    uid_t uid() const;

    /**
     * @brief Returns proccess ID of this audit identity.
     *
     * @return Proccess ID of this audit identity.
     */
    pid_t pid() const;

    /**
     * @brief Returns string representation of this audit identity.
     *
     * Format: { uid=<UID> pid=<PID> }
     *
     * @return String representation of this audit identity.
     */
    std::string toString() const;

  private:
    uid_t _uid; /**< Real user ID. */
    pid_t _pid; /**< Proccess ID. */
  };

  class AuditBackend;

  /**
   * @brief Describes policy or device event and its result that
   * should be audited using given audit identity and backend.
   */
  class DLL_PUBLIC AuditEvent
  {
    /**
     * @brief Constructs new audit event with given
     * \link AuditIdentity AuditIdentity\endlink and backend.
     *
     * Audit event is not commited by default.
     *
     * @param identity \link AuditIdentity AuditIdentity \endlink.
     * @param backend Audit backend that will be used to commit this
     * audit event.
     */
    AuditEvent(const AuditIdentity& identity, std::shared_ptr<AuditBackend>& backend);
  public:
    using Keys = std::unordered_map<std::string, std::string>;

    /**
     * @brief Constructs audit event from existing audit event.
     *
     * @param event Event from which this object will be constructed.
     */
    AuditEvent(AuditEvent&& event);

    AuditEvent(const AuditEvent& event) = delete;

    /**
     * @brief Destructs this audit event.
     *
     * If audit event was not commited, then it is assumed it failed.
     *
     * @see \link failure() failure()\endlink
     */
    ~AuditEvent();

    /**
     * @brief Commits this audit event with result \"SUCCESS\".
     */
    void success();

    /**
     * @brief Commits this audit event with result \"FAILURE\".
     */
    void failure();

    /**
     * @brief Returns \link AuditIdentity AuditIdentity\endlink.
     *
     * @return \link AuditIdentity AuditIdentity\endlink.
     */
    const AuditIdentity& identity() const;

    /**
     * @brief Returns map of key\=value pairs describing what happened/changed
     * that should be audited.
     *
     * @return Map of key\=value pairs.
     */
    const Keys& keys() const;

  private:
    /**
     * @brief Commits this audit event.
     *
     * Inserts result\=\<result\> into audit event keys and tries to commit
     * this audit event using backend. If there is no backend set, then it
     * silently fails (nothing will be commited, but this audit event will get
     * marked as commited).
     *
     * @param result Result of the event that is being audited.
     */
    void commit(const std::string& result);

    /**
     * @brief Sets whether this audit event has been commited or not.
     *
     * @param state If set to true, then it means that this audit event has
     * been commited.
     */
    void setCommited(bool state);

    /**
     * Inserts key\=value pair into audit event keys.
     *
     * @param key Key describes what changed.
     * @param value Value of the key.
     */
    void setKey(const std::string& key, const std::string& value);

    bool _commited; /**< True if this audit event has been commited,
                      false otherwise. */
    AuditIdentity _identity; /**< \link AuditIdentity AuditIdentity \endlink. */
    std::shared_ptr<AuditBackend> _backend; /**< Audit backend that will be
                                              used to commit this audit event. */
    Keys _keys; /**< Map of key=value pairs describing what happened/changed
                  that should be audited. */

    friend class Audit;
  };

  /**
   * @brief Commits and writes audit events into log.
   */
  class DLL_PUBLIC AuditBackend
  {
  public:
    /**
     * @brief Default constructor.
     */
    AuditBackend();

    /**
     * @brief Default destructor.
     */
    virtual ~AuditBackend();

    /**
     * @brief Writes given \link AuditEvent AuditEvent\endlink into log.
     *
     * @param event \link AuditEvent AuditEvent\endlink that should be written
     * into log.
     */
    virtual void write(const AuditEvent& event) = 0;

    /**
     * @brief Commits given \link AuditEvent AuditEvent\endlink.
     *
     * Locks backend mutex and writes given audit event into log.
     *
     * @param event \link AuditEvent AuditEvent\endlink to commit.
     * @see write()
     */
    void commit(const AuditEvent& event);

  private:
    std::mutex _mutex; /**< \link AuditBackend Audit backend\endlink lock. */
  };

  /**
   * @brief Generates audit events for given policy or device events.
   */
  class DLL_PUBLIC Audit
  {
  public:
    /**
     * @brief Constructs new audit object with given \link AuditIdentity
     * AuditIdentity\endlink and hidePII set to false.
     *
     * @param identity Audit identity.
     */
    Audit(const AuditIdentity& identity);

    /**
     * @brief Sets \link AuditBackend AuditBackend\endlink that will be used
     * to commit generated audit events.
     *
     * @param backend \link AuditBackend AuditBackend\endlink to use for
     * commiting audit events.
     */
    void setBackend(std::unique_ptr<AuditBackend> backend);

    /**
     * @brief Sets whether personally identifiable information such as device
     * serial numbers and hashes of the descriptors (which include the serial
     * number) should be excluded from audit entries.
     *
     * @param hide_pii If true then personally identifiable information will
     * be excluded from audit entries.
     */
    void setHidePII(bool hide_pii);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.\<evenType\>
     *   - rule\.id\=\<rule ID\>
     *   - rule\=\<rule\>
     *
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
     *
     * @param rule Rule to audit.
     * @param event Event to audit.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(std::shared_ptr<Rule> rule, Policy::EventType event);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.Update
     *   - rule\.id\=\<old rule ID\>
     *   - rule\.old\=\<old rule\>
     *   - rule\.new\=\<new rule\>
     *
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
     *
     * @param new_rule New rule to audit.
     * @param old_rule Old rule to audit.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(std::shared_ptr<Rule> new_rule, std::shared_ptr<Rule> old_rule);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.Device\.\<evenType\>
     *   - target\=\<device rule target\>
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\=\<device rule\>
     *
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
     *
     * @param device Device where the event occured.
     * @param event Event to audit.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(std::shared_ptr<Device> device, Policy::EventType event);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.Device\.Update
     *   - target\.old\=\<old rule target\>
     *   - target\.new\=\<new rule target\>
     *   - device\.system_name\=<device system name\>
     *   - device\.rule\=\<device rule\>
     *
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
     *
     * @param device Device where the rule target has changed.
     * @param old_target Old rule target.
     * @param new_target New rule target.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(std::shared_ptr<Device> device, Rule::Target old_target, Rule::Target new_target);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * device \link DeviceManager::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Device\.\<event type\>
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\=\<device rule\>
     *
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
     *
     * @param device Device where the event occured.
     * @param event Device event type.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent deviceEvent(std::shared_ptr<Device> device, DeviceManager::EventType event);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * device \link DeviceManager::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Device\.Update
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\.old\=\<old device rule\>
     *   - device\.rule\.new\=\<new device rule\>
     *
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
     *
     * @param new_device New device.
     * @param old_device Old device.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent deviceEvent(std::shared_ptr<Device> new_device, std::shared_ptr<Device> old_device);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.\<evenType\>
     *   - rule\.id\=\<rule ID\>
     *   - rule\=\<rule\>
     *
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
     *
     * @param identity Audit identity.
     * @param rule Rule to audit.
     * @param event Event to audit.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Rule> rule, Policy::EventType event);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.Update
     *   - rule\.id\=\<old rule ID\>
     *   - rule\.old\=\<old rule\>
     *   - rule\.new\=\<new rule\>
     *
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
     *
     * @param identity Audit identity.
     * @param new_rule New rule to audit.
     * @param old_rule Old rule to audit.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Rule> new_rule, std::shared_ptr<Rule> old_rule);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.Device\.\<evenType\>
     *   - target\=\<device rule target\>
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\=\<device rule\>
     *
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
     *
     * @param identity Audit identity.
     * @param device Device where the event occured.
     * @param event Event to audit.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, Policy::EventType event);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * policy \link Policy::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Policy\.Device\.Update
     *   - target\.old\=\<old rule target\>
     *   - target\.new\=\<new rule target\>
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\=\<device rule\>
     *
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
     *
     * @param identity Audit identity.
     * @param device Device where the rule target has changed.
     * @param old_target Old rule target.
     * @param new_target New rule target.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent policyEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, Rule::Target old_target,
      Rule::Target new_target);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * device \link DeviceManager::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Device\.\<event type\>
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\=\<device rule\>
     *
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
     *
     * @param identity Audit identity.
     * @param device Device where the event occured.
     * @param event Device event type.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent deviceEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, DeviceManager::EventType event);

    /**
     * @brief Constructs new \link AuditEvent AuditEvent\endlink for given
     * device \link DeviceManager::EventType event\endlink.
     *
     * Sets audit event keys:
     *   - type\=Device\.Update
     *   - device\.system_name\=\<device system name\>
     *   - device\.rule\.old\=\<old device rule\>
     *   - device\.rule\.new\=\<new device rule\>
     *
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
     *
     * @param identity Audit identity.
     * @param new_device New device.
     * @param old_device Old device.
     * @return \link AuditEvent Audit event\endlink.
     */
    AuditEvent deviceEvent(const AuditIdentity& identity, std::shared_ptr<Device> new_device, std::shared_ptr<Device> old_device);

  private:
    AuditIdentity _identity; /**< \link AuditIdentity AuditIdentity \endlink. */
    std::shared_ptr<AuditBackend> _backend; /**< \link AuditBackend
                                              AuditBackend\endlink that will
                                              be used to commit generated
                                              audit events. */
    bool _hide_pii; /**< Decides whether personally identifiable information
                      (PII) such as device serial numbers and hashes of
                      the descriptors (which include the serial number)
                      will be excluded from audit entries. */
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
