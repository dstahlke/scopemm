#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([scopemm], [1.0.0], [dstahlke@gmail.com])
AC_CONFIG_SRCDIR([scopemm/rastercanvas.cc])
AC_CONFIG_HEADERS([config.h])

PACKAGE_RELEASE=1.0
AC_SUBST(PACKAGE_RELEASE)
LIBRARY_VERSION=1:0:0
AC_SUBST(LIBRARY_VERSION)

AM_INIT_AUTOMAKE

# libtoolize told me to add these
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LIBTOOL
AM_SANITY_CHECK
PKG_PROG_PKG_CONFIG

AC_LANG_CPLUSPLUS

PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4 >= 2.10.0])
AC_SUBST(GTKMM_LIBS)
AC_SUBST(GTKMM_CFLAGS)

AC_MSG_CHECKING([for blitz])
PKG_CHECK_EXISTS([blitz], [
	AC_MSG_RESULT([yes])
	PKG_CHECK_MODULES([BLITZ], [blitz])
	AC_SUBST(BLITZ_LIBS)
	AC_SUBST(BLITZ_CFLAGS)
	have_blitz="true"
], [
	AC_MSG_RESULT([no (some demos won't be built)])
	have_blitz="false"
])
AM_CONDITIONAL(HAVE_BLITZ, test "z$have_blitz" = "ztrue")

# hopefully this is the right minimum version, I haven't really tested it
AX_BOOST_BASE([1.34.0], , [AC_MSG_ERROR(boost is required)])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([floor pow sqrt])

AC_CONFIG_FILES([
	scopemm-1.0.pc
	Makefile
	scopemm/Makefile
	demos/Makefile
])
AC_OUTPUT
