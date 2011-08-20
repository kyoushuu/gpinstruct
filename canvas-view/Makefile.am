## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS) \
	$(LIBXML_CFLAGS)

lib_LTLIBRARIES =  \
	libcanvas-view.la

libcanvas_view_la_SOURCES =  \
	canvas-view.c \
	canvas-view.h \
	canvas-view-private.h \
	canvas-lesson-directions-page.c \
	canvas-lesson-directions-page.h \
	canvas-lesson-discussion-page.c \
	canvas-lesson-discussion-page.h \
	canvas-lesson-reading-page.c \
	canvas-lesson-reading-page.h \
	canvas-lesson-score.c \
	canvas-lesson-score.h \
	canvas-lesson-score-page.c \
	canvas-lesson-score-page.h \
	canvas-lesson-test-multi-choice-page.c \
	canvas-lesson-test-multi-choice-page.h \
	canvas-lesson-test-order-page.c \
	canvas-lesson-test-order-page.h \
	canvas-lesson-test-word-pool-page.c \
	canvas-lesson-test-word-pool-page.h \
	canvas-lesson-view.c \
	canvas-lesson-view.h \
	canvas-lesson-view-page.c \
	canvas-lesson-view-page.h \
	canvas-log.h \
	canvas-log.c \
	canvas-message-pool.c \
	canvas-message-pool.h \
	canvas-project-view.c \
	canvas-project-view.h \
	gtktextbuffermarkup.c \
	gtktextbuffermarkup.h

libcanvas_view_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Canvas-View\" \
	$(AM_CPPFLAGS)

libcanvas_view_la_LDFLAGS = \
	-no-undefined

libcanvas_view_la_LIBADD = $(GTK_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/canvas/libcanvas.la

bin_PROGRAMS = canvas-view

canvas_view_SOURCES = \
	main.c

canvas_view_LDADD = $(GTK_LIBS) \
	$(top_builddir)/canvas/libcanvas.la \
	libcanvas-view.la

