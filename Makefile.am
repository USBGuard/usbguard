SUBDIRS=src/Tests/

ACLOCAL_AMFLAGS= -I m4
EXTRA_DIST =\
	LICENSE \
	dist/usbguard.service

BUILT_SOURCES=
DISTCLEANFILES=
CLEANFILES=

if SYSTEMD_SUPPORT_ENABLED
install-data-hook:
	mkdir -p $(DESTDIR)$(SYSTEMD_UNIT_DIR)
	$(INSTALL) -m 644 $(top_srcdir)/dist/usbguard.service \
	 $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard.service

uninstall-hook:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard.service
endif

dist_man8_MANS=\
	doc/usbguard-daemon.roff

dist_man5_MANS=\
	doc/usbguard-daemon.conf.roff \
	doc/usbguard-rules.conf.roff

dist_man1_MANS=\
	doc/usbguard.roff

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
	-I$(top_srcdir)/src/Library/RuleParser \
	@qb_CFLAGS@ \
	@json_CFLAGS@ \
	@spdlog_CFLAGS@ \
	@udev_CFLAGS@ \
	@sodium_CFLAGS@

libusbguard_la_LDFLAGS=\
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libusbguard_la_LIBADD=\
	@qb_LIBS@ \
	@spdlog_LIBS@ \
	@json_LIBS@ \
	@udev_LIBS@ \
	@sodium_LIBS@

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
	src/Library/RulePrivate.cpp \
	src/Library/RulePrivate.hpp \
	src/Library/RuleSet.cpp \
	src/Library/RuleSetPrivate.cpp \
	src/Library/RuleSetPrivate.hpp \
	src/Library/RuleParser/Lexer-configuration.hpp \
	src/Library/RuleParser/Lexer.cpp \
	src/Library/RuleParser/Lexer.hpp \
	src/Library/RuleParser/Lexer-token.hpp \
	src/Library/RuleParser/Lexer-token_ids.hpp \
	src/Library/RuleParser/Parser.h \
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
	src/ThirdParty/usbmon/src/usbmon.cpp \
	src/ThirdParty/usbmon/src/usbmon.hpp \
	src/ThirdParty/usbmon/src/usbpacket.cpp \
	src/ThirdParty/usbmon/src/usbpacket.hpp

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
	src/Library/Logger.hpp

EXTRA_DIST +=\
	src/Library/RuleParser/Lexer.qx \
	src/Library/RuleParser/Parser.y \
	src/Library/RuleParser/Parser.c \
	src/Library/RuleParser/quex \
	src/Library/RuleParser/README.md

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
	src/ThirdParty/Catch

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
	src/CLI/PolicyGenerator.cpp

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

install-data-local: $(top_builddir)/src/DBus/org.usbguard.service install-polkit-policy install-systemd-service
	mkdir -p $(DESTDIR)$(DBUS_SERVICES_DIR) && \
	  $(INSTALL_DATA) $(top_builddir)/src/DBus/org.usbguard.service $(DESTDIR)$(DBUS_SERVICES_DIR)
	mkdir -p $(DESTDIR)$(DBUS_BUSCONFIG_DIR) && \
	  $(INSTALL_DATA) $(top_srcdir)/src/DBus/org.usbguard.conf $(DESTDIR)$(DBUS_BUSCONFIG_DIR)

uninstall-local: uninstall-polkit-policy uninstall-systemd-service
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
install-systemd-service: $(top_builddir)/src/DBus/usbguard-dbus.service
	mkdir -p $(DESTDIR)$(SYSTEMD_UNIT_DIR) && \
	  $(INSTALL_DATA) $(top_builddir)/src/DBus/usbguard-dbus.service $(DESTDIR)$(SYSTEMD_UNIT_DIR)

uninstall-systemd-service:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard-dbus.service
	rmdir $(DESTDIR)$(SYSTEMD_UNIT_DIR)

else
install-systemd-service:
uninstall-systemd-service:
endif
endif #DBUS_ENABLED

#
# Qt 5.x GUI
#
if GUI_QT5_ENABLED
bin_PROGRAMS+= usbguard-applet-qt

desktopdir = $(datadir)/applications
desktop_DATA = $(top_builddir)/src/GUI.Qt5/usbguard-applet-qt.desktop

