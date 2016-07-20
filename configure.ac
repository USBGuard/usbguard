AC_PREREQ([2.69])
AC_INIT([usbguard],
        m4_esyscmd_s([sed -n 's|^\([0-9\.]\+\)-[0-9]\+.*|\1|p' .tito/packages/usbguard]),
        [dkopecek@redhat.com])
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

if test "x$coverage" = xyes; then
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
AC_ARG_WITH([bundled-json], AS_HELP_STRING([--with-bundled-json], [Build using the bundled json library]), [with_bundled_json=$withval], [with_bundled_json=no])
if test "x$with_bundled_json" = xyes; then
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
AC_ARG_WITH([bundled-spdlog], AS_HELP_STRING([--with-bundled-spdlog], [Build using the bundled spdlog library]), [with_bundled_spdlog=$withval], [with_bundled_spdlog=no])
if test "x$with_bundled_spdlog" = xyes; then
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

#
# Catch C++ library
#
AC_ARG_WITH([bundled-catch], AS_HELP_STRING([--with-bundled-catch], [Build using the bundled Catch library]), [with_bundled_catch=$withval], [with_bundled_catch=no])
if test "x$with_bundled_catch" = xyes; then
	catch_CFLAGS="-I\$(top_srcdir)/src/ThirdParty/Catch/include"
	catch_LIBS=""
	AC_MSG_NOTICE([Using bundled Catch library])
	catch_summary="bundled; $catch_CFLAGS $catch_LIBS"
else
	SAVE_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="-std=c++11 $CPPFLAGS -I/usr/include/catch"
	AC_LANG_PUSH([C++])
	AC_CHECK_HEADER([catch.hpp], [], [AC_MSG_FAILURE(catch.hpp not found or not usable. Re-run with --with-bundled-catch to use the bundled library.)])
	AC_LANG_POP
	catch_CFLAGS="-I/usr/include/catch"
	catch_LIBS=""
	CPPFLAGS=$SAVE_CPPFLAGS
	catch_summary="system-wide; $catch_CFLAGS $catch_LIBS"
fi
AC_SUBST([catch_CFLAGS])
AC_SUBST([catch_LIBS])

#
# PEGTL C++ library
#
AC_ARG_WITH([bundled-pegtl], AS_HELP_STRING([--with-bundled-pegtl], [Build using the bundled PEGTL library]), [with_bundled_pegtl=$withval], [with_bundled_pegtl=no])
if test "x$with_bundled_pegtl" = xyes; then
	pegtl_CFLAGS="-I\$(top_srcdir)/src/ThirdParty/PEGTL"
	pegtl_LIBS=""
	AC_MSG_NOTICE([Using bundled PEGTL library])
	pegtl_summary="bundled; $pegtl_CFLAGS $pegtl_LIBS"
else
	SAVE_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="-std=c++11 $CPPFLAGS"
	AC_LANG_PUSH([C++])
	AC_CHECK_HEADER([pegtl.hh], [], [AC_MSG_FAILURE(pegtl.hh not found or not usable. Re-run with --with-bundled-pegtl to use the bundled library.)])
	AC_LANG_POP
	pegtl_CFLAGS=""
	pegtl_LIBS=""
	CPPFLAGS=$SAVE_CPPFLAGS
	pegtl_summary="system-wide; $pegtl_CFLAGS $pegtl_LIBS"
fi
AC_SUBST([pegtl_CFLAGS])
AC_SUBST([pegtl_LIBS])

#
# Qt 5.x GUI
#
AC_ARG_WITH([gui-qt5], AS_HELP_STRING([--with-gui-qt5], [Build the Qt 5.x GUI components]), [], [with_gui_qt5=yes])
if test "x$with_gui_qt5" = xyes; then
  #
  # Check for Qt 5.x libraries
  #
  PKG_CHECK_MODULES([qt5], [Qt5Gui >= 5.0 Qt5Widgets >= 5.0],
  [AC_DEFINE([HAVE_QT5], [1], [Required Qt 5.x API available])
  qt5_summary="system-wide; $qt5_CFLAGS $qt5_LIBS"],
  [AC_MSG_FAILURE([Qt 5.x development files not found!])]
  )

  #
  # Check for Qt 5.x build tools
  #
  AC_CHECK_PROGS(MOCQT5, [moc-qt5 moc])
  AC_CHECK_PROGS(UICQT5, [uic-qt5 uic])
  AC_CHECK_PROGS(RCCQT5, [rcc-qt5 rcc])
  if test -z "$MOCQT5" || test -z "$UICQT5" || test -z "$RCCQT5"; then
    AC_MSG_FAILURE([Qt 5.x build tools are missing on your system. Make sure that moc-qt5, uic-qt5 and rcc-qt5 are installed.])
  fi
