##
## Copyright (C) 2016 Red Hat, Inc.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
## Authors: Daniel Kopecek <dkopecek@redhat.com>
##          Jiri Vymazal   <jvymazal@redhat.com>
##
SUBDIRS=src/Tests/

ACLOCAL_AMFLAGS= -I m4
EXTRA_DIST =\
	LICENSE \
	usbguard-daemon.conf.in \
	usbguard.service.in \
	VERSION \
	CHANGELOG.md \
	src/astylerc \
	scripts/usbguard-zsh-completion \
	scripts/modeline.vim \
	scripts/astyle.sh \
	scripts/reformat-sources.sh \
	scripts/usb-descriptor-collect.sh

BUILT_SOURCES=\
	src/build-config.h

$(top_builddir)/src/build-config.h: $(top_builddir)/src/build-config.h.in
	$(SED) \
	  -e "s|[$$]{localstatedir}|${localstatedir}|g" \
	  -e "s|[$$]{exec_prefix}|${prefix}|g" \
	  -e "s|[$$]{datarootdir}|${datarootdir}|g" \
	  -e "s|[$$]{datadir}|${pkgdatadir}|g" \
	  -e "s|[$$]{sysconfdir}|${sysconfdir}|g" \
	  -e "s|[$$]{prefix}|${prefix}|g" \
	  -e "s|[$$]{PACKAGE_TARNAME}|${PACKAGE_TARNAME}|g" \
	  $^ > $@ || rm -f $@

DISTCLEANFILES=\
	$(BUILT_SOURCES)

CLEANFILES=

man_ADOC_FILES=\
	doc/man/usbguard.1.adoc \
	doc/man/usbguard-dbus.8.adoc \
	doc/man/usbguard-applet-qt.1.adoc \
	doc/man/usbguard-daemon.8.adoc \
	doc/man/usbguard-daemon.conf.5.adoc \
	doc/man/usbguard-rules.conf.5.adoc

man_ROFF_FILES=\
	$(man_ADOC_FILES:.adoc=.roff)

EXTRA_DIST+=\
	$(man_ADOC_FILES)

BUILT_SOURCES+=\
	$(man_ROFF_FILES)

CLEANFILES+=\
	$(man_ROFF_FILES) \
	$(man_ROFF_FILES:.roff=)

$(top_builddir)/%.roff: %.adoc
	$(MKDIR_P) $$(dirname $@)
	$(ASCIIDOCTOR) -v -b manpage $^ -o $(top_builddir)/$@
	$(INSTALL) -m 644 $(top_builddir)/$@ $(top_builddir)/$(@:.roff=)

install-data-hook: install-daemon-conf install-systemd-service install-data-dbus
uninstall-hook: uninstall-daemon-conf uninstall-systemd-service uninstall-data-dbus

CLEANFILES+=\
	$(top_builddir)/usbguard-daemon.conf

usbguard-daemon.conf: $(top_builddir)/usbguard-daemon.conf.in
	$(SED) \
	  -e "s|%sysconfdir%|${sysconfdir}|g" \
	  -e "s|%localstatedir%|${localstatedir}|g" \
	  $^ > $@ || rm -f $@

usbguard_confdir= $(sysconfdir)/usbguard

distuninstallcheck_listfiles= find . -type f ! -name rules.conf -print

install-daemon-conf: $(top_builddir)/usbguard-daemon.conf
	$(MKDIR_P) $(DESTDIR)/$(usbguard_confdir)
	$(MKDIR_P) $(DESTDIR)/$(usbguard_confdir)/IPCAccessControl.d
	$(MKDIR_P) $(DESTDIR)/${localstatedir}/log/usbguard
	$(INSTALL) -m 600 $(top_builddir)/usbguard-daemon.conf \
	  $(DESTDIR)/$(usbguard_confdir)/usbguard-daemon.conf
	if ! test -f "$(DESTDIR)/$(usbguard_confdir)/rules.conf"; then \
	  touch $(DESTDIR)/$(usbguard_confdir)/rules.conf && \
	  chmod 600 $(DESTDIR)/$(usbguard_confdir)/rules.conf; \
	fi

uninstall-daemon-conf:
	rm -f $(DESTDIR)/$(usbguard_confdir)/usbguard-daemon.conf

