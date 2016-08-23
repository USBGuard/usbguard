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
##
SUBDIRS=src/Tests/

ACLOCAL_AMFLAGS= -I m4
EXTRA_DIST =\
	LICENSE \
	usbguard-daemon.conf.in \
	usbguard.service.in \
	VERSION \
	CHANGELOG.md

BUILT_SOURCES=
DISTCLEANFILES=
CLEANFILES=

CLEANFILES+=\
	$(top_builddir)/usbguard-daemon.conf

usbguard-daemon.conf: $(top_builddir)/usbguard-daemon.conf.in
	$(SED) "s|%sysconfdir%|${sysconfdir}|g" $^ > $@ || rm -f $@

usbguard_confdir= $(sysconfdir)/usbguard
dist_usbguard_conf_DATA= $(top_builddir)/usbguard-daemon.conf

ROFF_PAGES=\
	doc/usbguard.1.roff \
	doc/usbguard-dbus.8.roff \
	doc/usbguard-applet-qt.1.roff \
	doc/usbguard-daemon.8.roff \
	doc/usbguard-daemon.conf.5.roff \
	doc/usbguard-rules.conf.5.roff

MARKDOWN_PAGES=$(ROFF_PAGES:.roff=.md)

EXTRA_DIST+=\
	$(ROFF_PAGES:.roff=) \
	$(MARKDOWN_PAGES)

BUILT_SOURCES+=\
	$(ROFF_PAGES)

CLEANFILES+=\
	$(ROFF_PAGES) \
        $(ROFF_PAGES:.roff=)

if GENERATE_MANPAGES
$(top_builddir)/%.roff: %.md
	$(MKDIR_P) $$(dirname $@)
	$(PANDOC) -s -t man $^ -o $(top_builddir)/$@
	$(INSTALL) -m 644 $(top_builddir)/$@ $(top_builddir)/$(@:.roff=)
else
$(top_builddir)/%.roff: %.md
	$(INSTALL) -D -m 644 $(^:.md=) $(top_builddir)/$@
	$(INSTALL) -D -m 644 $(top_builddir)/$@ $(top_builddir)/$(@:.roff=)
endif

install-data-hook: install-systemd-service install-data-dbus
uninstall-hook: uninstall-systemd-service uninstall-data-dbus

if SYSTEMD_SUPPORT_ENABLED
CLEANFILES+=\
	$(top_builddir)/usbguard.service

usbguard.service: $(top_srcdir)/usbguard.service.in
	$(SED) -e "s|%sbindir%|${sbindir}|" -e "s|%sysconfdir%|${sysconfdir}|" $^ > $@ || rm -f $@

install-systemd-service: $(top_builddir)/usbguard.service
	mkdir -p $(DESTDIR)$(SYSTEMD_UNIT_DIR)
	$(INSTALL) -m 644 $(top_builddir)/usbguard.service \
	 $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard.service

uninstall-systemd-service:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard.service
else
install-systemd-service:
uninstall-systemd-service:
endif

man8_MANS=\
	$(top_builddir)/doc/usbguard-daemon.8 \
	$(top_builddir)/doc/usbguard-dbus.8

man5_MANS=\
	$(top_builddir)/doc/usbguard-daemon.conf.5 \
	$(top_builddir)/doc/usbguard-rules.conf.5

man1_MANS=\
	$(top_builddir)/doc/usbguard.1

AM_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libusbguard.pc

lib_LTLIBRARIES=\
	libusbguard.la

libusbguard_la_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library \
	@qb_CFLAGS@ \
	@json_CFLAGS@ \
	@spdlog_CFLAGS@ \
	@udev_CFLAGS@ \
	@crypto_CFLAGS@ \
	@pegtl_CFLAGS@

libusbguard_la_LDFLAGS=\
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libusbguard_la_LIBADD=\
	@qb_LIBS@ \
	@spdlog_LIBS@ \
	@json_LIBS@ \
	@udev_LIBS@ \
	@crypto_LIBS@ \
	@pegtl_LIBS@

