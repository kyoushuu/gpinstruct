## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_PREFIX=\""$(prefix)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS)

lib_LTLIBRARIES = \
	libgpinstruct-editor.la

libgpinstruct_editor_la_SOURCES = \
	gpinstruct-editor.h \
	gpinstruct-object-editor.h \
	gpinstruct-object-editor.c \
	gpinstruct-project-editor.h \
	gpinstruct-project-editor.c \
	gpinstruct-category-editor.h \
	gpinstruct-category-editor.c \
	gpinstruct-lesson-editor.h \
	gpinstruct-lesson-editor.c \
	gpinstruct-lesson-element-group-editor.h \
	gpinstruct-lesson-element-group-editor.c \
	gpinstruct-lesson-discussion-editor.h \
	gpinstruct-lesson-discussion-editor.c \
	gpinstruct-lesson-reading-editor.h \
	gpinstruct-lesson-reading-editor.c \
	gpinstruct-lesson-test-multi-choice-editor.h \
	gpinstruct-lesson-test-multi-choice-editor.c \
	gpinstruct-lesson-test-word-pool-editor.h \
	gpinstruct-lesson-test-word-pool-editor.c \
	gpinstruct-lesson-test-order-editor.h \
	gpinstruct-lesson-test-order-editor.c \
	gpinstruct-lesson-test-text-editor.h \
	gpinstruct-lesson-test-text-editor.c \
	gpinstruct-lesson-test-scrambled-editor.h \
	gpinstruct-lesson-test-scrambled-editor.c \
	gpinstruct-editor-window.h \
	gpinstruct-editor-window.c

libgpinstruct_editor_la_CPPFLAGS = \
	-DG_LOG_DOMAIN=\"GPInstruct-Editor\" \
	$(AM_CPPFLAGS)

libgpinstruct_editor_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_VERSION_INFO)

libgpinstruct_editor_la_LIBADD = \
	$(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	$(top_builddir)/compat/libcompat-gtk.la

bin_PROGRAMS = \
	gpinstruct-editor

gpinstruct_editor_SOURCES = \
	main.c

gpinstruct_editor_LDADD = \
	$(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	libgpinstruct-editor.la


if NATIVE_WIN32
gpinstruct_editor_LDFLAGS = -mwindows
gpinstruct_editor_LDADD += gpinstruct-editor-win32-res.o
libgpinstruct_editor_la_DEPENDENCIES = libgpinstruct-editor-win32-res.o
libgpinstruct_editor_la_LDFLAGS += -Wl,libgpinstruct-editor-win32-res.o
endif

gpinstruct-editor-win32-res.o: gpinstruct-editor.rc
	$(WINDRES) $< $@

libgpinstruct-editor-win32-res.o: libgpinstruct-editor.rc
	$(WINDRES) $< $@

EXTRA_DIST = \
	gpinstruct-editor.rc.in \
	libgpinstruct-editor.rc.in