appicondir = $(datadir)/icons/hicolor/scalable/apps
appicon_DATA = src/GUI.Qt5/resources/usbguard-icon.svg

dist_man1_MANS +=\
	doc/usbguard-applet-qt.roff

usbguard_applet_qt_UIHEADERS=\
	src/GUI.Qt5/DeviceDialog.ui.h \
	src/GUI.Qt5/MainWindow.ui.h

usbguard_applet_qt_RESOURCES=\
	src/GUI.Qt5/resources/usbguard-icon.svg \
	src/GUI.Qt5/resources/usbguard-icon-warning.svg

BUILT_SOURCES+=\
	$(usbguard_applet_qt_UIHEADERS) \
	src/GUI.Qt5/usbguard.qrc

EXTRA_DIST+=\
	src/GUI.Qt5/DeviceDialog.ui \
	src/GUI.Qt5/MainWindow.ui \
	src/GUI.Qt5/usbguard-applet-qt.desktop.in \
	$(usbguard_applet_qt_RESOURCES)

CLEANFILES +=\
	$(usbguard_applet_qt_UIHEADERS) \
	$(top_builddir)/src/GUI.Qt5/DeviceDialog.moc.cpp \
	$(top_builddir)/src/GUI.Qt5/MainWindow.moc.cpp \
	$(top_builddir)/src/GUI.Qt5/usbguard.qrc.cpp \
	$(top_builddir)/src/GUI.Qt5/usbguard.qrc \
	$(top_builddir)/src/GUI.Qt5/DeviceTableWidget.moc.cpp

usbguard_applet_qt_SOURCES=\
	src/GUI.Qt5/main.cpp \
	src/GUI.Qt5/DeviceDialog.cpp \
	src/GUI.Qt5/DeviceDialog.h \
	src/GUI.Qt5/DeviceDialog.moc.cpp \
	src/GUI.Qt5/MainWindow.cpp \
	src/GUI.Qt5/MainWindow.h \
	src/GUI.Qt5/MainWindow.moc.cpp \
	src/GUI.Qt5/usbguard.qrc.cpp \
	src/GUI.Qt5/DeviceTableWidget.h \
	src/GUI.Qt5/DeviceTableWidget.cpp \
	src/GUI.Qt5/DeviceTableWidget.moc.cpp

usbguard_applet_qt_LDADD=\
	$(top_builddir)/libusbguard.la \
	@qt5_LIBS@

usbguard_applet_qt_CPPFLAGS=\
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/GUI.Qt5 \
	-I$(top_builddir)/src/GUI.Qt5 \
	@spdlog_CFLAGS@ \
	-fPIC @qt5_CFLAGS@

%.moc.cpp: %.h
	@MOCQT5@ -f"$(shell basename $<)" -I$(top_srcdir)/src/GUI.Qt5 -o$(top_builddir)/$@ $(DEFS) $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(MOC_CPPFLAGS) $<

%.ui.h: %.ui
	@UICQT5@ -g cpp -o $(top_builddir)/$@ $<

%.qrc.cpp: %.qrc
	@RCCQT5@ -o $(top_builddir)/$@ $<

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

rpmbuild: dist
	test -n "$(RPMBUILD_FLAGS)"
	$(eval RPMBUILD_ROOT:=$(shell mktemp -d -t usbguard-srpm.XXXXXX))
	$(eval RPMBUILD_SOURCES:=$(RPMBUILD_ROOT)/SOURCE)
	$(eval RPMBUILD_SPECS:=$(RPMBUILD_ROOT)/SPECS)
	$(eval RPMBUILD_BUILDS:=$(RPMBUILD_ROOT)/BUILDS)
	mkdir -p $(RPMBUILD_SOURCES)
	mkdir -p $(RPMBUILD_SPECS)
	cp $(top_srcdir)/dist/usbguard-daemon.conf $(RPMBUILD_SOURCES)
	cp $(top_srcdir)/dist/usbguard.spec $(RPMBUILD_SPECS)
	cp $(top_builddir)/usbguard-*.tar.gz $(RPMBUILD_SOURCES)
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
	git clone . "$(GIT_CLONE_ROOT)" && \
	  src/Tests/Packaging/files-without-copyright.sh "$(GIT_CLONE_ROOT)"
	rm -rf "$(GIT_CLONE_ROOT)"
else
check-local:
endif
