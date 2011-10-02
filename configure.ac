dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.


# Making releases:
#   GPINSTRUCT_MICRO_VERSION += 1;
#   GPINSTRUCT_INTERFACE_AGE += 1;
#   GPINSTRUCT_BINARY_AGE += 1;
# if any functions have been added, set GPINSTRUCT_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set GPINSTRUCT_BINARY_AGE and GPINSTRUCT_INTERFACE_AGE to 0.

m4_define([gpinstruct_major_version], [0])
m4_define([gpinstruct_minor_version], [1])
m4_define([gpinstruct_micro_version], [0])
m4_define([gpinstruct_interface_age], [0])
m4_define([gpinstruct_binary_age],
          [m4_eval(100 * gpinstruct_minor_version + gpinstruct_micro_version)])
m4_define([gpinstruct_version],
          [gpinstruct_major_version.gpinstruct_minor_version.gpinstruct_micro_version])
# This is the X.Y used in -lgpinstruct-FOO-X.Y
m4_define([gpinstruct_api_version], [0.0])

# Define a string for the earliest version that this release has
# backwards binary compatibility with for all interfaces a module
# might. Unless we add module-only API with lower stability
# guarantees, this should be unchanged until we break binary compat
# for GPInstruct.
#
#GPINSTRUCT_BINARY_VERSION=$GPINSTRUCT_MAJOR_VERSION.$GPINSTRUCT_MINOR_VERSION.$LT_CURRENT
m4_define([gpinstruct_binary_version], [0.0.0])

AC_INIT([GPInstruct], [gpinstruct_version],
		[kyoushuu@yahoo.com],
		[gpinstruct],
		[http://gpinstruct.sourceforge.net/])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([ChangeLog])

GPINSTRUCT_MAJOR_VERSION=gpinstruct_major_version
GPINSTRUCT_MINOR_VERSION=gpinstruct_minor_version
GPINSTRUCT_MICRO_VERSION=gpinstruct_micro_version
GPINSTRUCT_INTERFACE_AGE=gpinstruct_interface_age
GPINSTRUCT_BINARY_AGE=gpinstruct_binary_age
GPINSTRUCT_VERSION=gpinstruct_version
GPINSTRUCT_API_VERSION=gpinstruct_api_version
GPINSTRUCT_BINARY_VERSION=gpinstruct_binary_version
AC_SUBST(GPINSTRUCT_MAJOR_VERSION)
AC_SUBST(GPINSTRUCT_MINOR_VERSION)
AC_SUBST(GPINSTRUCT_MICRO_VERSION)
AC_SUBST(GPINSTRUCT_INTERFACE_AGE)
AC_SUBST(GPINSTRUCT_BINARY_AGE)
AC_SUBST(GPINSTRUCT_API_VERSION)
AC_SUBST(GPINSTRUCT_VERSION)
AC_SUBST(GPINSTRUCT_BINARY_VERSION)

# libtool versioning
m4_define([lt_current], [m4_eval(100 * gpinstruct_minor_version + gpinstruct_micro_version - gpinstruct_interface_age)])
m4_define([lt_revision], [gpinstruct_interface_age])
m4_define([lt_age], [m4_eval(gpinstruct_binary_age - gpinstruct_interface_age)])
LT_VERSION_INFO="lt_current:lt_revision:lt_age"
LT_CURRENT_MINUS_AGE=m4_eval(lt_current - lt_age)
AC_SUBST(LT_VERSION_INFO)
AC_SUBST(LT_CURRENT_MINUS_AGE)


AM_INIT_AUTOMAKE([-Wall -Werror gnu no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE


LT_INIT([disable-static win32-dll])
LT_OUTPUT


AC_PROG_CC


dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])


##################################################
# Check for gtk-doc.
##################################################

GTK_DOC_CHECK([1.11],[--flavour no-tmpl])


PKG_CHECK_MODULES(LIBXML,libxml-2.0)

PKG_CHECK_MODULES(GOBJECT,gobject-2.0)

PKG_CHECK_MODULES(GIO,gio-2.0)

PKG_CHECK_MODULES(GLIB,glib-2.0)

PKG_CHECK_MODULES(GTK,gtk+-3.0,,[PKG_CHECK_MODULES(GTK,gtk+-2.0)])


AC_OUTPUT([
Makefile
compat/Makefile
gpinstruct/Makefile
gpinstruct-view/Makefile
gpinstruct-editor/Makefile
gpinstruct-analyzer/Makefile
po/Makefile.in])
