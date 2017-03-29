% USBGUARD-DAEMON.CONF(5)
% Daniel Kopeček <dkopecek@redhat.com>
% March 2017

# NAME

**usbguard-daemon.conf** -- USBGuard daemon configuration file

# DESCRIPTION

The **usbguard-daemon.conf** file is loaded by the USBGuard daemon after it parses its command-line options and is used to configure runtime parameters of the daemon. The default search path is */etc/usbguard/usbguard-daemon.conf*. It may be overridden using the **-c** command-line option, see **usbguard-daemon**(8) for further details.

# OPTIONS

**RuleFile**=<*path*>
:   The USBGuard daemon will use this file to load the policy rule set from it and to write new rules received via the IPC interface.

**IPCAllowedUsers**=<*username*> [<*username*> ...]
:   A space delimited list of usernames that the daemon will accept IPC connections from.

**IPCAllowedGroups**=<*groupname*> [<*groupname*> ...]
:   A space delimited list of groupnames that the daemon will accept IPC connections from.

**IPCAccessControlFiles**=<*path*>
:   Path to a directory holding the IPC access control files.

# IPC

# SECURITY CONSIDERATIONS

The daemon provides the USBGuard public IPC interface. Depending on your distribution defaults, access to this interface is limited to a certain group or a specific user only. Please set either the **IPCAllowedUsers**, **IPCAllowedGroups** or **IPCAccessControlFiles** options to limit access to the IPC interface. **Do not leave the ACL unconfigured as that will expose the IPC interface to all local users and will allow them to manipulate the authorization state of USB devices and modify the USBGuard policy**.

# ACCESS CONTROL

Access to the USBGuard IPC interface can be limited per user or group. Furthermore, by using the IPC Access Control files, it is possible to limit the access down to the level of Sections and Privileges as explained below.

## **Recommended**, fine-grained **IPCAccessControlFiles** option.

When you set **IPCAccessControlFiles**, the daemon will look for IPC access control files in the directory specified by the setting value. Each file in the directory is processed as follows:

 1. The basename of the file is interpreted as an username, UID, groupname or GID. If the names starts with `:` (colon), it is assumed that the rest of the name represents a group identifier (groupname or GID in case of a numeric-only string). Otherwise, it is interpreted as an user identifier (username or UID in case of numeric-only string).
 2. The contents of the file are parsed as `Section=privilege [privilege ...]` formatted lines which specify the section privileges. If a section is omitted, it is assumed that no privileges are given for that section.

Available sections and privileges:

 * **Devices**
   - modify: Change authorization state of devices including permanent changes (i.e. modification of device specific rules in the policy).
   - list: Ability to get a list of recognized devices and their attributes.
   - listen: Listen to device presence and device policy changes.
 * **Policy**
   - modify: Append rules to or remove any rules from the policy.
   - list: Ability to view the currently enforced policy.
 * **Exceptions**
   - listen: Receive exception messages.
 * **Parameters**
   - modify: Set values of run-time parameters.
   - list: Get values of run-time parameters.

The following is a generally usable and reasonably safe example of an access control file. It allows to modify USB device authorization state (`Devices=modify`), list USB devices (`Devices=list`), listen to USB device related events (`Devices=listen`), list USB authorization policy rules (`Policy=list`) and listen to exception events (`Exceptions=listen`):

```
Devices=modify list listen
Policy=list
Exceptions=listen
```

Instead of creating the access control files by yourself, you can use the `usbguard add-user` or `usbguard remove-user` CLI commands. See **usbguard**(1) for more details.

## Legacy **IPCAllowedUsers**  and **IPCAllowedGroups** options.

Example configuration allowing full IPC access to users *root*, *joe* and members of the group *wheel*:

```
IPCAllowedUsers=root joe
IPCAllowedGroups=wheel
```

# REPORTING BUGS

If you find a bug in this software or if you'd like to request a feature to be implemented, please file a ticket at <https://github.com/dkopecek/usbguard/issues/new>.

# COPYRIGHT

Copyright © 2017 Red Hat, Inc.  License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

# SEE ALSO

**usbguard-daemon**(8), **usbguard-rules.conf**(5)
