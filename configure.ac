dnl This file is part of CVPI.
dnl 
dnl Copyright (C) 2015
dnl 
dnl This program is free software: you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public License
dnl as published by the Free Software Foundation, either version 3 of
dnl the License, or (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU Lesser General Public License for more details.
dnl 
dnl You should have received a copy of the GNU Lesser General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.61])		dnl min autoconf version
AC_INIT([cvpi],[1.0])		dnl package: cvpi, version: 1.0
AC_CANONICAL_HOST		dnl get the system type

AC_USE_SYSTEM_EXTENSIONS
AC_GNU_SOURCE			dnl enable GNU extensions

AM_INIT_AUTOMAKE
AM_PROG_AR

LT_PREREQ([2.2])		dnl min libtool version
LT_INIT

AC_PROG_CC([gcc cl cc])
dnl c99 features: restrict, variadic macros, inline, compound literals
AC_PROG_CC_C99
AM_PROG_CC_C_O			dnl in case the C compiler cannot take -c and -o simultaneously

AC_PROG_LIBTOOL			dnl turn on processing for 'libtool'
AC_PROG_INSTALL			dnl set the install program
dnl AC_PROG_RANLIB              dnl using libtool instead

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([include/config.h])	dnl generated header file

AC_DEFINE([HAVE_CONFIG_H],[1], [Defining HAVE_CONFIG_H])

dnl test whether c99 is supported
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

raspberry_pi=yes
AC_ARG_ENABLE([raspberry-pi],
	[AS_HELP_STRING([--enable-raspberry-pi],[Force Raspberry-Pi options, work-around @<:@default: yes@:>@])],
	[raspberry_pi=${enableval}], [raspberry_pi=yes])
dnl directory existence testing fails
dnl || (test -d /opt/vc/lib && test -d /opt/vc/include/ && test -d /opt/vc/include/EGL/ && test -d /opt/vc/include/interface/vmcs_host/linux/ && test -d /opt/vc/include/interface/vcos/pthreads/ && test -d /opt/vc/include/VG/)
if test "x${raspberry_pi}"=xyes; then
CVPI_RASPBERRY_CPPFLAGS="-I/opt/vc/include/ -I/opt/vc/include/interface/vmcs_host/linux/ -I/opt/vc/include/interface/vcos/pthreads/"
CVPI_RASPBERRY_LDFLAGS="-L/opt/vc/lib"

dnl checks give false negatives
dnl AC_CHECK_HEADERS(/opt/vc/include/bcm_host.h, [AC_DEFINE([HAVE_BCM_HOST],[1], [Defining HAVE_BCM_HOST = 1])], [AC_MSG_WARN([Unable to find bcm_host.h.])])
dnl AC_CHECK_HEADERS(/opt/vc/include/VG/openvg.h,[],[AC_MSG_WARN([Unable to find openvg.h.])])
dnl AC_CHECK_HEADERS(/opt/vc/include/EGL/eglext_brcm.h,[],[AC_MSG_WARN([Unable to find eglext_brcm.h.])])
dnl AC_CHECK_HEADERS(/opt/vc/include/EGL/egl.h,[],[AC_MSG_WARN([Unable to find egl.h.])])
dnl AC_CHECK_HEADERS(/opt/vc/include/EGL/eglplatform.h,[],[AC_MSG_WARN([Unable to find eglplatform.h.])])

AC_DEFINE([HAVE_BCM_HOST],[1], [Defining HAVE_BCM_HOST = 1])

echo "Creating Raspberry Pi Paths.\n"
else
echo "Not Creating Raspberry Pi Paths.\n"
PKG_CHECK_MODULES(EGL, [egl])
PKG_CHECK_MODULES(VG, [vg])
AC_DEFINE([HAVE_BCM_HOST],[0],[Defining HAVE_BCM_HOST = 0])
fi

AC_SUBST(CVPI_RASPBERRY_CPPFLAGS)
AC_SUBST(CVPI_RASPBERRY_LDFLAGS)

