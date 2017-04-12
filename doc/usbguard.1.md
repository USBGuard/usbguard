% USBGUARD(1)
% Daniel Kopeček <dkopecek@redhat.com>
% April 2017

# NAME

**usbguard** -- USBGuard command-line interface

# SYNOPSIS

usbguard [*OPTIONS*] *command* [*COMMAND-OPTIONS*] ...

usbguard **list-devices**

usbguard **allow-device** <*id*>

usbguard **block-device** <*id*>

usbguard **reject-device** <*id*>

usbguard **list-rules**

usbguard **append-rule** <*rule*>

usbguard **remove-rule** <*id*>

usbguard **generate-policy**

usbguard **watch**

usbguard **read-descriptor** <*file*>

usbguard **add-user** <*name*>

usbguard **remove-user** <*name*>

# DESCRIPTION

The **usbguard** command provides a command-line interface (CLI) to the **usbguard-daemon**(8) instance and provides a tool for generating initial USBGuard policies.

# SUBCOMMANDS

**list-devices**

List all USB devices recognized by the USBGuard daemon.

Available options:

**-a**, **--allowed**
:   List allowed devices.

**-b**, **--blocked**
:   List blocked devices.

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**allow-device** [*OPTIONS*] <*id*>

Authorize a device identified by the device *id* to interact with the system.

Available options:

**-p**, **--permanent**
:   Make the decision permanent. A device specific allow rule will be appended to the current policy.

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**block-device** [*OPTIONS*] <*id*>

Deauthorize a device identified by the device *id*.

Available options:

**-p**, **--permanent**
:   Make the decision permanent. A device specific block rule will be appended to the current policy.

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**reject-device** [*OPTIONS*] <*id*>

Deauthorize and remove a device identified by the device *id*.

Available options:

**-p**, **--permanent**
:   Make the decision permanent. A device specific reject rule will be appended to the current policy.

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**list-rules** [*OPTIONS*]

List the rule set (policy) used by the USBGuard daemon.

Available options:

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**append-rule** [*OPTIONS*] <*rule*>

Append the *rule* to the current rule set.

Available options:

**-a**, **--after** <*id*>
:   Append the new rule after a rule with the specified rule *id*.

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**remove-rule** [*OPTIONS*] <*id*>

Remove a rule identified by the rule *id* from the rule set.

Available options:

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**generate-policy** [*OPTIONS*]

Generate a rule set (policy) which authorizes the currently connected USB devices.

Available options:

**-p**, **--with-ports**
:   Generate port specific rules for all devices. By default, port specific rules are generated only for devices which do not export an iSerial value.

**-P**, **--no-ports-sn**
:   Don't generate port specific rules for devices without an iSerial value. Without this option, the tool will add a via-port attribute to any device that doesn't provide a serial number. This is a security measure to limit devices that cannot be uniquely identified to connect only via a specific port. This makes it harder to bypass the policy since the real device will occupy the allowed USB port most of the time.

**-t**, **--target** <*target*>
:   Generate an explicit "catch all" rule with the specified target. The target can be one of the following values: **allow**, **block**, **reject**

**-X**, **--no-hashes**
:   Don't generate a hash attribute for each device.

**-H**, **--hash-only**
:   Generate a hash-only policy.

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**watch** [*OPTIONS*]

Watch the IPC interface events and print them to stdout.

Available options:

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**read-descriptor** [*OPTIONS*] <*file*>

Read a USB descriptor from a file and print it in human-readable form.

Available options:

**-h**, **--help**
:   Show help.

~ ~ ~ ~

**add-user** <*name*> [*OPTIONS*]

Create an IPC access control file allowing the user/group identified by *name* to use the USBGuard IPC bus. The change takes effect only after restarting the **usbguard-daemon**(8) instance.

Available options:

**-u**, **--user**
:   The specified *name* represents a username or UID (default).

**-g**, **--group**
:   The specified *name* represents a groupname or GID.

**-p**, **--policy** <*privileges*>
:   Policy related privileges.

**-d**, **--devices** <*privileges*>
:   Device related privileges.

**-e**, **--exceptions** <*privileges*>
:   Exceptions related privileges.

**-P**, **--parameters** <*privileges*>
:   Run-time parameter related privileges.

**-h**, **--help**
:   Show help.

Privileges:

When specifying *privileges*, they are expected to be in the form of a list separated by a colon:

```
    $ sudo usbguard add-user joe --devices=listen,modify
```

Consult the **usbguard-daemon.conf**(5) man-page for a detailed list of available privileges in each section.

~ ~ ~ ~

**remove-user** <*name*> [*OPTIONS*]

Remove an IPC access control file associated with the user/group identified by *name*. The change takes effect only after restarting the **usbguard-daemon**(8) instance.

Available options:

**-u**, **--user**
:   The specified *name* represents a username or UID (default).

**-g**, **--group**
:   The specified *name* represents a groupname or GID.

**-h**, **--help**
:   Show help.


# EXAMPLES

**Creating an initial policy**

```
    $ sudo usbguard generate-policy > rules.conf
    $ vi rules.conf
    (review/modify the rule set)
    $ sudo install -m 0600 -o root -g root \
       rules.conf /etc/usbguard/rules.conf
```

# BUGS

If you find a bug in this software or if you'd like to request a feature to be implemented, please file a ticket at <https://github.com/dkopecek/usbguard/issues/new>.

# COPYRIGHT

Copyright © 2017 Red Hat, Inc.  License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

# SEE ALSO

**usbguard-rules.conf**(5), **usbguard-daemon**(8), **usbguard-daemon.conf**(5)
