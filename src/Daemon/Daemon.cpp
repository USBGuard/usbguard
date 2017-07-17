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
#ifdef HAVE_BUILD_CONFIG_H
  #include <build-config.h>
#endif

#include "Daemon.hpp"
#include "FileAuditBackend.hpp"
#include "LinuxAuditBackend.hpp"
#include "Common/Utility.hpp"
#include "NSHandler.hpp"

#include "usbguard/Logger.hpp"
#include "usbguard/RuleParser.hpp"
#include "usbguard/Audit.hpp"

#include <array>

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
  static const std::vector<std::string> G_config_known_names = {
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
    "AuditFilePath",
    "AuditBackend"
  };

  static const std::vector<std::pair<std::string, Daemon::DevicePolicyMethod>> device_policy_method_strings = {
    { "allow", Daemon::DevicePolicyMethod::Allow },
    { "block", Daemon::DevicePolicyMethod::Block },
    { "reject", Daemon::DevicePolicyMethod::Reject },
    { "keep", Daemon::DevicePolicyMethod::Keep },
    { "apply-policy", Daemon::DevicePolicyMethod::ApplyPolicy }
  };

  Daemon::DevicePolicyMethod Daemon::devicePolicyMethodFromString(const std::string& policy_string)
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
    : _config(G_config_known_names),
      _audit(_audit_identity)
  {
    sigset_t signal_set;
    sigfillset(&signal_set);

    for (int del_signum : {
        SIGSEGV, SIGABRT, SIGKILL, SIGILL
      }) {
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
    pid_fd = -1;
  }

  Daemon::~Daemon()
  {
    _config.close();
  }

  int Daemon::checkPermissions(const std::string& path,
    const mode_t permissions)
  {
    struct stat file_stat;
    // from all enabled permissions we subtract the permissions we want to check
    // after this operation variable permission_bad contains the complement of
    // permissions we want to check.
    mode_t permission_bad { ( S_IRWXU | S_IRWXG | S_IRWXO ) - permissions };

    if (!stat(path.c_str(), &file_stat)) {
      if (S_ISREG(file_stat.st_mode)) {
        // this comparison inspect if file has the wanted permissions and if
        // the file does not contain the unwanted permissions.
        if (  !( file_stat.st_mode & permissions ) ||
          ( file_stat.st_mode & permission_bad)
        ) {
          USBGUARD_LOG(Error) << "Policy leaks are possible with this permissions!";
          throw Exception("Check permissions", path, "Policy leaks possible");
        }
        else {
          USBGUARD_LOG(Info) << "File has correct permissions.";
        }
      }
      else {
        USBGUARD_LOG(Error) << "ERROR: File is not a regular file.";
        throw Exception("Check permissions", path, "File not exists");
      }
    }
    else {
      USBGUARD_LOG(Error) << "ERROR: obtaining file permissions! Errno: " << errno;
      throw ErrnoException("Check permissions", path, errno);
    }

    return 0;
  }

  void Daemon::loadConfiguration(const std::string& path, const bool check_permissions)
  {

    USBGUARD_LOG(Info) << "Loading configuration from " << path;

    if (check_permissions) {
      checkPermissions(path, (S_IRUSR | S_IWUSR));
    }

    _config.open(path, /*readonly=*/true);

    USBGUARD_LOG(Info) << "Loading NSSwitch...";
    _nss.parseNSSwitch();

    /* RuleFile */
    if (_config.hasSettingValue("RuleFile")) {
      const std::string& rule_file = _config.getSettingValue("RuleFile");

      try {
        loadRules(rule_file, check_permissions);
      }
      catch (const RuleParserError& ex) {
        throw Exception("Configuration", rule_file, ex.hint());
      }
      catch (const std::exception& ex) {
        throw Exception("Configuration", rule_file, ex.what());
      }
      catch (...) {
        throw Exception("Configuration", rule_file, "unknown exception");
      }
    }
    else {
      USBGUARD_LOG(Warning) << "RuleFile not set; Modification of the permanent policy won't be possible.";
    }

    /* ImplicitPolicyTarget */
    if (_config.hasSettingValue("ImplicitPolicyTarget")) {
      const std::string& target_string = _config.getSettingValue("ImplicitPolicyTarget");
      Rule::Target target = Rule::targetFromString(target_string);
      setImplicitPolicyTarget(target);
    }

    /* PresentDevicePolicy */
    if (_config.hasSettingValue("PresentDevicePolicy")) {
      const std::string& policy_string = _config.getSettingValue("PresentDevicePolicy");
      DevicePolicyMethod policy = Daemon::devicePolicyMethodFromString(policy_string);
      setPresentDevicePolicyMethod(policy);
    }

    /* PresentControllerPolicy */
    if (_config.hasSettingValue("PresentControllerPolicy")) {
      const std::string& policy_string = _config.getSettingValue("PresentControllerPolicy");
      DevicePolicyMethod policy = Daemon::devicePolicyMethodFromString(policy_string);
      setPresentControllerPolicyMethod(policy);
    }

    /* InsertedDevicePolicy */
    if (_config.hasSettingValue("InsertedDevicePolicy")) {
      const std::string& policy_string = _config.getSettingValue("InsertedDevicePolicy");
      DevicePolicyMethod policy = Daemon::devicePolicyMethodFromString(policy_string);
      setInsertedDevicePolicyMethod(policy);
    }

    /* IPCAllowedUsers */
    if (_config.hasSettingValue("IPCAllowedUsers")) {
      const std::string users_value = _config.getSettingValue("IPCAllowedUsers");
      std::vector<std::string> users;
      tokenizeString(users_value, users, " ", /*trim_empty=*/true);
      USBGUARD_LOG(Debug) << "Setting IPCAllowedUsers to { " << users_value << " }";

      for (auto const& user : users) {
        addIPCAllowedUser(user);
      }
    }

    /* IPCAllowedGroups */
    if (_config.hasSettingValue("IPCAllowedGroups")) {
      const std::string groups_value =_config.getSettingValue("IPCAllowedGroups");
      std::vector<std::string> groups;
      tokenizeString(groups_value, groups, " ", /*trim_empty=*/true);
      USBGUARD_LOG(Debug) << "Setting IPCAllowedGroups to { " << groups_value << " }";

      for (auto const& group : groups) {
        addIPCAllowedGroup(group);
      }
    }

    /* DeviceRulesWithPort */
    if (_config.hasSettingValue("DeviceRulesWithPort")) {
      const std::string value = _config.getSettingValue("DeviceRulesWithPort");
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
      const std::string value = _config.getSettingValue("RestoreControllerDeviceState");

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
      const std::string value = _config.getSettingValue("IPCAccessControlFiles");
      loadIPCAccessControlFiles(value);
    }

    /* AuditBackend */
    if (_config.hasSettingValue("AuditBackend")) {
      const std::string value = _config.getSettingValue("AuditBackend");
      USBGUARD_LOG(Debug) << "Setting AuditBackend to " << value;

      if (value == "LinuxAudit") {
        std::unique_ptr<AuditBackend> backend(new LinuxAuditBackend());
        _audit.setBackend(std::move(backend));
      }
      else if (value == "FileAudit") {
        if (_config.hasSettingValue("AuditFilePath")) {
          const std::string value = _config.getSettingValue("AuditFilePath");
          USBGUARD_LOG(Debug) << "Setting AuditFilePath to " << value;
          USBGUARD_LOGGER.setAuditFile(true, value);
        }
        else {
          /* AuditFilePath value is required is AuditBackend is set to FileAudit */
          throw Exception("Configuration", "AuditBackend", "AuditFilePath not set");
        }

        std::unique_ptr<AuditBackend> backend(new FileAuditBackend());
        _audit.setBackend(std::move(backend));
      }
      else {
        throw Exception("Configuration", "AuditBackend", "Invalid value");
      }
    }
    else {
      /*
       * Left for backwards compatibility. If AuditBackend is NOT set, but
       * AuditFilePath is, we set the backend to FileAudit automatically.
       */

      /* AuditFilePath */
      if (_config.hasSettingValue("AuditFilePath")) {
        const std::string value = _config.getSettingValue("AuditFilePath");
        USBGUARD_LOG(Debug) << "Setting AuditFilePath to " << value;
        USBGUARD_LOGGER.setAuditFile(true, value);
        std::unique_ptr<AuditBackend> backend(new LinuxAuditBackend());
        _audit.setBackend(std::move(backend));
      }
    }

    USBGUARD_LOG(Info) << "Configuration loaded successfully.";
  }

  void Daemon::loadRules(const std::string& path, const bool check_permissions)
  {
    USBGUARD_LOG(Info) << "Loading permanent policy file " << path;

    if (check_permissions) {
      checkPermissions(path, (S_IRUSR | S_IWUSR));
    }

    _nss.setRulesPath(path);
    auto ruleset = _nss.getRuleSet(this);
    ruleset->load();
    _policy.setRuleSet(ruleset);
  }

  void Daemon::loadIPCAccessControlFiles(const std::string& path)
  {
    USBGUARD_LOG(Info) << "Loading IPC access control files at " << path;
    loadFiles(path,
    [](const std::string& path, const struct dirent * dir_entry) {
      (void)dir_entry;
      return filenameFromPath(path, /*include_extension=*/true);
    }
    ,
    [this](const std::string& basename, const std::string& fullpath) {
      return loadIPCAccessControlFile(basename, fullpath);
    });
  }

  void Daemon::checkIPCAccessControlName(const std::string& name)
  {
    IPCServer::checkAccessControlName(name);
  }

  void Daemon::parseIPCAccessControlFilename(const std::string& basename, std::string* const ptr_user,
    std::string* const ptr_group)
  {
    const auto ug_separator = basename.find_first_of(":");
    const bool has_group = ug_separator != std::string::npos;
    const std::string user = basename.substr(0, ug_separator);
    const std::string group = has_group ? basename.substr(ug_separator + 1) : std::string();
    checkIPCAccessControlName(user);
    checkIPCAccessControlName(group);
    *ptr_user = user;
    *ptr_group = group;
  }

  bool Daemon::loadIPCAccessControlFile(const std::string& basename, const std::string& fullpath)
  {
    USBGUARD_LOG(Info) << "Loading IPC access control file " << fullpath;
    std::string user;
    std::string group;
    IPCServer::AccessControl ac;

    try {
      parseIPCAccessControlFilename(basename, &user, &group);
    }
    catch (...) {
      USBGUARD_LOG(Warning) << "Ignoring access control file because of malformed name: " << basename;
      return false;
    }

    try {
      std::ifstream ac_stream(fullpath);
      ac.load(ac_stream);
    }
    catch (...) {
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

    switch (policy) {
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

      switch (signal_num) {
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
    }
    while (!exit_loop);

    if (pid_fd != -1) {
      (void)lockf(pid_fd, F_ULOCK, 0);
      (void)close(pid_fd);
    }

    IPCServer::stop();
    _dm->stop();
    USBGUARD_LOG(Trace) << "Leaving main loop.";
  }

  void Daemon::quit()
  {
  }

  void Daemon::daemonize(const std::string& pid_file)
  {
    USBGUARD_LOG(Trace) << "Starting daemonization";
    pid_t pid = 0;
    pid_t original_pid = getpid();
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, nullptr);
    USBGUARD_SYSCALL_THROW("Daemonize", (pid = fork()) < 0);

    if (pid > 0) {
      constexpr int timeout_val = 5;
      struct timespec timeout {
        timeout_val, 0
      };
      const time_t start = time(nullptr);
      siginfo_t info;

      do {
        const int signum = sigtimedwait(&mask, &info, &timeout);

        if (signum == SIGUSR1 && info.si_signo == SIGUSR1 && info.si_pid == pid) {
          USBGUARD_LOG(Trace) << "Finished daemonization";
          exit(EXIT_SUCCESS);
        }

        if (signum == -1 && errno == EAGAIN) {
          break; /* timed out */
        }

        timeout.tv_sec = timeout_val - difftime(time(nullptr), start); /* avoid potentially endless loop */
      }
      while (true);

      throw Exception("Deamonize", "signal",  "Waiting on pid file write timeout!");
    }

    /* Now we are forked */
    USBGUARD_SYSCALL_THROW("Daemonize", setsid() < 0);
    signal(SIGCHLD, SIG_IGN);
    USBGUARD_SYSCALL_THROW("Daemonize", (pid_fd = open(pid_file.c_str(), O_RDWR|O_CREAT, 0640)) < 0);
    USBGUARD_SYSCALL_THROW("Daemonize", (lockf(pid_fd, F_TLOCK, 0)) < 0);
    USBGUARD_SYSCALL_THROW("Daemonize", (pid = fork()) < 0);

    if (pid > 0) {
      try {
        std::string pid_str = std::to_string(pid);
        USBGUARD_SYSCALL_THROW("Daemonize", write(pid_fd, pid_str.c_str(), pid_str.size()) != static_cast<ssize_t>(pid_str.size()));
        kill(original_pid, SIGUSR1);
        exit(EXIT_SUCCESS);
      }
      catch (...) {
        kill(pid, SIGKILL);
        throw;
      }
    }

    /* Now we are forked 2nd time */
    umask(0047);  /* no need for world-accessible or executable files */
    (void)chdir("/");
    const std::array<int, 3> std_fds {{STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO}};
    int fd_null;
    USBGUARD_SYSCALL_THROW("Daemonize", (fd_null = open("/dev/null", O_RDWR)) < 0);

    /* We do not need to close all fds because there is only logging open at this point */
    for (auto fd : std_fds) {
      USBGUARD_SYSCALL_THROW("Daemonize", close(fd));
      USBGUARD_SYSCALL_THROW("Daemonize", (dup2(fd_null, fd)) < 0);
    }

    close(fd_null);
    USBGUARD_SYSCALL_THROW("Daemonize", (lockf(pid_fd, F_LOCK, 0)) < 0);
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
      _policy.getRuleSet()->save();
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

    const uint32_t id = _policy.getRuleSet()->appendRule(rule, parent_id);
    if (_config.hasSettingValue("RuleFile")) {
      _policy.getRuleSet()->save();
    }

    USBGUARD_LOG(Trace) << "return: id=" << id;
    return id;
  }

  void Daemon::removeRule(uint32_t id)
  {
    USBGUARD_LOG(Trace) << "id=" << id;
    
    _policy.removeRule(id);
    if (_config.hasSettingValue("RuleFile")) {
      _policy.getRuleSet()->save();
    }
  }

  const std::shared_ptr<RuleSet> Daemon::listRules(const std::string& query)
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
    std::shared_ptr<Device> device = _dm->getDevice(id);
    std::shared_ptr<Rule> rule;

    if (permanent) {
      rule = upsertDeviceRule(id, target);
    }
    else {
      rule = std::make_shared<Rule>();
      rule->setTarget(target);
    }

    dmApplyDevicePolicy(device, rule);
    USBGUARD_LOG(Trace) << "return:"
      << " id=" << rule->getRuleID();
    return rule->getRuleID();
  }

  void Daemon::dmHookDeviceEvent(DeviceManager::EventType event, std::shared_ptr<Device> device)
  {
    USBGUARD_LOG(Trace) << "event=" << DeviceManager::eventTypeToString(event)
      << " device_ptr=" << device.get();
    auto audit_event = _audit.deviceEvent(device, event);
    std::shared_ptr<const Rule> device_rule = \
      device->getDeviceRule(/*with_port*/true,
        /*with_parent_hash=*/true);
    DevicePresenceChanged(device->getID(),
      event,
      device->getTarget(),
      device_rule->toString());
    audit_event.success();
    std::shared_ptr<Rule> policy_rule = nullptr;

    switch (event) {
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

  void Daemon::dmHookDeviceException(const std::string& message)
  {
    USBGUARD_LOG(Warning) << message;
  }

  void Daemon::dmApplyDevicePolicy(std::shared_ptr<Device> device, std::shared_ptr<Rule> matched_rule)
  {
    USBGUARD_LOG(Trace) << "device_ptr=" << device.get()
      << " matched_rule_ptr=" << matched_rule.get();
    auto audit_event = _audit.policyEvent(device, device->getTarget(), matched_rule->getTarget());
    const Rule::Target target_old = device->getTarget();
    std::shared_ptr<Device> device_post = \
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

      std::shared_ptr<const Rule> device_rule = \
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

  std::shared_ptr<Rule> Daemon::getInsertedDevicePolicyRule(std::shared_ptr<Device> device)
  {
    USBGUARD_LOG(Trace) << "device_ptr=" << device.get();
    std::shared_ptr<const Rule> device_rule = \
      device->getDeviceRule(/*with_port=*/true,
        /*with_parent_hash=*/true,
        /*match_rule=*/true);
    Rule::Target target = Rule::Target::Invalid;
    std::shared_ptr<Rule> policy_rule;
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
      policy_rule = std::make_shared<Rule>();
      policy_rule->setTarget(target);
      policy_rule->setRuleID(Rule::RootID);
    }

    return policy_rule;
  }

  std::shared_ptr<Rule> Daemon::getPresentDevicePolicyRule(std::shared_ptr<Device> device)
  {
    USBGUARD_LOG(Trace) << "entry: device_ptr=" << device.get();
    std::shared_ptr<const Rule> device_rule = \
      device->getDeviceRule(/*with_port=*/true,
        /*with_parent_hash=*/true,
        /*match_rule=*/true);
    USBGUARD_LOG(Debug) << "device_rule=" << device_rule->toString();
    USBGUARD_LOG(Debug) << "isController=" << device->isController();
    const DevicePolicyMethod policy_method = \
      device->isController() ? _present_controller_policy_method : _present_device_policy_method;
    Rule::Target target = Rule::Target::Invalid;
    std::shared_ptr<Rule> matched_rule = nullptr;

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
      matched_rule = std::make_shared<Rule>();
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

  std::shared_ptr<Rule> Daemon::upsertDeviceRule(uint32_t id, Rule::Target target)
  {
    USBGUARD_LOG(Trace) << "entry:"
      << "id=" << id
      << "target=" << Rule::targetToString(target);
    std::shared_ptr<Device> device = _dm->getDevice(id);
    bool with_port = true && _device_rules_with_port;
    bool with_parent_hash = true;

    /*
     * Generate a port specific or agnostic rule depending on the target
     */
    switch (target) {
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
    std::shared_ptr<Rule> match_rule = device->getDeviceRule(false, false, /*match_rule=*/true);
    const std::string match_spec = match_rule->toString();
    USBGUARD_LOG(Debug) << "match_spec=" << match_spec;
    /* Generate new device rule */
    std::shared_ptr<Rule> device_rule = device->getDeviceRule(with_port, with_parent_hash);
    device_rule->setTarget(target);
    const std::string rule_spec = device_rule->toString();
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

  void Daemon::addIPCAllowedUID(const std::string& uid_string, const IPCServer::AccessControl& ac)
  {
    addIPCAllowedUID(stringToNumber<uid_t>(uid_string), ac);
  }

  void Daemon::addIPCAllowedGID(gid_t gid, const IPCServer::AccessControl& ac)
  {
    USBGUARD_LOG(Trace) << "gid=" << gid;
    IPCServer::addAllowedGID(gid, ac);
  }

  void Daemon::addIPCAllowedGID(const std::string& gid_string, const IPCServer::AccessControl& ac)
  {
    addIPCAllowedGID(stringToNumber<gid_t>(gid_string), ac);
  }

  void Daemon::addIPCAllowedUser(const std::string& user, const IPCServer::AccessControl& ac)
  {
    USBGUARD_LOG(Trace) << "user=" << user;

    if (isNumericString(user)) {
      addIPCAllowedUID(user, ac);
    }
    else {
      IPCServer::addAllowedUsername(user, ac);
    }
  }

  void Daemon::addIPCAllowedGroup(const std::string& group, const IPCServer::AccessControl& ac)
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

/* vim: set ts=2 sw=2 et */
