## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = compat gpinstruct gpinstruct-view gpinstruct-editor gpinstruct-analyzer po

dist_doc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = autogen.sh


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
