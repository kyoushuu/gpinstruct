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
	                                          "comments", _("GPInstruct Project Analyzer"),
	                                          "website", PACKAGE_URL,
	                                          "copyright", _("Copyright (c) 2011  Arnel A. Borja"),
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
		{"file-close", GTK_STOCK_CLOSE, NULL, "<Control>W", NULL, G_CALLBACK (file_close_action)},
		{"file-quit", GTK_STOCK_QUIT, NULL, "<Control>Q", NULL, G_CALLBACK (file_quit_action)},
		{"edit", NULL, _("_Edit")},
		{"edit-preferences", GTK_STOCK_PREFERENCES, NULL, NULL, NULL, G_CALLBACK (edit_preferences_action)},
		{"help", NULL, _("_Help")},
		{"help-about", GTK_STOCK_ABOUT, NULL, "F1", NULL, G_CALLBACK (help_about_action)}
	};

	gchar *ui =
		"<ui>"
		"  <menubar name=\"menubar\">"
		"    <menu name=\"FileMenu\" action=\"file\">"
		"      <menuitem name=\"New\" action=\"file-new\" />"
		"      <menuitem name=\"Add Log File\" action=\"file-add\" />"
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
		"      <separator/>"
		"    </placeholder>"
		"  </toolbar>"
		"</ui>";

	gtk_window_set_title (GTK_WINDOW (object), _("GPInstruct Analyzer"));
	gtk_window_set_default_size (GTK_WINDOW (object), 800, 600);

	priv->main_vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (object), priv->main_vbox);


	priv->manager = gtk_ui_manager_new ();
	gtk_window_add_accel_group (GTK_WINDOW (object), gtk_ui_manager_get_accel_group (priv->manager));

	priv->action_group = gtk_action_group_new ("gpinstruct-analyzer-window");
	gtk_action_group_add_actions (priv->action_group, actions, G_N_ELEMENTS (actions), object);
	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-add"),
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

	GtkWidget *view_hbox = gtk_hbox_new (FALSE, 3);
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

	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-add"),
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

	gtk_widget_destroy (priv->view);

	g_object_unref (priv->analyzer);

	priv->analyzer = NULL;

	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-add"),
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
