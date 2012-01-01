## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS)

noinst_LTLIBRARIES =  \
	libcompat-glib.la \
	libcompat-gtk.la

libcompat_glib_la_SOURCES = \
	compat-glib.h \
	g-list-free-full.h \
	g-list-free-full.c \
	g-slist-free-full.h \
	g-slist-free-full.c

libcompat_glib_la_CPPFLAGS = \
	$(AM_CPPFLAGS)

libcompat_glib_la_LIBADD = \
	$(GLIB_LIBS)

libcompat_gtk_la_SOURCES = \
	compat-gtk.h \
	gtk-switch.h \
	gtk-switch.c \
	gtk-tree-model-iter-previous.h \
	gtk-tree-model-iter-previous.c

libcompat_gtk_la_CPPFLAGS = \
	$(AM_CPPFLAGS)

libcompat_gtk_la_LIBADD = \
	$(GLIB_LIBS) \
	$(GTK_LIBS)

