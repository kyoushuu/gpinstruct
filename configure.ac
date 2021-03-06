dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.


dnl Making releases:
dnl   GPINSTRUCT_MICRO_VERSION += 1;
dnl   GPINSTRUCT_INTERFACE_AGE += 1;
dnl   GPINSTRUCT_BINARY_AGE += 1;
dnl if any functions have been added, set GPINSTRUCT_INTERFACE_AGE to 0.
dnl if backwards compatibility has been broken,
dnl set GPINSTRUCT_BINARY_AGE and GPINSTRUCT_INTERFACE_AGE to 0.

m4_define([gpinstruct_major_version], [0])
m4_define([gpinstruct_minor_version], [1])
m4_define([gpinstruct_micro_version], [1])
m4_define([gpinstruct_interface_age], [1])
m4_define([gpinstruct_binary_age],
          [m4_eval(100 * gpinstruct_minor_version + gpinstruct_micro_version)])
m4_define([gpinstruct_version],
          [gpinstruct_major_version.gpinstruct_minor_version.gpinstruct_micro_version])
dnl This is the X.Y used in -lgpinstruct-FOO-X.Y
m4_define([gpinstruct_api_version], [0.0])

dnl Define a string for the earliest version that this release has
dnl backwards binary compatibility with for all interfaces a module
dnl might. Unless we add module-only API with lower stability
dnl guarantees, this should be unchanged until we break binary compat
dnl for GPInstruct.
dnl
dnl GPINSTRUCT_BINARY_VERSION=$GPINSTRUCT_MAJOR_VERSION.$GPINSTRUCT_MINOR_VERSION.$LT_CURRENT
m4_define([gpinstruct_binary_version], [0.0.0])

AC_INIT([GPInstruct], [gpinstruct_version],
		[kyoushuu@yahoo.com],
		[gpinstruct],
		[http://gpinstruct.sourceforge.net/])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_MACRO_DIR([m4])

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

dnl libtool versioning
m4_define([lt_current], [m4_eval(100 * gpinstruct_minor_version + gpinstruct_micro_version - gpinstruct_interface_age)])
m4_define([lt_revision], [gpinstruct_interface_age])
m4_define([lt_age], [m4_eval(gpinstruct_binary_age - gpinstruct_interface_age)])
LT_VERSION_INFO="lt_current:lt_revision:lt_age"
LT_CURRENT_MINUS_AGE=m4_eval(lt_current - lt_age)
AC_SUBST(LT_VERSION_INFO)
AC_SUBST(LT_CURRENT_MINUS_AGE)


AM_INIT_AUTOMAKE([-Wall -Werror gnu dist-xz no-dist-gzip])
AM_MAINTAINER_MODE([enable])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


AC_PROG_CC


LT_INIT([disable-static win32-dll])
LT_OUTPUT


dnl ***************************************************************************
dnl Configure options
dnl ***************************************************************************

AC_ARG_WITH([gstreamer],
            [AS_HELP_STRING([--with-gstreamer],
                            [add gstreamer support @<:@default=check@:>@])],
            [],
            [with_gstreamer=check])


dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************

IT_PROG_INTLTOOL([0.40.1])

AM_GNU_GETTEXT_VERSION([0.17])
AM_GNU_GETTEXT([external])

GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])


dnl ***************************************************************************
dnl Check for gtk-doc.
dnl ***************************************************************************

GTK_DOC_CHECK([1.4])


dnl ***************************************************************************
dnl User manual
dnl ***************************************************************************

YELP_HELP_INIT


dnl ***************************************************************************
dnl Check for windows
dnl ***************************************************************************

case $host_os in
  *mingw* | pw32* | cygwin*)
    platform_win32=yes
    ;;
  *)
    platform_win32=no
    ;;
esac
AM_CONDITIONAL(PLATFORM_WIN32, test x"$platform_win32" = "xyes")

case $host_os in
  *mingw*)
    native_win32=yes
    ;;
  *)
    native_win32=no
    ;;
esac
AM_CONDITIONAL(NATIVE_WIN32, test x"$native_win32" = "xyes")

if test "$native_win32" = "yes"; then
  AC_CHECK_TOOL(WINDRES, windres, no)
  if test "$WINDRES" = no; then
    AC_MSG_ERROR([*** Could not find an implementation of windres in your PATH.])
  fi
fi


PKG_CHECK_MODULES(LIBXML,libxml-2.0)

PKG_CHECK_MODULES(GOBJECT,gobject-2.0)

PKG_CHECK_MODULES(GIO,gio-2.0)

PKG_CHECK_MODULES(GLIB,glib-2.0)

PKG_CHECK_MODULES(LIBSOUP,libsoup-2.4)

PKG_CHECK_MODULES(LIBSOUP_SOAP,libsoup-soap-0.1)

PKG_CHECK_MODULES(GTK,gtk+-3.0,,[PKG_CHECK_MODULES(GTK,gtk+-2.0)])

if test "$with_gstreamer" != "no" ; then
	PKG_CHECK_MODULES(GSTREAMER,
		              gstreamer-0.10,
		              [AC_DEFINE(HAVE_GSTREAMER, 1, [Define if gstreamer-0.10 is available]) have_gstreamer=yes],
		              have_gstreamer=no)
	AC_SUBST(GSTREAMER_CFLAGS)
	AC_SUBST(GSTREAMER_LIBS)
	AM_CONDITIONAL(HAVE_GSTREAMER, [test "x$have_gstreamer" = "xyes"])
fi


AC_CONFIG_FILES([
Makefile
compat/Makefile
gpinstruct/Makefile
gpinstruct/libgpinstruct.rc
gpinstruct-view/Makefile
gpinstruct-view/gpinstruct-view.rc
gpinstruct-view/libgpinstruct-view.rc
gpinstruct-editor/Makefile
gpinstruct-editor/gpinstruct-editor.rc
gpinstruct-editor/libgpinstruct-editor.rc
gpinstruct-analyzer/Makefile
gpinstruct-analyzer/gpinstruct-analyzer.rc
gpinstruct-analyzer/libgpinstruct-analyzer.rc
gpinstruct-server/Makefile
gpinstruct-server/gpinstruct-server.rc
gpinstruct-server/libgpinstruct-server.rc
gpinstruct-client/Makefile
gpinstruct-client/gpinstruct-client.rc
gpinstruct-client/libgpinstruct-client.rc
data/gpinstruct-analyzer.desktop.in
data/gpinstruct-client.desktop.in
data/gpinstruct-editor.desktop.in
data/gpinstruct-server.desktop.in
data/gpinstruct-view.desktop.in
data/Makefile
data/icons/Makefile
help/Makefile
po/Makefile.in])
AC_OUTPUT
