##
## Copyright (C) 2018 Red Hat, Inc.
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
## Authors: Allen Webb <allenwebb@google.com>
##
AM_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library \
	-I$(top_srcdir)/src/Library/public \
	@pegtl_CFLAGS@

AM_LDFLAGS=\
	-static $(top_builddir)/libusbguard.la -lFuzzingEngine

EXTRA_PROGRAMS=\
	fuzzer-rules \
	fuzzer-uevent \
	fuzzer-usb-descriptor

fuzzer_rules_SOURCES=\
	fuzzer-rules.cpp

fuzzer_uevent_SOURCES=\
	fuzzer-uevent.cpp

fuzzer_usb_descriptor_SOURCES=\
	fuzzer-usb-descriptor.cpp
