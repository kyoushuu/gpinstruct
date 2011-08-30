dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(gpinstruct, 0.1)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([ChangeLog])

AM_INIT_AUTOMAKE([-Wall -Werror gnu])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE

LT_INIT([disable-static win32-dll])

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
gpinstruct/Makefile
gpinstruct-view/Makefile
gpinstruct-editor/Makefile
gpinstruct-analyzer/Makefile
po/Makefile.in])
