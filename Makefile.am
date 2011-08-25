## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = gpinstruct gpinstruct-view gpinstruct-editor gpinstruct-analyzer po

gpinstructdocdir = ${prefix}/doc/gpinstruct
gpinstructdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(gpinstructdoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gpinstructdocdir)
