AM_CPPFLAGS=\
	-I$(top_srcdir)/src\
	-I$(top_srcdir)/src/Library\
	-I$(top_srcdir)/src/ThirdParty/Catch/include

EXTRA_DIST=\
	$(top_srcdir)/src/ThirdParty/Catch \
	$(top_srcdir)/src/Tests/Packaging/files-without-copyright.sh

TESTS=\
	test-unit \
	test-regression

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
	Regression/test_Rule_ghi37.cpp

test_regression_LDADD=\
	$(top_builddir)/libusbguard.la
