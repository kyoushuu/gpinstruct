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

#include "compat/compat-glib.h"
#include "compat/compat-gtk.h"

struct _GPInstructAnalyzerWindowPrivate
{
	GtkWidget *main_vbox;

	GtkUIManager *manager;
	GtkActionGroup *action_group;

	GtkWidget *view_combobox;

	GtkWidget *view;

	GtkWidget *statusbar;

	GPInstructLogAnalyzer *analyzer;
};

#define GPINSTRUCT_ANALYZER_WINDOW_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_ANALYZER_WINDOW, GPInstructAnalyzerWindowPrivate))


static gboolean
window_delete_event (GtkWidget *widget,
                     GdkEvent  *event,
                     gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (widget);

	if (gpinstruct_analyzer_window_quit (window))
		return FALSE;
	else
		return TRUE;
}

static void
view_combobox_changed (GtkComboBox *widget,
                       gpointer     user_data)
{
	gint active = gtk_combo_box_get_active (widget);

	if (active < 0)
		return;

	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	if (priv->analyzer == NULL)
		return;

	if (priv->view)
		gtk_widget_destroy (priv->view);
	priv->view = NULL;

	switch (active)
	{
		case 0:
			priv->view = gpinstruct_analyzer_project_view_new (priv->analyzer);
			gtk_box_pack_start (GTK_BOX (priv->main_vbox), priv->view, TRUE, TRUE, 0);
			gtk_widget_show_all (priv->view);
			break;
		case 1:
			priv->view = gpinstruct_analyzer_examinee_view_new (priv->analyzer);
			gtk_box_pack_start (GTK_BOX (priv->main_vbox), priv->view, TRUE, TRUE, 0);
			gtk_widget_show_all (priv->view);
			break;
	}
}


static void
file_new_action (GtkAction *action,
                 gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);

	GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Choose Project File"),
	                                                 GTK_WINDOW (window),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (dialog), FALSE);

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("GPInstruct project file"));
	gtk_file_filter_add_pattern (filter, "*.gpinstruct-project");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *project_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		gpinstruct_analyzer_window_new_session (window, project_file);
		g_free (project_file);
	}

	gtk_widget_destroy (dialog);
}


static void
file_add_action (GtkAction *action,
                 gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Choose Log Files"),
	                                                 GTK_WINDOW (window),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (dialog), TRUE);

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("GPInstruct log file"));
	gtk_file_filter_add_pattern (filter, "*.gpinstruct-log");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		GSList *log_files = gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (dialog));
		GSList *current_log_files = log_files;

		while (current_log_files)
		{
			gpinstruct_analyzer_window_add_log_file (window, current_log_files->data);

			current_log_files = current_log_files->next;
		}

		g_slist_free_full (log_files, (GDestroyNotify)g_free);
	}
	else
	{
		g_object_unref (priv->analyzer);
		priv->analyzer = NULL;
	}

	gtk_widget_destroy (dialog);
}


