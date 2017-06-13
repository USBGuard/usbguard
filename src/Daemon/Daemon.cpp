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
#include <build-config.h>

#include "Daemon.hpp"
#include "Logger.hpp"
#include "Common/Utility.hpp"
#include "IPCPrivate.hpp"
#include "RulePrivate.hpp"
#include "RuleParser.hpp"
#include "Audit.hpp"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <grp.h>
#include <pwd.h>

namespace usbguard
{
  /*
   * Recognized configuration option names. If an
   * unknown setting is found in the config file,
   * a warning message will be displayed.
   */
  static const StringVector G_config_known_names = {
    "RuleFile",
    "ImplicitPolicyTarget",
    "PresentDevicePolicy",
    "PresentControllerPolicy",
    "IPCAllowedUsers",
    "IPCAllowedGroups",
    "DeviceRulesWithPort",
    "InsertedDevicePolicy",
    "RestoreControllerDeviceState",
    "DeviceManagerBackend",
    "IPCAccessControlFiles",
    "AuditFilePath"
  };

  static const std::vector<std::pair<String,Daemon::DevicePolicyMethod> > device_policy_method_strings = {
    { "allow", Daemon::DevicePolicyMethod::Allow },
    { "block", Daemon::DevicePolicyMethod::Block },
    { "reject", Daemon::DevicePolicyMethod::Reject },
    { "keep", Daemon::DevicePolicyMethod::Keep },
    { "apply-policy", Daemon::DevicePolicyMethod::ApplyPolicy }
  };

  Daemon::DevicePolicyMethod Daemon::devicePolicyMethodFromString(const String& policy_string)
  {
    for (auto device_policy_method_string : device_policy_method_strings) {
      if (device_policy_method_string.first == policy_string) {
	return device_policy_method_string.second;
      }
    }
    throw Exception("DevicePolicyMethod", policy_string, "invalid policy method string");
  }

  const std::string Daemon::devicePolicyMethodToString(DevicePolicyMethod policy)
  {
    for (auto device_policy_method_string : device_policy_method_strings) {
      if (device_policy_method_string.second == policy) {
	return device_policy_method_string.first;
      }
    }
    throw USBGUARD_BUG("Invalid device policy method value");
  }

  Daemon::Daemon()
    : _config(G_config_known_names)
  {
    sigset_t signal_set;
    sigfillset(&signal_set);

    for (int del_signum : { SIGSEGV, SIGABRT, SIGKILL, SIGILL }) {
      sigdelset(&signal_set, del_signum);
    }

    USBGUARD_SYSCALL_THROW("Daemon initialization",
      (errno = pthread_sigmask(SIG_BLOCK, &signal_set, nullptr)) != 0);

    _device_manager_backend = "udev";
    _implicit_policy_target = Rule::Target::Block;
    _present_device_policy_method = DevicePolicyMethod::ApplyPolicy;
    _present_controller_policy_method = DevicePolicyMethod::Keep;
    _inserted_device_policy_method = DevicePolicyMethod::ApplyPolicy;

    _device_rules_with_port = false;
    _restore_controller_device_state = false;
  }

  Daemon::~Daemon()
  {
    _config.close();
  }

