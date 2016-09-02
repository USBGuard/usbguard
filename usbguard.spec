%global _hardened_build 1

Name:           usbguard
Version:        0.6.0
Release:        1%{?dist}
Summary:        A tool for implementing USB device usage policy
Group:          System Environment/Daemons
License:        GPLv2+
## Not installed
# src/ThirdParty/Catch: Boost Software License - Version 1.0
URL:            https://dkopecek.github.io/usbguard
Source0:        https://github.com/dkopecek/usbguard/releases/download/%{name}-%{version}/%{name}-%{version}.tar.gz

Requires: systemd
Requires(post): systemd
Requires(preun): systemd
Requires(postun): systemd
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

BuildRequires: libqb-devel
BuildRequires: libgcrypt-devel
BuildRequires: systemd systemd-devel
BuildRequires: libstdc++-devel
BuildRequires: protobuf-devel protobuf-compiler
BuildRequires: spdlog-static
BuildRequires: PEGTL-static
BuildRequires: qt5-qtbase-devel qt5-qtsvg-devel
BuildRequires: dbus-glib-devel
BuildRequires: dbus-devel
BuildRequires: glib2-devel
BuildRequires: polkit-devel
BuildRequires: libxslt
BuildRequires: libxml2
BuildRequires: catch-devel
BuildRequires: pandoc
BuildRequires: autoconf automake libtool

%description
The USBGuard software framework helps to protect your computer against rogue USB
devices by implementing basic whitelisting/blacklisting capabilities based on
USB device attributes.

%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       pkgconfig
Requires:       libstdc++-devel

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%package        tools
Summary:        USBGuard Tools
Group:          Applications/System
Requires:       %{name} = %{version}-%{release}

%description    tools
The %{name}-tools package contains optional tools from the USBGuard
software framework.

%package        applet-qt
Summary:        USBGuard Qt 5.x Applet
Group:          Applications/System
Requires:       %{name} = %{version}-%{release}
Obsoletes:      usbguard-applet-qt <= 0.3

%description    applet-qt
The %{name}-applet-qt package contains an optional Qt 5.x desktop applet
for interacting with the USBGuard daemon component.

%package        dbus
Summary:        USBGuard D-Bus Service
Group:          Applications/System
Requires:       %{name} = %{version}-%{release}
Requires:       dbus
Requires:       polkit

%description    dbus
The %{name}-dbus package contains an optional component that provides
a D-Bus interface to the USBGuard daemon component.

%prep
%setup -q
# Remove bundled library sources before build
rm -rf src/ThirdParty/{spdlog,Catch}

%build
mkdir -p ./m4
autoreconf -i -v --no-recursive ./
%configure \
    --disable-silent-rules \
    --disable-static \
    --without-bundled-spdlog \
    --without-bundled-catch \
    --enable-systemd \
    --with-gui-qt=qt5 \
    --with-dbus \
    --with-polkit \
    --with-crypto-library=gcrypt

make %{?_smp_mflags}

%check
make check

%install
make install INSTALL='install -p' DESTDIR=%{buildroot}

# Cleanup
find %{buildroot} -name '*.la' -exec rm -f {} ';'

%preun
%systemd_preun usbguard.service

%post
/sbin/ldconfig
%systemd_post usbguard.service

%postun
/sbin/ldconfig
%systemd_postun usbguard.service

%files
%defattr(-,root,root,-)
%doc README.md CHANGELOG.md
%license LICENSE
%{_libdir}/*.so.*
%{_sbindir}/usbguard-daemon
%{_bindir}/usbguard
%dir %{_sysconfdir}/usbguard
%config(noreplace) %{_sysconfdir}/usbguard/usbguard-daemon.conf
%{_unitdir}/usbguard.service
%{_datadir}/man/man8/usbguard-daemon.8.gz
%{_datadir}/man/man5/usbguard-daemon.conf.5.gz
%{_datadir}/man/man5/usbguard-rules.conf.5.gz
%{_datadir}/man/man1/usbguard.1.gz

%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%files tools
%defattr(-,root,root,-)
%{_bindir}/usbguard-rule-parser

%files applet-qt
%defattr(-,root,root,-)
%{_bindir}/usbguard-applet-qt
%{_mandir}/man1/usbguard-applet-qt.1.gz
%{_datadir}/applications/usbguard-applet-qt.desktop
%{_datadir}/icons/hicolor/scalable/apps/usbguard-icon.svg

%files dbus
%defattr(-,root,root,-)
%{_sbindir}/usbguard-dbus
%{_datadir}/dbus-1/system-services/org.usbguard.service
%{_datadir}/dbus-1/system.d/org.usbguard.conf
%{_datadir}/polkit-1/actions/org.usbguard.policy
%{_unitdir}/usbguard-dbus.service
%{_mandir}/man8/usbguard-dbus.8.gz

%preun dbus
%systemd_preun usbguard-dbus.service

%post dbus
%systemd_post usbguard-dbus.service

%postun dbus
%systemd_postun_with_restart usbguard-dbus.service


%changelog
* Sun Sep 04 2016 Daniel Kopeček <dnk1618@gmail.com> 0.6.0-1
 - Update to 0.6.0