libusbguard_la_SOURCES=\
	src/Common/Thread.hpp \
	src/Common/JSON.hpp \
	src/Common/ByteOrder.hpp \
	src/Common/Utility.hpp \
	src/Common/Utility.cpp \
	src/Library/ConfigFile.cpp \
	src/Library/ConfigFilePrivate.cpp \
	src/Library/ConfigFilePrivate.hpp \
	src/Library/IPCClient.cpp \
	src/Library/IPCClientPrivate.hpp \
	src/Library/IPCClientPrivate.cpp \
	src/Library/IPCPrivate.hpp \
	src/Library/IPCPrivate.cpp \
	src/Library/USB.cpp \
	src/Library/Rule.cpp \
	src/Library/RuleParser.cpp \
	src/Library/RuleParser.hpp \
	src/Library/RuleParser/Grammar.hpp \
	src/Library/RuleParser/Actions.hpp \
	src/Library/RulePrivate.cpp \
	src/Library/RulePrivate.hpp \
	src/Library/RuleSet.cpp \
	src/Library/RuleSetPrivate.cpp \
	src/Library/RuleSetPrivate.hpp \
	src/Library/Typedefs.cpp \
	src/Library/DeviceManagerHooks.cpp \
	src/Library/Device.cpp \
	src/Library/DevicePrivate.hpp \
	src/Library/DevicePrivate.cpp \
	src/Library/DeviceManager.cpp \
	src/Library/DeviceManagerPrivate.hpp \
	src/Library/DeviceManagerPrivate.cpp \
	src/Library/LinuxDeviceManager.cpp \
	src/Library/LinuxDeviceManager.hpp \
	src/Library/LinuxSysIO.hpp \
	src/Library/LinuxSysIO.cpp \
	src/Library/LoggerPrivate.hpp \
	src/Library/LoggerPrivate.cpp \
	src/Library/Init.cpp \
	src/Library/RuleCondition.hpp \
	src/Library/RuleCondition.cpp \
	src/Library/AllowedMatchesCondition.hpp \
	src/Library/AllowedMatchesCondition.cpp \
	src/Library/FixedStateCondition.hpp \
	src/Library/FixedStateCondition.cpp \
	src/Library/RandomStateCondition.hpp \
	src/Library/RandomStateCondition.cpp \
	src/Library/LocaltimeCondition.hpp \
	src/Library/LocaltimeCondition.cpp \
	src/Library/RuleAppliedCondition.cpp \
	src/Library/RuleAppliedCondition.hpp \
	src/Library/RuleEvaluatedCondition.cpp \
	src/Library/RuleEvaluatedCondition.hpp \
	src/Library/Utility.cpp \
	src/Library/Base64.cpp \
	src/Library/Base64.hpp \
	src/Library/Hash.cpp \
	src/Library/Hash.hpp

pkginclude_HEADERS=\
	src/Library/ConfigFile.hpp \
	src/Library/Interface.hpp \
	src/Library/IPC.hpp \
	src/Library/IPCClient.hpp \
	src/Library/USB.hpp \
	src/Library/Rule.hpp \
	src/Library/RuleSet.hpp \
	src/Library/Typedefs.hpp \
	src/Library/DeviceManagerHooks.hpp \
	src/Library/Device.hpp \
	src/Library/DeviceManager.hpp \
	src/Library/Logger.hpp \
	src/Library/Predicates.hpp \
	src/Library/Utility.hpp

sbin_PROGRAMS=\
	usbguard-daemon

usbguard_daemon_SOURCES=\
	src/Daemon/Daemon.cpp \
	src/Daemon/Daemon.hpp \
	src/Daemon/Exceptions.hpp \
	src/Daemon/main.cpp \
	src/Common/CCBQueue.hpp \
	src/Common/Utility.hpp \
	src/Common/Utility.cpp

usbguard_daemon_CPPFLAGS=\
	$(AM_CPPFLAGS) \
	@spdlog_CFLAGS@ \
	@json_CFLAGS@ \
	@seccomp_CFLAGS@ \
	@libcapng_CFLAGS@ \
	@qb_CFLAGS@

usbguard_daemon_LDADD=\
	$(top_builddir)/libusbguard.la \
	@spdlog_LIBS@ \
	@json_LIBS@ \
	@seccomp_LIBS@ \
	@libcapng_LIBS@ \
	@qb_LIBS@

EXTRA_DIST +=\
	README.md \
	src/ThirdParty/spdlog \
	src/ThirdParty/json/src/json.hpp \
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
	src/CLI/usbguard-read-descriptor.cpp