  void Daemon::loadConfiguration(const String& path)
  {
    USBGUARD_LOG(Info) << "Loading configuration from " << path;
    _config.open(path);

    /* RuleFile */
    if (_config.hasSettingValue("RuleFile")) {
      const String& rule_file = _config.getSettingValue("RuleFile");
      try {
        loadRules(rule_file);
      }
      catch(const RuleParserError& ex) {
        throw Exception("Configuration", rule_file, ex.hint());
      }
      catch(const std::exception& ex) {
        throw Exception("Configuration", rule_file, ex.what());
      }
      catch(...) {
        throw Exception("Configuration", rule_file, "unknown exception");
      }
    } else {
      USBGUARD_LOG(Warning) << "RuleFile not set; Modification of the permanent policy won't be possible.";
    }

    /* ImplicitPolicyTarget */
    if (_config.hasSettingValue("ImplicitPolicyTarget")) {
      const String& target_string = _config.getSettingValue("ImplicitPolicyTarget");
      Rule::Target target = Rule::targetFromString(target_string);
      setImplicitPolicyTarget(target);
    }

    /* PresentDevicePolicy */
    if (_config.hasSettingValue("PresentDevicePolicy")) {
      const String& policy_string = _config.getSettingValue("PresentDevicePolicy");
      DevicePolicyMethod policy = Daemon::devicePolicyMethodFromString(policy_string);
      setPresentDevicePolicyMethod(policy);
    }

    /* PresentControllerPolicy */
    if (_config.hasSettingValue("PresentControllerPolicy")) {
      const String& policy_string = _config.getSettingValue("PresentControllerPolicy");
      DevicePolicyMethod policy = Daemon::devicePolicyMethodFromString(policy_string);
      setPresentControllerPolicyMethod(policy);
    }

    /* InsertedDevicePolicy */
    if (_config.hasSettingValue("InsertedDevicePolicy")) {
      const String& policy_string = _config.getSettingValue("InsertedDevicePolicy");
      DevicePolicyMethod policy = Daemon::devicePolicyMethodFromString(policy_string);
      setInsertedDevicePolicyMethod(policy);
    }

    /* IPCAllowedUsers */
    if (_config.hasSettingValue("IPCAllowedUsers")) {
      const String users_value = _config.getSettingValue("IPCAllowedUsers"); 
      StringVector users;
      tokenizeString(users_value, users, " ", /*trim_empty=*/true);
      USBGUARD_LOG(Debug) << "Setting IPCAllowedUsers to { " << users_value << " }";

      for (auto const& user : users) {
        addIPCAllowedUser(user);
      }
    }

    /* IPCAllowedGroups */
    if (_config.hasSettingValue("IPCAllowedGroups")) {
      const String groups_value =_config.getSettingValue("IPCAllowedGroups"); 
      StringVector groups;
      tokenizeString(groups_value, groups, " ", /*trim_empty=*/true);
      USBGUARD_LOG(Debug) << "Setting IPCAllowedGroups to { " << groups_value << " }";

      for (auto const& group : groups) {
        addIPCAllowedGroup(group);
      }
    }

    /* DeviceRulesWithPort */
    if (_config.hasSettingValue("DeviceRulesWithPort")) {
      const String value = _config.getSettingValue("DeviceRulesWithPort");
      USBGUARD_LOG(Debug) << "Setting DeviceRulesWithPort to " << value;
      if (value == "true") {
        _device_rules_with_port = true;
      }
      else if (value == "false") {
        _device_rules_with_port = false;
      }
      else {
        throw Exception("Configuration", "DeviceRulesWithPort", "Invalid value");
      }
    }

    /* DeviceManagerBackend */
    if (_config.hasSettingValue("DeviceManagerBackend")) {
      _device_manager_backend = _config.getSettingValue("DeviceManagerBackend");
    }

    _dm = DeviceManager::create(*this, _device_manager_backend);

    /* RestoreControllerDeviceState */
    if (_config.hasSettingValue("RestoreControllerDeviceState")) {
      const String value = _config.getSettingValue("RestoreControllerDeviceState");

      if (value == "true") {
        _restore_controller_device_state = true;
      }
      else if (value == "false") {
        _restore_controller_device_state = false;
      }
      else {
        throw Exception("Configuration", "RestoreControllerDeviceState", "Invalid value");
      }

      _dm->setRestoreControllerDeviceState(_restore_controller_device_state);
    }

    /* IPCAccessControlFiles */
    if (_config.hasSettingValue("IPCAccessControlFiles")) {
      const String value = _config.getSettingValue("IPCAccessControlFiles");
      loadIPCAccessControlFiles(value);
    }

    /* AuditFilePath */
    if (_config.hasSettingValue("AuditFilePath")) {
      const String value = _config.getSettingValue("AuditFilePath");
      USBGUARD_LOG(Debug) << "Setting AuditFilePath to " << value;
      USBGUARD_LOGGER.setAuditFile(true, value);
    }

    USBGUARD_LOG(Info) << "Configuration loaded successfully.";
  }

