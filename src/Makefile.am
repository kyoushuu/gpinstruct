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

bin_PROGRAMS = canvas \
	canvas-editor

canvas_SOURCES = \
	main.c \
	canvas-object.h \
	canvas-object.c \
	canvas-marshalers.h \
	canvas-marshalers.c \
	canvas.h \
	canvas-project.h \
	canvas-project.c \
	canvas-category.h \
	canvas-category.c \
	canvas-lesson.h \
	canvas-lesson.c \
	canvas-lesson-element.h \
	canvas-lesson-element.c \
	canvas-parser.h \
	canvas-parser.c \
	canvas-lesson-discussion.h \
	canvas-lesson-discussion.c \
	canvas-lesson-test.h \
	canvas-lesson-test.c \
	canvas-lesson-reading.h \
	canvas-lesson-reading.c \
	canvas-lesson-test-multi-choice.h \
	canvas-lesson-test-multi-choice.c \
	canvas-lesson-test-word-pool.h \
	canvas-lesson-test-word-pool.c \
	canvas-lesson-test-multi-choice-question.h \
	canvas-lesson-test-multi-choice-question.c \
	canvas-lesson-test-word-pool-question.h \
	canvas-lesson-test-word-pool-question.c \
	canvas-project-view.h \
	canvas-project-view.c \
	canvas-lesson-view.h \
	canvas-lesson-view.c \
	canvas-lesson-view-page.h \
	canvas-lesson-view-page.c \
	canvas-lesson-discussion-page.h \
	canvas-lesson-discussion-page.c \
	canvas-lesson-reading-page.h \
	canvas-lesson-reading-page.c \
	canvas-lesson-directions-page.h \
	canvas-lesson-directions-page.c \
	canvas-lesson-score.h \
	canvas-lesson-score.c \
	canvas-lesson-score-page.h \
	canvas-lesson-score-page.c \
	canvas-lesson-test-multi-choice-page.h \
	canvas-lesson-test-multi-choice-page.c \
	canvas.c \
	canvas-private.h \
	canvas-lesson-test-word-pool-page.c \
	canvas-lesson-test-word-pool-page.h \
	gtktextbuffermarkup.c \
	gtktextbuffermarkup.h

canvas_LDFLAGS = \
	-Wl,--export-dynamic

canvas_LDADD = $(CANVAS_LIBS)

canvas_editor_SOURCES =  \
	canvas.h \
	canvas-object.h \
	canvas-object.c \
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
	canvas-lesson-test-word-pool.c \
	canvas-lesson-test-word-pool.h \
	canvas-lesson-test-word-pool-question.c \
	canvas-lesson-test-word-pool-question.h \
	canvas-parser.c \
	canvas-parser.h \
	canvas-project.c \
	canvas-project.h \
	canvas-editor.c

canvas_editor_LDADD = $(CANVAS_LIBS)

