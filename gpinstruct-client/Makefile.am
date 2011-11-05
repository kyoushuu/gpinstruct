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
	$(LIBSOUP_CFLAGS)

bin_PROGRAMS =  \
	gpinstruct-client

gpinstruct_client_SOURCES =  \
	main.c

gpinstruct_client_LDADD = \
	$(LIBXML_LIBS) \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(LIBSOUP_LIBS) \
	$(top_builddir)/libsoup-soap/libsoup-soap.la \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	$(top_builddir)/compat/libcompat-gtk.la
