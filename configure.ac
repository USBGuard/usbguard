AC_PREREQ([2.69])
AC_INIT([usbguard], [0.1], [foobar@localhost])
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
CXXFLAGS="-std=c++11 -Wall -Wformat=2 $CXXFLAGS"

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

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL
AC_PROG_LIBTOOL

# libtool versioning
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

# Checks for libraries.
PKG_CHECK_MODULES([udev], [libudev >= 200], [],
                  AC_MSG_FAILURE([libudev development files not found!]))
PKG_CHECK_MODULES([qb], [libqb >= 0.16.0], [],
                  AC_MSG_FAILURE([libqb development files not found!]))
PKG_CHECK_MODULES([sodium], [libsodium >= 0.4.5], [],
                  AC_MSG_FAILURE([libsodium development files not found!]))

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

if test "$debug" = "yes"; then
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_ENABLED"
else
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_DISABLED"
   AC_DEFINE([NDEBUG], [1], [Debug build disabled])
fi

AC_CONFIG_FILES([Makefile libusbguard.pc src/Makefile])
AC_OUTPUT
