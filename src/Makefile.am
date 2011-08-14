## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GOBJECT_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS) \
	$(LIBXML_CFLAGS)

bin_PROGRAMS = canvas \
	canvas-editor

canvas_SOURCES = \
	main.c

canvas_LDADD = $(GTK_LIBS) \
	libcanvas.la \
	libcanvas-view.la

canvas_editor_SOURCES =  \
	canvas-editor.c

canvas_editor_LDADD = $(GTK_LIBS) \
	libcanvas.la \
	libcanvas-view.la \
	libcanvas-editor.la

lib_LTLIBRARIES =  \
	libcanvas.la \
	libcanvas-view.la \
	libcanvas-editor.la

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
	libcanvas.la

libcanvas_editor_la_SOURCES =  \
	canvas-editor.h \
	canvas-object-editor.h \
	canvas-object-editor.c \
	canvas-project-editor.h \
	canvas-project-editor.c \
	canvas-category-editor.h \
	canvas-category-editor.c \
	canvas-lesson-editor.h \
	canvas-lesson-editor.c \
	canvas-lesson-discussion-editor.h \
	canvas-lesson-discussion-editor.c \
	canvas-lesson-reading-editor.h \
	canvas-lesson-reading-editor.c \
	canvas-editor-window.h \
	canvas-editor-window.c

libcanvas_editor_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Canvas-Editor\" \
	$(AM_CPPFLAGS)

libcanvas_editor_la_LDFLAGS = -no-undefined

libcanvas_editor_la_LIBADD = $(GTK_LIBS) \
	libcanvas.la
