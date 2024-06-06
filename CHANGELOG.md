# Change Log

## 1.1.3 - 2024-06-06

### Fixed
- Addressed several cases where either RuleFile or RuleFolder was not set.
- Resolved a race condition during fork where the parent process did not wait until everything was initialized in the child process.
- Included missing documentation in the tarball.
- Fixed compatibility issues with GCC 13+.

### Added
- Implemented detection of integer overflow for device IDs to ensure that each device can be uniquely identified by a single ID.
- Enhanced the service file to disable the Linux kernel's Out-Of-Memory (OOM) killing of processes for this unit.
- D-bus: check if the client requested interactive authentication, as some clients do not prompt for passwords.
- Made minor adjustments to the documentation.
- RuleFolder is enabled by default. At startup, the path to folder must exist and be accessible by the daemon.

## 1.1.2 - 2022-09-02

### Fixed
- Polkit: Always allow getParameter/listDevices/listRules in active sessions
- D-Bus: Send reply on auth failure
- Polkit: Unreference PolkitAuthorizationResult and PolkitAuthority structs if needed

## 1.1.1 - 2022-03-15

### Fixed
- Restore support for access control filenames without a group

## 1.1.0 - 2022-02-24

### Added
- Started building with C++17
- Tree-like list-devices output
- Added CAP_AUDIT_WRITE capability to service file
- Added support for lower OpenSSL versions prior to 1.1.0
- Added a new signal: DevicePolicyApplied

### Fixed/Changed
- Moved PIDFile from /var/run to /run
- Fixed linker isssues with disable-static
- Enhanced bash-completion script
- Make username/group checking consistent with useradd manual page definition 
  (with addition of capital letters)
- Fixed multiple IPC related bugs
- Fixed race condition when accessing port/connect_type for USB devices
- Using bundled catch v2.13.8 
- Using bundled PEGTL v3.2.5
- Fixed usbguard-rule-parser file opening
- Fix unauthorized access via D-Bus [CVE-2019-25058]


## 1.0.0 - 2021-01-13

### Added
- Added openssl support
- Starting with libtool versioning
- Added interface for IPC permission query
- Introduced partial rule concept fo CLI
- Added WithConnectType for ldap rule

### Fixed/Changed
- Daemon does not apply the policy when
  "change" action event appears anymore
- IPCClientPrivate@disconnect is thread safe
- Enforced loading of files from .d/ directory
  in alphabetical order
- Improved CLI behaviour to be consistent
- Clarified rule's label documentation


## 0.7.8 - 2020-05-20

### Fixed
- Fixed segfaults with rules.d feature


## 0.7.7 - 2020-05-19

### Added
- Added readwritepath to service file
- Added match-all keyword to rules language
- Added rules.d feature
 - daemon can load multiple rule files from rules.d/
- Included with-connect-type in dbus signal

### Fixed/Changed
- Fixed sigwaitinfo handling
- Fixed possible data corruption on stack with appendRule via dbus
- Fixed ENOBUFS errno handling on netlink socket
 - daemon can survive and wait until socket is readable again

### Removed
- Dropped unused PIDFile from service file
- Dropped deprecated dbus-glib dependency


## 0.7.6 - 2019-11-08

### Added
- Added missing options in manpage usbguard-daemon(8)
- Extended the functionality of allow/block/reject commands
 - the command can handle rule as a param and not only its ID
 - e.g. in case of allow, command will allow each device that matches provided rule
- Added debug info for malformed descriptors

### Fixed/Changed
- Changed default backend to uevent
- Fixed handling of add uevents during scanning
 - now we are sure that the enumeration is completed before processing any uevent
 - we are trying to avoid a race where the kernel is still enumerating the devices
 - and send the uevent while the parent is being authorised
- Silenced 'bind' and 'unbind' uevents


## 0.7.5 - 2019-07-02

### Added
- Added daemon configuration option HidePII
- Added check to avoid conflict between ASAN and TSAN
- Added daemon configuration option for authorized_default
- Added devpath option to generate-policy
- Added # line comments to the rule grammar
- Added ImplicitPolicyTarget to get/set parameter methods
- Added option to filter rules by label when listing
- Added the label attribute to rule
- Added PropertyParameterChanged signal
- Added support for portX/connect_type attribute
- Added temporary option to append-rule
- Added versioning to DBus service
- Added optional LDAP support

### Fixed/Changed
- Fixed invalid return value in Rule::Attribute::setSolveEqualsOrdered
- Fixed KeyValueParser to validate keys only when known names are set
- Fixed uninitialized variables found by coverity
- Fixes and cleanups based on LGTM.com report
- Hardened systemd service
- Rename ListRules parameter 'query' to 'label'
- Skip empty lines in usbguard-rule-parser