static void
file_save_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Save"),
	                                                 GTK_WINDOW (window),
	                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
	gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled.csv");

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("Text CSV"));
	gtk_file_filter_add_pattern (filter, "*.csv");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		GError *error = NULL;
		GFile *file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
		GFileOutputStream *output = g_file_replace (file, NULL, TRUE,
		                                            G_FILE_CREATE_REPLACE_DESTINATION,
		                                            NULL, &error);

		if (!error)
		{
			char *buffer, *buffer2;


			GPInstructProject *project = gpinstruct_log_analyzer_get_project (priv->analyzer)->object;

			GList *categories = gpinstruct_project_get_categories (project);
			GList *current_category = categories;

			while (current_category && !error)
			{
				GPInstructCategory *category = (GPInstructCategory*) current_category->data;

				GList *lessons = gpinstruct_category_get_lessons (category);
				GList *current_lesson = lessons;

				while (current_lesson && !error)
				{
					GPInstructLesson *lesson = (GPInstructLesson*) current_lesson->data;

					GList *elements = gpinstruct_lesson_get_lesson_elements (lesson);
					GList *current_element = elements;

					buffer2 = g_strescape (gpinstruct_lesson_get_title (lesson), NULL);

					buffer = g_strconcat (", \"", buffer2, "\"", NULL);

					g_free (buffer2);

					g_output_stream_write (G_OUTPUT_STREAM (output),
										   buffer, strlen (buffer),
										   NULL, &error);

					g_free (buffer);

					int num = 0;

					while (current_element && !error)
					{
						if (GPINSTRUCT_IS_LESSON_TEST (current_element->data) || GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (current_element->data))
						{
							if (num)
							{
								buffer = ",";

								g_output_stream_write (G_OUTPUT_STREAM (output),
								                       buffer, strlen (buffer),
								                       NULL, &error);
							}

							num++;
						}

						current_element = current_element->next;
					}

					g_list_free (elements);

					current_lesson = current_lesson->next;
				}

				g_list_free (lessons);

				current_category = current_category->next;
			}

			buffer = "\n";

			g_output_stream_write (G_OUTPUT_STREAM (output),
			                       buffer, strlen (buffer),
			                       NULL, &error);


			current_category = categories;

			while (current_category && !error)
			{
				GPInstructCategory *category = (GPInstructCategory*) current_category->data;

				GList *lessons = gpinstruct_category_get_lessons (category);
				GList *current_lesson = lessons;

				while (current_lesson && !error)
				{
					GPInstructLesson *lesson = (GPInstructLesson*) current_lesson->data;

					GList *elements = gpinstruct_lesson_get_lesson_elements (lesson);
					GList *current_element = elements;

					while (current_element && !error)
					{
						if (GPINSTRUCT_IS_LESSON_TEST (current_element->data) || GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (current_element->data))
						{
							buffer2 = g_strescape (gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (current_element->data)), NULL);

							buffer = g_strconcat (", \"", buffer2, "\"", NULL);

							g_free (buffer2);

							g_output_stream_write (G_OUTPUT_STREAM (output),
												   buffer, strlen (buffer),
												   NULL, &error);

							g_free (buffer);
						}

						current_element = current_element->next;
					}

					g_list_free (elements);

					current_lesson = current_lesson->next;
				}

				g_list_free (lessons);

				current_category = current_category->next;
			}

			g_list_free (categories);

			buffer = "\n";

			g_output_stream_write (G_OUTPUT_STREAM (output),
			                       buffer, strlen (buffer),
			                       NULL, &error);


			GList *examinees = gpinstruct_log_analyzer_get_examinees (priv->analyzer);
			GList *current_examinees = examinees;
			guint score;

			while (current_examinees && !error)
			{
				GPInstructLogAnalyzerExaminee *examinee = current_examinees->data;

				buffer = g_strdup_printf ("%s, %s",
				                          examinee->last_name,
				                          examinee->first_name);

				buffer2 = g_strescape (buffer, NULL);

				g_free (buffer);

				buffer = g_strconcat ("\"", buffer2, "\"", NULL);

				g_free (buffer2);

				g_output_stream_write (G_OUTPUT_STREAM (output),
				                       buffer, strlen (buffer),
				                       NULL, &error);

				g_free (buffer);


				GPInstructLogAnalyzerProject *project = examinee->project;

				GList *categories = project->categories;

				while (categories && !error)
				{
					GPInstructLogAnalyzerCategory *category = (GPInstructLogAnalyzerCategory*) categories->data;

					GList *lessons = category->lessons;

					while (lessons && !error)
					{
						GPInstructLogAnalyzerLesson *lesson = (GPInstructLogAnalyzerLesson*) lessons->data;

						GList *elements = lesson->elements;

						while (elements && !error)
						{
							if (((GPInstructLogAnalyzerLessonElement*) elements->data)->is_test)
							{
								score = ((GPInstructLogAnalyzerLessonElement*) elements->data)->test->items_correctly_answered;
							}
							else
							{
								score = ((GPInstructLogAnalyzerLessonElement*) elements->data)->group->items_correctly_answered;
							}

							buffer = g_strdup_printf (", %u", score);

							g_output_stream_write (G_OUTPUT_STREAM (output),
											       buffer, strlen (buffer),
											       NULL, &error);

							g_free (buffer);

							elements = elements->next;
						}

						lessons = lessons->next;
					}

					categories = categories->next;
				}


				buffer = "\n";

				g_output_stream_write (G_OUTPUT_STREAM (output),
				                       buffer, strlen (buffer),
				                       NULL, &error);

				current_examinees = current_examinees->next;
			}

			g_list_free (examinees);
		}

		if (error)
		{
			g_critical (_("Error: %s\n"), error->message);
			g_error_free (error);
			error = NULL;
		}

		if (output)
		{
			g_output_stream_close (G_OUTPUT_STREAM (output),
			                       NULL, &error);
			if (error)
			{
				g_critical (_("Error: %s\n"), error->message);
				g_error_free (error);
			}
		}

		g_object_unref (file);
	}

	gtk_widget_destroy (dialog);
}


