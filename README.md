# usbguard

The usbguard software framework helps to protect your computer against rogue USB devices by implementing basic
whitelisting/blacklisting capabilities based on USB device attributes.

**WARNING**: The 0.x releases are not production ready packages. They serve for tech-preview and user feedback purposes
only. Please share your feedback or request a feature in the Github issue trackers for each project:

Report a bug or request a feature in [usbguard](https://github.com/dkopecek/usbguard/issues/new)
Report a bug or request a feature in [usbguard-applet-qt](https://github.com/dkopecek/usbguard-applet-qt/issues/new)

## Supported Operating Systems

Currently, usbguard works only on Linux. To enforce the user-defined policy, it uses the USB device authorization feature
implemented in the Linux kernel since the year 2007. Read [this document](https://www.kernel.org/doc/Documentation/usb/authorization.txt)
if you want to know more.

## Compilation

If you want to compile the sources from a release tarball, you'll have to install development files for:

 * [libqb](https://github.com/ClusterLabs/libqb) - used for IPC
 * [libsodium](http://libsodium.org) - used for hashing
 * systemd-devel - used for udev

And then do:

    $ ./configure
    $ make
    $ sudo make install

If you want to compile the sources in a cloned repository, there's one additional step required:

	$ git submodule init

This will fetch the sources of [cppformat](https://github.com/cppformat/cppformat), [json](https://github.com/nlohmann/json/)
and [spdlog](https://github.com/gabime/spdlog) which are used in this project too.

## Pre-compiled packages

### Fedora Linux

Pre-compiled packages for Fedora 20, 21, 22 and rawhide are distributes using a Copr [repository](https://copr.fedoraproject.org/coprs/mildew/usbguard/).
You can install the repository by executing the following steps:

    $ sudo yum install yum-plugin-copr
    $ sudo yum copr enable mildew/usbguard
    $ sudo yum install usbguard

To actually start the daemon, use:

    $ sudo systemctl start usbguard.service

## Rules

The usbguard daemon decides which USB device to authorize based on a policy defined by a set of rules. When an USB device is inserted into
the system, the daemon scans the existing rules sequentially and when a matching rule is found, it either authorizes (allows), deauthorizes
(blocks) or removes (rejects) the device, based on the rule target. If no matching rule is found, the decision is based on an implicit default
target. This implicit default is to block the device until a decision is made by the user.

The rule language grammar, expressed in a BNF-like syntax, is the following:

    rule ::= target device.

    target ::= "allow" | "block" | "reject".

    device ::= device_id device_attributes.
    device ::= .

    device_id ::= "*:*" | vendor_id ":*" | vendor_id ":" product_id.

    device_attributes ::= device_attributes | attribute.
    device_attributes ::= .

    attribute ::= name string.

### Targets

The target of a rule specifies whether the device will be authorized for use or not. Three types of target are recognized:

 * `allow` - authorize the device
 * `block` - deauthorize the device
 * `reject` - remove the device from the system

### Device specification

Except the target, all the other fields of a rule need not be specified. Such a minimal rule will match any device and
allows the policy creator to write an explicit default target (there's an implicit one too, more on that later).
However, if one want's to narrow the applicability of a rule to a set of devices or one device only, it's possible to
do so with a device id and/or device attributes.

#### Device ID

A USB device ID, as understood by usbguard, is the colon delimited pair *vendor\_id:product\_id*. All USB devices have this
ID assigned by the manufacturer and it should uniquely identify a USB product. Both *vendor\_id* and *product\_id* are 16-bit
numbers represented in hexadecimal base.

In the rule, it's possible to use an asterisk character to match either any device ID `*:*` or any product ID from a
specific vendor, e.g. `1234:*`.

#### Device attributes

Device attributes are specific value read from the USB device after it's inserted to the system. Which attributes are
available is defined bellow. Some of the attributes are derived or based on attributes read directly from the device.
The value of an attribute is represented as a double-quoted string.

List of attributes:

 * `class "NN"`
 * `hash "[0-9a-f]{32}"`
 * `name "..."`
 * `port "[0-9]{1,2}-[0-9]{1,2}"`
 * `port { "[0-9]{1,2}-[0-9]{1,2}" "[0-9]{1,2}-[0-9]{1,2}" ... }`

### Initial policy

There's currently no tool to generate an initial rule set for devices that the user wants to allow by default.
However, using the following bash oneliner, you can generate a simple allow policy for USB devices connected
to the system at the moment of generating the policy:

    # lsusb | sed -n 's|.*\([0-9a-f]\{4\}:[0-9a-f]\{4\}\) \(.*\)|allow \1|p' > /etc/usbguard/rules.conf

Future releases will include a tool to generate more complex initial policies.
