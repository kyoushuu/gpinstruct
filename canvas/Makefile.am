## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_srcdir) \
	$(GOBJECT_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBXML_CFLAGS)

lib_LTLIBRARIES =  \
	libcanvas.la

libcanvas_la_SOURCES =  \
	canvas.c \
	canvas.h \
	canvas-category.c \
	canvas-category.h \
	canvas-lesson.c \
	canvas-lesson.h \
	canvas-lesson-discussion.c \
	canvas-lesson-discussion.h \
	canvas-lesson-element.c \
	canvas-lesson-element.h \
	canvas-lesson-reading.c \
	canvas-lesson-reading.h \
	canvas-lesson-test.c \
	canvas-lesson-test.h \
	canvas-lesson-test-multi-choice.c \
	canvas-lesson-test-multi-choice.h \
	canvas-lesson-test-multi-choice-question.c \
	canvas-lesson-test-multi-choice-question.h \
	canvas-lesson-test-order.c \
	canvas-lesson-test-order.h \
	canvas-lesson-test-order-item.c \
	canvas-lesson-test-order-item.h \
	canvas-lesson-test-word-pool.c \
	canvas-lesson-test-word-pool.h \
	canvas-lesson-test-word-pool-question.c \
	canvas-lesson-test-word-pool-question.h \
	canvas-marshalers.c \
	canvas-marshalers.h \
	canvas-marshalers.list \
	canvas-object.c \
	canvas-object.h \
	canvas-parser.c \
	canvas-parser.h \
	canvas-private.h \
	canvas-project.c \
	canvas-project.h

libcanvas_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Canvas\" \
	$(AM_CPPFLAGS)

libcanvas_la_LDFLAGS = \
	-no-undefined

libcanvas_la_LIBADD = $(GOBJECT_LIBS) \
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(LIBXML_LIBS)

