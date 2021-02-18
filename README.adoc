USBGuard
========
:toc:

[.clearfix]
--
[.left]
image::https://travis-ci.com/USBGuard/usbguard.svg?branch=master[Travis CI, link=https://travis-ci.com/USBGuard/usbguard]
[.left]
image::https://coveralls.io/repos/github/USBGuard/usbguard/badge.svg[Coverage, link=https://coveralls.io/github/USBGuard/usbguard]
[.left]
image::https://img.shields.io/github/license/USBGuard/usbguard.svg[License, link=https://github.com/USBGuard/usbguard/#license]
--

== About

USBGuard is a software framework for implementing USB device authorization policies (what kind of USB devices are authorized) as well as method of use policies (how a USB device may interact with the system).
Simply put, it is a USB device allowlisting tool.

WARNING: The 0.x releases are not production ready packages.
They serve for tech-preview and user feedback purposes only.
Please share your feedback or request a feature in the Github issue trackers for each project:

 * https://github.com/USBGuard/usbguard/issues/new[Report a bug or request a feature in *usbguard*]

== Documentation

 * User Guide (TBA)
 * Manual Pages
 ** <<doc/man/usbguard-daemon.8.adoc#name, usbguard-daemon(8)>>
 ** <<doc/man/usbguard-daemon.conf.5.adoc#name, usbguard-daemon.conf(5)>>
 ** <<doc/man/usbguard-rules.conf.5.adoc#name, usbguard-rules.conf(5)>>
 ** <<doc/man/usbguard.1.adoc#name, usbguard(1)>>
 ** <<doc/man/usbguard-dbus.8.adoc#name, usbguard-dbus(8)>>

== Compilation & Installation

To compile the source code, you will require at least C{plus}{plus}17. +
If you are compiling sources from a release tarball, you'll need the development files for:

 * https://github.com/ClusterLabs/libqb[libqb] - used for local UNIX socket based IPC
 * https://github.com/google/protobuf[protobuf] - used for IPC message (de)serialization
 * https://download.libsodium.org[libsodium] or https://www.gnupg.org/software/libgcrypt[libgcrypt] - used for hashing
 * https://asciidoc.org[asciidoc (a2x)] - needed to generate documentation

Optionally, you may want to install:

 * https://github.com/seccomp/libseccomp[libseccomp] - used to implement a syscall whitelist
 * https://people.redhat.com/sgrubb/libcap-ng/[libcap-ng] - used to drop process capabilities

And then do:

    $ ./configure --with-crypto-library=sodium # or "gcrypt", based on your preference
    $ make
    $ sudo make install

After the sources are successfully built, you can run the test suite by executing:

    $ make check

If you want to compile the sources in a cloned repository, you'll have to run the `./autogen.sh` script.
It will fetch the sources (via git submodules) of https://github.com/taocpp/PEGTL/[PEGTL] and https://github.com/philsquared/Catch[Catch].
The script will then initialize the autotools based build system.

== License

Copyright (C) 2015-2019 Red Hat, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