usbguard_CPPFLAGS=\
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/CLI \
	@spdlog_CFLAGS@

usbguard_LDADD=\
	$(top_builddir)/libusbguard.la

usbguard_rule_parser_SOURCES=\
	src/CLI/usbguard-rule-parser.cpp

usbguard_rule_parser_CPPFLAGS=\
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/CLI \
	@spdlog_CFLAGS@

usbguard_rule_parser_LDADD=\
	$(top_builddir)/libusbguard.la

#
# DBus Bridge
#
if DBUS_ENABLED
sbin_PROGRAMS+= usbguard-dbus

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
	$(AM_CPPFLAGS) \
	-I$(top_builddir)/src/DBus \
	@spdlog_CFLAGS@ \
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
	mkdir -p $(DESTDIR)$(DBUS_SERVICES_DIR) && \
	  $(INSTALL_DATA) $(top_builddir)/src/DBus/org.usbguard.service $(DESTDIR)$(DBUS_SERVICES_DIR)
	mkdir -p $(DESTDIR)$(DBUS_BUSCONFIG_DIR) && \
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
	mkdir -p "$(top_builddir)/src/DBus"
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
	mkdir -p $(DESTDIR)$(POLKIT_POLICY_DIR) && \
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
	mkdir -p $(DESTDIR)$(SYSTEMD_UNIT_DIR) && \
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
	src/GUI.Qt/TargetDelegate.cpp \
	src/GUI.Qt/TargetDelegate.h

EXTRA_DIST+=\
	src/GUI.Qt/DeviceDialog.ui \
	src/GUI.Qt/MainWindow.ui \
	src/GUI.Qt/usbguard-applet-qt.desktop.in \
	$(usbguard_applet_qt_RESOURCES) \
	$(usbguard_applet_qt_SOURCES_BASE)

if GUI_QT_ENABLED
bin_PROGRAMS+= usbguard-applet-qt

desktopdir = $(datadir)/applications
desktop_DATA = $(top_builddir)/src/GUI.Qt/usbguard-applet-qt.desktop

appicondir = $(datadir)/icons/hicolor/scalable/apps
appicon_DATA = src/GUI.Qt/resources/usbguard-icon.svg

man1_MANS +=\
	$(top_builddir)/doc/usbguard-applet-qt.1

BUILT_SOURCES+=\
	$(usbguard_applet_qt_UIHEADERS) \
	src/GUI.Qt/usbguard.qrc

CLEANFILES +=\
	$(usbguard_applet_qt_UIHEADERS) \
	$(top_builddir)/src/GUI.Qt/DeviceDialog.moc.cpp \
	$(top_builddir)/src/GUI.Qt/MainWindow.moc.cpp \
	$(top_builddir)/src/GUI.Qt/usbguard.qrc.cpp \
	$(top_builddir)/src/GUI.Qt/usbguard.qrc \
	$(top_builddir)/src/GUI.Qt/DeviceModel.moc.cpp \
	$(top_builddir)/src/GUI.Qt/TargetDelegate.moc.cpp

nodist_usbguard_applet_qt_SOURCES=\
	$(usbguard_applet_qt_SOURCES_BASE) \
	src/GUI.Qt/DeviceDialog.moc.cpp \
	src/GUI.Qt/MainWindow.moc.cpp \
	src/GUI.Qt/usbguard.qrc.cpp \
	src/GUI.Qt/DeviceModel.moc.cpp \
	src/GUI.Qt/TargetDelegate.moc.cpp

usbguard_applet_qt_LDADD=\
	$(top_builddir)/libusbguard.la \
	@qt_LIBS@

usbguard_applet_qt_CPPFLAGS=\
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/GUI.Qt \
	-I$(top_builddir)/src/GUI.Qt \
	@spdlog_CFLAGS@ \
	-fPIC @qt_CFLAGS@

%.moc.cpp: %.h
	@QTMOC@ -f"$(shell basename $<)" -I$(top_srcdir)/src/GUI.Qt -o$(top_builddir)/$@ $(DEFS) $(DEFAULT_INCLUDES) $(MOC_CPPFLAGS) $<

%.ui.h: %.ui
	@QTUIC@ -g cpp -o $(top_builddir)/$@ $<

