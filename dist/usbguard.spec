%global _hardened_build 1

Name:           usbguard
Version:        0.4
Release:        1%{?dist}
Summary:        A tool for implementing USB device usage policy
Group:          System Environment/Daemons
License:        GPLv2+
## Not installed
# src/ThirdParty/Catch: Boost Software License - Version 1.0
URL:            https://dkopecek.github.io/usbguard
Source0:        https://dkopecek.github.io/usbguard/dist/%{name}-%{version}.tar.gz
Source1:        usbguard-daemon.conf
Source2:        usbguard.service

Requires: systemd
Requires(post): systemd
Requires(preun): systemd
Requires(postun): systemd
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

BuildRequires: libqb-devel
BuildRequires: libsodium-devel
BuildRequires: systemd systemd-devel
BuildRequires: libstdc++-devel
BuildRequires: json-static
BuildRequires: spdlog-static

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

%prep
%setup -q
# Remove bundled library sources before build
rm -rf src/ThirdParty/{json,spdlog}

%build
%configure \
    --disable-silent-rules \
    --disable-static \
    --without-bundled-json \
    --without-bundled-spdlog

make %{?_smp_mflags}

%check
make check

%install
make install INSTALL='install -p' DESTDIR=%{buildroot}

# Install configuration
mkdir -p %{buildroot}%{_sysconfdir}/usbguard
install -p -m 644 %{SOURCE1} %{buildroot}%{_sysconfdir}/usbguard/usbguard-daemon.conf

# Install systemd unit
mkdir -p %{buildroot}%{_unitdir}
install -p -m 644 %{SOURCE2} %{buildroot}%{_unitdir}/usbguard.service

# Cleanup
find %{buildroot} -name '*.la' -exec rm -f {} ';'

%preun
%systemd_preun usbguard.service

%post
/sbin/ldconfig
%systemd_post usbguard.service

%postun
/sbin/ldconfig
%systemd_postun_with_restart usbguard.service 

%files
%defattr(-,root,root,-)
%doc README.md
%license LICENSE
%{_libdir}/*.so.*
%{_sbindir}/usbguard-daemon
%dir %{_sysconfdir}/usbguard
%config(noreplace) %{_sysconfdir}/usbguard/usbguard-daemon.conf
%{_unitdir}/usbguard.service
%{_datadir}/man/man8/usbguard-daemon.8.gz
%{_datadir}/man/man5/usbguard-daemon.conf.5.gz
%{_datadir}/man/man5/usbguard-rules.conf.5.gz

%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%files tools
%defattr(-,root,root,-)
%{_bindir}/usbguard-generate-policy
%{_datadir}/man/man1/usbguard-generate-policy.1.gz

%changelog
* Tue May 19 2015 Daniel Kopecek <dkopecek@redhat.com> 0.4-1
- Update to version 0.4
- Added a tools subpackage which contains the usbguard-generate-policy
  tool

* Tue Apr 14 2015 Daniel Kopecek <dkopecek@redhat.com> 0.3p3-1
- Update to version 0.3p3
- added %check section
- removed explicit -devel requires on systemd, libqb and
  libsodium devel files
- added -devel requires on libstdc++-devel

* Sat Apr 11 2015 Daniel Kopecek <dkopecek@redhat.com> 0.3p2-1
- Update to version 0.3p2
- use system-wide json and spdlog packages

* Fri Apr 10 2015 Daniel Kopecek <dkopecek@redhat.com> 0.3p1-1
- Update to version 0.3p1
- removed bundled cppformat copylib

* Thu Apr 09 2015 Daniel Kopecek <dkopecek@redhat.com> 0.3-1
- Update to version 0.3
- disabled silent rules
- install license file
- added man pages
- use _hardened_build 1 instead of custom compilation flags
- fix file permissions on files in /etc
- do not install an empty rule set file

* Fri Apr 03 2015 Daniel Kopecek <dkopecek@redhat.com> 0.2-1
- Update to version 0.2
- Updated description
- Corrected package group

* Tue Mar 17 2015 Daniel Kopecek <dkopecek@redhat.com> 0.1-1
- Initial package