static void
file_close_action (GtkAction *action,
                   gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);

	gpinstruct_analyzer_window_close_session (window);
}


static void
file_quit_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);

	if (gpinstruct_analyzer_window_quit (window))
		gtk_widget_destroy (GTK_WIDGET (window));
}


static void
edit_preferences_action (GtkAction *action,
                         gpointer   user_data)
{
}


static void
help_contents_action (GtkAction *action,
                      gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);

	GAppInfo *app_info;
	gchar *uri;

	app_info = g_app_info_get_default_for_uri_scheme ("help");

#ifdef G_OS_WIN32
        gchar *prefix = g_win32_get_package_installation_directory_of_module (NULL);
	gchar *index_filename = g_build_filename (prefix, "share", "help", "C", PACKAGE, "index.html", NULL);
#else
	gchar *index_filename = g_build_filename (PACKAGE_PREFIX, "share", "help", "C", PACKAGE, "index.html", NULL);
#endif

	if (app_info)
	{
		uri = g_strdup ("help:" PACKAGE);
	}
	else if (g_file_test (index_filename, G_FILE_TEST_IS_REGULAR))
	{
		uri = g_filename_to_uri (index_filename, NULL, NULL);
	}
	else
	{
		uri = g_strdup (PACKAGE_URL);
	}

	gtk_show_uri (gtk_widget_get_screen (GTK_WIDGET (window)),
	              uri,
	              GDK_CURRENT_TIME,
	              NULL);

	g_free (uri);
	g_free (index_filename);

	if (app_info)
		g_object_unref (app_info);

#ifdef G_OS_WIN32
	g_free (prefix);
#endif
}


static void
help_about_action (GtkAction *action,
                   gpointer   user_data)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (user_data);

	static gchar *authors[] = {"Arnel A. Borja <kyoushuu@yahoo.com>", NULL};
	gchar *license = _(
	                   "This program is free software; you can redistribute it and/or modify "
	                   "it under the terms of the GNU General Public License as published by "
	                   "the Free Software Foundation; either version 3 of the License, or "
	                   "(at your option) any later version.\n"
	                   "\n"
	                   "This program is distributed in the hope that it will be useful, "
	                   "but WITHOUT ANY WARRANTY; without even the implied warranty of "
	                   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
	                   "GNU General Public License for more details.\n"
	                   "\n"
	                   "You should have received a copy of the GNU General Public License along "
	                   "with this program.  If not, see <http://www.gnu.org/licenses/>.");

	gtk_show_about_dialog (GTK_WINDOW (window),
		                  "program-name", _("GPInstruct Analyzer"),
		                  "version", PACKAGE_VERSION,
		                  "title", _("About GPInstruct Analyzer"),
		                  "comments", _("View and analyze results from GPInstruct Viewer and Server"),
		                  "website", PACKAGE_URL,
		                  "copyright", _("Copyright (c) 2011-2012  Arnel A. Borja"),
#if GTK_MAJOR_VERSION >= 3
		                  "license-type", GTK_LICENSE_GPL_3_0,
#endif
		                  "license", license,
		                  "wrap-license", TRUE,
		                  "authors", authors,
		                  "logo-icon-name", "gpinstruct-analyzer",
		                  NULL);
}



G_DEFINE_TYPE (GPInstructAnalyzerWindow, gpinstruct_analyzer_window, GTK_TYPE_WINDOW);

