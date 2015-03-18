ACLOCAL_AMFLAGS= -I m4

AM_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Common \
	@udev_CFLAGS@ \
	@qb_CFLAGS@ \
	@sodium_CFLAGS@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libusbguard.pc

lib_LTLIBRARIES=\
	libusbguard.la

sbin_PROGRAMS=\
	usbguard-daemon

libusbguard_la_SOURCES=\
	src/Common/Utility.cpp \
	src/Common/Logging.cpp \
	src/Common/SysIO.cpp \
	src/Common/IPCClient.cpp \
	src/Common/Format.hpp

pkginclude_HEADERS=\
	src/Common/Types.hpp \
	src/Common/Utility.hpp	\
	src/Common/SysIO.hpp \
	src/Common/CCBQueue.hpp \
	src/Common/Thread.hpp \
	src/Common/IPC.hpp \
	src/Common/IPCClient.hpp \
	src/Common/Logging.hpp \
	src/Common/JSON.hpp

usbguard_daemon_SOURCES=\
	src/Daemon/ConfigFile.cpp \
	src/Daemon/ConfigFile.hpp \
	src/Daemon/USBFirewall.cpp \
	src/Daemon/USBFirewall.hpp \
	src/Daemon/Daemon.cpp \
	src/Daemon/Daemon.hpp \
	src/Daemon/Exceptions.hpp \
	src/Daemon/UDev.cpp \
	src/Daemon/UDev.hpp \
	src/Daemon/RuleParser.cpp \
	src/Daemon/RuleParser.hpp \
        src/Daemon/RuleParser/Lexer-configuration.hpp \
        src/Daemon/RuleParser/Lexer-token.hpp \
        src/Daemon/RuleParser/Lexer-token_ids.hpp \
        src/Daemon/RuleParser/Lexer.cpp \
        src/Daemon/RuleParser/Lexer.hpp \
	src/Daemon/RuleParser/Parser.h \
	src/Daemon/main.cpp

usbguard_daemon_CPPFLAGS=\
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/src/Daemon/RuleParser/

usbguard_daemon_LDADD=\
	$(top_builddir)/libusbguard.la \
	@udev_LIBS@ \
	@qb_LIBS@ \
	@sodium_LIBS@

EXTRA_DIST=\
	README.md \
	src/ThirdParty/spdlog \
	src/ThirdParty/cppformat/format.h \
	src/ThirdParty/json/src/json.hpp \
	src/Daemon/RuleParser/quex \
	src/Daemon/RuleParser/Parser.c

SUBDIRS=src/