  void Daemon::loadRules(const String& path)
  {
    USBGUARD_LOG(Info) << "Loading permanent policy file " << path;
    std::shared_ptr<RuleSet> ptr(new RuleSet(this));
    _policy.setRuleSet(ptr);
    _policy.load(path);
  }

  void Daemon::loadIPCAccessControlFiles(const String& path)
  {
    USBGUARD_LOG(Info) << "Loading IPC access control files at " << path;
    loadFiles(path,
      [](const String& path, const struct dirent * dir_entry)
      {
        (void)dir_entry;
        return filenameFromPath(path, /*include_extension=*/true);
      }
      ,
      [this](const String& basename, const String& fullpath)
      {
        return loadIPCAccessControlFile(basename, fullpath);
      });
  }

  void Daemon::checkIPCAccessControlName(const String& name)
  {
    IPCServer::checkAccessControlName(name);
  }

  void Daemon::parseIPCAccessControlFilename(const String& basename, String * const ptr_user, String * const ptr_group)
  {
    const auto ug_separator = basename.find_first_of(":");
    const bool has_group = ug_separator != std::string::npos;

    const String user = basename.substr(0, ug_separator);
    const String group = has_group ? basename.substr(ug_separator + 1) : String();

    checkIPCAccessControlName(user);
    checkIPCAccessControlName(group);

    *ptr_user = user;
    *ptr_group = group;
  }

  bool Daemon::loadIPCAccessControlFile(const String& basename, const String& fullpath)
  {
    USBGUARD_LOG(Info) << "Loading IPC access control file " << fullpath;

    String user;
    String group;
    IPCServer::AccessControl ac;

    try {
      parseIPCAccessControlFilename(basename, &user, &group);
    }
    catch(...) {
      USBGUARD_LOG(Warning) << "Ignoring access control file because of malformed name: " << basename;
      return false;
    }

    try {
      std::ifstream ac_stream(fullpath);
      ac.load(ac_stream);
    }
    catch(...) {
      USBGUARD_LOG(Warning) << "Failed to load IPC access control file " << fullpath;
      return false;
    }

    if (!user.empty()) {
      addIPCAllowedUser(user, ac);
    }
    if (!group.empty()) {
      addIPCAllowedGroup(group, ac);
    }

    return true;
  }

  void Daemon::setImplicitPolicyTarget(Rule::Target target)
  {
    USBGUARD_LOG(Debug) << "Setting ImplicitPolicyTarget to " << Rule::targetToString(target);
    _implicit_policy_target = target;
    _policy.setDefaultTarget(target);
  }

  void Daemon::setPresentDevicePolicyMethod(DevicePolicyMethod policy)
  {
    USBGUARD_LOG(Debug) << "Setting PresentDevicePolicy to " << devicePolicyMethodToString(policy);
    _present_device_policy_method = policy;
  }

  void Daemon::setPresentControllerPolicyMethod(DevicePolicyMethod policy)
  {
    USBGUARD_LOG(Debug) << "Setting PresentControllerPolicy to " << devicePolicyMethodToString(policy);
    _present_controller_policy_method = policy;
  }

