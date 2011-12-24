## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_PREFIX=\""$(prefix)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(LIBSOUP_CFLAGS) \
	$(LIBSOUP_SOAP_CFLAGS)

lib_LTLIBRARIES =  \
	libgpinstruct-server.la

libgpinstruct_server_la_SOURCES = \
gpinstruct-server-window.h \
	gpinstruct-server-window.c \
	gpinstruct-server.h \
	gpinstruct-server-session.h \
	gpinstruct-server-session.c
libgpinstruct_server_la_CPPFLAGS = -DG_LOG_DOMAIN=\"GPInstruct-Server\" \
	$(AM_CPPFLAGS)

libgpinstruct_server_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_VERSION_INFO)

libgpinstruct_server_la_LIBADD = $(GTK_LIBS) \
	$(LIBXML_LIBS) \
	$(LIBSOUP_LIBS) \
	$(LIBSOUP_SOAP_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/compat/libcompat-glib.la \
	$(top_builddir)/compat/libcompat-gtk.la

bin_PROGRAMS = gpinstruct-server

gpinstruct_server_SOURCES = \
	main.c

gpinstruct_server_LDADD = $(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	libgpinstruct-server.la

