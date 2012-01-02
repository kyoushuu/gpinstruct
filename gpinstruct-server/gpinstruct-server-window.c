/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-server/gpinstruct-server.h"


struct _GPInstructServerWindowPrivate
{
	GtkWidget *main_vbox;

	GtkUIManager *manager;
	GtkActionGroup *action_group;


	GtkWidget *title_entry;
	GtkWidget *port_entry;

	GtkWidget *tree_view;
	GtkTreeStore *store;


	GtkWidget *statusbar;


	GPInstructServerSession *session;
};

#define GPINSTRUCT_SERVER_WINDOW_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_SERVER_WINDOW, GPInstructServerWindowPrivate))


enum
{
	SERVER_LAST_NAME_COLUMN,
	SERVER_FIRST_NAME_COLUMN,
	SERVER_STATUS_COLUMN,
	SERVER_N_COLUMNS
};


enum
{
	SERVER_STATUS_CONNECTED,
	SERVER_STATUS_DISCONNECTED
};


static void
file_new_action (GtkAction *action,
                 gpointer   user_data)
{
	GPInstructServerWindow *window = GPINSTRUCT_SERVER_WINDOW (user_data);

	gpinstruct_server_window_new_session (window);
}


static void
file_close_action (GtkAction *action,
                   gpointer   user_data)
{
	GPInstructServerWindow *window = GPINSTRUCT_SERVER_WINDOW (user_data);

	gpinstruct_server_window_close_session (window);
}


static void
file_quit_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructServerWindow *window = GPINSTRUCT_SERVER_WINDOW (user_data);

	if (gpinstruct_server_window_quit (window))
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
	GPInstructServerWindow *window = GPINSTRUCT_SERVER_WINDOW (user_data);

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
		                  "program-name", _("GPInstruct Server"),
		                  "version", PACKAGE_VERSION,
		                  "title", _("About GPInstruct Server"),
		                  "comments", _("Serve GPInstruct projects through a local network"),
		                  "website", PACKAGE_URL,
		                  "copyright", _("Copyright (c) 2011  Arnel A. Borja"),
#if GTK_MAJOR_VERSION >= 3
		                  "license-type", GTK_LICENSE_GPL_3_0,
#endif
		                  "license", license,
		                  "wrap-license", TRUE,
		                  "authors", authors,
		                  "logo-icon-name", "gpinstruct-server",
		                  NULL);
}


G_DEFINE_TYPE (GPInstructServerWindow, gpinstruct_server_window, GTK_TYPE_WINDOW);


static void
gpinstruct_server_window_init (GPInstructServerWindow *object)
{
	object->priv = GPINSTRUCT_SERVER_WINDOW_GET_PRIVATE (object);
	GPInstructServerWindowPrivate *priv = object->priv;

	g_object_set (object, "icon-name", "gpinstruct-view", NULL);

	GError *error = NULL;

	GtkActionEntry actions[] =
	{
		{"file", NULL, _("_File")},
		{"file-new", GTK_STOCK_NEW, NULL, "<Control>N", NULL, G_CALLBACK (file_new_action)},
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
		"      <toolitem name=\"Close\" action=\"file-close\" />"
		"      <separator/>"
		"    </placeholder>"
		"  </toolbar>"
		"</ui>";


	gtk_window_set_title (GTK_WINDOW (object), _("GPInstruct Server"));
	gtk_window_set_default_size (GTK_WINDOW (object), 800, 600);

	priv->main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER (object), priv->main_vbox);


	priv->manager = gtk_ui_manager_new ();
	gtk_window_add_accel_group (GTK_WINDOW (object), gtk_ui_manager_get_accel_group (priv->manager));

	priv->action_group = gtk_action_group_new ("gpinstruct-server-window");
	gtk_action_group_add_actions (priv->action_group, actions, G_N_ELEMENTS (actions), object);
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


	GtkWidget *info_frame = gtk_frame_new (_("Project Information"));
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), info_frame,
	                    FALSE, TRUE, 0);

	GtkWidget *info_table = gtk_table_new (2, 2, FALSE);
	gtk_container_add (GTK_CONTAINER (info_frame), info_table);

	GtkWidget *title_label = gtk_label_new (_("Title:"));
	gtk_table_attach (GTK_TABLE (info_table), title_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  0, 0);
	priv->title_entry = gtk_entry_new ();
	gtk_editable_set_editable (GTK_EDITABLE (priv->title_entry), FALSE);
	gtk_table_attach (GTK_TABLE (info_table), priv->title_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  0, 0);

	GtkWidget *port_label = gtk_label_new (_("Port:"));
	gtk_table_attach (GTK_TABLE (info_table), port_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  0, 0);
	priv->port_entry = gtk_entry_new ();
	gtk_editable_set_editable (GTK_EDITABLE (priv->port_entry), FALSE);
	gtk_table_attach (GTK_TABLE (info_table), priv->port_entry,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  0, 0);

	priv->store = gtk_tree_store_new (SERVER_N_COLUMNS,
	                                          G_TYPE_STRING,
	                                          G_TYPE_STRING,
	                                          G_TYPE_UINT);

	priv->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->store));
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), priv->tree_view, FALSE, TRUE, 0);


	priv->statusbar = gtk_statusbar_new ();
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), priv->statusbar, FALSE, TRUE, 0);
}