if SYSTEMD_SUPPORT_ENABLED
CLEANFILES+=\
	$(top_builddir)/usbguard.service

usbguard.service: $(top_srcdir)/usbguard.service.in
	$(SED) -e "s|%sbindir%|${sbindir}|" -e "s|%sysconfdir%|${sysconfdir}|" $^ > $@ || rm -f $@

install-systemd-service: $(top_builddir)/usbguard.service
	$(MKDIR_P) $(DESTDIR)$(SYSTEMD_UNIT_DIR)
	$(INSTALL) -m 644 $(top_builddir)/usbguard.service \
	 $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard.service

uninstall-systemd-service:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard.service
else
install-systemd-service:
uninstall-systemd-service:
endif

man8_MANS=\
	$(top_builddir)/doc/man/usbguard-daemon.8

man5_MANS=\
	$(top_builddir)/doc/man/usbguard-daemon.conf.5 \
	$(top_builddir)/doc/man/usbguard-rules.conf.5

man1_MANS=\
	$(top_builddir)/doc/man/usbguard.1

AM_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library/public

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libusbguard.pc

lib_LTLIBRARIES=\
	libusbguard.la

libusbguard_la_CPPFLAGS=\
	-fPIC \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library \
	-I$(top_srcdir)/src/Library/public \
	-I$(top_builddir)/src/Library/IPC \
	@qb_CFLAGS@ \
	@protobuf_CFLAGS@ \
	@crypto_CFLAGS@ \
	@pegtl_CFLAGS@ \
	@umockdev_CFLAGS@

libusbguard_la_LDFLAGS=\
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libusbguard_la_LIBADD=\
	@qb_LIBS@ \
	@protobuf_LIBS@ \
	@crypto_LIBS@ \
	@pegtl_LIBS@ \
	@atomic_LIBS@ \
	@umockdev_LIBS@

EXTRA_DIST+=\
	src/Library/IPC/Devices.proto \
	src/Library/IPC/Exception.proto \
	src/Library/IPC/Message.proto \
	src/Library/IPC/Policy.proto \
	src/Library/IPC/Rule.proto \
	src/Library/IPC/Parameter.proto

nodist_libusbguard_la_SOURCES=\
	src/Library/IPC/Message.pb.cc \
	src/Library/IPC/Message.pb.h \
	src/Library/IPC/Rule.pb.cc \
	src/Library/IPC/Rule.pb.h \
	src/Library/IPC/Devices.pb.cc \
	src/Library/IPC/Devices.pb.h \
	src/Library/IPC/Exception.pb.cc \
	src/Library/IPC/Exception.pb.h \
	src/Library/IPC/Policy.pb.cc \
	src/Library/IPC/Policy.pb.h \
	src/Library/IPC/Parameter.pb.cc \
	src/Library/IPC/Parameter.pb.h

BUILT_SOURCES+=\
	$(nodist_libusbguard_la_SOURCES)

CLEANFILES+=\
	$(nodist_libusbguard_la_SOURCES)

