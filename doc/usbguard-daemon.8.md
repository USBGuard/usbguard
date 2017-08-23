% USBGUARD-DAEMON(8)
% Daniel Kopeček <dkopecek@redhat.com>
% June 2016

# NAME

**usbguard-daemon** -- USBGuard daemon

# SYNOPSIS

**usbguard-daemon** [*OPTIONS*]

# DESCRIPTION

The **usbguard-daemon** is the main component of the USBGuard software framework. It runs as a service in the background and enforces the USB device authorization policy for all USB devices. The policy is defined by a set of rules using a rule language described in **usbguard-rules.conf**(5). The policy and the authorization state of USB devices can be modified during runtime using the **usbguard**(1) tool.

# OPTIONS

**-d**
:   Enable debugging messages in the log.

**-f**
:   Enable classical daemon behavior (fork at start, sysV compliant).

**-s**
:   Log to syslog.

**-k**
:   Log to console. (default)

**-K**
:   Disable Logging to console.

**-l** <*path*>
:   Log to a file at *path*.

**-p** <*path*>
:   Write PID to a file at *path* (default: */var/run/usbguard.pid*).

**-c** <*path*>
:   Load configuration from a file at *path* (default: */etc/usbguard/usbguard-daemon.conf*).

**-h**
:   Show the help/usage screen.

# SECURITY CONSIDERATIONS

The daemon provides the USBGuard public IPC interface. Depending on your distribution defaults, the access to this interface is limited to a certain group or a specific user only. Please refer to the **usbguard-daemon.conf**(5) man page for more information on how to configure the ACL correctly. **Do not leave the ACL unconfigured as that will expose the IPC interface to all local users and will allow them to manipulate the authorization state of USB devices and modify the USBGuard policy**.

# BUGS

If you find a bug in this software or if you'd like to request a feature to be implemented, please file a ticket at <https://github.com/dkopecek/usbguard/issues/new>.

# COPYRIGHT

Copyright © 2015 Red Hat, Inc.  License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

# SEE ALSO

**usbguard-daemon.conf**(5), **usbguard-rules.conf**(5), **usbguard**(1), **usbguard-applet-qt**(1)
