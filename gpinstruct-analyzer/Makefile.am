## Process this file with automake to produce Makefile.in


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_srcdir) \
	$(GTK_CFLAGS)

lib_LTLIBRARIES =  \
	libgpinstruct-analyzer.la

libgpinstruct_analyzer_la_SOURCES =  \
	gpinstruct-analyzer-window.h \
	gpinstruct-analyzer-window.c \
	gpinstruct-analyzer.h \
	gpinstruct-log-analyzer.h \
	gpinstruct-log-analyzer.c \
	gpinstruct-analyzer-project-view.h \
	gpinstruct-analyzer-project-view.c \
	gpinstruct-analyzer-examinee-view.h \
	gpinstruct-analyzer-examinee-view.c

libgpinstruct_analyzer_la_CPPFLAGS = -DG_LOG_DOMAIN=\"GPInstruct-Editor\" \
	$(AM_CPPFLAGS)

libgpinstruct_analyzer_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_VERSION_INFO)

libgpinstruct_analyzer_la_LIBADD = $(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	$(top_builddir)/compat/libcompat-glib.la \
	$(top_builddir)/compat/libcompat-gtk.la

bin_PROGRAMS =  \
	gpinstruct-analyzer

gpinstruct_analyzer_SOURCES =  \
	main.c

gpinstruct_analyzer_LDADD = $(GTK_LIBS) \
	$(top_builddir)/gpinstruct/libgpinstruct.la \
	$(top_builddir)/gpinstruct-view/libgpinstruct-view.la \
	libgpinstruct-analyzer.la
