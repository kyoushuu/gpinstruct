## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS)

lib_LTLIBRARIES =  \
	libcanvas-editor.la

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
	canvas-lesson-test-multi-choice-editor.h \
	canvas-lesson-test-multi-choice-editor.c \
	canvas-lesson-test-word-pool-editor.h \
	canvas-lesson-test-word-pool-editor.c \
	canvas-lesson-test-order-editor.h \
	canvas-lesson-test-order-editor.c \
	canvas-editor-window.h \
	canvas-editor-window.c

libcanvas_editor_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Canvas-Editor\" \
	$(AM_CPPFLAGS)

libcanvas_editor_la_LDFLAGS = -no-undefined

libcanvas_editor_la_LIBADD = $(GTK_LIBS) \
	$(top_builddir)/canvas/libcanvas.la \
	$(top_builddir)/canvas-view/libcanvas-view.la

bin_PROGRAMS = canvas-editor

canvas_editor_SOURCES =  \
	main.c

canvas_editor_LDADD = $(GTK_LIBS) \
	$(top_builddir)/canvas/libcanvas.la \
	$(top_builddir)/canvas-view/libcanvas-view.la \
	libcanvas-editor.la