dnl these checks never work on the Raspberry Pi
dnl AC_CHECK_HEADER([openvg.h],,
dnl 	AC_MSG_ERROR([openvg.h not found.]))
dnl AC_CHECK_HEADER([vgu.h],,
dnl 	AC_MSG_ERROR([vgu.h not found.]))
dnl AC_CHECK_HEADER([egl.h],,
dnl 	AC_MSG_ERROR([egl.h not found.]))
dnl AC_CHECK_HEADER([eglplatform.h],,
dnl 	AC_MSG_ERROR([eglplatform.h not found.]))

AC_CHECK_HEADERS([stdlib.h stdio.h errno.h math.h unistd.h stdarg.h])
dnl these checks never work on the Raspberry Pi
dnl AC_CHECK_HEADER([openvg.h])
dnl AC_CHECK_HEADER([vgu.h])
dnl AC_CHECK_HEADER([egl.h])
dnl AC_CHECK_HEADER([eglplatform.h])

dnl logging type options
logging_sync=yes
logging_async=no
logging_none=no
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
dnl if test "x${logging_async}" = xyes; then
dnl have_pthreads=no
dnl AC_SEARCH_LIBS([pthread_create], [pthread], [have_pthreads=yes])

dnl     if test "x${have_pthreads}" = xyes; then
dnl         AC_CHECK_HEADERS([pthread.h], [], [have_pthreads=no])
dnl     fi

dnl     if test "x${have_pthreads}" = xno; then
dnl         AC_MSG_WARN([Unable to find pthreads on this system.])
dnl         async_exec=no
dnl     fi
dnl fi

dnl check logging options
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

if test "x${logging_sync}" = xyes; then
   AC_DEFINE([CVPI_LOGGING],[1],[synchronous logging enabled])

elif test "x${logging_async}" = xyes; then

dnl m4_include([m4/ax_pthread.m4]) 

dnl dnl Check for pthreads
dnl AX_PTHREAD(AC_DEFINE([CVPI_LOGGING],[2], [asynchronous logging enabled]),
dnl AC_MSG_ERROR([POSIX Threads not found, either enable synchronous logging or disable logging.]))

dnl LIBS="$PTHREAD_LIBS $LIBS"
dnl CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
dnl CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
dnl CC="$PTHREAD_CC"
dnl CXX="$PTHREAD_CXX"

AC_DEFINE([CVPI_LOGGING],[0],[logging disabled, async])

else
AC_DEFINE([CVPI_LOGGING],[0],[logging disabled])
fi

dnl careful error checking
checking_careful=no
AC_ARG_ENABLE([checking-careful],
	[AS_HELP_STRING([--enable-checking-careful],
		[call vgGetError() after every OpenVG function call @<:@default: no@:>@])],
	[checking_careful=${enableval}], [checking_careful=no])

if test "x${checking_careful}" = xyes; then
AC_DEFINE([CVPI_CAREFUL],[1], [Careful error checking enabled])
else
AC_DEFINE([CVPI_CAREFUL],[0], [Careful error checking disabled])
fi

dnl inline assembly
inline_assembly=yes
AC_ARG_ENABLE([inline-assembly], [AS_HELP_STRING([--disable-inline-assembly], [Do not use GCC inline assembly. @<:@default: yess@:>@])],
				 [inline_assembly=${enableval}], [inline_assembly=yes])
if test "x${inline_assembly}" = xno; then
AC_DEFINE([CVPI_ASSEMBLY], [0], [Inline assembly disabled.])
else
AC_DEFINE([CVPI_ASSEMBLY], [1], [Inline assembly enabled.])
fi

dnl AC_CHECK_PROG(CSC, csc, YES)
dnl AC_CHECK_PROG(CHICKEN, chicken, YES)

dnl CHICKENFLAGS="-debug-level 0"

dnl there should only be one
AC_CONFIG_FILES([Makefile
		src/Makefile
		include/Makefile
		tests/Makefile
		])
dnl		bindings/Makefile
dnl		examples/Makefile
AC_OUTPUT