libusbguard_la_SOURCES=\
	src/Common/ByteOrder.hpp \
	src/Common/FDInputStream.hpp \
	src/Common/Thread.hpp \
	src/Common/Utility.cpp \
	src/Common/Utility.hpp \
	src/Library/AllowedMatchesCondition.cpp \
	src/Library/AllowedMatchesCondition.hpp \
	src/Library/Base64.cpp \
	src/Library/Base64.hpp \
	src/Library/ConfigFilePrivate.cpp \
	src/Library/ConfigFilePrivate.hpp \
	src/Library/DeviceManagerPrivate.cpp \
	src/Library/DeviceManagerPrivate.hpp \
	src/Library/DevicePrivate.cpp \
	src/Library/DevicePrivate.hpp \
	src/Library/FixedStateCondition.cpp \
	src/Library/FixedStateCondition.hpp \
	src/Library/Hash.cpp \
	src/Library/Hash.hpp \
	src/Library/IPCClientPrivate.cpp \
	src/Library/IPCClientPrivate.hpp \
	src/Library/IPCPrivate.cpp \
	src/Library/IPCPrivate.hpp \
	src/Library/IPCServerPrivate.cpp \
	src/Library/IPCServerPrivate.hpp \
	src/Library/Init.cpp \
	src/Library/KeyValueParser.cpp \
	src/Library/KeyValueParser.hpp \
	src/Library/LocaltimeCondition.cpp \
	src/Library/LocaltimeCondition.hpp \
	src/Library/RandomStateCondition.cpp \
	src/Library/RandomStateCondition.hpp \
	src/Library/RuleAppliedCondition.cpp \
	src/Library/RuleAppliedCondition.hpp \
	src/Library/RuleEvaluatedCondition.cpp \
	src/Library/RuleEvaluatedCondition.hpp \
	src/Library/RuleParser/Actions.hpp \
	src/Library/RuleParser/Grammar.hpp \
	src/Library/RulePrivate.cpp \
	src/Library/RulePrivate.hpp \
	src/Library/RuleSetPrivate.cpp \
	src/Library/RuleSetPrivate.hpp \
	src/Library/SysFSDevice.cpp \
	src/Library/SysFSDevice.hpp \
	src/Library/UEvent.cpp \
	src/Library/UEvent.hpp \
	src/Library/UEventDeviceManager.cpp \
	src/Library/UEventDeviceManager.hpp \
	src/Library/UEventParser.cpp \
	src/Library/UEventParser.hpp \
	src/Library/UMockdevDeviceManager.cpp \
	src/Library/UMockdevDeviceManager.hpp \
	src/Library/UMockdevDeviceDefinition.cpp \
	src/Library/UMockdevDeviceDefinition.hpp \
	src/Library/Utility.cpp \
	src/Library/Utility.hpp \
	src/Library/public/usbguard/Audit.cpp \
	src/Library/public/usbguard/Audit.hpp \
	src/Library/public/usbguard/ConfigFile.cpp \
	src/Library/public/usbguard/Device.cpp \
	src/Library/public/usbguard/DeviceManager.cpp \
	src/Library/public/usbguard/DeviceManagerHooks.cpp \
	src/Library/public/usbguard/IPCClient.cpp \
	src/Library/public/usbguard/IPCServer.cpp \
	src/Library/public/usbguard/Logger.cpp \
	src/Library/public/usbguard/Policy.cpp \
	src/Library/public/usbguard/Rule.cpp \
	src/Library/public/usbguard/RuleCondition.cpp \
	src/Library/public/usbguard/RuleParser.cpp \
	src/Library/public/usbguard/RuleParser.hpp \
	src/Library/public/usbguard/RuleSet.cpp \
	src/Library/public/usbguard/Typedefs.cpp \
	src/Library/public/usbguard/USB.cpp \
	src/Library/public/usbguard/USBGuard.cpp

pkginclude_HEADERS=\
	src/Library/public/usbguard/Audit.hpp \
	src/Library/public/usbguard/ConfigFile.hpp \
	src/Library/public/usbguard/Device.hpp \
	src/Library/public/usbguard/DeviceManager.hpp \
	src/Library/public/usbguard/DeviceManagerHooks.hpp \
	src/Library/public/usbguard/Exception.hpp \
	src/Library/public/usbguard/IPCClient.hpp \
	src/Library/public/usbguard/IPCServer.hpp \
	src/Library/public/usbguard/Interface.hpp \
	src/Library/public/usbguard/Logger.hpp \
	src/Library/public/usbguard/Policy.hpp \
	src/Library/public/usbguard/Predicates.hpp \
	src/Library/public/usbguard/Rule.hpp \
	src/Library/public/usbguard/RuleCondition.hpp \
	src/Library/public/usbguard/RuleSet.hpp \
	src/Library/public/usbguard/Typedefs.hpp \
	src/Library/public/usbguard/USB.hpp \
	src/Library/public/usbguard/USBGuard.hpp

# Workaround for generated protobuf code mishaps
#```
# error: no previous declaration for 'void usbguard::IPC::protobuf_Message_2eproto::AddDescriptorImpl()' [-Werror=missing-declarations]
#  void AddDescriptorImpl() {
#       ^~~~~~~~~~~~~~~~~
#```
%.pb.lo : CXXFLAGS += -Wno-missing-declarations

%.pb.cc %.pb.h: %.proto
	$(MKDIR_P) $(top_builddir)/src/Library/IPC
	@PROTOC@ --cpp_out=$(top_builddir)/src/Library/IPC --proto_path=$(top_srcdir)/src/Library/IPC "$<"
	sed -i -e '1i#ifndef __clang_analyzer__' -e '$$a#endif' "$@"

