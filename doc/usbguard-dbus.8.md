% USBGUARD-DBUS(8)
% Daniel Kopeček <dkopecek@redhat.com>; Muri Nicanor <muri@immerda.ch>
% July 2016

# NAME

**usbguard-dbus** -- USBGuard D-Bus interface service

# SYNOPSIS

**usbguard-dbus** [*OPTIONS*]

# DESCRIPTION

The usbguard-dbus component exposes a D-Bus interface to a running instance of the **usbguard-daemon(8)**.

# OPTIONS

**-s**, **--system**
:   Listen on the system bus.

**-S**, **--session**
:   Listen on the session bus.

**-h**, **--help**
:   Show the help/usage screen.

# BUGS

If you find a bug in this software or if you'd like to request a feature to be implemented, please file a ticket at <https://github.com/dkopecek/usbguard/issues/new>.

# COPYRIGHT

Copyright © 2015 Red Hat, Inc.  License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

# SEE ALSO

**usbguard**(1), **usbguard-applet-qt**(1)