static void
gpinstruct_analyzer_window_init (GPInstructAnalyzerWindow *object)
{
	object->priv = GPINSTRUCT_ANALYZER_WINDOW_GET_PRIVATE (object);
	GPInstructAnalyzerWindowPrivate *priv = object->priv;

	priv->analyzer = NULL;

	g_object_set (object, "icon-name", "gpinstruct-view", NULL);

	GError *error = NULL;

	GtkActionEntry actions[] =
	{
		{"file", NULL, _("_File")},
		{"file-new", GTK_STOCK_NEW, NULL, "<Control>N", NULL, G_CALLBACK (file_new_action)},
		{"file-add", GTK_STOCK_ADD, _("Add Log File"), "<Control>A", NULL, G_CALLBACK (file_add_action)},
		{"file-save", GTK_STOCK_SAVE, _("Save"), "<Control>S", NULL, G_CALLBACK (file_save_action)},
		{"file-close", GTK_STOCK_CLOSE, NULL, "<Control>W", NULL, G_CALLBACK (file_close_action)},
		{"file-quit", GTK_STOCK_QUIT, NULL, "<Control>Q", NULL, G_CALLBACK (file_quit_action)},
		{"edit", NULL, _("_Edit")},
		{"edit-preferences", GTK_STOCK_PREFERENCES, NULL, NULL, NULL, G_CALLBACK (edit_preferences_action)},
		{"help", NULL, _("_Help")},
		{"help-contents", NULL, _("_Contents"), "F1", NULL, G_CALLBACK (help_contents_action)},
		{"help-about", GTK_STOCK_ABOUT, NULL, NULL, NULL, G_CALLBACK (help_about_action)}
	};

	gchar *ui =
		"<ui>"
		"  <menubar name=\"menubar\">"
		"    <menu name=\"FileMenu\" action=\"file\">"
		"      <menuitem name=\"New\" action=\"file-new\" />"
		"      <menuitem name=\"Add Log File\" action=\"file-add\" />"
		"      <menuitem name=\"Save\" action=\"file-save\" />"
		"      <separator/>"
		"      <menuitem name=\"Close\" action=\"file-close\" />"
		"      <separator/>"
		"      <menuitem name=\"Quit\" action=\"file-quit\" />"
		"      <placeholder name=\"FileMenuAdditions\" />"
		"    </menu>"
		"    <menu name=\"EditMenu\" action=\"edit\">"
		"      <placeholder name=\"EditMenuAdditions\" />"
		"      <separator/>"
		"      <menuitem name=\"Preferences\" action=\"edit-preferences\"/>"
		"    </menu>"
		"    <menu name=\"HelpMenu\" action=\"help\">"
		"      <menuitem name=\"Contents\" action=\"help-contents\"/>"
		"      <separator/>"
		"      <placeholder name=\"HelpMenuAdditions\" />"
		"      <separator/>"
		"      <menuitem name=\"About\" action=\"help-about\"/>"
		"    </menu>"
		"  </menubar>"
		"  <toolbar action=\"toolbar\">"
		"    <placeholder name=\"FileToolItems\">"
		"      <separator/>"
		"      <toolitem name=\"New\" action=\"file-new\" />"
		"      <toolitem name=\"Add Log File\" action=\"file-add\" />"
		"      <toolitem name=\"Save\" action=\"file-save\" />"
		"      <separator/>"
		"    </placeholder>"
		"  </toolbar>"
		"</ui>";

	gtk_window_set_title (GTK_WINDOW (object), _("GPInstruct Analyzer"));
	gtk_window_set_default_size (GTK_WINDOW (object), 800, 600);

	priv->main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER (object), priv->main_vbox);


	priv->manager = gtk_ui_manager_new ();
	gtk_window_add_accel_group (GTK_WINDOW (object), gtk_ui_manager_get_accel_group (priv->manager));

	priv->action_group = gtk_action_group_new ("gpinstruct-analyzer-window");
	gtk_action_group_add_actions (priv->action_group, actions, G_N_ELEMENTS (actions), object);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-add"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-save"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-close"),
	                          FALSE);

	gtk_ui_manager_insert_action_group (priv->manager, priv->action_group, 0);
	gtk_ui_manager_add_ui_from_string (priv->manager, ui, -1, &error);

	if (error)
	{
		g_critical ("%s", error->message);
		g_error_free (error);
	}

	GtkWidget *main_menu = gtk_ui_manager_get_widget (priv->manager, "/menubar");
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), main_menu, FALSE, TRUE, 0);

	GtkWidget *toolbar = gtk_ui_manager_get_widget (priv->manager, "/toolbar");
#if GTK_MAJOR_VERSION >= 3
	gtk_style_context_add_class (gtk_widget_get_style_context (toolbar),
	                             GTK_STYLE_CLASS_PRIMARY_TOOLBAR);
