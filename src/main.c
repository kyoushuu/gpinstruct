/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Arnel A. Borja 2011 <galeon@ymail.com>
 * 
 * canvas is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * canvas is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "canvas.h"
#include "canvas-view.h"



int
main (int argc, char *argv[])
{
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	
	gtk_init (&argc, &argv);

	GError* error = NULL;

	gchar* contents;
	g_file_get_contents ("default-project.xml", &contents, NULL, &error);
	if (error)
	{
		printf(_("Error: %s\n"), error->message);
		g_error_free (error);
	}

	CanvasParser* parser = canvas_parser_new ();

	CanvasProject* project = canvas_parser_open (parser, contents, &error);
	if (error)
	{
		printf(_("Error: %s\n"), error->message);
		g_error_free (error);
	}
	g_free (contents);

	canvas_parser_save (parser, project, "default-project-resave.xml", &error);
	if (error)
	{
		printf(_("Error: %s\n"), error->message);
		g_error_free (error);
	}

	CanvasMessagePool* message_pool = canvas_message_pool_new ();
	canvas_message_pool_load_from_file (message_pool, "messages.ini");

	GtkWidget* window = canvas_project_view_new (project, message_pool);

	g_object_unref (message_pool);

	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_show_all (window);

	gtk_main ();

	g_object_unref (project);
	g_object_unref (parser);

	return 0;
}
