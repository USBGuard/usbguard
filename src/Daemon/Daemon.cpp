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
#include "LoggerPrivate.hpp"
#include "Common/Utility.hpp"
#include "IPCPrivate.hpp"
#include "RulePrivate.hpp"
#include "RuleParser.hpp"

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
    "DeviceRulesWithPort"
  };

  static const std::vector<std::pair<String,Daemon::PresentDevicePolicy> > present_device_policy_strings = {
    { "allow", Daemon::PresentDevicePolicy::Allow },
    { "block", Daemon::PresentDevicePolicy::Block },
    { "reject", Daemon::PresentDevicePolicy::Reject },
    { "keep", Daemon::PresentDevicePolicy::Keep },
    { "apply-policy", Daemon::PresentDevicePolicy::ApplyPolicy }
  };

  Daemon::PresentDevicePolicy Daemon::presentDevicePolicyFromString(const String& policy_string)
  {
    for (auto present_device_policy_string : present_device_policy_strings) {
      if (present_device_policy_string.first == policy_string) {
	return present_device_policy_string.second;
      }
    }
    throw USBGUARD_BUG("Invalid present device policy string");
  }

  Daemon::Daemon()
    : _config(G_config_known_names),
      _ruleset(this)
  {
    sigset_t signal_set;
    sigfillset(&signal_set);
    sigdelset(&signal_set, SIGSEGV);
    sigdelset(&signal_set, SIGABRT);
    sigdelset(&signal_set, SIGKILL);
    sigdelset(&signal_set, SIGILL);

    pthread_sigmask(SIG_BLOCK, &signal_set, nullptr);

    try {
      _dm = DeviceManager::create(*this);
    } catch(...) {
      throw;
    }

    _implicit_policy_target = Rule::Target::Block;
    _present_device_policy = PresentDevicePolicy::Keep;
    _present_controller_policy = PresentDevicePolicy::Allow;
    _device_rules_with_port = false;
  }

  Daemon::~Daemon()
  {
    _config.close();
  }

  void Daemon::loadConfiguration(const String& path)
  {
    logger->debug("Loading configuration from {}", path);
    _config.open(path);

    /* RuleFile */
    if (_config.hasSettingValue("RuleFile")) {
      logger->debug("Setting rules file path from configuration file");
      const String& rule_file = _config.getSettingValue("RuleFile");
      try {
	loadRules(rule_file);
      }
      catch(const RuleParserError& ex) {
        logger->error("Syntax error in the rule file on line {}: {}", ex.line(), ex.hint());
        throw Exception("Configuration", rule_file, ex.hint());
      }
      catch(const std::exception& ex) {
        logger->warn("The configured rule file doesn't yet exists. Starting with an empty rule set.");
      }
    } else {
      logger->debug("No rules file path specified.");
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
      PresentDevicePolicy policy = Daemon::presentDevicePolicyFromString(policy_string);
      setPresentDevicePolicy(policy);
    }

    /* PresentControllerPolicy */
    if (_config.hasSettingValue("PresentControllerPolicy")) {
      const String& policy_string = _config.getSettingValue("PresentControllerPolicy");
      PresentDevicePolicy policy = Daemon::presentDevicePolicyFromString(policy_string);
      setPresentControllerPolicy(policy);
    }

    /* IPCAllowedUsers */
    if (_config.hasSettingValue("IPCAllowedUsers")) {
      logger->debug("Setting allowed IPC users");
      StringVector usernames;
      tokenizeString(_config.getSettingValue("IPCAllowedUsers"),
		     usernames, " ", /*trim_empty=*/true);
      for (auto const& username : usernames) {
	logger->debug("Allowed IPC user: {}", username);
	addIPCAllowedUID(username);
      }
    }

    /* IPCAllowedGroups */
    if (_config.hasSettingValue("IPCAllowedGroups")) {
      logger->debug("Setting allowed IPC groups");
      StringVector groupnames;
      tokenizeString(_config.getSettingValue("IPCAllowedGroups"),
		     groupnames, " ", /*trim_empty=*/true);
      for (auto const& groupname : groupnames) {
	logger->debug("Allowed IPC group: {}", groupname);
	addIPCAllowedGID(groupname);
      }
    }

    /* DeviceRulesWithPort */
    if (_config.hasSettingValue("DeviceRulesWithPort")) {
      const String value = _config.getSettingValue("DeviceRulesWithPort");
      if (value == "true") {
        _device_rules_with_port = true;
      }
      else if (value == "false") {
        _device_rules_with_port = false;
      }
      else {
        throw Exception("Configuration", "DeviceRulesWithPort", "Invalid value");
      }
      logger->debug("DeviceRulesWithPort set to {}", _device_rules_with_port);
    }

    logger->debug("Configuration loaded successfully");
  }

  void Daemon::loadRules(const String& path)
  {
    _ruleset.load(path);
  }

  void Daemon::setImplicitPolicyTarget(Rule::Target target)
  {
    _implicit_policy_target = target;
    _ruleset.setDefaultTarget(target);
  }

  void Daemon::setPresentDevicePolicy(PresentDevicePolicy policy)
  {
    _present_device_policy = policy;
  }

  void Daemon::setPresentControllerPolicy(PresentDevicePolicy policy)
  {
    _present_controller_policy = policy;
  }

  void Daemon::run()
  {
    _dm->start();
    IPCServer::start();

    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGINT);
    sigaddset(&signal_set, SIGTERM);
    sigaddset(&signal_set, SIGSYS);

    bool exit_loop = false;

    do {
      siginfo_t signal_info = { 0 };
      const int signal_num = sigwaitinfo(&signal_set, &signal_info);

      if (signal_num <= 0) {
        logger->error("sigwaitinfo failed: {}", strerror(errno));
        break;
      }

      switch(signal_num) {
        case SIGTERM:
        case SIGINT:
          logger->debug("Received SIGTERM/SIGINT. Stopping.");
          exit_loop = true;
          break;
        case SIGSYS:
          logger->warn("Stopping due to SIGSYS: System call whitelist violation");
          exit_loop = true;
          break;
      }
    } while(!exit_loop);

    IPCServer::stop();
    _dm->stop();
  }

  void Daemon::quit()
  {
  }

  uint32_t Daemon::assignID()
  {
    return _ruleset.assignID();
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
    const Rule match_rule = Rule::fromString(match_spec);
    const Rule new_rule = Rule::fromString(rule_spec);
    logger->debug("Upserting rule: match={}, new={}", match_spec, rule_spec);
    const uint32_t id = _ruleset.upsertRule(match_rule, new_rule, parent_insensitive);
    if (_config.hasSettingValue("RuleFile")) {
      _ruleset.save(_config.getSettingValue("RuleFile"));
    }
    return id;
  }

  /*
   * IPC service methods
   */
  uint32_t Daemon::appendRule(const std::string& rule_spec,
			      uint32_t parent_id)
  {
    const Rule rule = Rule::fromString(rule_spec);
    /* TODO: reevaluate the firewall rules for all active devices */
    logger->debug("Appending rule: {}", rule_spec);
    const uint32_t id = _ruleset.appendRule(rule, parent_id);
    if (_config.hasSettingValue("RuleFile")) {
      _ruleset.save(_config.getSettingValue("RuleFile"));
    }
    return id;
  }

  void Daemon::removeRule(uint32_t id)
  {
    logger->debug("Removing rule: id={}", id);
    _ruleset.removeRule(id);
    if (_config.hasSettingValue("RuleFile")) {
      _ruleset.save(_config.getSettingValue("RuleFile"));
    }
  }

  const RuleSet Daemon::listRules(const std::string& query)
  {
    return _ruleset;
  }

  uint32_t Daemon::applyDevicePolicy(uint32_t id, Rule::Target target, bool permanent)
  {
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
    return rule->getRuleID();
  }

  void Daemon::dmHookDeviceEvent(DeviceManager::EventType event, Pointer<Device> device)
  {
    Pointer<const Rule> device_rule = \
      device->getDeviceRule(/*with_port*/true,
                            /*with_parent_hash=*/true);

    DevicePresenceChanged(device->getID(),
                          event,
                          device->getTarget(),
                          device_rule->toString());

    Pointer<Rule> policy_rule = nullptr;

    switch(event) {
      case DeviceManager::EventType::Present:
        policy_rule = getPresentDevicePolicyRule(device);
        break;
      case DeviceManager::EventType::Insert:
      case DeviceManager::EventType::Update:
        policy_rule = getDevicePolicyRule(device);
        break;
      case DeviceManager::EventType::Remove:
        /* NOOP */
        return;
      default:
        throw USBGUARD_BUG("Unknown DeviceManager event type");
    }

    dmApplyDevicePolicy(device, policy_rule);
  }

  void Daemon::dmApplyDevicePolicy(Pointer<Device> device, Pointer<Rule> matched_rule)
  {
    const Rule::Target target_old = device->getTarget();
    Pointer<Device> device_post = \
      _dm->applyDevicePolicy(device->getID(),
                             matched_rule->getTarget());

    if (target_old != device_post->getTarget()
        || matched_rule->getRuleID() == Rule::ImplicitID) {
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
  }

  Pointer<Rule> Daemon::getDevicePolicyRule(Pointer<Device> device)
  {
    Pointer<const Rule> device_rule = \
      device->getDeviceRule(/*with_port=*/true,
                            /*with_parent_hash=*/true,
                            /*match_rule=*/true);

    return _ruleset.getFirstMatchingRule(device_rule);
  }

  Pointer<Rule> Daemon::getPresentDevicePolicyRule(Pointer<Device> device)
  {
    Pointer<const Rule> device_rule = \
      device->getDeviceRule(/*with_port=*/true,
                            /*with_parent_hash=*/true,
                            /*match_rule=*/true);

    const PresentDevicePolicy policy = \
      device->isController() ? _present_controller_policy : _present_device_policy;

    Rule::Target target = Rule::Target::Invalid;
    Pointer<Rule> matched_rule = nullptr;

    switch (policy) {
    case PresentDevicePolicy::Allow:
      target = Rule::Target::Allow;
      break;
    case PresentDevicePolicy::Block:
      target = Rule::Target::Block;
      break;
    case PresentDevicePolicy::Reject:
      target = Rule::Target::Reject;
      break;
    case PresentDevicePolicy::Keep:
      target = device->getTarget();
      break;
    case PresentDevicePolicy::ApplyPolicy:
      matched_rule = _ruleset.getFirstMatchingRule(device_rule);
      target = matched_rule->getTarget();
      break;
    }

    if (matched_rule == nullptr) {
      auto rule = makePointer<Rule>();
      rule->setTarget(target);
      rule->setRuleID(Rule::ImplicitID);
      matched_rule = rule;
    }

    return matched_rule;
  }

  uint32_t Daemon::dmHookAssignID()
  {
    return assignID();
  }

  const std::vector<Rule> Daemon::listDevices(const std::string& query)
  {
    std::vector<Rule> device_rules;
    const Rule query_rule = Rule::fromString(query);

    for (auto const& device : _dm->getDeviceList(query_rule)) {
      device_rules.push_back(*device->getDeviceRule());
    }

    return device_rules;
  }

  Pointer<Rule> Daemon::upsertDeviceRule(uint32_t id, Rule::Target target)
  {
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
      default:
        throw Exception("upsertDeviceRule", "device rule", "Invalid target");
    }

    /* Generate a match rule for upsert */
    Pointer<Rule> match_rule = device->getDeviceRule(false, false, /*match_rule=*/true);
    const String match_spec = match_rule->toString();

    /* Generate new device rule */
    Pointer<Rule> device_rule = device->getDeviceRule(with_port, with_parent_hash); 
    device_rule->setTarget(target);
    const String rule_spec = device_rule->toString();

    /* Upsert */
    const uint32_t rule_id = upsertRule(match_spec, rule_spec, /*parent_insensitive=*/true);

    return _ruleset.getRule(rule_id);
  }

  void Daemon::addIPCAllowedUID(uid_t uid)
  {
    IPCServer::addAllowedUID(uid);
  }

  void Daemon::addIPCAllowedGID(gid_t gid)
  {
    IPCServer::addAllowedGID(gid);
  }

  void Daemon::addIPCAllowedUID(const String& username)
  {
    char string_buffer[4096];
    struct passwd pw, *pwptr = nullptr;

    USBGUARD_SYSCALL_THROW("IPC ACL",
                           getpwnam_r(username.c_str(), &pw,
                                      string_buffer, sizeof string_buffer,
                                      &pwptr) != 0);

    addIPCAllowedUID(pw.pw_uid);
  }

  void Daemon::addIPCAllowedGID(const String& groupname)
  {
    char string_buffer[4096];
    struct group gr, *grptr = nullptr;

    USBGUARD_SYSCALL_THROW("IPC ACL",
                           getgrnam_r(groupname.c_str(), &gr,
		                      string_buffer, sizeof string_buffer,
                                      &grptr) != 0);
    
    addIPCAllowedGID(gr.gr_gid);
  }
} /* namespace usbguard */
