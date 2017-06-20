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

#include "Typedefs.hpp"
#include "ConfigFile.hpp"
#include "IPCServer.hpp"
#include "RuleSet.hpp"
#include "Policy.hpp"
#include "Rule.hpp"
#include "Device.hpp"
#include "DeviceManager.hpp"
#include "DeviceManagerHooks.hpp"
#include "Audit.hpp"

#include "Common/Thread.hpp"

#include <mutex>
#include <atomic>
#include <signal.h>

namespace usbguard
{
  class Daemon : public IPCServer, public DeviceManagerHooks
  {
  public:
    enum DevicePolicyMethod {
      Allow,
      Block,
      Reject,
      Keep,
      ApplyPolicy
    };

    static DevicePolicyMethod devicePolicyMethodFromString(const String& policy_string);
    static const std::string devicePolicyMethodToString(DevicePolicyMethod policy);

    Daemon();
    ~Daemon();

    void loadConfiguration(const String& path);
    void loadRules(const String& path);
    void loadIPCAccessControlFiles(const String& path);
    bool loadIPCAccessControlFile(const String& basename, const String& fullpath);
    void checkIPCAccessControlName(const String& basename);
    void parseIPCAccessControlFilename(const String& basename, String * const ptr_user, String * const ptr_group); 

    void setImplicitPolicyTarget(Rule::Target target);
    void setPresentDevicePolicyMethod(DevicePolicyMethod policy);
    void setPresentControllerPolicyMethod(DevicePolicyMethod policy);
    void setInsertedDevicePolicyMethod(DevicePolicyMethod policy);

    /* Start the daemon */
    void run();
    /* Stop the daemon */
    void quit();

    uint32_t assignID();
    uint32_t upsertRule(const std::string& match_spec, const std::string& rule_spec, bool parent_insensitive = false);

    /* IPC methods */
    std::string setParameter(const std::string& name, const std::string& value) override;
    std::string getParameter(const std::string& name) override;

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id) override;
    void removeRule(uint32_t id) override;
    const RuleSet listRules(const std::string& query) override;

    uint32_t applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent) override;
    const std::vector<Rule> listDevices(const std::string& query) override;

    /* Device manager hooks */
    void dmHookDeviceEvent(DeviceManager::EventType event, Pointer<Device> device) override;
    uint32_t dmHookAssignID() override;
    void dmHookDeviceException(const String& message) override;

#define USBGUARD_IPCSERVER_DEFAULT_AC \
  IPCServer::AccessControl(IPCServer::AccessControl::Section::ALL, IPCServer::AccessControl::Privilege::ALL)

    void addIPCAllowedUID(uid_t uid, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedUID(const String& uid_string, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedGID(gid_t gid, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedGID(const String& gid_string, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedUser(const String& user, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedGroup(const String& group, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);

  private:
    void dmApplyDevicePolicy(Pointer<Device> device, Pointer<Rule> matched_rule);
    Pointer<Rule> getInsertedDevicePolicyRule(Pointer<Device> device);
    Pointer<Rule> getPresentDevicePolicyRule(Pointer<Device> device);

    Pointer<Rule> upsertDeviceRule(uint32_t id, Rule::Target target);

    ConfigFile _config;
    Policy _policy;

    String _device_manager_backend;
    Pointer<DeviceManager> _dm;

    std::atomic<Rule::Target> _implicit_policy_target;
    std::atomic<DevicePolicyMethod> _present_device_policy_method;
    std::atomic<DevicePolicyMethod> _present_controller_policy_method;
    std::atomic<DevicePolicyMethod> _inserted_device_policy_method;

    bool _device_rules_with_port;
    bool _restore_controller_device_state;

    AuditIdentity _audit_identity;
  };
} /* namespace usbguard */
