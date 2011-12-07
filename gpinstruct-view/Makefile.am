## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_PREFIX=\""$(prefix)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS) \
	$(LIBXML_CFLAGS)

lib_LTLIBRARIES =  \
	libgpinstruct-view.la

libgpinstruct_view_la_SOURCES =  \
	gpinstruct-view.c \
	gpinstruct-view.h \
	gpinstruct-view-private.h \
	gpinstruct-lesson-directions-page.c \
	gpinstruct-lesson-directions-page.h \
	gpinstruct-lesson-discussion-page.c \
	gpinstruct-lesson-discussion-page.h \
	gpinstruct-lesson-reading-page.c \
	gpinstruct-lesson-reading-page.h \
	gpinstruct-lesson-score.c \
	gpinstruct-lesson-score.h \
	gpinstruct-lesson-score-page.c \
	gpinstruct-lesson-score-page.h \
	gpinstruct-lesson-test-multi-choice-page.c \
	gpinstruct-lesson-test-multi-choice-page.h \
	gpinstruct-lesson-test-order-page.c \
	gpinstruct-lesson-test-order-page.h \
	gpinstruct-lesson-test-scrambled-page.c \
	gpinstruct-lesson-test-scrambled-page.h \
	gpinstruct-lesson-test-text-page.c \
	gpinstruct-lesson-test-text-page.h \
	gpinstruct-lesson-test-word-pool-page.c \
	gpinstruct-lesson-test-word-pool-page.h \
	gpinstruct-lesson-view.c \
	gpinstruct-lesson-view.h \
	gpinstruct-lesson-view-page.c \
	gpinstruct-lesson-view-page.h \
	gpinstruct-log.h \
	gpinstruct-log.c \
	gpinstruct-message-pool.c \
	gpinstruct-message-pool.h \
	gpinstruct-project-view.c \
	gpinstruct-project-view.h \
	gtktextbuffermarkup.c \
	gtktextbuffermarkup.h

libgpinstruct_view_la_CPPFLAGS = -DG_LOG_DOMAIN=\"GPInstruct-View\" \
	$(AM_CPPFLAGS)

libgpinstruct_view_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_VERSION_INFO)

libgpinstruct_view_la_LIBADD = $(GTK_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/compat/libcompat-glib.la \
	$(top_builddir)/compat/libcompat-gtk.la

bin_PROGRAMS = gpinstruct-view

gpinstruct_view_SOURCES = \
	main.c

gpinstruct_view_LDADD = $(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	libgpinstruct-view.la

if HAVE_GSTREAMER
AM_CPPFLAGS += $(GSTREAMER_CFLAGS)
libgpinstruct_view_la_LIBADD += $(GSTREAMER_LIBS)
endif

