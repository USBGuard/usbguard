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

#include "Audit.hpp"

#include <Common/Utility.hpp>
#include <Exception.hpp>
#include <Logger.hpp>

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

  std::string AuditIdentity::toString() const
  {
    std::string identity_string;

    identity_string.append("{ uid=");
    identity_string.append(numberToString(_uid));
    identity_string.append(" pid=");
    identity_string.append(numberToString(_pid));
    identity_string.append(" }");

    return identity_string;
  }

  AuditEvent::AuditEvent(const AuditIdentity& identity)
    : _confirmed(false),
      _identity(identity)
  {

  }

  AuditEvent::AuditEvent(AuditEvent&& event)
    : _confirmed(event._confirmed),
      _identity(std::move(event._identity)),
      _message(std::move(event._message))
  {
    event.setConfirmed(true);
  }

  AuditEvent::~AuditEvent()
  {
    if (!_confirmed) {
      failure();
    }
  }

  void AuditEvent::confirm(const std::string& result)
  {
    USBGUARD_LOG(Audit) << "result=" << result \
      << " identity=" << _identity.toString() \
      << " " << _message;
    setConfirmed(true);
  }

  void AuditEvent::success()
  {
    confirm("SUCCESS");
  }

  void AuditEvent::failure()
  {
    confirm("FAILURE");
  }

  void AuditEvent::setConfirmed(bool state)
  {
    _confirmed = state;
  }

  std::string& AuditEvent::refMessage()
  {
    return _message;
  }

  Audit::Audit(const AuditIdentity& identity)
    : _identity(identity)
  {

  }

  AuditEvent Audit::policyEvent(Pointer<Rule> rule, Policy::EventType event)
  {
    return policyEvent(_identity, rule, event);
  }
  
  AuditEvent Audit::policyEvent(Pointer<Rule> new_rule, Pointer<Rule> old_rule)
  {
    return policyEvent(_identity, new_rule, old_rule);
  }

  AuditEvent Audit::policyEvent(Pointer<Device> device, Policy::EventType event)
  {
    return policyEvent(_identity, device, event);
  }

  AuditEvent Audit::policyEvent(Pointer<Device> device, Rule::Target old_target, Rule::Target new_target)
  {
    return policyEvent(_identity, device, old_target, new_target);
  }

  AuditEvent Audit::deviceEvent(Pointer<Device> device, DeviceManager::EventType event)
  {
    return deviceEvent(_identity, device, event);
  }
  
  AuditEvent Audit::deviceEvent(Pointer<Device> new_device, Pointer<Device> old_device)
  {
    return deviceEvent(_identity, new_device, old_device);
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, Pointer<Rule> rule, Policy::EventType event)
  {
    AuditEvent audit_event(identity);
    auto& message = audit_event.refMessage();

    message += "type=Policy.";
    message += Policy::eventTypeToString(event);

    message += " rule.id=";
    message += numberToString(rule->getRuleID());

    message += " rule='";
    message += rule->toString();
    message += "'";

    return audit_event;
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, Pointer<Rule> new_rule, Pointer<Rule> old_rule)
  {
    AuditEvent audit_event(identity);
    auto& message = audit_event.refMessage();

    message += "type=Policy.";
    message += Policy::eventTypeToString(Policy::EventType::Update);

    message += " rule.id=";
    message += numberToString(old_rule->getRuleID());

    message += " rule.old='";
    message += old_rule->toString();
    message += "'";
    
    message += " rule.new='";
    message += new_rule->toString();
    message += "'";

    return audit_event;
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, Pointer<Device> device, Policy::EventType event)
  {
    AuditEvent audit_event(identity);
    auto& message = audit_event.refMessage();

    message += "type=Policy.Device.";
    message += Policy::eventTypeToString(event);

    message += " target=";
    message += Rule::targetToString(device->getTarget());

    message += " device='";
    message += device->getDeviceRule()->toString();
    message += "'";

    return audit_event;
  }

  AuditEvent Audit::policyEvent(const AuditIdentity& identity, Pointer<Device> device, Rule::Target old_target, Rule::Target new_target)
  {
    AuditEvent audit_event(identity);
    auto& message = audit_event.refMessage();

    message += "type=Policy.Device.";
    message += Policy::eventTypeToString(Policy::EventType::Update);
    
    message += " target.old=";
    message += Rule::targetToString(old_target);

    message += " target.new=";
    message += Rule::targetToString(new_target);
    
    message += " device='";
    message += device->getDeviceRule()->toString();
    message += "'";

    return audit_event;
  }

  AuditEvent Audit::deviceEvent(const AuditIdentity& identity, Pointer<Device> device, DeviceManager::EventType event)
  {
    AuditEvent audit_event(identity);
    auto& message = audit_event.refMessage();

    message += "type=Device.";
    message += DeviceManager::eventTypeToString(event);

    message += " device='";
    message += device->getDeviceRule()->toString();
    message += "'";

    return audit_event;
  }

  AuditEvent Audit::deviceEvent(const AuditIdentity& identity, Pointer<Device> new_device, Pointer<Device> old_device)
  {
    AuditEvent audit_event(identity);
    auto& message = audit_event.refMessage();

    message += "type=Device.";
    message += DeviceManager::eventTypeToString(DeviceManager::EventType::Update);
    
    message += " device.old='";
    message += old_device->getDeviceRule()->toString();
    message += "'";

    message += " device.new='";
    message += new_device->getDeviceRule()->toString();
    message += "'";

    return audit_event;
  }
} /* namespace usbguard */
