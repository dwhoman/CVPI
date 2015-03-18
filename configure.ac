AC_PREREQ([2.61])
AC_INIT([cvpi],[1.0])
AC_CANONICAL_HOST

dnl subdir-objects: for non-recursive make
AM_INIT_AUTOMAKE([-Wall subdir-objects])
AM_PROG_AR

AC_LANG([C])

LT_PREREQ([2.2])
LT_INIT

AC_PROG_CC([gcc cl cc])
dnl AC_PROG_LIBTOOL
AM_PROG_CC_C_O

AC_PROG_INSTALL

dnl AC_PROG_RANLIB

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/cvpi_base.c])
AC_CONFIG_HEADERS([config.h])

dnl c99 features: restrict, variadic macros, inline, compound literals
AC_PROG_CC_C99
if test "x${ac_cv_prog_cc_c99}" = xno; then
   AC_MSG_WARN([C99 compatible compiler not found. Compilation might fail.])
fi

dnl assuming that the program will not be ran on mixed-endian
AC_C_BIGENDIAN(AC_MSG_WARN([CVPI has not been tested on big endian.]),,AC_MSG_WARN([Unknown endianness.]))

dnl Make sure the pkg-config macros are defined
m4_ifndef([PKG_PROG_PKG_CONFIG],
    [m4_fatal([Could not locate the pkg-config autoconf macros.
  These are usually located in /usr/share/aclocal/pkg.m4. If your macros
  are in a different location, try setting the environment variable
  ACLOCAL="aclocal -I/other/macro/dir" before running autoreconf.])])
PKG_PROG_PKG_CONFIG()


dnl CVPI_RASPBERRY_CPPFLAGS="-I /opt/vc/include/ -I /opt/vc/include/EGL/ -I /opt/vc/include/interface/vmcs_host/linux/ -I /opt/vc/include/interface/vcos/pthreads/"
dnl CVPI_RASPBERRY_LDFLAGS="-L /opt/vc/lib"
dnl AC_SUBST(CVPI_RASPBERRY_CPPFLAGS)
dnl AC_SUBST(CVPI_RASPBERRY_LDFLAGS)

CVPI_RASPBERRY_CPPFLAGS=""
CVPI_RASPBERRY_LDFLAGS=""

AC_DEFINE([HAVE_BCM_HOST],[0],[Defining HAVE_BCM_HOST = 0])

if test -d /opt/vc/lib && test -d /opt/vc/include/ && test -d /opt/vc/include/EGL/ && test -d /opt/vc/include/interface/vmcs_host/linux/ && test -d /opt/vc/include/interface/vcos/pthreads/ && test -d /opt/vc/include/VG/
then
CVPI_RASPBERRY_CPPFLAGS="-I /opt/vc/include/ -I /opt/vc/include/interface/vmcs_host/linux/ -I /opt/vc/include/interface/vcos/pthreads/"
CVPI_RASPBERRY_LDFLAGS="-L /opt/vc/lib"

AC_CHECK_HEADERS(/opt/vc/include/bcm_host.h, [AC_DEFINE([HAVE_BCM_HOST],[1], [Defining HAVE_BCM_HOST = 1])], [AC_MSG_WARN([Unable to find bcm_host.h.])])
AC_CHECK_HEADERS(/opt/vc/include/VG/openvg.h,[],[AC_MSG_WARN([Unable to find openvg.h.])])
AC_CHECK_HEADERS(/opt/vc/include/EGL/eglext_brcm.h,[],[AC_MSG_WARN([Unable to find eglext_brcm.h.])])
AC_CHECK_HEADERS(/opt/vc/include/EGL/egl.h,[],[AC_MSG_WARN([Unable to find egl.h.])])
AC_CHECK_HEADERS(/opt/vc/include/EGL/eglplatform.h,[],[AC_MSG_WARN([Unable to find eglplatform.h.])])

echo "Creating Raspberry Pi Paths.\n"
else
echo "Not Creating Raspberry Pi Paths.\n"
PKG_CHECK_MODULES(EGL, [egl])
PKG_CHECK_MODULES(VG, [vg])
fi

AC_SUBST(CVPI_RASPBERRY_CPPFLAGS)
AC_SUBST(CVPI_RASPBERRY_LDFLAGS)

dnl AC_CHECK_HEADERS([stdlib.h stdio.h errno.h math.h unistd.h stdarg.h],,
dnl 			   AC_MSG_ERROR([stdlib.h, stdio.h, errno.h, math.h, unistd.h, or stdarg.h not found.]))
dnl AC_CHECK_HEADER([openvg.h],,
dnl 	AC_MSG_ERROR([openvg.h not found.]))
dnl AC_CHECK_HEADER([vgu.h],,
dnl 	AC_MSG_ERROR([vgu.h not found.]))
dnl AC_CHECK_HEADER([egl.h],,
dnl 	AC_MSG_ERROR([egl.h not found.]))
dnl AC_CHECK_HEADER([eglplatform.h],,
dnl 	AC_MSG_ERROR([eglplatform.h not found.]))

