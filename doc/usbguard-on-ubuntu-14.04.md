How to install the USBGuard framework on Ubuntu 14.04
=====================================================

This how to will shows how to install the USBGuard software framework
on Ubuntu 14.04. It should work for other versions of Ubuntu and on
Debian too. However, the steps showed in this document were tested only
on Ubuntu 14.04.

## 1. Install packages required to fetch and build the sources

    $ sudo apt-get install autoconf automake libtool g++ git checkinstall

## 2. Fetch, build and install the USBGuard daemon and CLI tools

    $ sudo apt-get install libudev-dev libqb-dev libcap-ng-dev libseccomp-dev
    $ git clone https://github.com/dkopecek/usbguard
    $ cd usbguard/
    $ ./autogen.sh
    $ ./configure \
        --prefix=/usr \
        --sysconfdir=/etc \
        --with-bundled-json \
        --with-bundled-spdlog \
        --with-bundled-sodium
    $ make
    $ sudo checkinstall -t debian --nodoc --pkgname=usbguard --pkgversion=0.git

## 3. Fetch, build and install the USBGuard Qt applet

    $ sudo apt-get install cmake libqt4-dev
    $ git clone https://github.com/dkopecek/usbguard-applet-qt
    $ cd usbguard-applet-qt/
    $ cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr .
    $ make
    $ sudo checkinstall -t debian --nodoc --pkgname=usbguard-applet-qt --pkgversion=0.git

## 4. Customize the configuration files

By default, the *usbguard-daemon* process allows only processes which belong to
the *wheel* group to connect to the IPC interface. If don't want to add users
that will use the Qt applet to this group, change the *IPCAllowedUsers* or
*IPCAllowedGroups* values in */etc/usbguard/usbguard-daemon.conf*.

There's no policy installed by default. If you'd like to generate an initial one from
the currently connected USB devices to your system, see
[Rule Language/Initial policy](https://dkopecek.github.io/usbguard/documentation/rule-language.html#initial-policy)