sbin_PROGRAMS=\
	usbguard-daemon

usbguard_daemon_SOURCES=\
	src/Daemon/Daemon.cpp \
	src/Daemon/Daemon.hpp \
	src/Daemon/main.cpp \
	src/Daemon/Seccomp.c \
	src/Daemon/Seccomp.h \
	src/Common/Utility.hpp \
	src/Common/Utility.cpp \
	src/Daemon/FileAuditBackend.hpp \
	src/Daemon/FileAuditBackend.cpp \
	src/Daemon/LinuxAuditBackend.hpp \
	src/Daemon/LinuxAuditBackend.cpp

usbguard_daemon_CPPFLAGS=\
	-fPIE \
	$(AM_CPPFLAGS) \
	@seccomp_CFLAGS@ \
	@libcapng_CFLAGS@ \
	@audit_CFLAGS@

usbguard_daemon_LDADD=\
	$(top_builddir)/libusbguard.la \
	@seccomp_LIBS@ \
	@libcapng_LIBS@ \
	@audit_LIBS@

EXTRA_DIST +=\
	README.adoc \
	src/ThirdParty/Catch \
	src/ThirdParty/PEGTL

bin_PROGRAMS=\
	usbguard \
	usbguard-rule-parser

usbguard_SOURCES=\
	src/CLI/usbguard.cpp \
	src/CLI/usbguard.hpp \
	src/CLI/usbguard-list-devices.hpp \
	src/CLI/usbguard-list-devices.cpp \
	src/CLI/usbguard-allow-device.hpp \
	src/CLI/usbguard-allow-device.cpp \
	src/CLI/usbguard-block-device.hpp \
	src/CLI/usbguard-block-device.cpp \
	src/CLI/usbguard-reject-device.hpp \
	src/CLI/usbguard-reject-device.cpp \
	src/CLI/usbguard-list-rules.hpp \
	src/CLI/usbguard-list-rules.cpp \
	src/CLI/usbguard-append-rule.hpp \
	src/CLI/usbguard-append-rule.cpp \
	src/CLI/usbguard-remove-rule.hpp \
	src/CLI/usbguard-remove-rule.cpp \
	src/CLI/usbguard-generate-policy.cpp \
	src/CLI/usbguard-generate-policy.hpp \
	src/CLI/usbguard-watch.hpp \
	src/CLI/usbguard-watch.cpp \
	src/CLI/IPCSignalWatcher.hpp \
	src/CLI/IPCSignalWatcher.cpp \
	src/CLI/PolicyGenerator.hpp \
	src/CLI/PolicyGenerator.cpp \
	src/CLI/usbguard-read-descriptor.hpp \
	src/CLI/usbguard-read-descriptor.cpp \
	src/CLI/usbguard-set-parameter.hpp \
	src/CLI/usbguard-set-parameter.cpp \
	src/CLI/usbguard-get-parameter.hpp \
	src/CLI/usbguard-get-parameter.cpp \
	src/CLI/usbguard-add-user.cpp \
	src/CLI/usbguard-add-user.hpp \
	src/CLI/usbguard-remove-user.cpp \
	src/CLI/usbguard-remove-user.hpp

usbguard_CPPFLAGS=\
	-fPIE \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/CLI

usbguard_LDADD=\
	$(top_builddir)/libusbguard.la

if BASH_COMPLETION_ENABLED
bashcompletiondir = $(BASH_COMPLETION_DIR)
dist_bashcompletion_DATA = $(top_srcdir)/scripts/bash_completion/usbguard
endif

usbguard_rule_parser_SOURCES=\
	src/CLI/usbguard-rule-parser.cpp

usbguard_rule_parser_CPPFLAGS=\
	-fPIE \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/CLI

usbguard_rule_parser_LDADD=\
	$(top_builddir)/libusbguard.la

#
# DBus Bridge
#
if DBUS_ENABLED
sbin_PROGRAMS+= usbguard-dbus

man8_MANS+=\
	$(top_builddir)/doc/man/usbguard-dbus.8

BUILT_SOURCES+=\
	src/DBus/DBusInterface.xml.cstr \
	src/DBus/org.usbguard.service \
	src/DBus/usbguard-dbus.service

