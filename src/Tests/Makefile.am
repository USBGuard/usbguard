AM_CPPFLAGS=\
	-I$(top_srcdir)/src\
	-I$(top_srcdir)/src/Library\
	-I$(top_srcdir)/src/ThirdParty/Catch/include

TESTS=\
	test-unit

check_PROGRAMS=\
	test-unit

test_unit_SOURCES=\
	Unit/main.cpp \
	Unit/test_Rule.cpp \
	Unit/test_RuleParser.cpp

test_unit_LDADD=\
	$(top_builddir)/libusbguard.la

EXTRA_DIST=\
	$(top_srcdir)/src/ThirdParty/Catch