### Removed
- The proof-of-concept Qt applet was removed. It is going to be maintained in a simplified form as a separate project.


## 0.7.4 - 2018-07-12

### Fixed/Changed

- Fixed conditional manual page generation & installation
- Replaced Boost library based ext/stdio_filebuf.h implementation
  with a custom FDStreamBuf implementation


## 0.7.3 - 2018-07-11

### Changed

- usbguard-daemon will now exit with an error if it fails to open
  a logging file or audit event file.
- Updated PEGTL submodule and dropped support for older PEGTL API
- Modified the present device enumeration algorithm to be more
  reliable. Enumeration timeouts won't cause usbguard-daemon process
  to exit anymore.
- Manual pages are now generated using asciidoc (a2x) instead of
  asciidoctor.
- Generation and installation of manual pages is now optional.
- Fixed several bugs D-Bus interface XML specification

### Added

- umockdev based device manager capable of simulating devices based
  on umockdev-record files.
- Boost libraries can be used as ext/stdio_filebuf.h header file source.

### Removed

- Removed DummyDevices.tar.xz tarball that was supposed to be used for
  testing.

## 0.7.2 - 2018-01-22

### Fixed/Changed

- Fixed memory leaks in usbguard::Hash class.
- Fixed file descriptor leaks in usbguard::SysFSDevice class.
- Skip audit backend logging when no backend was set.

### Added

- Added zsh completion & other scripts to the distribution tarball.

## 0.7.1 - 2017-11-30

### Added
- CLI: usbguard watch command now includes an -e <path> option to run an executable
  for every received event. Event data are passed to the executable via environment
  variables.
- usbguard-daemon: added "-K" option which can disable logging to console.
- Added zsh autocompletion support.
- usbguard-daemon: added "-f" option which enabled double-fork daemonization procedure.
- Added AuditBackend usbguard-daemon configuration option for selecting audit log backend.
- Linux Audit support via new LinuxAudit backend.
- Added missing RuleCondition.hpp header file to the public API headers.
- Code Style specification via AStyle configuration file.

### Removed
- Removed Utility.hpp header file from public API headers
- Reduced usage of raw C pointers throughout the code

### Changed
- Qt Applet: disabled session management
- usbguard-daemon console logging output is enabled by default now. Previously,
  the -k option had to be passed to enable the output.
- Replaced --enable-maintainer-mode configure option with --enable-full-test-suite
  option. When the new option is not used during the configure phase, only a basic
  set of test is run during the make check phase.
- usbguard-daemon now opens configuration in read-only mode
- Fixed UEventDeviceManager to work with Linux Kernel >= 4.13
- Refactored audit logging to support different audit log backends
- Reformatted source code to conform to the code style.
- Made the configuration parser strict. Unknown directives and wrong syntax will
  cause an error.
- Reformatted documentation from markdown to asciidoc format.

## 0.7.0 - 2017-04-12
### Added
- Added InsertedDevicePolicy configuration option to control the policy
  method for inserted devices.
- Added RestoreControllerDeviceState configuration option.
- Added DeviceManagerBackend configuration option. This option can be used
  to select from several device manager backend implementations.
- Implemented an uevent based device manager backend.
- Added setParameter, getParameter IPC (incl. D-Bus) methods.
- Added set-parameter, get-parameter CLI subcommands.
- Qt Applet: Added Spanish (es_AR) translation.
- Create empty rules.conf file at install time (make install).
- Support for numeric UID/GID values in IPCAllowedUsers and IPCAllowedGroups
  settings.
- If bash completion support is detected at configure time, install the bash
  completion script during make install.
- Added new configuration setting: IPCAccessControlFiles.
- IPC access is now configurable down to a section and privilege
  level per user and/or group.
- Added add-user, remove-user usbuard CLI subcommands for creating, removing
  IPC access control files.
- Added AuditFilePath configuration option for setting the location of the
  USBGuard audit events log file path. If set, the usbguard-daemon will log
  policy and device related actions and whether they succeeded or not.

### Removed
- Removed UDev based device manager backend and UDev related dependencies.
- Removed UDev development files/API dependency

### Changed
- Reset Linux root hub bcdDevice value before updating device hash. This is
  a backwards incompatible change because it changes how the device hash is
  computed for Linux root hub devices.
- Refactored low-level USB device handling into SysFSDevice class which
  represents a device in the /sys filesystem (sysfs).
- Removed usage of `readdir_r` because it's obsolete. Replaced with readdir
  with the assumption that its usage is thread-safe if the directory handle
  passed to it is not shared between threads.
- Extended test suite with use case tests.
- Install the usbguard-daemon configuration and policy file with strict file
  permissions to prevent policy leaks.
