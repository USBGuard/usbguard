AC_PREREQ([2.69])
AC_INIT([usbguard], [0.4], [dkopecek@redhat.com])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([src/build-config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([subdir-objects foreign tar-pax])

# If automake supports "silent rules", enable them by default
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

#
# Compiler flags
#
CXXFLAGS="-std=c++11 -Wall -Wformat=2 -fvisibility=hidden $CXXFLAGS"

#
# Additional CXXFLAGS used when --enable-debug-build is used
#
CXXFLAGS_DEBUG_ENABLED="-g -O0"

#
# Additional CXXFLAGS used when --disable-debug-build is used
# and when --enable-debug-build is
#
# Don't add -DNDEBUG here. It's added to the generated config.h
# header file.
#
CXXFLAGS_DEBUG_DISABLED=""

# Libtool Versioning
# See http://sources.redhat.com/autobook/autobook/autobook_91.html#SEC91 for details

## increment if the interface has additions, changes, removals.
LT_CURRENT=0

## increment any time the source changes; set 0 to if you increment CURRENT
LT_REVISION=0

## increment if any interfaces have been added; set to 0
## if any interfaces have been changed or removed. removal has
## precedence over adding, so set to 0 if both happened.
LT_AGE=0

AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_ARG_ENABLE([coverage],
     [AC_HELP_STRING([--enable-coverage], [enable instrumented compilation for code coverage testing (default=no)])],
     [case "${enableval}" in
       yes) coverage=yes ;;
       no)  coverage=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-coverage]) ;;
     esac], [coverage=no])

if test "$coverage" = "yes"; then
   CXXFLAGS="$CXXFLAGS --coverage"
fi

#
# Checks for programs.
#
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL
AC_PROG_LIBTOOL

#
# Checks for required libraries.
#
PKG_CHECK_MODULES([udev], [libudev >= 200],
[libudev_summary="system-wide; $udev_CFLAGS $udev_LIBS"],
[AC_MSG_FAILURE([libudev development files not found!])]
)

#
# qb library
#
PKG_CHECK_MODULES([qb], [libqb >= 0.16.0],
[AC_DEFINE([HAVE_LIBQB], [1], [libqb API available])
libqb_summary="system-wide; $qb_CFLAGS $qb_LIBS"],
[AC_MSG_FAILURE([libqb development files not found!])]
)

#
# sodium library
#
PKG_CHECK_MODULES([sodium], [libsodium >= 0.4.5],
[AC_DEFINE([HAVE_LIBSODIUM], [1], [libsodium API available])
libsodium_summary="system-wide; $sodium_CFLAGS $sodium_LIBS"],
[AC_MSG_FAILURE([libsodium development files not found!])]
)

#
# Check for optional libraries
#
PKG_CHECK_MODULES([seccomp], [libseccomp >= 2.0.0],
[AC_DEFINE([HAVE_SECCOMP], [1], [seccomp API usable])
libseccomp_summary="system-wide; $seccomp_LIBS"],
[AC_MSG_NOTICE([libseccomp development files not found! Seccomp support will be turned OFF])
libseccomp_summary="not found"]
)

PKG_CHECK_MODULES([libcapng], [libcap-ng >= 0.7.0],
[AC_DEFINE([HAVE_LIBCAPNG], [1], [cap-ng API usable])
libcap_ng_summary="system-wide; $libcapng_LIBS"],
[AC_MSG_NOTICE([libseccomp development files not found! Seccomp support will be turned OFF])
libcap_ng_summary="not found"]
)

#
# json C++ library
#
with_bundled_json=yes
AC_ARG_WITH([bundled-json], AS_HELP_STRING([--with-bundled-json], [Build using the bundled json library]))
if test "x$with_bundled_json" = "xyes"; then
	json_CFLAGS="-I\$(top_srcdir)/src/ThirdParty/json/src"
	json_LIBS=""
	AC_MSG_NOTICE([Using bundled json library])
	json_summary="bundled; $json_CFLAGS $json_LIBS"
