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
//          Jiri Vymazal   <jvymazal@redhat.com>
//          Radovan Sroka <rsroka@redhat.com>
//
#pragma once
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Common/Thread.hpp"

#include "NSHandler.hpp"
#include "usbguard/RuleSet.hpp"
#include "usbguard/Policy.hpp"

#include "usbguard/Typedefs.hpp"
#include "usbguard/ConfigFile.hpp"
#include "usbguard/IPCServer.hpp"
#include "usbguard/Rule.hpp"
#include "usbguard/Device.hpp"
#include "usbguard/DeviceManager.hpp"
#include "usbguard/DeviceManagerHooks.hpp"
#include "usbguard/Audit.hpp"

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

    static DevicePolicyMethod devicePolicyMethodFromString(const std::string& policy_string);
    static const std::string devicePolicyMethodToString(DevicePolicyMethod policy);

    Daemon();
    ~Daemon();

    int checkFilePermissions(const std::string& path, const mode_t permissions);
    void checkFolderPermissions(const std::string& path, const mode_t permissions);
    void loadConfiguration(const std::string& path, const bool check_permissions);
    void loadRules();
    void loadIPCAccessControlFiles(const std::string& path);
    bool loadIPCAccessControlFile(const std::string& basename, const std::string& fullpath);
    void checkIPCAccessControlName(const std::string& basename);
    void parseIPCAccessControlFilename(const std::string& basename, std::string* const ptr_user, std::string* const ptr_group);

    void setImplicitPolicyTarget(Rule::Target target);
    void setPresentDevicePolicyMethod(DevicePolicyMethod policy);
    void setPresentControllerPolicyMethod(DevicePolicyMethod policy);
    void setInsertedDevicePolicyMethod(DevicePolicyMethod policy);

    /* Start the daemon */
    void run();
    /* Handle process daemonization */
    void daemonize(const std::string& pid_file);

    uint32_t assignID();
    uint32_t upsertRule(const std::string& match_spec, const std::string& rule_spec, bool parent_insensitive = false);

    /* IPC methods */
    std::string setParameter(const std::string& name, const std::string& value) override;
    std::string getParameter(const std::string& name) override;

    uint32_t appendRule(const std::string& rule_spec, uint32_t parent_id, bool permanent) override;
    void removeRule(uint32_t id) override;
    const std::vector<Rule> listRules(const std::string& query) override;

    uint32_t applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent) override;
    const std::vector<Rule> listDevices(const std::string& query) override;

    /* Device manager hooks */
    void dmHookDeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device) override;
    uint32_t dmHookAssignID() override;
    void dmHookDeviceException(const std::string& message) override;

#define USBGUARD_IPCSERVER_DEFAULT_AC \
  IPCServer::AccessControl(IPCServer::AccessControl::Section::ALL, IPCServer::AccessControl::Privilege::ALL)

    void addIPCAllowedUID(uid_t uid, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedUID(const std::string& uid_string, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedGID(gid_t gid, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedGID(const std::string& gid_string, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedUser(const std::string& user, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);
    void addIPCAllowedGroup(const std::string& group, const IPCServer::AccessControl& ac = USBGUARD_IPCSERVER_DEFAULT_AC);

  private:
    void dmApplyDevicePolicy(std::shared_ptr<Device> device, std::shared_ptr<Rule> matched_rule);
    std::shared_ptr<Rule> getInsertedDevicePolicyRule(std::shared_ptr<Device> device);
    std::shared_ptr<Rule> getPresentDevicePolicyRule(std::shared_ptr<Device> device);

    std::vector<std::string> getRuleFiles(const std::string& ruledir_path);

    std::shared_ptr<Rule> upsertDeviceRule(uint32_t id, Rule::Target target);

    ConfigFile _config;

    NSHandler _nss;
    Policy _policy;

    int pid_fd;

    std::string _device_manager_backend;
    std::shared_ptr<DeviceManager> _dm;

    std::atomic<Rule::Target> _implicit_policy_target;
    std::atomic<DevicePolicyMethod> _present_device_policy_method;
    std::atomic<DevicePolicyMethod> _present_controller_policy_method;
    std::atomic<DevicePolicyMethod> _inserted_device_policy_method;

    bool _device_rules_with_port;
    bool _restore_controller_device_state;

    AuditIdentity _audit_identity;
    Audit _audit;
  };
} /* namespace usbguard */

/* vim: set ts=2 sw=2 et */