else
  qt5_summary="None; building without Qt 5.x GUI"
fi

#
# GLib D-Bus
#
AC_ARG_WITH([dbus], AC_HELP_STRING([--with-dbus], [Build the DBus Bridge service]), [], [with_dbus=yes])
if test "x$with_dbus" = xyes; then
  #
  # Check for required D-Bus modules 
  #
  PKG_CHECK_MODULES([dbus], [dbus-1 dbus-glib-1 >= 0.100 gio-2.0],
  [AC_DEFINE([HAVE_DBUS], [1], [Required GLib DBus API available])
  dbus_summary="system-wide; $dbus_CFLAGS $dbus_LIBS"],
  [AC_MSG_FAILURE([Required D-Bus modules (dbus-1, dbus-glib-1, gio-2.0) not found!])]
  )

  #
  # Check for xmllint tool
  #
  AC_CHECK_PROGS(XMLLINT, [xmllint])
  if test -z "$XMLLINT" ; then
    AC_MSG_FAILURE([The required xmllint tool is missing on your system. Make sure that xmllint is installed.])
  fi
  #
  # Check for xsltproc tool
  #
  AC_CHECK_PROGS(XSLTPROC, [xsltproc])
  if test -z "$XSLTPROC"; then
    AC_MSG_FAILURE([The required xsltproc tool is missing on your system. Make sure that xsltproc is installed.])
  fi
  #
  # Check for gdbus-codegen tool
  #
  AC_CHECK_PROGS(GDBUS_CODEGEN, [gdbus-codegen])
  if test -z "$GDBUS_CODEGEN"; then
    AC_MSG_FAILURE([The required gdbus-codegen tool is missing on your system. Make sure that gdbus-codegen is installed.])
  fi

  dbus_services_dir=`$PKG_CONFIG dbus-1 --define-variable=datadir='${datadir}' --define-variable=prefix='${prefix}' --variable=system_bus_services_dir`
  if test "x$dbus_services_dir" = x; then
    AC_MSG_FAILURE([Cannot autodetect the D-Bus system services directory])
  else
    #
    # Hack around expanded paths in system_bus_services_dir. Without this distcheck won't
    # work, because it uses ./configure --prefix=$somedir. The expanded path ignores this
    # prefix (and DESTDIR won't help here either)
    #
    case "$dbus_services_dir" in
      /usr/share/*|/usr/local/share/*)
        dbus_services_dir=$(sed -r 's,^(/usr/share|/usr/local/share),${datadir},'<<<$dbus_services_dir)
      ;;
      /usr/*|/usr/local/*)
        dbus_services_dir=$(sed -r 's,^(/usr|/usr/local),${prefix},'<<<$dbus_services_dir)
      ;;
      /*)
        dbus_services_dir='${prefix}'"$dbus_services_dir"
      ;;
    esac
    AC_SUBST(DBUS_SERVICES_DIR, $dbus_services_dir)
    AC_DEFINE([DBUS_SERVICES_DIR], [$dbus_services_dir], [D-Bus system services directory])
  fi

  dbus_data_dir=`$PKG_CONFIG dbus-1 --define-variable=datadir='${datadir}' --define-variable=prefix='${prefix}' --variable=datadir`
  if test "x$dbus_data_dir" = x; then
    AC_MSG_FAILURE([Cannot autodetect the D-Bus data directory])
  else
    dbus_busconfig_dir="$dbus_data_dir/dbus-1/system.d"
    AC_SUBST(DBUS_BUSCONFIG_DIR, $dbus_busconfig_dir)
    AC_DEFINE([DBUS_BUSCONFIG_DIR], [$dbus_busconfig_dir], [D-Bus busconfig directory])
  fi
else
  dbus_summary="None; building without DBus support"
  dbus_services_dir="-"
  dbus_busconfig_dir="-"
fi

#
# PolicyKit
#
AC_ARG_WITH([polkit], AC_HELP_STRING([--with-polkit], [Install the PolicyKit configuration if D-Bus support is also enabled]), [], [with_polkit=yes])
if test "x$with_polkit" = xyes; then
  #
  # Check for required PolicyKit modules
  #
  PKG_CHECK_MODULES([polkit], [polkit-gobject-1],
  [AC_DEFINE([HAVE_POLKIT], [1], [Required PolicyKit modules available])
  polkit_summary="system-wide; `$PKG_CONFIG polkit-gobject-1 --define-variable=prefix='${prefix}' --define-variable=datadir='${datadir}' --variable=policydir`"],
  [AC_MSG_FAILURE([Required PolicyKit modules (polkit-gobject-1) not found!])]
  )

  polkit_policy_dir="`$PKG_CONFIG polkit-gobject-1 --define-variable=prefix='${prefix}' --define-variable=datadir='${datadir}' --variable=policydir`"
  if test "x$polkit_policy_dir" = x; then
    AC_MSG_FAILURE([Cannot autodetect the PolicyKit policy directory])
  else
    AC_SUBST(POLKIT_POLICY_DIR, $polkit_policy_dir)
    AC_DEFINE([POLKIT_POLICY_DIR], [$polkit_policy_dir], [PolicyKit policy directory]) 
  fi
else
  polkit_summary="None; building without PolicyKit support"
  polkit_policy_dir="-"
fi

#
# AddressSanitizer
#
# Based on: https://github.com/machinezone/tcpkali/blob/15903a7692abecbd28899d44e822a6f894caa23c/configure.aci#L73
#
# Enable Address Sanitizer, if supported by gcc (4.8+) or clang.
# http://clang.llvm.org/docs/AddressSanitizer.html
# https://code.google.com/p/address-sanitizer/wiki/HowToBuild
#
AC_ARG_ENABLE([asan],
  [AS_HELP_STRING([--enable-asan], [Enable Address Sanitizer])],
  [enable_asan=$enableval], [enable_asan=no])

if test "x$enable_asan" = xyes; then
  AC_LANG_PUSH([C++])
  AX_CHECK_COMPILE_FLAG([-faddress-sanitizer],
                        [ASAN_FLAGS="$ASAN_FLAGS -faddress-sanitizer"],
                        [enable_asan=no], [])
  if test "x$enable_asan" = xno; then
    AX_CHECK_COMPILE_FLAG([-fsanitize=address],
                          [ASAN_FLAGS="$ASAN_FLAGS -fsanitize=address"
                           enable_asan=yes],
                          [enable_asan=no], [])
    if test "x$enable_asan" = xno; then
      AC_MSG_FAILURE([--enable-asan is given, but not supported on this platform. Check out https://code.google.com/p/address-sanitizer/wiki/HowToBuild])
    fi
  fi
  # Keep error messages nice. Also consider:
  # export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
  # export ASAN_OPTIONS=symbolize=1
  AX_CHECK_COMPILE_FLAG([-fno-omit-frame-pointer],
                        [ASAN_FLAGS="$ASAN_FLAGS -fno-omit-frame-pointer"],
                        [], [])
  AC_LANG_POP
fi
#
# Enable Thread Sanitizer, if supported by clang.
# http://clang.llvm.org/docs/ThreadSanitizer.html
# https://code.google.com/p/address-sanitizer/wiki/HowToBuild
#
AC_ARG_ENABLE([tsan],
    [AS_HELP_STRING([--enable-tsan], [Enable Thread Sanitizer])],
    [enable_tsan=$enableval], [enable_tsan=no])

if test "x$enable_tsan" = xyes; then
  AX_CHECK_COMPILE_FLAG([-fsanitize=thread],
                        [TSAN_FLAGS="$TSAN_FLAGS -fsanitize=thread"],
                        [enable_tsan=no], [])
  if test "x$enable_tsan" = xno; then
    AC_MSG_FAILURE([--enable-tsan is given, but not supported on this platform. Check out https://code.google.com/p/address-sanitizer/wiki/HowToBuild])
  fi
  # Keep error messages nice.
  AX_CHECK_COMPILE_FLAG([-fno-omit-frame-pointer],
                        [TSAN_FLAGS="$TSAN_FLAGS -fno-omit-frame-pointer"],
                        [], [])
  # -fsanitize=thread linking must be done with -pie or -shared
  # We can't do anything about this message in GCC; use clang.
fi

#
# Check whether the pandoc utility is present.
#
# If pandoc is not present, manpages won't be
# generated from source and their pre-generated
# version will be used instead.
#
AC_CHECK_PROGS(PANDOC, [pandoc])
if test -z "$PANDOC"; then
  AC_MSG_WARN([Unable to find manpage generator (pandoc). Installation will use pre-generated files.])
fi
AM_CONDITIONAL([GENERATE_MANPAGES], [test -n "$PANDOC"])

#
# Check whether the aspell utility is present.
#
# If aspell is not present, the test suite will skip spell
# checking of documentation related files.
#
AC_CHECK_PROGS(ASPELL, [aspell])
if test -z "$ASPELL"; then
  AC_MSG_WARN([Unable to find the Aspell spell checker. The test suite will skip spell checking of documentation related files.])
fi

#
# Maintainer mode.
#
# Runs several additional taks for certain make targets (e.g. tests)
#
AC_ARG_ENABLE([maintainer-mode],
              [AS_HELP_STRING([--enable-maintainer-mode], [Enable maintainer mode (default=no)])],
              [maintainer_mode=$enableval], [maintainer_mode=no])

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

if test "x$debug" = xyes; then
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_ENABLED"
else
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_DISABLED"
   AC_DEFINE([NDEBUG], [1], [Debug build disabled])
   AC_DEFINE([QUEX_OPTION_ASSERTS_DISABLED], [1], [Disable asserts in the Quex generated lexer])
fi

if test "x$enable_asan" = xyes; then
  CXXFLAGS="$CXXFLAGS $ASAN_FLAGS"
  AC_DEFINE([ASAN_ENABLED], [1], [Address Sanitizer is enabled])
fi

if test "x$enable_tsan" = xyes; then
  CXXFLAGS="$CXXFLAGS $TSAN_FLAGS"
  AC_DEFINE([TSAN_ENABLED], [1], [Thread Sanitizer is enabled])
fi

if test "x$systemd" = xyes; then
   systemd_unit_dir="`$PKG_CONFIG systemd --variable=systemdsystemunitdir`"
   if test "x$systemd_unit_dir" = x; then
     AC_MSG_FAILURE([Cannot detect the systemd system unit dir])
   fi
   AC_SUBST(SYSTEMD_UNIT_DIR, $systemd_unit_dir)
   AC_DEFINE([SYSTEMD_SUPPORT_ENABLED], [1], [Enable systemd support int the project])
else
  systemd_unit_dir="-"
fi

AC_SUBST(ANALYZE_CONFIGURE_ARGS, $ac_configure_args)

AM_CONDITIONAL([SYSTEMD_SUPPORT_ENABLED], [test "x$systemd" = xyes ])
AM_CONDITIONAL([GUI_QT5_ENABLED], [test "x$with_gui_qt5" = xyes ])
AM_CONDITIONAL([DBUS_ENABLED], [test "x$with_dbus" = xyes ])
AM_CONDITIONAL([POLICYKIT_ENABLED], [test "x$with_polkit" = xyes])
AM_CONDITIONAL([MAINTAINER_MODE], [test "x$maintainer_mode" = xyes])

AC_CONFIG_FILES([Makefile
                 src/Tests/Makefile
                 libusbguard.pc
                 src/GUI.Qt5/usbguard-applet-qt.desktop])
AC_OUTPUT

echo
echo "    Build Configuration Summary    "
echo "==================================="
echo
echo " Maintainer mode: $maintainer_mode"
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
echo "      Catch: $catch_summary"
echo "      PEGTL: $pegtl_summary"
echo "     Qt 5.x: $qt5_summary"
echo " GLib D-Bus: $dbus_summary"
echo
echo "## Directories"
echo
echo " D-Bus System Services: $dbus_services_dir"
echo "       D-Bus Busconfig: $dbus_busconfig_dir"
echo "    PolicyKit Policies: $polkit_policy_dir"
echo "      systemd unit dir: $systemd_unit_dir"
echo
echo "## Compilation Flags"
echo
echo " Debug Mode: $debug"
echo "   CXXFLAGS: $CXXFLAGS"
echo "   CPPFLAGS: $CPPFLAGS"
echo "    LDFLAGS: $LDFLAGS"
echo " ASAN_FLAGS: $ASAN_FLAGS"
echo " TSAN_FLAGS: $TSAN_FLAGS"
echo

