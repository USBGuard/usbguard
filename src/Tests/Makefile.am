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
	-I$(top_srcdir)/src\
	-I$(top_srcdir)/src/Library\
	@catch_CFLAGS@ \
	@json_CFLAGS@

EXTRA_DIST=\
	$(top_srcdir)/src/ThirdParty/Catch \
	$(top_srcdir)/src/Tests/Packaging/files-without-copyright.sh \
	$(top_srcdir)/src/Tests/USB/test-descriptor-parser.sh \
	$(top_srcdir)/src/Tests/USB/data/0001.bin \
	$(top_srcdir)/src/Tests/USB/data/0001.out \
	$(top_srcdir)/src/Tests/Packaging/spell-check.sh \
	$(top_srcdir)/src/Tests/Packaging/spell-check.rws

TESTS_ENVIRONMENT=\
	builddir=$(top_builddir) \
	srcdir=$(top_srcdir)

TESTS=\
	test-unit \
	test-regression \
	USB/test-descriptor-parser.sh \
	Packaging/spell-check.sh

check_PROGRAMS=\
	test-unit \
	test-regression

test_unit_SOURCES=\
	main.cpp \
	Unit/test_Rule.cpp \
	Unit/test_RuleParser.cpp

test_unit_LDADD=\
	$(top_builddir)/libusbguard.la

test_regression_SOURCES=\
	main.cpp \
	Regression/test_Rule_ghi37.cpp \
	Regression/test_json_locale.cpp

test_regression_LDADD=\
	$(top_builddir)/libusbguard.la