- Fixed several memory leaks.
- Don't pre-resolve user and group names in IPCAllowedUsers and IPCAllowedGroups
  settings. Instead, resolve the name during the IPC authentication phase.
- Changed default values of PresentDevicePolicy (to apply-policy) and
  IPCAllowedGroups (removed wheel group entry) settings in the shipped
  usbguard-daemon.conf configuration file.

## 0.6.2 - 2016-09-18
### Changed
- Wait for disconnect in IPCClient dtor if needed
- Qt Applet: Fixed loading of decision method and default decision settings

## 0.6.1 - 2016-09-16
### Changed
- Refactored logging subsystem
- Fixed handling of IPC disconnect in the IPCClient class
- Qt Applet: Fixed handling of main window minimization and maximization
- Fixed building on architectures that don't provide required atomic operations.
  The libatomic emulation library will be used in such cases.
- Fixed several typos in the documentation

### Added
- Implemented a simple internal logger
- Access to the logger via public API
- Improved logging coverage. Logging output can be enabled either via
  CLI options or by setting the `USBGUARD_DEBUG` environment variable to 1.
- Qt Applet: UI translation support.
- Qt Applet: Czech (cs_CZ) translation

### Removed
- Removed spdlog dependency

## 0.6.0 - 2016-09-04
### Changed
- Fixed the daemon to use the match target when matching device rules.
  This fixes matching devices against the rules in the policy.
- Switched to protobuf based IPC
- Simplified the IPC and D-Bus interfaces
- Refactored custom exceptions

### Added
- Added DevicePresenceChanged signal for notifying about device
  insertions, updates and removals
- Added DevicePolicyChanged signal for notifying about device
  policy changes.
- Added ExceptionMessage signal for sending asynchronous exceptions
  to the IPC/D-Bus clients.
- Extended the usbguard CLI watch subcommand with the ability to
  wait for the IPC connection to become available. See the new `-w`
  and `-o` options for details.

### Removed
- Removed nlohmann/json submodule
- Removed allowDevice, blockDevice and rejectDevice methods from
  the IPC and D-Bus interface.
- Removed DeviceAllowed, DeviceBlocked and DeviceRejected signals
  from the IPC and D-Bus interface (replaced by single signal)
- Removed DeviceInserted, DevicePresent and DeviceRemoved signals
  from the IPC and D-Bus interface (replaced by single signal)

## 0.5.14 - 2016-08-18
### Changed
- Fixed unknown descriptor type handling

## 0.5.13 - 2016-08-16
### Changed
- refactored USB parser to support multiple handlers per USB descriptor type
- Qt Applet: device list should be always expanded

### Added
- added support for an audio device related endpoint descriptor

## 0.5.12 - 2016-08-14
### Changed
- Fixed a bug in matching USB interface types with wildcards
- usbguard-daemon will now abort at startup if the rule file contains
  syntax error instead of continuing with an empty rule set

## 0.5.11 - 2016-08-13
### Added
- Maintainer script for spellchecking source archive files
- Rule::Attribute class for representing the rule attributes
- USBDeviceID class for representing the USB device ID
- configure script option to control the bundling of PEGTL source files
- id attribute to the rule language for specifying the USB device ID
- Added a parent device ID field (and methods) to the Device class which
  tracks the ID of the parent device
- Implemented "parent-hash" attribute for associating a device with its
  parent device.
