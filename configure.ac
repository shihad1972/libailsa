#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libailsa], [0.2.3], [iain@ailsatech.net])
AC_CONFIG_SRCDIR([lib/data.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([include/config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Add m4 macros directory
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR

# Checks for libraries.
LT_INIT

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h fcntl.h stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strchr strstr])

AX_LIB_LDAP
AX_LIB_UUID
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4], [HAVE_CHECK="true"], [HAVE_CHECK="false"])

AM_CONDITIONAL([HAVE_LIBLDAP], [test x"$HAVE_LIBLDAP" = xtrue])
AM_CONDITIONAL([HAVE_CHECK], [test x"$HAVE_CHECK" = xtrue])
AM_CONDITIONAL([HAVE_LIBUUID], [test x"$HAVE_LIBUUID" = "xtrue"])

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 tests/Makefile])
AC_OUTPUT
