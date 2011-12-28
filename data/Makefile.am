SUBDIRS = icons

desktopdir 	 = $(datadir)/applications
desktop_in_files = \
	gpinstruct-analyzer.desktop.in \
	gpinstruct-client.desktop.in \
	gpinstruct-editor.desktop.in \
	gpinstruct-server.desktop.in \
	gpinstruct-view.desktop.in
desktop_DATA 	 = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = \
	$(desktop_in_files)

CLEANFILES = \
	$(desktop_DATA)

DISTCLEANFILES = \
	$(desktop_DATA)