- The QtSvg module/library is now needed to compile the Qt applet
- Qt Applet: Device Dialog settings work now
- Qt Applet: The settings state is now remembered (via Qt's QSettings class)
- Qt Applet: Implemented serial number masking
- Qt Applet: Initial implementation of DeviceDialog window position randomization
- Qt Applet: IPC exceptions are shown in the message log
- Qt Applet: added an initial implementation of the device list with the ability
             to change authorization target for each device
- Qt Applet: show a grey version of the USBGuard icon in IPC disconnected state
- usbguard-daemon.conf: added DeviceRulesWithPort setting (set to false by default)
- Added support for selecting crypto backend library at compile time using the
  --with-crypto-library configure script switch

### Changed
- **IMPORTANT**: The device hash value computation was changed to include the
  USB descriptor data. Additionally, the algorithm was changed to SHA-256 and
  the hash value representation to base64. These changes are backwards incompatible
  and existing policies that use the hash attribute need to be updated.
- Reimplemented the rule parser using PEGTL
- Changed public API of the Rule and Device classes because of the new
  Rule::Attribute class rule attribute representation
- Extended the public IPCClient::IPCDisconnected method to include exception
  related information
- All rule attributes now support both the single and multivalued form
- A rule attribute can now be specified only once
- The default usbguard-daemon.conf and usbguard.service files now respect
  the paths set by the configure script
- New Qt applet icons with
- Fixed Qt applet so that it doesn't show an empty window when starting
- Qt Applet: Reject button is hidden by default.
- Updated usbguard-daemon manual page
- Permanent device specific rules managed by allowDevice, blockDevice and
  rejectDevice actions are now handled properly. Existing device rules are
  updated instead of just appending new rules to the policy.
- usbguard-daemon.conf: changed the default configuration value of
  PresentControllerPolicy to keep
- Changed the device hashing algorithm to SHA-256
- Switched hash value representation from hex to base64

### Removed
- Removed Quex related files
- The "from Rule" Device class constructor was removed because it's use case
  is unclear and it wasn't used anywhere

## 0.5.10 - 2016-07-08
### Changed
- Changed versioning scheme to include a release number. Releases will be
  rolled out more often during a milestone (x.y -- currently 0.5) development.
- Enhancements of the usbguard command-line interface.
- Manual pages are now generated from their source (markdown) during the build
  process.
- A default usbguard-daemon configuration file is now installed.

### Added
- Merged usbguard-applet-qt (sub)project into usbguard.
- Added a D-Bus bridge component which exposes a D-Bus interface to usbguard.

### Fixed
- Fixed USB descriptor data parsing. Previously, many real-world cases of
  descriptors weren't parsed correctly because of a naive implementation of the
  parser.
- Fixed systemd integration.
- Fixed issues: #84, #83, #80, #79, #78, #77, #76, #75, #74, #73, #72, #71,
  #70, #69, #68, #56, #54, #53, #51, #50, #44.

## 0.4 - 2016-02-07
### Changed
- The daemon is now capable of dropping process capabilities and uses a seccomp
  based syscall allowlist. Options to enable these features were added to the
  usbguard-daemon command.
- Devices connected at the start of the daemon are now recognized and the
  DevicePresent signal is sent for each of them.
- The library API was extended with the Device and DeviceManager classes.
- Implemented the usbguard CLI, see usbguard(1) for available commands.
- Initial authorization policies can be now easily generated using the usbguard
  generate-policy command.
- Extended the rule language with rule conditions. See usbguard-rules.conf(5)
  for details.
- Moved logging code into the shared library. You can use static methods of the
  Logger class to configure logging behaviour.
- Removed the bundled libsodium and libqb libraries.

### Added
- New configuration options for setting the implicit policy target and how to
  handle the present devices are now available.

### Fixed
- String values read from the device are now properly escaped and length limits
  on these values are enforced.
- Fixed several bugs.
- Resolved issues: #46, #45, #41, #40, #37, #32, #31, #28, #25, #24, #21, #16,
  #13, #9, #4

### Removed
- The device hashing procedure was altered and generates different hash values.
  If you are using the hash attribute in your rules, you'll have to update the
  values.
- The bundled libsodium and libqb were removed. You'll have to compile and
  install them separately if your distribution doesn't provide them as
  packages.

## 0.3p3 - 2015-04-14
### Changed
- use AC_CHECK_HEADER instead of a pkg-config based check for json and spdlog
- make check target available

## 0.3p2 - 2015-04-02
### Changed
- SHM permissions are set to uid of the client process and gid of the daemon
  with rw-rw---- mode (0660).
- added --with-bundled-json and --with-bundled-spdlog configure options to
  enable/disable the usage of the bundled libraries

## 0.3p1 - 2015-04-10
### Removed
- removed bundled cppformat copylib

## 0.3 - 2015-04-09
### Changed
- Implemented a DAC based IPC access control

## Added
- Ship man pages for usbguard-daemon, usbguard-daemon.conf and
  usbguard-rules.conf
- Ship the LICENSE file

### Fixed
- Fixed appending of permanent rules
- Fixed distribution RPM spec file. Thanks to Petr Lautrbach and Ralf
  Corsepius for review.
- Resolved issues: #18 #19 #13

## 0.2 - 2015-04-03
### Changed
- Reworked device hashing.
- Rule language changes
  - set operators
  - renamed "port" to "via-port"
  - added "with-interface" matching attribute
  - removed the "class" attribute
- The IPCClient, ConfigFile, Rule and RuleSet classes are now shipped in a
  shared library.
- Created abstract interface for OS specific USB device handling.
- Changed default daemon config path to /etc/usbguard/usbguard-daemon.conf.
- Implemented basic USB descriptor structure parsing and improved
  interface type handling
- The IPC API was changed:
  - added new signal, DevicePresent, which signals that a device was
    already present at the start of the IPC session
  - the DeviceInserted and DevicePresent signals pass interface types that
    the device supports
  - the explicit string arguments of the signals are now passed as a map

### Added
- Support for modifying permanent rules over the IPC interface.

### Fixed
- Resolved issues: #1 #2 #5 #6 #10 #11
