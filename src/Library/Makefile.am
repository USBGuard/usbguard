lib_LTLIBRARIES=\
	libusbguard.la

libusbguard_la_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/Library \
	-I$(top_srcdir)/src/Library/RuleParser

libusbguard_la_SOURCES=\
	ConfigFile.cpp \
	ConfigFilePrivate.cpp \
	ConfigFilePrivate.hpp \
	IPCClient.cpp \
	IPCClientPrivate.hpp \
	Rule.cpp \
	RuleParser.cpp \
	RuleParser.hpp \
	RulePrivate.cpp \
	RulePrivate.hpp \
	RuleSet.cpp \
	RuleSetPrivate.cpp \
	RuleSetPrivate.hpp

pkginclude_HEADERS=\
	ConfigFile.hpp \
	IPC.hpp \
	IPCClient.hpp \
	Rule.hpp \
	RuleSet.hpp \
	Thread.hpp \
	Typedefs.hpp
