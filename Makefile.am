SUBDIRS=src/Tests/

ACLOCAL_AMFLAGS= -I m4
EXTRA_DIST=\
	LICENSE

dist_man8_MANS=\
	doc/usbguard-daemon.roff

dist_man5_MANS=\
	doc/usbguard-daemon.conf.roff \
	doc/usbguard-rules.conf.roff

dist_man1_MANS=\
	doc/usbguard-generate-policy.roff

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
	src/Library/LoggerPrivate.cpp

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
	@libcapng_CFLAGS@

usbguard_daemon_LDADD=\
	$(top_builddir)/libusbguard.la \
	@spdlog_LIBS@ \
	@json_LIBS@ \
	@seccomp_LIBS@ \
	@libcapng_LIBS@

EXTRA_DIST +=\
	README.md \
	src/ThirdParty/spdlog \
	src/ThirdParty/json/src/json.hpp \
	src/ThirdParty/Catch

bin_PROGRAMS=\
	usbguard-generate-policy

usbguard_generate_policy_SOURCES=\
	src/CLI/usbguard-generate-policy.cpp \
	src/CLI/PolicyGenerator.hpp \
	src/CLI/PolicyGenerator.cpp \
	src/Common/Utility.cpp

usbguard_generate_policy_CPPFLAGS=\
	$(AM_CPPFLAGS)

usbguard_generate_policy_LDADD=\
	$(top_builddir)/libusbguard.la

#usbguard_SOURCES=\
#	src/CLI/usbguard.cpp \
#	src/CLI/usbguard-cli-parser.h \
#	src/CLI/CLICommand.cpp \
#	src/CLI/CLICommand.hpp \
#	src/CLI/RuleAppend.cpp \
#	src/CLI/RuleAppend.hpp \
#	src/CLI/DeviceSetPolicy.cpp \
#	src/CLI/DeviceSetPolicy.hpp \
#	src/CLI/CLIArgv_Lexer-configuration.hpp \
#	src/CLI/CLIArgv_Lexer.cpp \
#	src/CLI/CLIArgv_Lexer.hpp \
#	src/CLI/CLIArgv_Lexer-token.hpp \
#	src/CLI/CLIArgv_Lexer-token_ids.hpp \
#	src/CLI/CLILine_Lexer-configuration.hpp \
#	src/CLI/CLILine_Lexer.cpp \
#	src/CLI/CLILine_Lexer.hpp \
#	src/CLI/CLILine_Lexer-token.hpp \
#	src/CLI/CLILine_Lexer-token_ids.hpp

#usbguard_CPPFLAGS=\
#	$(AM_CPPFLAGS) \
#	-I$(top_srcdir)/src/CLI \
#	-DQUEX_OPTION_MULTI

#usbguard_LDADD=\
#	$(top_builddir)/libusbguard.la

#EXTRA_DIST+=\
#	usbguard-cli-argv-lexer.qx \
#	usbguard-cli-lexer.qx \
#	usbguard-cli-parser.y \
#	usbguard-cli-parser.c
