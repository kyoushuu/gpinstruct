## Process this file with automake to produce Makefile.in


AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(LIBXML_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBSOUP_CFLAGS)

noinst_LTLIBRARIES =  \
	libsoup-soap.la

libsoup_soap_la_SOURCES =  \
	soup-soap.h \
	soup-soap-message.c \
	soup-soap-message.h \
	soup-soap-response.c \
	soup-soap-response.h

libsoup_soap_la_CPPFLAGS = \
	$(AM_CPPFLAGS)

libsoup_soap_la_LIBADD = \
	$(LIBXML_LIBS) \
	$(GLIB_LIBS) \
	$(LIBSOUP_LIBS)
