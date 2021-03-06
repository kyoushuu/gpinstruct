/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja <kyoushuu@yahoo.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-analyzer/gpinstruct-analyzer.h"


GtkWidget *window;

int
main (int argc,
      char *argv[])
{
#ifdef G_OS_WIN32
	gchar *prefix = g_win32_get_package_installation_directory_of_module (NULL);
#endif

#ifdef ENABLE_NLS
#ifdef G_OS_WIN32
	gchar *localedir = g_build_filename (prefix, "share", "locale", NULL);
#else
	gchar *localedir = g_strdup (PACKAGE_LOCALE_DIR);
#endif
	bindtextdomain (GETTEXT_PACKAGE, localedir);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	g_free (localedir);
#endif


	gtk_init (&argc, &argv);


	window = gpinstruct_analyzer_window_new ();


	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_show_all (window);

	if (argc > 1)
	{
		gpinstruct_analyzer_window_new_session (GPINSTRUCT_ANALYZER_WINDOW (window),
		                                        argv[1]);
		if (argc > 2)
		{
			int i;

			for (i = 2; i < argc; i++)
				gpinstruct_analyzer_window_add_log_file (GPINSTRUCT_ANALYZER_WINDOW (window),
				                                         argv[i]);
		}
	}

	gtk_main ();

#ifdef G_OS_WIN32
	g_free (prefix);
#endif

	return 0;
}
