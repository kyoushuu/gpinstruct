## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_PREFIX=\""$(prefix)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(datadir)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(top_srcdir) \
	$(GOBJECT_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBXML_CFLAGS)

lib_LTLIBRARIES = \
	libgpinstruct.la

libgpinstruct_la_SOURCES = \
	gpinstruct.c \
	gpinstruct.h \
	gpinstruct-category.c \
	gpinstruct-category.h \
	gpinstruct-lesson.c \
	gpinstruct-lesson.h \
	gpinstruct-lesson-discussion.c \
	gpinstruct-lesson-discussion.h \
	gpinstruct-lesson-element.c \
	gpinstruct-lesson-element.h \
	gpinstruct-lesson-element-group.h \
	gpinstruct-lesson-element-group.c \
	gpinstruct-lesson-reading.c \
	gpinstruct-lesson-reading.h \
	gpinstruct-lesson-test.c \
	gpinstruct-lesson-test.h \
	gpinstruct-lesson-test-multi-choice.c \
	gpinstruct-lesson-test-multi-choice.h \
	gpinstruct-lesson-test-multi-choice-question.c \
	gpinstruct-lesson-test-multi-choice-question.h \
	gpinstruct-lesson-test-order.c \
	gpinstruct-lesson-test-order.h \
	gpinstruct-lesson-test-order-item.c \
	gpinstruct-lesson-test-order-item.h \
	gpinstruct-lesson-test-scrambled.c \
	gpinstruct-lesson-test-scrambled.h \
	gpinstruct-lesson-test-scrambled-question.c \
	gpinstruct-lesson-test-scrambled-question.h \
	gpinstruct-lesson-test-text.c \
	gpinstruct-lesson-test-text.h \
	gpinstruct-lesson-test-text-question.c \
	gpinstruct-lesson-test-text-question.h \
	gpinstruct-lesson-test-word-pool.c \
	gpinstruct-lesson-test-word-pool.h \
	gpinstruct-lesson-test-word-pool-question.c \
	gpinstruct-lesson-test-word-pool-question.h \
	gpinstruct-marshalers.c \
	gpinstruct-marshalers.h \
	gpinstruct-marshalers.list \
	gpinstruct-object.c \
	gpinstruct-object.h \
	gpinstruct-parser.c \
	gpinstruct-parser.h \
	gpinstruct-private.h \
	gpinstruct-project.c \
	gpinstruct-project.h

libgpinstruct_la_CPPFLAGS = \
	-DG_LOG_DOMAIN=\"GPInstruct\" \
	$(AM_CPPFLAGS)

libgpinstruct_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_VERSION_INFO)

libgpinstruct_la_LIBADD = \
	$(GOBJECT_LIBS) \
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/compat/libcompat-glib.la


if NATIVE_WIN32
libgpinstruct_la_DEPENDENCIES = libgpinstruct-win32-res.o
libgpinstruct_la_LDFLAGS += -Wl,libgpinstruct-win32-res.o
endif

libgpinstruct-win32-res.o: libgpinstruct.rc
	$(WINDRES) $< $@

EXTRA_DIST = \
	libgpinstruct.rc.in