EXTRA_DIST+=\
	src/DBus/org.usbguard.conf \
	src/DBus/org.usbguard.service.in \
	src/DBus/usbguard-dbus.service.in \
	src/DBus/org.usbguard.policy \
	src/DBus/DBusInterface.xml

CLEANFILES+=\
	$(top_builddir)/src/DBus/org.usbguard.service \
	$(top_builddir)/src/DBus/usbguard-dbus.service \
	$(top_builddir)/src/DBus/DBusInterface.xml.cstr

usbguard_dbus_SOURCES=\
	src/DBus/gdbus-server.cpp \
	src/DBus/DBusBridge.cpp \
	src/DBus/DBusBridge.hpp

usbguard_dbus_CPPFLAGS=\
	-fPIE \
	$(AM_CPPFLAGS) \
	-I$(top_builddir)/src/DBus \
	@dbus_CFLAGS@

usbguard_dbus_LDADD=\
	$(top_builddir)/libusbguard.la \
	@dbus_LIBS@

%.xml:
	xmllint "$(top_srcdir)/$@" > /dev/null

%.xml.cstr: %.xml
	XMLLINT_INDENT="" xmllint --noblanks --format "$<" |\
	  sed -n -e '/<!--/,/-->/ d; s|\"|\\"|g; s|.*|"&"|; p' > "$(top_builddir)/$@"

%.service: %.service.in
	sed -e "s|%{sbindir}%|$(sbindir)|" "$<" > "$(top_builddir)/$@"

install-data-dbus: $(top_builddir)/src/DBus/org.usbguard.service install-polkit-policy install-systemd-dbus-service
	$(MKDIR_P) $(DESTDIR)$(DBUS_SERVICES_DIR) && \
	  $(INSTALL_DATA) $(top_builddir)/src/DBus/org.usbguard.service $(DESTDIR)$(DBUS_SERVICES_DIR)
	$(MKDIR_P) $(DESTDIR)$(DBUS_BUSCONFIG_DIR) && \
	  $(INSTALL_DATA) $(top_srcdir)/src/DBus/org.usbguard.conf $(DESTDIR)$(DBUS_BUSCONFIG_DIR)

uninstall-data-dbus: uninstall-polkit-policy uninstall-systemd-dbus-service
	rm -f $(DESTDIR)$(DBUS_SERVICES_DIR)/org.usbguard.service
	rmdir $(DESTDIR)$(DBUS_SERVICES_DIR)
	rm -f $(DESTDIR)$(DBUS_BUSCONFIG_DIR)/org.usbguard.conf
	rmdir $(DESTDIR)$(DBUS_BUSCONFIG_DIR)

dbus-docs: $(top_srcdir)/src/DBus/DBusInterface.xml
#
# Copy the input files to builddir because gdbus-codegen
# doesn't provide an option to store the output files somewhere
# else than in the directory where the input files are. And we
# don't want to create files in the srcdir when building.
#
	$(MKDIR_P) "$(top_builddir)/src/DBus"
	cp "$<" "$(top_builddir)/src/DBus/"
	$(GDBUS_CODEGEN) --interface-prefix org.usbguard --generate-docbook=docbook \
	  "$(top_builddir)/src/DBus/DBusInterface.xml"
	for docbook in $(top_builddir)/docbook-org.usbguard.*.xml; do \
	  $(XSLTPROC) -o "$$(basename -s .xml $$docbook).html" \
	  "$(top_srcdir)/src/ThirdParty/xsl-stylesheets/xhtml-1_1/docbook.xsl" \
	  "$(top_builddir)/$$docbook"; \
	done

if POLICYKIT_ENABLED
install-polkit-policy:
	$(MKDIR_P) $(DESTDIR)$(POLKIT_POLICY_DIR) && \
	  $(INSTALL_DATA) $(top_srcdir)/src/DBus/org.usbguard.policy $(DESTDIR)$(POLKIT_POLICY_DIR)

uninstall-polkit-policy:
	rm -f $(DESTDIR)$(POLKIT_POLICY_DIR)/org.usbguard.policy
	rmdir $(DESTDIR)$(POLKIT_POLICY_DIR)

else
install-polkit-policy:
uninstall-polkit-policy:
endif

