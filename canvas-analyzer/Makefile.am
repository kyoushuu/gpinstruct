## Process this file with automake to produce Makefile.in


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS)

lib_LTLIBRARIES =  \
	libcanvas-analyzer.la

libcanvas_analyzer_la_SOURCES =  \
	canvas-analyzer-window.h \
	canvas-analyzer-window.c \
	canvas-analyzer.h \
	canvas-log-analyzer.h \
	canvas-log-analyzer.c \
	canvas-analyzer-project-view.h \
	canvas-analyzer-project-view.c \
	canvas-analyzer-examinee-view.h \
	canvas-analyzer-examinee-view.c

libcanvas_analyzer_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Canvas-Editor\" \
	$(AM_CPPFLAGS)

libcanvas_analyzer_la_LDFLAGS = -no-undefined

libcanvas_analyzer_la_LIBADD = $(GTK_LIBS) \
	$(top_builddir)/canvas/libcanvas.la \
	$(top_builddir)/canvas-view/libcanvas-view.la

bin_PROGRAMS =  \
	canvas-analyzer

canvas_analyzer_SOURCES =  \
	main.c

canvas_analyzer_LDADD = $(GTK_LIBS) \
	$(top_builddir)/canvas/libcanvas.la \
	$(top_builddir)/canvas-view/libcanvas-view.la \
	libcanvas-analyzer.la