  void Daemon::setInsertedDevicePolicyMethod(DevicePolicyMethod policy)
  {
    USBGUARD_LOG(Debug) << "Setting InsertedDevicePolicy to " << devicePolicyMethodToString(policy);
    switch(policy) {
      case DevicePolicyMethod::ApplyPolicy:
      case DevicePolicyMethod::Block:
      case DevicePolicyMethod::Reject:
        _inserted_device_policy_method = policy;
        break;
      case DevicePolicyMethod::Keep:
      case DevicePolicyMethod::Allow:
      default:
        throw Exception("setInsertedDevicePolicyMethod", devicePolicyMethodToString(policy), "invalid policy method");
    }
  }

  void Daemon::run()
  {
    USBGUARD_LOG(Trace) << "Entering main loop";
   
    _dm->start();
    _dm->scan();
    IPCServer::start();

    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGINT);
    sigaddset(&signal_set, SIGTERM);
    sigaddset(&signal_set, SIGSYS);

    bool exit_loop = false;

    do {
      siginfo_t signal_info = { };
      const int signal_num = sigwaitinfo(&signal_set, &signal_info);

      if (signal_num <= 0) {
        USBGUARD_LOG(Error) << "sigwaitinfo failed: errno=" << errno << "; Shutting down.";
        break;
      }

      switch(signal_num) {
        case SIGTERM:
        case SIGINT:
          USBGUARD_LOG(Info) << "Received SIGTERM/SIGINT. Shutting down.";
          exit_loop = true;
          break;
        case SIGSYS:
          USBGUARD_LOG(Error) << "Received SIGSYS: Seccomp whitelist violation!";
          exit_loop = false;
          break;
        default:
          USBGUARD_LOG(Warning) << "Received signal " << signal_num << ". Ignoring!";
      }
    } while(!exit_loop);