if SYSTEMD_SUPPORT_ENABLED
install-systemd-dbus-service: $(top_builddir)/src/DBus/usbguard-dbus.service
	$(MKDIR_P) $(DESTDIR)$(SYSTEMD_UNIT_DIR) && \
	  $(INSTALL_DATA) $(top_builddir)/src/DBus/usbguard-dbus.service $(DESTDIR)$(SYSTEMD_UNIT_DIR)

uninstall-systemd-dbus-service:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard-dbus.service
	rmdir $(DESTDIR)$(SYSTEMD_UNIT_DIR)

else
install-systemd-dbus-service:
uninstall-systemd-dbus-service:
endif
else
install-data-dbus:
uninstall-data-dbus:
endif #DBUS_ENABLED

#
# Qt GUI
#
usbguard_applet_qt_TRANSLATIONS=\
	src/GUI.Qt/translations/cs_CZ.ts \
	src/GUI.Qt/translations/es_AR.ts

usbguard_applet_qt_UIHEADERS=\
	src/GUI.Qt/DeviceDialog.ui.h \
	src/GUI.Qt/MainWindow.ui.h

usbguard_applet_qt_RESOURCES=\
	src/GUI.Qt/resources/usbguard-icon.svg \
	src/GUI.Qt/resources/usbguard-icon-warning.svg \
	src/GUI.Qt/resources/usbguard-icon-inactive.svg

usbguard_applet_qt_SOURCES_BASE=\
	src/GUI.Qt/main.cpp \
	src/GUI.Qt/DeviceDialog.cpp \
	src/GUI.Qt/DeviceDialog.h \
	src/GUI.Qt/MainWindow.cpp \
	src/GUI.Qt/MainWindow.h \
	src/GUI.Qt/DeviceModel.cpp \
	src/GUI.Qt/DeviceModel.h \
	src/GUI.Qt/SessionBlocker.cpp \
	src/GUI.Qt/SessionBlocker.h \
	src/GUI.Qt/TargetDelegate.cpp \
	src/GUI.Qt/TargetDelegate.h

EXTRA_DIST+=\
	src/GUI.Qt/DeviceDialog.ui \
	src/GUI.Qt/MainWindow.ui \
	src/GUI.Qt/usbguard-applet-qt.desktop.in \
	$(usbguard_applet_qt_RESOURCES) \
	$(usbguard_applet_qt_SOURCES_BASE) \
	$(usbguard_applet_qt_TRANSLATIONS)

if GUI_QT_ENABLED
bin_PROGRAMS+= usbguard-applet-qt

desktopdir = $(datadir)/applications
desktop_DATA = $(top_builddir)/src/GUI.Qt/usbguard-applet-qt.desktop

appicondir = $(datadir)/icons/hicolor/scalable/apps
appicon_DATA = src/GUI.Qt/resources/usbguard-icon.svg

man1_MANS +=\
	$(top_builddir)/doc/man/usbguard-applet-qt.1

BUILT_SOURCES+=\
	$(usbguard_applet_qt_UIHEADERS) \
	$(usbguard_applet_qt_TRANSLATIONS:.ts=.qm) \
	src/GUI.Qt/usbguard.qrc

CLEANFILES +=\
	$(usbguard_applet_qt_UIHEADERS) \
	$(usbguard_applet_qt_TRANSLATIONS:.ts=.qm) \
	$(top_builddir)/src/GUI.Qt/DeviceDialog.moc.cpp \
	$(top_builddir)/src/GUI.Qt/MainWindow.moc.cpp \
	$(top_builddir)/src/GUI.Qt/usbguard.qrc.cpp \
	$(top_builddir)/src/GUI.Qt/usbguard.qrc \
	$(top_builddir)/src/GUI.Qt/DeviceModel.moc.cpp \
	$(top_builddir)/src/GUI.Qt/SessionBlocker.moc.cpp \
	$(top_builddir)/src/GUI.Qt/TargetDelegate.moc.cpp

nodist_usbguard_applet_qt_SOURCES=\
	$(usbguard_applet_qt_SOURCES_BASE) \
	src/GUI.Qt/DeviceDialog.moc.cpp \
	src/GUI.Qt/MainWindow.moc.cpp \
	src/GUI.Qt/usbguard.qrc.cpp \
	src/GUI.Qt/DeviceModel.moc.cpp \
	src/GUI.Qt/SessionBlocker.moc.cpp \
	src/GUI.Qt/TargetDelegate.moc.cpp