%.qrc.cpp: %.qrc
	@QTRCC@ -o $(top_builddir)/$@ $<

%.qrc: $(usbguard_applet_qt_RESOURCES)
	echo '<RCC>' > "$@.tmp"
	echo '<qresource prefix="/">' >> "$@.tmp"
	for file in $(usbguard_applet_qt_RESOURCES); do \
	  base=$$(basename $$file); \
	  echo "<file alias=\"$$base\">$(abs_top_srcdir)/$$file</file>" >> "$@.tmp"; \
	done
	echo '</qresource>' >> "$@.tmp"
	echo '</RCC>' >> "$@.tmp"
	mv "$@.tmp" "$@"

endif

$(DIST_ARCHIVES):
	test -f "$@" || $(MAKE) dist

rpmbuild: $(DIST_ARCHIVES)
	test -n "$(RPMBUILD_FLAGS)"
	$(eval RPMBUILD_ROOT:=$(shell mktemp -d -t usbguard-srpm.XXXXXX))
	$(eval RPMBUILD_SOURCES:=$(RPMBUILD_ROOT)/SOURCE)
	$(eval RPMBUILD_SPECS:=$(RPMBUILD_ROOT)/SPECS)
	$(eval RPMBUILD_BUILDS:=$(RPMBUILD_ROOT)/BUILDS)
	mkdir -p $(RPMBUILD_SOURCES)
	mkdir -p $(RPMBUILD_SPECS)
	cp $(top_srcdir)/usbguard.spec $(RPMBUILD_SPECS)
	cp $(top_builddir)/$(DIST_ARCHIVES) $(RPMBUILD_SOURCES)
	rpmbuild $(RPMBUILD_FLAGS) \
		--define "_sourcedir $(RPMBUILD_SOURCES)" \
		--define "_srcrpmdir $(top_builddir)" \
		--define "_rpmdir $(top_builddir)" \
		--define "_builddir $(RPMBUILD_BUILDS)" \
		"$(RPMBUILD_SPECS)/usbguard.spec"
	rm -rf "$(RPMBUILD_ROOT)"

rpmbuild-bs:
	$(MAKE) rpmbuild RPMBUILD_FLAGS=-bs

rpmbuild-ba:
	$(MAKE) rpmbuild RPMBUILD_FLAGS=-ba

release-files: rpmbuild-bs
	$(eval RELEASE_DIR:="release.$(PACKAGE)-$(VERSION)")
	$(eval GPG:=$(shell test -n "$$QUBES_GPG_DOMAIN" && echo qubes-gpg-client || echo gpg2))
	mkdir -p $(RELEASE_DIR)
	cp $(DIST_ARCHIVES) $(RELEASE_DIR)
	cp $(PACKAGE)-$(VERSION)-*.src.rpm $(RELEASE_DIR)
	cd $(RELEASE_DIR) && $(GPG) -b -a $(DIST_ARCHIVES) > $(DIST_ARCHIVES).sig
	cd $(RELEASE_DIR) && openssl sha -sha256 $(DIST_ARCHIVES) > $(DIST_ARCHIVES).sum

analyze-clang:
	$(eval ANALYSIS_ROOT:=$(shell mktemp -d -t usbguard-analyze-clang.XXXXXX))
	$(eval JOBS:=$(shell grep -c '^processor' /proc/cpuinfo 2> /dev/null || echo 1))
	cd "$(ANALYSIS_ROOT)" && scan-build $(abs_top_srcdir)/configure $(ANALYZE_CONFIGURE_ARGS)
	cd "$(ANALYSIS_ROOT)" && scan-build \
	  --status-bugs -maxloop 8 -disable-checker deadcode.DeadStores -o "$(abs_top_builddir)" \
	  make -j$(JOBS)
	rm -rf "$(ANALYSIS_ROOT)"

if MAINTAINER_MODE
check-local: check-copyright

check-copyright:
	$(eval GIT_CLONE_ROOT:=$(shell mktemp -d -t usbguard-git-clone.XXXXXX))
	git clone "$(abs_top_srcdir)" "$(GIT_CLONE_ROOT)" && \
	  $(top_srcdir)/src/Tests/Packaging/files-without-copyright.sh "$(GIT_CLONE_ROOT)"
	rm -rf "$(GIT_CLONE_ROOT)"
else
check-local:
endif
