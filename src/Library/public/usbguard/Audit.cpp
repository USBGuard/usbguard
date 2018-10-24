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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Audit.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <Common/Utility.hpp>

namespace usbguard
{

  AuditIdentity::AuditIdentity()
  {
    _uid = getuid();
    _pid = getpid();
  }

  AuditIdentity::AuditIdentity(uid_t uid, pid_t pid)
  {
    _uid = uid;
    _pid = pid;
  }

  uid_t AuditIdentity::uid() const
  {
    return _uid;
  }

  pid_t AuditIdentity::pid() const
  {
    return _pid;
  }

  std::string AuditIdentity::toString() const
  {
    std::string identity_string;
    identity_string.append("{ uid=");
    identity_string.append(numberToString(uid()));
    identity_string.append(" pid=");
    identity_string.append(numberToString(pid()));
    identity_string.append(" }");
    return identity_string;
  }

  AuditEvent::AuditEvent(const AuditIdentity& identity, std::shared_ptr<AuditBackend>& backend)
    : _commited(false),
      _identity(identity),
      _backend(backend)
  {
  }

  AuditEvent::AuditEvent(AuditEvent&& event)
    : _commited(event._commited),
      _identity(std::move(event._identity)),
      _backend(std::move(event._backend)),
      _keys(std::move(event._keys))
  {
    /*
     * Mark the source event as commited so that
     * when it gets destructed, it won't be commited
     * to the backend.
     */
    event.setCommited(true);
  }

  AuditEvent::~AuditEvent()
  {
    /*
     * If the event was not commited explicitly, assume
     * that that the result is a failure.
     */
    if (!_commited) {
      failure();
    }
  }

  void AuditEvent::commit(const std::string& result)
  {
    setKey("result", result);

    /* Skip backend commit if there's no backend set */
    if (_backend != nullptr) {
      _backend->commit(*this);
    }

    /*
     * Mark as commited even if it wasn't actually commited
     * because of no valid backend. Otherwise the destructor
     * would try to commit it again as failed.
     */
    setCommited(true);
  }

  void AuditEvent::success()
  {
    commit("SUCCESS");
  }

  void AuditEvent::failure()
  {
    commit("FAILURE");
  }

  const AuditIdentity& AuditEvent::identity() const
  {
    return _identity;
  }

  const AuditEvent::Keys& AuditEvent::keys() const
  {
    return _keys;
  }

  void AuditEvent::setCommited(bool state)
  {
    _commited = state;
  }

  void AuditEvent::setKey(const std::string& key, const std::string& value)
  {
    _keys.emplace(key, value);
  }

  AuditBackend::AuditBackend()
  {
  }

  AuditBackend::~AuditBackend()
  {
  }

  void AuditBackend::commit(const AuditEvent& event)
  {
    std::unique_lock<std::mutex> lock(_mutex);
    write(event);
  }

  Audit::Audit(const AuditIdentity& identity)
    : _identity(identity), _hide_pii(false)
  {
  }

  void Audit::setBackend(std::unique_ptr<AuditBackend> backend)
  {
    _backend = std::shared_ptr<AuditBackend>(std::move(backend));
  }

  void Audit::setHidePII(bool hide_pii)
  {
    _hide_pii = hide_pii;
  }

  AuditEvent Audit::policyEvent(std::shared_ptr<Rule> rule, Policy::EventType event)
  {
    return policyEvent(_identity, rule, event);
  }

  AuditEvent Audit::policyEvent(std::shared_ptr<Rule> new_rule, std::shared_ptr<Rule> old_rule)
  {
    return policyEvent(_identity, new_rule, old_rule);
  }

  AuditEvent Audit::policyEvent(std::shared_ptr<Device> device, Policy::EventType event)
  {
    return policyEvent(_identity, device, event);
  }

  AuditEvent Audit::policyEvent(std::shared_ptr<Device> device, Rule::Target old_target, Rule::Target new_target)
  {
    return policyEvent(_identity, device, old_target, new_target);
  }

  AuditEvent Audit::deviceEvent(std::shared_ptr<Device> device, DeviceManager::EventType event)
  {
    return deviceEvent(_identity, device, event);
  }

  AuditEvent Audit::deviceEvent(std::shared_ptr<Device> new_device, std::shared_ptr<Device> old_device)
  {
    return deviceEvent(_identity, new_device, old_device);
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, std::shared_ptr<Rule> rule, Policy::EventType event_type)
  {
    AuditEvent event(identity, _backend);
    event.setKey("type", std::string("Policy.") + Policy::eventTypeToString(event_type));
    event.setKey("rule.id", numberToString(rule->getRuleID()));
    event.setKey("rule", rule->toString(false, _hide_pii));
    return event;
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, std::shared_ptr<Rule> new_rule, std::shared_ptr<Rule> old_rule)
  {
    AuditEvent event(identity, _backend);
    event.setKey("type", std::string("Policy.") + Policy::eventTypeToString(Policy::EventType::Update));
    event.setKey("rule.id", numberToString(old_rule->getRuleID()));
    event.setKey("rule.old", old_rule->toString(false, _hide_pii));
    event.setKey("rule.new", new_rule->toString(false, _hide_pii));
    return event;
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, Policy::EventType event_type)
  {
    AuditEvent event(identity, _backend);
    event.setKey("type", std::string("Policy.Device.") + Policy::eventTypeToString(event_type));
    event.setKey("target", Rule::targetToString(device->getTarget()));
    event.setKey("device.system_name", device->getSystemName());
    event.setKey("device.rule", device->getDeviceRule()->toString(false, _hide_pii));
    return event;
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, std::shared_ptr<Device> device, Rule::Target old_target,
    Rule::Target new_target)
  {
    AuditEvent event(identity, _backend);
    event.setKey("type", std::string("Policy.Device.") + Policy::eventTypeToString(Policy::EventType::Update));
    event.setKey("target.old", Rule::targetToString(old_target));
    event.setKey("target.new", Rule::targetToString(new_target));
    event.setKey("device.system_name", device->getSystemName());
    event.setKey("device.rule", device->getDeviceRule()->toString(false, _hide_pii));
    return event;
  }

  AuditEvent Audit::deviceEvent(const AuditIdentity& identity, std::shared_ptr<Device> device,
    DeviceManager::EventType event_type)
  {
    AuditEvent event(identity, _backend);
    event.setKey("type", std::string("Device.") + DeviceManager::eventTypeToString(event_type));
    event.setKey("device.system_name", device->getSystemName());
    event.setKey("device.rule", device->getDeviceRule()->toString(false, _hide_pii));
    return event;
  }

  AuditEvent Audit::deviceEvent(const AuditIdentity& identity, std::shared_ptr<Device> new_device,
    std::shared_ptr<Device> old_device)
  {
    AuditEvent event(identity, _backend);
    event.setKey("type", std::string("Device.") + DeviceManager::eventTypeToString(DeviceManager::EventType::Update));
    event.setKey("device.system_name", new_device->getSystemName());
    event.setKey("device.rule.old", old_device->getDeviceRule()->toString(false, _hide_pii));
    event.setKey("device.rule.new", new_device->getDeviceRule()->toString(false, _hide_pii));
    return event;
  }
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
