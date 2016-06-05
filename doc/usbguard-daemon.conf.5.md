% USBGUARD-DAEMON.CONF(5)
% Daniel Kopeček <dkopecek@redhat.com>
% June 2016

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

# SECURITY CONSIDERATIONS

The daemon provides the USBGuard public IPC interface. Depending on your distribution defaults, the access to this interface is limited to a certain group or a specific user only. Please set either the **IPCAllowedUsers** or **IPCAllowedGroups** options to limit access to the IPC interface. **Do not leave the ACL unconfigured as that will expose the IPC interface to all local users and will allow them to manipulate the authorization state of USB devices and modify the USBGuard policy**.

# BUGS

If you find a bug in this software or if you'd like to request a feature to be implemented, please file a ticket at <https://github.com/dkopecek/usbguard/issues/new>.

# COPYRIGHT

Copyright © 2015 Red Hat, Inc.  License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

# SEE ALSO

**usbguard-daemon**(8), **usbguard-rules.conf**(5)