usbguard_applet_qt_LDADD=\
	$(top_builddir)/libusbguard.la \
	@qt_LIBS@

usbguard_applet_qt_CPPFLAGS=\
	-fPIE -fPIC \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/GUI.Qt \
	-I$(top_builddir)/src/GUI.Qt \
	@qt_CFLAGS@

%.qm: %.ts
	$(MKDIR_P) "$(top_builddir)/$(shell dirname $@)"
	@QTLRELEASE@ "$<" -qm "$@"

%.moc.cpp: %.h
	@QTMOC@ -f"$(shell basename $<)" -I$(top_srcdir)/src/GUI.Qt -o$(top_builddir)/$@ $(DEFS) $(DEFAULT_INCLUDES) $(MOC_CPPFLAGS) $<

%.ui.h: %.ui
	@QTUIC@ -g cpp -o $(top_builddir)/$@ $<

# Workaround for older Qt generated code addressing:
#```
#  src/GUI.Qt/usbguard.qrc.cpp: In function ‘int qInitResources()’:
#  src/GUI.Qt/usbguard.qrc.cpp:2171:25: error: no previous declaration for
#    ‘int qInitResources()’ [-Werror=missing-declarations]
#  int QT_MANGLE_NAMESPACE(qInitResources)()
#```
%.qrc.$(OBJEXT) : CXXFLAGS += -Wno-missing-declarations

%.qrc.cpp: %.qrc
	@QTRCC@ -o $(top_builddir)/$@ $<

%.qrc: $(usbguard_applet_qt_RESOURCES) $(usbguard_applet_qt_TRANSLATIONS:.ts=.qm)
	echo '<RCC>' > "$@.tmp"
	echo '<qresource prefix="/">' >> "$@.tmp"
	for file in $(usbguard_applet_qt_RESOURCES); do \
	  base=$$(basename $$file); \
	  echo "<file alias=\"$$base\">$(abs_top_srcdir)/$$file</file>" >> "$@.tmp"; \
	done
	echo '</qresource>' >> "$@.tmp"
	echo '<qresource prefix="/translations">' >> "$@.tmp"
	for file in $(usbguard_applet_qt_TRANSLATIONS); do \
	  base=$$(basename -s .ts $$file); \
	  echo "<file alias=\"$$base.qm\">$(abs_top_builddir)/src/GUI.Qt/translations/$$base.qm</file>" >> "$@.tmp"; \
	done
	echo '</qresource>' >> "$@.tmp"
	echo '</RCC>' >> "$@.tmp"
	mv "$@.tmp" "$@"

endif

$(DIST_ARCHIVES):
	test -f "$@" || $(MAKE) dist

release-files: $(DIST_ARCHIVES)
	$(eval RELEASE_DIR:="release.$(PACKAGE)-$(VERSION)")
	$(eval GPG:=$(shell test -n "$$QUBES_GPG_DOMAIN" && echo qubes-gpg-client || echo gpg2))
	$(MKDIR_P) $(RELEASE_DIR)
	cp $(DIST_ARCHIVES) $(RELEASE_DIR)
	cd $(RELEASE_DIR) && $(GPG) -b -a -o - $(DIST_ARCHIVES) > $(DIST_ARCHIVES).sig
	cd $(RELEASE_DIR) && openssl sha256 $(DIST_ARCHIVES) > $(DIST_ARCHIVES).sum

analyze-clang:
	$(eval ANALYSIS_ROOT:=$(shell mktemp -d -t usbguard-analyze-clang.XXXXXX))
	$(eval JOBS:=$(shell grep -c '^processor' /proc/cpuinfo 2> /dev/null || echo 1))
	cd "$(ANALYSIS_ROOT)" && scan-build $(abs_top_srcdir)/configure $(ANALYZE_CONFIGURE_ARGS)
	cd "$(ANALYSIS_ROOT)" && scan-build \
	  --status-bugs -maxloop 8 -disable-checker deadcode.DeadStores -o "$(abs_top_builddir)" \
	  make -j$(JOBS)
	rm -rf "$(ANALYSIS_ROOT)"

#
# overriding 'check' target to enable "make check" to perform build as well
#
check: all
	$(MAKE) $(AM_MAKEFLAGS) check-recursive
