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
AM_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library \
	-I$(top_srcdir)/src/Library/public \
	@catch_CFLAGS@ 

EXTRA_DIST=\
	$(top_srcdir)/src/Tests/custom.supp \
	$(top_srcdir)/src/Tests/test-driver \
	$(top_srcdir)/src/Tests/bash-testlib.sh \
	$(top_srcdir)/src/ThirdParty/Catch \
	$(top_srcdir)/src/Tests/USB/test-descriptor-parser.sh \
	$(top_srcdir)/src/Tests/USB/data/0001.bin \
	$(top_srcdir)/src/Tests/USB/data/0001.out \
	$(top_srcdir)/src/Tests/Rules/test-rules.sh \
	$(top_srcdir)/src/Tests/Rules/test-rules.good \
	$(top_srcdir)/src/Tests/Rules/test-rules.bad \
	$(top_srcdir)/src/Tests/UseCase/000_executable.sh \
	$(top_srcdir)/src/Tests/UseCase/001_cli_policy.sh \
	$(top_srcdir)/src/Tests/UseCase/002_cli_devices.sh \
	$(top_srcdir)/src/Tests/UseCase/003_cli_devices_umockdev.sh \
	$(top_srcdir)/src/Tests/UseCase/004_daemonize.sh \
	$(top_srcdir)/src/Tests/UseCase/devices.umockdev \
	$(top_srcdir)/src/Tests/Source/check-driver.sh \
	$(top_srcdir)/src/Tests/Source/CheckScripts/copyright.sh \
	$(top_srcdir)/src/Tests/Source/CheckScripts/vim-modeline.sh \
	$(top_srcdir)/src/Tests/Source/CheckScripts/spell-check.sh \
	$(top_srcdir)/src/Tests/Source/CheckScripts/spell-check.rws \
	$(top_srcdir)/src/Tests/Source/CheckScripts/private-with-build-config.sh \
	$(top_srcdir)/src/Tests/Source/CheckScripts/public-without-build-config.sh \
	$(top_srcdir)/src/Tests/Source/CheckScripts/code-style.sh

LOG_DRIVER=\
	$(top_srcdir)/src/Tests/test-driver

TESTS_ENVIRONMENT=\
	builddir=$(top_builddir) \
	srcdir=$(top_srcdir) \
	abs_top_srcdir=$(abs_top_srcdir) \
	USBGUARD_TESTLIB_BASH=$(top_srcdir)/src/Tests/bash-testlib.sh \
	USBGUARD=$(top_builddir)/usbguard \
	USBGUARD_DAEMON=$(top_builddir)/usbguard-daemon \
	USBGUARD_DBUS=$(top_builddir)/usbguard-dbus

TESTS=\
	test-unit \
	test-regression \
	USB/test-descriptor-parser.sh \
	Rules/test-rules.sh

if FULL_TEST_SUITE_ENABLED
TESTS+=\
	Source/check-driver.sh \
	UseCase/000_executable.sh \
	UseCase/001_cli_policy.sh \
	UseCase/002_cli_devices.sh \
	UseCase/003_cli_devices_umockdev.sh \
	UseCase/004_daemonize.sh
endif

check_PROGRAMS=\
	test-unit \
	test-regression

test_unit_SOURCES=\
	main.cpp \
	Unit/test_Rule.cpp \
	Unit/test_RuleParser.cpp \
	Unit/test_Base64.cpp \
	Unit/test_UEvent.cpp \
	Unit/test_UEventParser.cpp \
	Unit/test_Utility.cpp \
	Unit/test_RuleAttribute_id.cpp \
	Unit/test_IPCServer_AccessControl.cpp \
	Unit/test_UMockdevDeviceDefinition.cpp \
	Unit/test_UMockdevDeviceDefinition.data.hpp

test_unit_LDADD=\
	$(top_builddir)/libusbguard.la

test_unit_LDFLAGS=\
	-static

test_regression_SOURCES=\
	main.cpp \
	Regression/test_Rule_ghi37.cpp \
	Regression/test_Rule_ghi113.cpp \
	Regression/github-PR209-config-parser.cpp

test_regression_LDADD=\
	$(top_builddir)/libusbguard.la

