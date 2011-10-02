/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011  Arnel A. Borja <kyoushuu@yahoo.com>
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

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"



int
main (int argc,
      char *argv[])
{
#ifdef G_OS_WIN32
	gchar* prefix = g_win32_get_package_installation_directory_of_module (NULL);
	gchar* datadir = g_build_filename (prefix, "share", PACKAGE, NULL);
#else
	gchar* datadir = g_strdup (PACKAGE_DATA_DIR);
#endif

#ifdef ENABLE_NLS
#ifdef G_OS_WIN32
	gchar* localedir = g_build_filename (prefix, "share", "locale", NULL);
#else
	gchar* localedir = g_strdup (PACKAGE_LOCALE_DIR);
#endif
	bindtextdomain (GETTEXT_PACKAGE, localedir);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	g_free (localedir);
#endif


	gtk_init (&argc, &argv);

	GError* error = NULL;

	GPInstructParser* parser = gpinstruct_parser_new ();

	gchar* project_file = g_build_filename (datadir, "default-project.gpinstruct-project", NULL);

	if (g_file_test (project_file, G_FILE_TEST_IS_REGULAR) == FALSE)
	{
		GtkWidget* dialog = gtk_file_chooser_dialog_new (_("Open Project File"),
		                                                 NULL,
		                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
		                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		                                                 GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		                                                 NULL);

		GtkFileFilter *filter = gtk_file_filter_new ();
		gtk_file_filter_set_name (filter, _("GPInstruct project file"));
		gtk_file_filter_add_pattern (filter, "*.gpinstruct-project");
		gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			g_free (project_file);
			project_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		}
		gtk_widget_destroy (dialog);
	}

	GPInstructProject* project = gpinstruct_parser_load_from_file (parser, project_file, &error);
	if (error)
	{
		printf(_("Error: %s\n"), error->message);
		g_error_free (error);
		error = NULL;
	}

	if (project)
	{
		GtkWidget* dialog = gtk_dialog_new_with_buttons (gpinstruct_project_get_title (project),
		                                                 NULL, GTK_DIALOG_MODAL,
		                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
		                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
		                                                 NULL);
		gtk_window_set_default_size (GTK_WINDOW (dialog), 400, -1);

		GtkWidget* dialog_table = gtk_table_new (5, 2, FALSE);
		gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), dialog_table,
		                    FALSE, TRUE, 3);

		GtkWidget* last_name_label = gtk_label_new (_("Last Name:"));
		gtk_table_attach (GTK_TABLE (dialog_table), last_name_label,
		                  0, 1, 0, 1,
		                  GTK_FILL, GTK_FILL,
		                  0, 0);

		GtkWidget* last_name_entry = gtk_entry_new ();
		gtk_table_attach (GTK_TABLE (dialog_table), last_name_entry,
		                  1, 2, 0, 1,
		                  GTK_EXPAND | GTK_FILL, GTK_FILL,
		                  0, 0);

		GtkWidget* first_name_label = gtk_label_new (_("First Name:"));
		gtk_table_attach (GTK_TABLE (dialog_table), first_name_label,
		                  0, 1, 1, 2,
		                  GTK_FILL, GTK_FILL,
		                  0, 0);

		GtkWidget* first_name_entry = gtk_entry_new ();
		gtk_table_attach (GTK_TABLE (dialog_table), first_name_entry,
		                  1, 2, 1, 2,
		                  GTK_EXPAND | GTK_FILL, GTK_FILL,
		                  0, 0);

		GtkWidget* log_dir_label = gtk_label_new (_("Save Log File to:"));
		gtk_table_attach (GTK_TABLE (dialog_table), log_dir_label,
		                  0, 1, 2, 3,
		                  GTK_FILL, GTK_FILL,
		                  0, 0);

		GtkWidget* log_dir_filechooser = gtk_file_chooser_button_new (_("Open Project File"),
		                                                              GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
		gtk_table_attach (GTK_TABLE (dialog_table), log_dir_filechooser,
		                  1, 2, 2, 3,
		                  GTK_EXPAND | GTK_FILL, GTK_FILL,
		                  0, 0);

		gtk_widget_show_all (dialog_table);

		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			gtk_widget_hide (dialog);

			const gchar* last_name = gtk_entry_get_text (GTK_ENTRY (last_name_entry));
			const gchar* first_name = gtk_entry_get_text (GTK_ENTRY (first_name_entry));

			gchar* log_dir = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (log_dir_filechooser));
			gchar* log_filename = g_strdup_printf ("%s, %s - %s.gpinstruct-log", last_name, first_name,
				                                   gpinstruct_project_get_title (project));
			gchar* log_file = g_build_filename (log_dir, log_filename, NULL);
			g_free (log_filename);
			g_free (log_dir);

			GPInstructLog* log = gpinstruct_log_new ();

			if (log)
			{
				gpinstruct_log_set_last_name (log, last_name);
				gpinstruct_log_set_first_name (log, first_name);
			}

			GPInstructMessagePool* message_pool = gpinstruct_message_pool_new ();

			if (message_pool)
			{
				gchar* message_file = g_build_filename (datadir, "messages.ini", NULL);
				gpinstruct_message_pool_load_from_file (message_pool, message_file);
				g_free (message_file);
			}

			GtkWidget* window = gpinstruct_project_view_new (project, message_pool, log);

			g_object_unref (message_pool);

			if (window)
			{
				gtk_window_set_modal (GTK_WINDOW (window), TRUE);

				/* Exit when the window is closed */
				g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

				gtk_widget_show_all (window);
			}

			gtk_main ();

			gpinstruct_log_save_to_file (log, log_file, NULL);

			g_free (log_file);

			g_object_unref (log);
		}

		gtk_widget_destroy (dialog);

		g_object_unref (project);
	}

	g_free (project_file);

	g_object_unref (parser);

#ifdef G_OS_WIN32
	g_free (prefix);
#endif

	g_free (datadir);

	return 0;
}