else
	SAVE_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="-std=c++11 $CPPFLAGS"
	AC_LANG_PUSH([C++])
	AC_CHECK_HEADER([json.hpp], [], [AC_MSG_FAILURE(json.hpp not found or not usable. Re-run with --with-bundled-json to use the bundled library.)])
	AC_LANG_POP
	json_CFLAGS=""
	json_LIBS=""
	CPPFLAGS=$SAVE_CPPFLAGS
	json_summary="system-wide; $json_CFLAGS $json_LIBS"
fi
AC_SUBST([json_CFLAGS])
AC_SUBST([json_LIBS])

#
# spdlog C++ library
#
with_bundled_spdlog=yes
AC_ARG_WITH([bundled-spdlog], AS_HELP_STRING([--with-bundled-spdlog], [Build using the bundled spdlog library]))
if test "x$with_bundled_spdlog" = "xyes"; then
	spdlog_CFLAGS="-I\$(top_srcdir)/src/ThirdParty/spdlog/include"
	spdlog_LIBS=""
	AC_MSG_NOTICE([Using bundled spdlog library])
	spdlog_summary="bundled; $spdlog_CFLAGS $spdlog_LIBS"
else
	SAVE_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="-std=c++11 $CPPFLAGS"
	AC_LANG_PUSH([C++])
	AC_CHECK_HEADER([spdlog/spdlog.h], [], [AC_MSG_FAILURE(spdlog.h not found or not usable. Re-run with --with-bundled-spdlog to use the bundled library.)])
	AC_LANG_POP
	spdlog_CFLAGS=""
	spdlog_LIBS=""
	CPPFLAGS=$SAVE_CPPFLAGS
	spdlog_summary="system-wide; $spdlog_CFLAGS $spdlog_LIBS"
fi
AC_SUBST([spdlog_CFLAGS])
AC_SUBST([spdlog_LIBS])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h locale.h netdb.h stdint.h stdlib.h string.h sys/time.h syslog.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 gettimeofday localtime_r memset mkdir regcomp setlocale strerror strstr])

# ./configure script options
AC_ARG_ENABLE([debug-build],
     [AC_HELP_STRING([--enable-debug-build], [enable debugging flags (default=no)])],
     [case "${enableval}" in
       yes) debug=yes ;;
       no)  debug=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug-build]) ;;
     esac], [debug=no])

AC_ARG_ENABLE([systemd],
     [AC_HELP_STRING([--enable-systemd], [install the systemd service unit file (default=no)])],
     [case "${enableval}" in
       yes) systemd=yes ;;
       no)  systemd=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-systemd]) ;;
     esac], [systemd=no])

if test "$debug" = "yes"; then
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_ENABLED"
else
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_DISABLED"
   AC_DEFINE([NDEBUG], [1], [Debug build disabled])
   AC_DEFINE([QUEX_OPTION_ASSERTS_DISABLED], [1], [Disable asserts in the Quex generated lexer])
fi

if test "$systemd" = "yes"; then
   AC_DEFINE([SYSTEMD_SUPPORT_ENABLED], [1], [Enable systemd support int the project])
fi

AM_CONDITIONAL([SYSTEMD_SUPPORT_ENABLED], [test "$systemd" = "yes" ])

AC_CONFIG_FILES([Makefile libusbguard.pc src/Tests/Makefile])
AC_OUTPUT

echo
echo "    Build Configuration Summary    "
echo "==================================="
echo
echo "## Libraries"
echo
echo "    libudev: $libudev_summary"
echo "      libqb: $libqb_summary"
echo "  libsodium: $libsodium_summary"
echo " libseccomp: $libseccomp_summary"
echo "  libcap-ng: $libcap_ng_summary"
echo "       json: $json_summary"
echo "     spdlog: $spdlog_summary"
echo
echo "## Compilation Flags"
echo
echo " Debug Mode: $debug"
echo "   CXXFLAGS: $CXXFLAGS"
echo "   CPPFLAGS: $CPPFLAGS"
echo "    LDFLAGS: $LDFLAGS"
echo
echo "## Compile-Time Parameter Values"
echo
echo ""
echo ""
echo
