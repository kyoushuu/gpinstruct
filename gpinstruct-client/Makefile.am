## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_PREFIX=\""$(prefix)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(top_srcdir) \
	$(LIBXML_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS) \
	$(LIBSOUP_CFLAGS) \
	$(LIBSOUP_SOAP_CFLAGS)

lib_LTLIBRARIES = \
	libgpinstruct-client.la

libgpinstruct_client_la_SOURCES = \
	gpinstruct-client.h \
	gpinstruct-client-window.h \
	gpinstruct-client-window.c

libgpinstruct_client_la_CPPFLAGS = -DG_LOG_DOMAIN=\"GPInstruct-Client\" \
	$(AM_CPPFLAGS)

libgpinstruct_client_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_VERSION_INFO)

libgpinstruct_client_la_LIBADD = \
	$(LIBXML_LIBS) \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(LIBSOUP_LIBS) \
	$(LIBSOUP_SOAP_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	$(top_builddir)/compat/libcompat-gtk.la

bin_PROGRAMS =  \
	gpinstruct-client

gpinstruct_client_SOURCES =  \
	main.c

gpinstruct_client_LDADD = \
	$(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	libgpinstruct-client.la
