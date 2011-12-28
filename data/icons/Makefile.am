public_icons_themes = \
	hicolor	\
	HighContrastLargePrint \
	HighContrastLargePrintInverse \
	LowContrastLargePrint

public_icons = \
	hicolor_apps_16x16_gpinstruct-analyzer.png \
	hicolor_apps_16x16_gpinstruct-client.png \
	hicolor_apps_16x16_gpinstruct-editor.png \
	hicolor_apps_16x16_gpinstruct-server.png \
	hicolor_apps_16x16_gpinstruct-view.png \
	hicolor_apps_22x22_gpinstruct-analyzer.png \
	hicolor_apps_22x22_gpinstruct-client.png \
	hicolor_apps_22x22_gpinstruct-editor.png \
	hicolor_apps_22x22_gpinstruct-server.png \
	hicolor_apps_22x22_gpinstruct-view.png \
	hicolor_apps_24x24_gpinstruct-analyzer.png \
	hicolor_apps_24x24_gpinstruct-client.png \
	hicolor_apps_24x24_gpinstruct-editor.png \
	hicolor_apps_24x24_gpinstruct-server.png \
	hicolor_apps_24x24_gpinstruct-view.png \
	hicolor_apps_32x32_gpinstruct-analyzer.png \
	hicolor_apps_32x32_gpinstruct-client.png \
	hicolor_apps_32x32_gpinstruct-editor.png \
	hicolor_apps_32x32_gpinstruct-server.png \
	hicolor_apps_32x32_gpinstruct-view.png \
	hicolor_apps_48x48_gpinstruct-analyzer.png \
	hicolor_apps_48x48_gpinstruct-client.png \
	hicolor_apps_48x48_gpinstruct-editor.png \
	hicolor_apps_48x48_gpinstruct-server.png \
	hicolor_apps_48x48_gpinstruct-view.png \
	hicolor_apps_64x64_gpinstruct-analyzer.png \
	hicolor_apps_64x64_gpinstruct-client.png \
	hicolor_apps_64x64_gpinstruct-editor.png \
	hicolor_apps_64x64_gpinstruct-server.png \
	hicolor_apps_64x64_gpinstruct-view.png \
	hicolor_apps_128x128_gpinstruct-analyzer.png \
	hicolor_apps_128x128_gpinstruct-client.png \
	hicolor_apps_128x128_gpinstruct-editor.png \
	hicolor_apps_128x128_gpinstruct-server.png \
	hicolor_apps_128x128_gpinstruct-view.png \
	hicolor_apps_scalable_gpinstruct-analyzer.svg \
	hicolor_apps_scalable_gpinstruct-client.svg \
	hicolor_apps_scalable_gpinstruct-editor.svg \
	hicolor_apps_scalable_gpinstruct-server.svg \
	hicolor_apps_scalable_gpinstruct-view.svg

private_icons = \
	gpinstruct-analyzer.svg \
	gpinstruct-client.svg \
	gpinstruct-editor.svg \
	gpinstruct-server.svg \
	gpinstruct-view.svg

gtk_update_icon_cache = gtk-update-icon-cache -f -t

update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		for theme in $(public_icons_themes); do \
			$(gtk_update_icon_cache) $(datadir)/icons/$$theme; \
		done; \
	else \
		echo "*** Icon cache not updated.  After (un)install, run this:"; \
		for theme in $(public_icons_themes); do \
			echo "***   $(gtk_update_icon_cache) $(datadir)/icons/$$theme"; \
		done; \
	fi

install-icons:
	for icon in $(public_icons); do \
		THEME=`echo $$icon | cut -d_ -f1`; \
		CONTEXT=`echo $$icon | cut -d_ -f2`; \
		SIZE=`echo $$icon | cut -d_ -f3`; \
		ICONFILE=`echo $$icon | cut -d_ -f4`; \
		mkdir -p $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT; \
		$(INSTALL_DATA) $(srcdir)/$$icon $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT/$$ICONFILE; \
	done;

uninstall-icons:
	-for icon in $(public_icons); do \
		THEME=`echo $$icon | cut -d_ -f1`; \
		CONTEXT=`echo $$icon | cut -d_ -f2`; \
		SIZE=`echo $$icon | cut -d_ -f3`; \
		ICONFILE=`echo $$icon | cut -d_ -f4`; \
		rm -f $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT/$$ICONFILE; \
	done;

install-data-local: install-icons update-icon-cache

uninstall-local: uninstall-icons update-icon-cache

EXTRA_DIST = \
	$(public_icons) \
	$(private_icons)