    IPCServer::stop();
    _dm->stop();
    USBGUARD_LOG(Trace) << "Leaving main loop.";
  }

  void Daemon::quit()
  {
  }

  uint32_t Daemon::assignID()
  {
    return _policy.assignID();
  }

  /*
   * Search for a rule that matches `match_spec' rule and
   * update it with a rule specified by `rule_spec'. Fail
   * if multiple rules match. If there are no matching
   * rules, append the `rule_spec' rule.
   *
   * Return the id of the updated or new rule.
   */
  uint32_t Daemon::upsertRule(const std::string& match_spec,
                              const std::string& rule_spec,
                              const bool parent_insensitive)
  {
    USBGUARD_LOG(Trace) << "entry:"
                        << " match_spec=" << match_spec
                        << " rule_spec=" << rule_spec
                        << " parent_insensitive=" << parent_insensitive;

    const Rule match_rule = Rule::fromString(match_spec);
    const Rule new_rule = Rule::fromString(rule_spec);

    const uint32_t id = _policy.upsertRule(match_rule, new_rule, parent_insensitive);
    if (_config.hasSettingValue("RuleFile")) {
      _policy.save(_config.getSettingValue("RuleFile"));
    }

    USBGUARD_LOG(Trace) << "return: id=" << id;
    return id;
  }

  /*
   * IPC service methods
   */
  std::string Daemon::setParameter(const std::string& name, const std::string& value)
  {
    if (name == "InsertedDevicePolicy") {
      const auto previous_value = devicePolicyMethodToString(_inserted_device_policy_method);
      setInsertedDevicePolicyMethod(devicePolicyMethodFromString(value));
      return previous_value;
    }
    throw Exception("setParameter", name, "unknown parameter");
  }

  std::string Daemon::getParameter(const std::string& name)
  {
    if (name == "InsertedDevicePolicy") {
      return devicePolicyMethodToString(_inserted_device_policy_method);
    }
    throw Exception("getParameter", name, "unknown parameter");
  }

  uint32_t Daemon::appendRule(const std::string& rule_spec,
			      uint32_t parent_id)
  {
    USBGUARD_LOG(Trace) << "entry:"
                        << " rule_spec=" << rule_spec
                        << " parent_id=" << parent_id;

    const Rule rule = Rule::fromString(rule_spec);
    /* TODO: reevaluate the firewall rules for all active devices */

    const uint32_t id = _policy.appendRule(rule, parent_id);
    if (_config.hasSettingValue("RuleFile")) {
      _policy.save(_config.getSettingValue("RuleFile"));
    }
    USBGUARD_LOG(Trace) << "return: id=" << id;
    return id;
  }

  void Daemon::removeRule(uint32_t id)
  {
    USBGUARD_LOG(Trace) << "id=" << id;
    _policy.removeRule(id);
    if (_config.hasSettingValue("RuleFile")) {
      _policy.save(_config.getSettingValue("RuleFile"));
    }
  }

  const RuleSet Daemon::listRules(const std::string& query)
  {
    USBGUARD_LOG(Trace) << "entry: query=" << query; 
    return _policy.getRuleSet();
  }

  uint32_t Daemon::applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent)
  {
    USBGUARD_LOG(Trace) << "entry:"
                        << " id=" << id
                        << " target=" << Rule::targetToString(target)
                        << " permanent=" << permanent;

    Pointer<Device> device = _dm->getDevice(id);
    Pointer<Rule> rule;

    if (permanent) {
      rule = upsertDeviceRule(id, target);
    }
    else {
      rule = makePointer<Rule>();
      rule->setTarget(target);
    }

    dmApplyDevicePolicy(device, rule);
    USBGUARD_LOG(Trace) << "return:"
                        << " id=" << rule->getRuleID();

    return rule->getRuleID();
  }

  void Daemon::dmHookDeviceEvent(DeviceManager::EventType event, Pointer<Device> device)
  {
    USBGUARD_LOG(Trace) << "event=" << DeviceManager::eventTypeToString(event)
                        << " device_ptr=" << device.get();

    auto audit_event = Audit::deviceEvent(_audit_identity, device, event);

    Pointer<const Rule> device_rule = \
      device->getDeviceRule(/*with_port*/true,
                            /*with_parent_hash=*/true);

    DevicePresenceChanged(device->getID(),
                          event,
                          device->getTarget(),
                          device_rule->toString());

    audit_event.success();

    Pointer<Rule> policy_rule = nullptr;

    switch(event) {
      case DeviceManager::EventType::Present:
        policy_rule = getPresentDevicePolicyRule(device);
        break;
      case DeviceManager::EventType::Insert:
      case DeviceManager::EventType::Update:
        policy_rule = getInsertedDevicePolicyRule(device);
        break;
      case DeviceManager::EventType::Remove:
        /* NOOP */
        return;
      default:
        throw USBGUARD_BUG("Unknown DeviceManager event type");
    }

    dmApplyDevicePolicy(device, policy_rule);
  }

  void Daemon::dmHookDeviceException(const String& message)
  {
    USBGUARD_LOG(Warning) << message;
  }

  void Daemon::dmApplyDevicePolicy(Pointer<Device> device, Pointer<Rule> matched_rule)
  {
    USBGUARD_LOG(Trace) << "device_ptr=" << device.get()
                        << " matched_rule_ptr=" << matched_rule.get();

    auto audit_event = Audit::policyEvent(_audit_identity,
        device, device->getTarget(), matched_rule->getTarget());

    const Rule::Target target_old = device->getTarget();
    Pointer<Device> device_post = \
      _dm->applyDevicePolicy(device->getID(),
                             matched_rule->getTarget());

    const bool target_changed = target_old != device_post->getTarget();
 
    if (target_changed || matched_rule->getRuleID() == Rule::ImplicitID) {
      if (target_changed) {
        USBGUARD_LOG(Debug) << "Device target changed:"
                            << " old=" << Rule::targetToString(target_old)
                            << " new=" << Rule::targetToString(device_post->getTarget());
      }
      else {
        USBGUARD_LOG(Debug) << "Implicit rule matched";
      }

      Pointer<const Rule> device_rule = \
        device_post->getDeviceRule(/*with_port=*/true,
                                   /*with_parent_hash=*/true);

      DevicePolicyChanged(device->getID(),
                          target_old,
                          device_post->getTarget(),
                          device_rule->toString(),
                          matched_rule->getRuleID());
    }

    matched_rule->updateMetaDataCounters(/*applied=*/true);
    audit_event.success();
  }

  Pointer<Rule> Daemon::getInsertedDevicePolicyRule(Pointer<Device> device)
  {
    USBGUARD_LOG(Trace) << "device_ptr=" << device.get();

    Pointer<const Rule> device_rule = \
      device->getDeviceRule(/*with_port=*/true,
                            /*with_parent_hash=*/true,
                            /*match_rule=*/true);

    Rule::Target target = Rule::Target::Invalid;
    Pointer<Rule> policy_rule;
    const DevicePolicyMethod policy_method = _inserted_device_policy_method;

    switch (policy_method) {
      case DevicePolicyMethod::Block:
        target = Rule::Target::Block;
        break;
      case DevicePolicyMethod::Reject:
        target = Rule::Target::Reject;
        break;
      case DevicePolicyMethod::ApplyPolicy:
        policy_rule = _policy.getFirstMatchingRule(device_rule);
        break;
      case DevicePolicyMethod::Allow:
      case DevicePolicyMethod::Keep:
      default:
        throw USBGUARD_BUG("Invalid DevicePolicyMethod value");
    }

    if (policy_rule == nullptr) {
      policy_rule = makePointer<Rule>();
      policy_rule->setTarget(target);
      policy_rule->setRuleID(Rule::RootID);
    }

    return policy_rule;
  }

  Pointer<Rule> Daemon::getPresentDevicePolicyRule(Pointer<Device> device)
  {
    USBGUARD_LOG(Trace) << "entry: device_ptr=" << device.get();

    Pointer<const Rule> device_rule = \
      device->getDeviceRule(/*with_port=*/true,
                            /*with_parent_hash=*/true,
                            /*match_rule=*/true);

    USBGUARD_LOG(Debug) << "device_rule=" << device_rule->toString();
    USBGUARD_LOG(Debug) << "isController=" << device->isController();

    const DevicePolicyMethod policy_method = \
      device->isController() ? _present_controller_policy_method : _present_device_policy_method;

    Rule::Target target = Rule::Target::Invalid;
    Pointer<Rule> matched_rule = nullptr;

    switch (policy_method) {
    case DevicePolicyMethod::Allow:
      target = Rule::Target::Allow;
      break;
    case DevicePolicyMethod::Block:
      target = Rule::Target::Block;
      break;
    case DevicePolicyMethod::Reject:
      target = Rule::Target::Reject;
      break;
    case DevicePolicyMethod::Keep:
      target = device->getTarget();
      break;
    case DevicePolicyMethod::ApplyPolicy:
      matched_rule = _policy.getFirstMatchingRule(device_rule);
      target = matched_rule->getTarget();
      break;
    default:
      throw USBGUARD_BUG("Invalid DevicePolicyMethod value");
    }

    if (matched_rule == nullptr) {
      matched_rule = makePointer<Rule>();
      matched_rule->setTarget(target);
      matched_rule->setRuleID(Rule::ImplicitID);
    }

    USBGUARD_LOG(Trace) << "return:"
                        << " matched_rule=" << matched_rule->toString();

    return matched_rule;
  }

  uint32_t Daemon::dmHookAssignID()
  {
    return assignID();
  }

  const std::vector<Rule> Daemon::listDevices(const std::string& query)
  {
    USBGUARD_LOG(Trace) << "entry:"
                        << " query=" << query;

    std::vector<Rule> device_rules;
    const Rule query_rule = Rule::fromString(query);

    for (auto const& device : _dm->getDeviceList(query_rule)) {
      device_rules.push_back(*device->getDeviceRule());
    }

    USBGUARD_LOG(Trace) << "return:"
                        << " count(device_rules)=" << device_rules.size();

    return device_rules;
  }

  Pointer<Rule> Daemon::upsertDeviceRule(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "entry:"
                        << "id=" << id
                        << "target=" << Rule::targetToString(target);

    Pointer<Device> device = _dm->getDevice(id);

    bool with_port = true && _device_rules_with_port;
    bool with_parent_hash = true;

    /*
     * Generate a port specific or agnostic rule depending on the target
     */
    switch(target) {
      case Rule::Target::Allow:
        with_port = true && with_port;
        with_parent_hash = true;
        break;
      case Rule::Target::Block:
        /*
         * Block the device using a port agnostic rule, so that the same device
         * inserted in a different port is still blocked. Note that allowDevice
         * generates a port specific rule and the same device won't be allowed
         * when inserted in a different port.
         */
        with_port = false;
        with_parent_hash = false;
        break;
      case Rule::Target::Reject:
        /*
         * Reject the device using a port agnostic port. When we explicitly
         * reject a device, we don't want to reject it again when the same
         * device is inserted in a different port.
         */
        with_port = false;
        with_parent_hash = false;
        break;
      case Rule::Target::Invalid:
      case Rule::Target::Unknown:
      case Rule::Target::Match:
      case Rule::Target::Device:
      default:
        throw Exception("upsertDeviceRule", "device rule", "Invalid target");
    }

    /* Generate a match rule for upsert */
    Pointer<Rule> match_rule = device->getDeviceRule(false, false, /*match_rule=*/true);
    const String match_spec = match_rule->toString();

    USBGUARD_LOG(Debug) << "match_spec=" << match_spec;

    /* Generate new device rule */
    Pointer<Rule> device_rule = device->getDeviceRule(with_port, with_parent_hash); 
    device_rule->setTarget(target);
    const String rule_spec = device_rule->toString();

    USBGUARD_LOG(Debug) << "rule_spec=" << rule_spec;

    /* Upsert */
    const uint32_t rule_id = upsertRule(match_spec, rule_spec, /*parent_insensitive=*/true);
    auto upsert_rule = _policy.getRule(rule_id);

    USBGUARD_LOG(Trace) << "return:"
                        << " upsert_rule=" << upsert_rule->toString();

    return upsert_rule;
  }

  void Daemon::addIPCAllowedUID(uid_t uid, const IPCServer::AccessControl& ac)
  {
    USBGUARD_LOG(Trace) << "uid=" << uid;
    IPCServer::addAllowedUID(uid, ac);
  }

  void Daemon::addIPCAllowedUID(const String& uid_string, const IPCServer::AccessControl& ac)
  {
    addIPCAllowedUID(stringToNumber<uid_t>(uid_string), ac);
  }

  void Daemon::addIPCAllowedGID(gid_t gid, const IPCServer::AccessControl& ac)
  {
    USBGUARD_LOG(Trace) << "gid=" << gid;
    IPCServer::addAllowedGID(gid, ac);
  }

  void Daemon::addIPCAllowedGID(const String& gid_string, const IPCServer::AccessControl& ac)
  {
    addIPCAllowedGID(stringToNumber<gid_t>(gid_string), ac);
  }

  void Daemon::addIPCAllowedUser(const String& user, const IPCServer::AccessControl& ac)
  {
    USBGUARD_LOG(Trace) << "user=" << user;

    if (isNumericString(user)) {
      addIPCAllowedUID(user, ac);
    }
    else {
      IPCServer::addAllowedUsername(user, ac);
    }
  }

  void Daemon::addIPCAllowedGroup(const String& group, const IPCServer::AccessControl& ac)
  {
    USBGUARD_LOG(Trace) << "group=" << group;

    if (isNumericString(group)) {
      addIPCAllowedGID(group, ac);
    }
    else {
      IPCServer::addAllowedGroupname(group, ac);
    }
  }
} /* namespace usbguard */
