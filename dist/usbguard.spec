Name:           usbguard
Version:        0.2
Release:        1%{?dist}
Summary:        A tool for implementing USB device usage policy
Group:          System Environment/Daemons
License:        GPLv2+
URL:            https://dkopecek.github.io/usbguard
Source0:        https://dkopecek.github.io/usbguard/dist/%{name}-%{version}.tar.gz
Source1:        usbguard-daemon.conf
Source2:        usbguard.service
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Requires: systemd
Requires(post): systemd
Requires(preun): systemd
Requires(postun): systemd
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

BuildRequires: libqb-devel
BuildRequires: libsodium-devel
BuildRequires: systemd systemd-devel

%description
The USBGuard software framework helps to protect your computer against rogue USB
devices by implementing basic whitelisting/blacklisting capabilities based on
USB device attributes.

%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       pkgconfig
Requires:       libqb-devel
Requires:       libsodium-devel
Requires:       systemd-devel

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q

%build
%ifarch sparc64
#sparc64 need big PIE
export CXXFLAGS="$RPM_OPT_FLAGS -fPIE"
export CFLAGS=$CXXFLAGS
export LDFLAGS="-pie -Wl,-z,relro -Wl,-z,now"
%else
export CXXFLAGS="$RPM_OPT_FLAGS -fpie"
export CFLAGS=$CXXFLAGS
export LDFLAGS="-pie -Wl,-z,relro -Wl,-z,now"
%endif

%configure

make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT

make install INSTALL='install -p' DESTDIR=$RPM_BUILD_ROOT

# Install configuration
mkdir -p $RPM_BUILD_ROOT%{_sysconfdir}/usbguard
install -p %{SOURCE1} $RPM_BUILD_ROOT%{_sysconfdir}/usbguard/usbguard-daemon.conf
# Create en empty rules.conf file
touch $RPM_BUILD_ROOT%{_sysconfdir}/usbguard/rules.conf

# Install systemd unit
mkdir -p $RPM_BUILD_ROOT%{_unitdir}
install -p %{SOURCE2} $RPM_BUILD_ROOT%{_unitdir}/usbguard.service

# Cleanup
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%clean
rm -rf $RPM_BUILD_ROOT

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
%{_libdir}/*.so.*
%{_sbindir}/usbguard-daemon
%dir %{_sysconfdir}/usbguard
%config(noreplace) %{_sysconfdir}/usbguard/usbguard-daemon.conf
%config(noreplace) %{_sysconfdir}/usbguard/rules.conf
%{_unitdir}/usbguard.service

%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/*.a
%{_libdir}/pkgconfig/*.pc

%changelog
* Fri Apr 03 2015 Daniel Kopecek <dkopecek@redhat.com> 0.2-1
- Update to version 0.2
- Updated description
- Corrected package group

* Tue Mar 17 2015 Daniel Kopecek <dkopecek@redhat.com> 0.1-1
- Initial package