#endif
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), toolbar, FALSE, TRUE, 0);

	GtkWidget *view_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), view_hbox, FALSE, TRUE, 0);

	GtkWidget *view_label = gtk_label_new (_("Select View:"));
	gtk_box_pack_start (GTK_BOX (view_hbox), view_label, FALSE, TRUE, 0);

	GtkTreeIter view_iter;
	GtkListStore *view_store = gtk_list_store_new (1, G_TYPE_STRING);

	gtk_list_store_append (view_store, &view_iter);
	gtk_list_store_set (view_store, &view_iter, 0, "Whole Project", -1);
	gtk_list_store_append (view_store, &view_iter);
	gtk_list_store_set (view_store, &view_iter, 0, "Examinee", -1);

	GtkWidget *view_combobox = gtk_combo_box_new_with_model (GTK_TREE_MODEL (view_store));
	gtk_widget_set_sensitive (view_combobox, FALSE);
	g_signal_connect (view_combobox, "changed", G_CALLBACK (view_combobox_changed), object);
	gtk_box_pack_start (GTK_BOX (view_hbox), view_combobox, TRUE, TRUE, 0);
	priv->view_combobox = view_combobox;

	g_object_unref (view_store);

	GtkCellRenderer *view_textrenderer = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (view_combobox), view_textrenderer, TRUE);
	gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (view_combobox), view_textrenderer, "text", 0);

	priv->view = NULL;

	priv->statusbar = gtk_statusbar_new ();
	gtk_box_pack_end (GTK_BOX (priv->main_vbox), priv->statusbar, FALSE, TRUE, 0);


	g_signal_connect (object, "delete-event", G_CALLBACK (window_delete_event), NULL);
}

static void
gpinstruct_analyzer_window_finalize (GObject *object)
{
	GPInstructAnalyzerWindow *window = GPINSTRUCT_ANALYZER_WINDOW (object);
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	if (priv->action_group)
		g_object_unref (priv->action_group);

	G_OBJECT_CLASS (gpinstruct_analyzer_window_parent_class)->finalize (object);
}

static void
gpinstruct_analyzer_window_class_init (GPInstructAnalyzerWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass *parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructAnalyzerWindowPrivate));

	object_class->finalize = gpinstruct_analyzer_window_finalize;
}


GtkWidget *
gpinstruct_analyzer_window_new (void)
{
	return GTK_WIDGET (g_object_new (GPINSTRUCT_TYPE_ANALYZER_WINDOW, NULL));
}

void
gpinstruct_analyzer_window_new_session (GPInstructAnalyzerWindow *window,
                                        const gchar *file)
{
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	if (priv->analyzer)
	{
		if (gpinstruct_analyzer_window_close_session (window) == FALSE)
			return;
	}

	GError *error = NULL;

	GPInstructParser *parser = gpinstruct_parser_new ();
	GPInstructProject *project = gpinstruct_parser_load_from_file (parser, file, &error);
	g_object_unref (parser);

	if (error)
	{
		g_critical (_("Error: %s\n"), error->message);
		g_error_free (error);
		error = NULL;
		return;
	}

	if (project == NULL)
		return;

	priv->analyzer = gpinstruct_log_analyzer_new (project);

	g_object_unref (project);

	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-add"),
	                          TRUE);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-save"),
	                          TRUE);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-close"),
	                          TRUE);

	gtk_widget_set_sensitive (priv->view_combobox, TRUE);

	gtk_combo_box_set_active (GTK_COMBO_BOX (priv->view_combobox), -1);
	gtk_combo_box_set_active (GTK_COMBO_BOX (priv->view_combobox), 0);
}

gboolean
gpinstruct_analyzer_window_close_session (GPInstructAnalyzerWindow *window)
{
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	if (priv->analyzer == NULL)
		return FALSE;

	if (priv->view)
		gtk_widget_destroy (priv->view);
	priv->view = NULL;

	g_object_unref (priv->analyzer);

	priv->analyzer = NULL;

	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-add"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-save"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-close"),
	                          FALSE);

	gtk_widget_set_sensitive (priv->view_combobox, FALSE);

	return TRUE;
}


gboolean
gpinstruct_analyzer_window_add_log_file (GPInstructAnalyzerWindow *window,
                                         const gchar *file)
{
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	if (priv->analyzer == NULL)
		return FALSE;

	GPInstructLog *log = gpinstruct_log_new ();
	if (gpinstruct_log_load_from_file (log, file, NULL) == FALSE)
	{
		g_object_unref (log);
		return FALSE;
	}

	gpinstruct_log_analyzer_add_log (priv->analyzer, log);

	g_object_unref (log);

	gtk_combo_box_set_active (GTK_COMBO_BOX (priv->view_combobox), -1);
	gtk_combo_box_set_active (GTK_COMBO_BOX (priv->view_combobox), 0);

	return TRUE;
}

gboolean
gpinstruct_analyzer_window_quit (GPInstructAnalyzerWindow *window)
{
	GPInstructAnalyzerWindowPrivate *priv = window->priv;

	if (priv->analyzer)
	{
		if (gpinstruct_analyzer_window_close_session (window) == FALSE)
			return FALSE;
	}

	return TRUE;
}