AC_CHECK_HEADERS([stdlib.h stdio.h errno.h math.h unistd.h stdarg.h])
dnl AC_CHECK_HEADER([openvg.h])
dnl AC_CHECK_HEADER([vgu.h])
dnl AC_CHECK_HEADER([egl.h])
dnl AC_CHECK_HEADER([eglplatform.h])

dnl logging type options
AC_ARG_ENABLE([logging-sync],
	[AS_HELP_STRING([--enable-logging-sync],[log printing is done synchronously @<:@default: yes@:>@])],
	[logging_sync=${enableval}], [logging_sync=yes])
AC_ARG_ENABLE([logging-async],
	[AS_HELP_STRING([--enable-logging-async],[log printing is done asynchronously @<:@default: no@:>@])],
	[logging_async=${enableval}], [logging_async=no])
AC_ARG_ENABLE([logging-none],
	[AS_HELP_STRING([--enable-logging-none],[log printing is skipped @<:@default: no@:>@])],
	[logging_none=${enableval}], [logging_none=no])

dnl can't get AX_PTHREAD or ACX_PTHREAD to work
dnl using example from configure.ax ch.7 of Autotools by John Calcote
if test "x${logging_async}" = xyes; then
have_pthreads=no
AC_SEARCH_LIBS([pthread_create], [pthread], [have_pthreads=yes])

    if test "x${have_pthreads}" = xyes; then
        AC_CHECK_HEADERS([pthread.h], [], [have_pthreads=no])
    fi

    if test "x${have_pthreads}" = xno; then
        AC_MSG_WARN([Unable to find pthreads on this system.])
        async_exec=no
    fi
fi


if test "x${logging_async}" = xyes && test "x${have_pthreads}" = xno; then
   AC_MSG_ERROR([System does not support PTHREADS.])
fi
if test "x${logging_sync}" = xyes && test "x${logging_async}" = xyes; then
   AC_MSG_ERROR([logging-sync and logging-async are mutually exclusive: both are enabled])
fi
if test "x${logging_sync}" = xyes && test "x${logging_none}" = xyes; then
   AC_MSG_ERROR([logging-sync and logging-none are mutually exclusive: both are enabled])
fi
if test "x${logging_none}" = xyes && test "x${logging_async}" = xyes; then
   AC_MSG_ERROR([logging-async and logging-none are mutually exclusive: both are enabled])
fi

dnl AC_DEFINE([CVPI_ASYNC_LOGGING],0,[synchronous logging enabled])
dnl AC_DEFINE([CVPI_NO_LOGGING],0, [synchronous logging enabled])
if test "x${logging_sync}" = xyes; then
   AC_DEFINE([CVPI_ASYNC_LOGGING],[0],[synchronous logging enabled])
   AC_DEFINE([CVPI_NO_LOGGING],[0], [synchronous logging enabled])

elif test "x${logging_async}" = xyes; then
dnl dnl ACX_PTHREAD from configure.ac flaim project AUTOTOOLS A Practitioner’s Guide
dnl dnl to GNU Autoconf, Automake, and Libtool by John Calcote

dnl dnl Check for pthreads
AC_DEFINE([CVPI_ASYNC_LOGGING],[1], [asynchronous logging enabled])
AC_DEFINE([CVPI_NO_LOGGING],[0], [asynchronous logging enabled])

LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
CC="$PTHREAD_CC"
CXX="$PTHREAD_CXX"

else
AC_DEFINE([CVPI_ASYNC_LOGGING],[0],[logging disabled])
AC_DEFINE([CVPI_NO_LOGGING],[1], [logging disabled])
fi

dnl dnl careful error checking
dnl AC_ARG_ENABLE([checking-careful],
dnl 	[AS_HELP_STRING([--enable-checking-careful],
dnl 		[call vgGetError() after every OpenVG function call @<:@default: no@:>@])],
dnl 	[checking_careful=${enableval}], [checking_careful=no])

dnl if test "x${checking_careful}" = xyes; then
dnl    AC_DEFINE([CVPI_CAREFUL],[1], [Careful error checking enabled])
dnl else
dnl    AC_DEFINE([CVPI_CAREFUL],[0], [Careful error checking disabled])
dnl fi

dnl dnl debugging
dnl AC_ARG_ENABLE([debug-vg],
dnl 	[AS_HELP_STRING([--enable-debug-vg],
dnl 		[Copy OpenVG objects to cpu memory, for use with a debugger @<:@default: no@:>@])],
dnl 	[debug_vg=${enableval}], [debug_vg=no])

dnl if test "x${debug_vg}" = xyes; then
dnl    AC_DEFINE([CVPI_DEBUG],[1], [CVPI OpenVG debugging support enabled])
dnl else
dnl    AC_DEFINE([CVPI_DEBUG],[0], [CVPI OpenVG debugging support disabled])
dnl fi

AC_CHECK_PROG(CSC, csc, YES)
AC_CHECK_PROG(CHICKEN, chicken, YES)

CHICKENFLAGS="-debug-level 0"

dnl there should only be one
AC_CONFIG_FILES([Makefile
		src/Makefile
		include/Makefile
		])
dnl		bindings/Makefile
dnl		tests/Makefile
dnl		examples/Makefile
AC_OUTPUT
