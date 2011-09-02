## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(CANVAS_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = canvas

canvas_SOURCES = \
	main.c \
	canvas-project.h \
	canvas-project.c \
	canvas-category.h \
	canvas-category.c \
	canvas-lesson.h \
	canvas-lesson.c \
	canvas.h \
	canvas-lesson-element.h \
	canvas-lesson-element.c \
	canvas-parser.h \
	canvas-parser.c

canvas_LDFLAGS = \
	-Wl,--export-dynamic

canvas_LDADD = $(CANVAS_LIBS)