static void
gpinstruct_server_window_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gpinstruct_server_window_parent_class)->finalize (object);
}

static void
gpinstruct_server_window_class_init (GPInstructServerWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass *parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructServerWindowPrivate));

	object_class->finalize = gpinstruct_server_window_finalize;
}


GtkWidget *
gpinstruct_server_window_new (void)
{
	return GTK_WIDGET (g_object_new (GPINSTRUCT_TYPE_SERVER_WINDOW, NULL));
}

void
gpinstruct_server_window_new_session (GPInstructServerWindow *window)
{
	GPInstructServerWindowPrivate *priv = window->priv;

	if (priv->session)
	{
		if (gpinstruct_server_window_close_session (window) == FALSE)
			return;
	}

	GtkWidget *dialog = gtk_dialog_new_with_buttons (_("Create New Session"),
	                                                 GTK_WINDOW (window),
	                                                 GTK_DIALOG_MODAL,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                                 NULL);

	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	GtkWidget *content_table = gtk_table_new (3, 2, FALSE);
	gtk_box_pack_start (GTK_BOX (content_area), content_table,
	                    TRUE, TRUE, 0);

	gtk_table_attach (GTK_TABLE (content_table),
	                  gtk_label_new (_("Project File:")),
	                  0, 1, 0, 1,
	                  GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget *project_file_chooser =
		gtk_file_chooser_button_new (_("Choose Project File"),
		                             GTK_FILE_CHOOSER_ACTION_OPEN);

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("GPInstruct project file"));
	gtk_file_filter_add_pattern (filter, "*.gpinstruct-project");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (project_file_chooser),
	                             filter);

	gtk_table_attach (GTK_TABLE (content_table),
	                  project_file_chooser,
	                  1, 2, 0, 1,
	                  GTK_FILL | GTK_EXPAND, GTK_FILL,
	                  0, 0);

	gtk_table_attach (GTK_TABLE (content_table),
	                  gtk_label_new (_("Log Folder:")),
	                  0, 1, 1, 2,
	                  GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget *log_folder_chooser =
		gtk_file_chooser_button_new (_("Choose Log Folder"),
		                             GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

	gtk_table_attach (GTK_TABLE (content_table),
	                  log_folder_chooser,
	                  1, 2, 1, 2,
	                  GTK_FILL | GTK_EXPAND, GTK_FILL,
	                  0, 0);

	gtk_widget_show_all (content_table);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *project_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (project_file_chooser));
		gchar *log_folder = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (log_folder_chooser));

		if (!g_file_test (project_file, G_FILE_TEST_IS_REGULAR) ||
		    !g_file_test (log_folder, G_FILE_TEST_IS_DIR))
			goto error;

		priv->session = gpinstruct_server_session_new (project_file,
		                                                       log_folder);

		gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
		                    gpinstruct_project_get_title (gpinstruct_server_session_get_project (priv->session)));

		gchar *port = g_strdup_printf ("%d",
		                               gpinstruct_server_session_get_port (priv->session));
		gtk_entry_set_text (GTK_ENTRY (priv->port_entry),
		                    port);
		g_free (port);


		gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
			                                                   "file-close"),
			                      TRUE);

		error:
			g_free (project_file);
			g_free (log_folder);
	}

	gtk_widget_destroy (dialog);
}

gboolean
gpinstruct_server_window_close_session (GPInstructServerWindow *window)
{
	GPInstructServerWindowPrivate *priv = window->priv;

	if (priv->session == NULL)
		return FALSE;
	else
	{
		g_object_unref (priv->session);
		priv->session = NULL;
	}

	gtk_action_set_sensitive (gtk_action_group_get_action (priv->action_group,
	                                                       "file-close"),
	                          FALSE);

	return TRUE;
}

gboolean
gpinstruct_server_window_quit (GPInstructServerWindow *window)
{
	GPInstructServerWindowPrivate *priv = window->priv;

	if (priv->session)
	{
		if (gpinstruct_server_window_close_session (window) == FALSE)
			return FALSE;
	}

	return TRUE;
}
