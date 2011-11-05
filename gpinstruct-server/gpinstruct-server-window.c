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
	GtkWidget* main_vbox;

	GtkUIManager* manager;
	GtkActionGroup* action_group;


	GtkWidget* tree_view;
	GtkTreeStore* store;


	GtkWidget* statusbar;

	GPInstructProject* project;
	gchar* project_file;
};

#define GPINSTRUCT_SERVER_WINDOW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_SERVER_WINDOW, GPInstructServerWindowPrivate))


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
	GPInstructServerWindow* window = GPINSTRUCT_SERVER_WINDOW (user_data);

	gpinstruct_server_window_new_session (window);
}


static void
file_close_action (GtkAction *action,
                   gpointer   user_data)
{
	GPInstructServerWindow* window = GPINSTRUCT_SERVER_WINDOW (user_data);

	gpinstruct_server_window_close_session (window);
}


static void
file_quit_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructServerWindow* window = GPINSTRUCT_SERVER_WINDOW (user_data);

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
	GPInstructServerWindow* window = GPINSTRUCT_SERVER_WINDOW (user_data);

	static gchar* authors[] = {"Arnel A. Borja <kyoushuu@yahoo.com>", NULL};
	gchar* license = _(
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
	                                          "comments", _("GPInstruct Project Server"),
	                                          "website", PACKAGE_URL,
	                                          "copyright", _("Copyright (c) 2011  Arnel A. Borja"),
#if GTK_MAJOR_VERSION >= 3
	                                          "license-type", GTK_LICENSE_GPL_3_0,
#endif
	                                          "license", license,
	                                          "wrap-license", TRUE,
	                                          "authors", authors,
	                                          NULL);
                   }


G_DEFINE_TYPE (GPInstructServerWindow, gpinstruct_server_window, GTK_TYPE_WINDOW);


static void
gpinstruct_server_window_init (GPInstructServerWindow *object)
{
	object->priv = GPINSTRUCT_SERVER_WINDOW_PRIVATE (object);

	object->priv->project = NULL;
	object->priv->project_file = NULL;

	GError* error = NULL;

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

	gchar* ui =
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

	object->priv->main_vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (object), object->priv->main_vbox);


	object->priv->manager = gtk_ui_manager_new ();
	gtk_window_add_accel_group (GTK_WINDOW (object), gtk_ui_manager_get_accel_group (object->priv->manager));

	object->priv->action_group = gtk_action_group_new ("gpinstruct-server-window");
	gtk_action_group_add_actions (object->priv->action_group, actions, G_N_ELEMENTS (actions), object);
	gtk_action_set_sensitive (gtk_action_group_get_action (object->priv->action_group,
	                                                       "file-close"),
	                          FALSE);

	gtk_ui_manager_insert_action_group (object->priv->manager, object->priv->action_group, 0);
	gtk_ui_manager_add_ui_from_string (object->priv->manager, ui, -1, &error);

	if (error)
	{
		g_critical ("%s", error->message);
		g_error_free (error);
	}

	GtkWidget* main_menu = gtk_ui_manager_get_widget (object->priv->manager, "/menubar");
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), main_menu, FALSE, TRUE, 0);

	GtkWidget* toolbar = gtk_ui_manager_get_widget (object->priv->manager, "/toolbar");
#if GTK_MAJOR_VERSION >= 3
	gtk_style_context_add_class (gtk_widget_get_style_context (toolbar),
	                             GTK_STYLE_CLASS_PRIMARY_TOOLBAR);
#endif
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), toolbar, FALSE, TRUE, 0);


	object->priv->store = gtk_tree_store_new (SERVER_N_COLUMNS,
	                                          G_TYPE_STRING,
	                                          G_TYPE_STRING,
	                                          G_TYPE_UINT);

	object->priv->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->store));
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), object->priv->tree_view, FALSE, TRUE, 0);


	object->priv->statusbar = gtk_statusbar_new ();
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), object->priv->statusbar, FALSE, TRUE, 0);
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
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructServerWindowPrivate));

	object_class->finalize = gpinstruct_server_window_finalize;
}


GtkWidget*
gpinstruct_server_window_new (void)
{
	return GTK_WIDGET (g_object_new (GPINSTRUCT_TYPE_SERVER_WINDOW, NULL));
}

const gchar*
gpinstruct_server_window_get_filename (GPInstructServerWindow* window)
{
	return window->priv->project_file;
}

static void
gpinstruct_server_window_set_filename (GPInstructServerWindow* window,
                                       const gchar* file)
{
	if (window->priv->project_file)
		g_free (window->priv->project_file);
	window->priv->project_file = g_strdup (file);
}

void
gpinstruct_server_window_new_session (GPInstructServerWindow* window)
{
	if (window->priv->project)
	{
		if (gpinstruct_server_window_close_session (window) == FALSE)
			return;
	}

	window->priv->project = gpinstruct_project_new ();

	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-close"),
	                          TRUE);

	gpinstruct_server_window_set_filename (window, NULL);
}

gboolean
gpinstruct_server_window_close_session (GPInstructServerWindow* window)
{
	if (window->priv->project == NULL)
		return FALSE;

	if (window->priv->project)
		g_object_unref (window->priv->project);
	window->priv->project = NULL;

	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-close"),
	                          FALSE);

	gpinstruct_server_window_set_filename (window, NULL);

	return TRUE;
}

gboolean
gpinstruct_server_window_quit (GPInstructServerWindow* window)
{
	if (window->priv->project)
	{
		if (gpinstruct_server_window_close_session (window) == FALSE)
			return FALSE;
	}

	return TRUE;
}
