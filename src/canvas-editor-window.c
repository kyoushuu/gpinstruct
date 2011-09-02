/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * canvas
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
#include <gdk/gdkkeysyms.h>
#ifndef GDK_KEY_Menu
#define GDK_KEY_Menu GDK_Menu
#endif

#include "canvas.h"
#include "canvas-editor.h"

typedef struct _CanvasEditorWindowPrivate CanvasEditorWindowPrivate;
struct _CanvasEditorWindowPrivate
{
	GtkWidget* main_vbox;

	GtkUIManager* manager;

	GtkWidget* infobar;
	GtkWidget* infobarlabel;


	GtkWidget* hpaned;

	GtkWidget *tree_view;
	GtkTreeStore* store;
	GtkTreeViewColumn *column;

	GtkWidget *scrolled_window;
	GtkWidget* object_editor;


	GtkWidget* popup_menu;

	GtkWidget* popup_new_category_menu_item;
	GtkWidget* popup_new_lesson_menu_item;
	GtkWidget* popup_new_discussion_menu_item;
	GtkWidget* popup_new_reading_menu_item;
	GtkWidget* popup_new_test_multi_choice_menu_item;
	GtkWidget* popup_new_test_word_pool_menu_item;
	GtkWidget* popup_new_test_order_menu_item;


	CanvasParser* parser;
	CanvasProject *project;
	gchar* project_file;
	gboolean modified;

	GtkTreeIter iter_popup;
};

#define CANVAS_EDITOR_WINDOW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_EDITOR_WINDOW, CanvasEditorWindowPrivate))



enum
{
	TITLE_COLUMN,
	DATA_COLUMN,
	N_COLUMNS
};

static void canvas_editor_window_log (const gchar *log_domain,
                                      GLogLevelFlags log_level,
                                      const gchar *message,
                                      gpointer user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	GtkMessageType type = GTK_MESSAGE_INFO;
	if (log_level & G_LOG_LEVEL_CRITICAL)
		type = GTK_MESSAGE_ERROR;
	else if (log_level & G_LOG_LEVEL_WARNING)
		type = GTK_MESSAGE_WARNING;
	else if (log_level & G_LOG_LEVEL_MESSAGE)
		type = GTK_MESSAGE_INFO;
	else if (log_level & G_LOG_LEVEL_INFO)
		type = GTK_MESSAGE_INFO;
	else if (log_level & G_LOG_LEVEL_DEBUG)
		type = GTK_MESSAGE_INFO;

	gtk_info_bar_set_message_type (GTK_INFO_BAR (priv->infobar), type);
	gtk_label_set_text (GTK_LABEL (priv->infobarlabel), message);

	gtk_widget_show (priv->infobarlabel);
	gtk_widget_show (priv->infobar);
}

static void
info_bar_response (GtkInfoBar *info_bar,
                   gint        response_id,
                   gpointer    user_data)
{
	gtk_widget_hide (GTK_WIDGET (info_bar));
}

static gboolean
window_delete_event (GtkWidget *widget,
                     GdkEvent  *event,
                     gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (widget);

	if (canvas_editor_window_quit (window))
		return FALSE;
	else
		return TRUE;
}


static void
tree_view_realized (GtkWidget *widget,
                    gpointer   user_data)
{
	gdk_window_set_events (gtk_widget_get_window (widget),
	                       GDK_KEY_RELEASE_MASK);
}

static gint
tree_view_press_event (GtkWidget *widget,
                       GdkEvent *event,
                       gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	GdkEventButton *event_button = NULL;
	GdkEventKey *event_key;

	GtkTreePath* path = NULL;

	CanvasObject* object_popup;

	gboolean show_popup = FALSE;
	guint button = 0;

	g_return_val_if_fail (widget != NULL, FALSE);
	g_return_val_if_fail (event != NULL, FALSE);

	if (event->type == GDK_BUTTON_PRESS)
	{
		event_button = (GdkEventButton *) event;
		button = event_button->button;
		if (button == 3)
		{
			gtk_tree_view_get_cursor (GTK_TREE_VIEW (priv->tree_view), &path, NULL);
			if (path)
			{
				if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->store),
				                             &priv->iter_popup, path))
					show_popup = TRUE;
				gtk_tree_path_free (path);
			}
		}
	}
	else if (event->type == GDK_KEY_PRESS)
	{
		event_key = (GdkEventKey *) event;
		if (event_key->state == 0 && event_key->keyval == GDK_KEY_Menu)
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->tree_view)),
			                                     NULL, &priv->iter_popup))
			show_popup = TRUE;
	}

	if (show_popup)
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->store), &priv->iter_popup,
		                    DATA_COLUMN, &object_popup,
		                    -1);

		gtk_widget_hide (priv->popup_new_category_menu_item);
		gtk_widget_hide (priv->popup_new_lesson_menu_item);
		gtk_widget_hide (priv->popup_new_discussion_menu_item);
		gtk_widget_hide (priv->popup_new_reading_menu_item);
		gtk_widget_hide (priv->popup_new_test_multi_choice_menu_item);
		gtk_widget_hide (priv->popup_new_test_word_pool_menu_item);
		gtk_widget_hide (priv->popup_new_test_order_menu_item);

		if (CANVAS_IS_PROJECT (object_popup))
		{
			gtk_widget_show (priv->popup_new_category_menu_item);
		}
		else if (CANVAS_IS_CATEGORY (object_popup))
		{
			gtk_widget_show (priv->popup_new_lesson_menu_item);
		}
		else if (CANVAS_IS_LESSON (object_popup))
		{
			gtk_widget_show (priv->popup_new_discussion_menu_item);
			gtk_widget_show (priv->popup_new_reading_menu_item);
			gtk_widget_show (priv->popup_new_test_multi_choice_menu_item);
			gtk_widget_show (priv->popup_new_test_word_pool_menu_item);
			gtk_widget_show (priv->popup_new_test_order_menu_item);
		}
		else
			return FALSE;

		gtk_menu_popup (GTK_MENU (priv->popup_menu), NULL, NULL, NULL, NULL,
		                button, (event_button)? event_button->time : event_key->time);
		return TRUE;
	}

	return FALSE;
}


static void
tree_view_selection_changed (GtkTreeSelection *treeselection,
                             gpointer          user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	GtkTreeIter iter;
	GtkTreeModel *model;
	CanvasObject *object;

	if (priv->object_editor)
	{
		gtk_widget_destroy (priv->object_editor);
		priv->object_editor = NULL;
	}

	if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->tree_view)),
	                                     &model, &iter))
	{
		gtk_tree_model_get (model, &iter,
		                    DATA_COLUMN, &object,
		                    -1);

		if (CANVAS_IS_PROJECT (object))
		{
			priv->object_editor = GTK_WIDGET (canvas_project_editor_new (window,
			                                                             CANVAS_PROJECT (object)));
		}
		else if (CANVAS_IS_CATEGORY (object))
		{
			priv->object_editor = GTK_WIDGET (canvas_category_editor_new (window,
			                                                              CANVAS_CATEGORY (object)));
		}
		else if (CANVAS_IS_LESSON (object))
		{
			priv->object_editor = GTK_WIDGET (canvas_lesson_editor_new (window,
			                                                            CANVAS_LESSON (object)));
		}
		else if (CANVAS_IS_LESSON_DISCUSSION (object))
		{
			priv->object_editor = GTK_WIDGET (canvas_lesson_discussion_editor_new (window,
			                                                                       CANVAS_LESSON_DISCUSSION (object)));
		}
		else if (CANVAS_IS_LESSON_READING (object))
		{
			priv->object_editor = GTK_WIDGET (canvas_lesson_reading_editor_new (window,
			                                                                    CANVAS_LESSON_READING (object)));
		}

		if (priv->object_editor)
		{
			gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (priv->scrolled_window),
			                                       priv->object_editor);
			gtk_widget_show_all (priv->scrolled_window);
		}
	}
}


static void
file_new_action (GtkAction *action,
                 gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);

	canvas_editor_window_new_file (window);
}


static void
file_open_action (GtkAction *action,
                  gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);

	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new (_("Open File"),
	                                      GTK_WINDOW (window),
	                                      GTK_FILE_CHOOSER_ACTION_OPEN,
	                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	                                      NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		canvas_editor_window_open_file (window, filename);
		g_free (filename);
	}
	gtk_widget_destroy (dialog);
}


static void
file_save_action (GtkAction *action,
                  gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);

	canvas_editor_window_save_file (window);
}


static void
file_saveas_action (GtkAction *action,
                    gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	if (priv->project == NULL)
		return;

	GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Save As File"),
	                                                 GTK_WINDOW (window),
	                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

	gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), _("Untitled.xml"));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar* filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		canvas_editor_window_save_file_as (window, filename);
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
}


static void
file_close_action (GtkAction *action,
                   gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);

	canvas_editor_window_close_current_file (window);
}


static void
file_quit_action (GtkAction *action,
                  gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);

	if (canvas_editor_window_quit (window))
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
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);

	static gchar* authors[] = {"Arnel A. Borja <galeon@ymail.com>", NULL};
	static gchar* license =
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
		"with this program.  If not, see <http://www.gnu.org/licenses/>.";

	gtk_show_about_dialog (GTK_WINDOW (window),
	                       "program-name", _("Canvas Editor"),
	                       "version", PACKAGE_VERSION,
	                       "title", _("About Canvas Editor"),
	                       "comments", _("Canvas Project Editor"),
	                       "website", "http://kyoushuu.users.sourceforge.net/project_canvas",
	                       "copyright", _("Copyright (c) 2011  Arnel A. Borja"),
#if GTK_MAJOR_VERSION >= 3
	                       "license-type", GTK_LICENSE_GPL_3_0,
#endif
	                       "license", license,
	                       "wrap-license", TRUE,
	                       "authors", authors,
	                       NULL);
}


static void
new_object_activate (GtkWidget *menuitem,
                     gpointer   user_data)
{
	CanvasEditorWindow* window = CANVAS_EDITOR_WINDOW (user_data);
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	CanvasObject* object_popup;
	GtkTreeIter iter;
	gchar* title;

	gtk_tree_model_get (GTK_TREE_MODEL (priv->store), &priv->iter_popup,
	                    DATA_COLUMN, &object_popup,
	                    -1);

	if (menuitem == priv->popup_new_category_menu_item)
	{
		CanvasCategory* category = canvas_category_new ();
		canvas_project_add_category (CANVAS_PROJECT (object_popup), category);

		title = g_strdup_printf (_("Category: \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, category,
		                    -1);
		g_free (title);
	}
	else if (menuitem == priv->popup_new_lesson_menu_item)
	{
		CanvasLesson* lesson = canvas_lesson_new ();
		canvas_category_add_lesson (CANVAS_CATEGORY (object_popup), lesson);

		title = g_strdup_printf (_("Lesson: \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, lesson,
		                    -1);
		g_free (title);
	}
	else if (menuitem == priv->popup_new_discussion_menu_item)
	{
		CanvasLessonDiscussion* discussion = canvas_lesson_discussion_new ();
		canvas_lesson_add_lesson_element (CANVAS_LESSON (object_popup),
		                                  CANVAS_LESSON_ELEMENT (discussion));

		title = g_strdup_printf (_("Lesson Discussion: \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, discussion,
		                    -1);
		g_free (title);
	}
	else if (menuitem == priv->popup_new_reading_menu_item)
	{
		CanvasLessonReading* reading = canvas_lesson_reading_new ();
		canvas_lesson_add_lesson_element (CANVAS_LESSON (object_popup),
		                                  CANVAS_LESSON_ELEMENT (reading));

		title = g_strdup_printf (_("Lesson Reading: \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, reading,
		                    -1);
		g_free (title);
	}
	else if (menuitem == priv->popup_new_test_multi_choice_menu_item)
	{
		CanvasLessonTestMultiChoice* test = canvas_lesson_test_multi_choice_new ();
		canvas_lesson_add_lesson_element (CANVAS_LESSON (object_popup),
		                                  CANVAS_LESSON_ELEMENT (test));

		title = g_strdup_printf (_("Lesson Test (Multi-Choice): \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, test,
		                    -1);
		g_free (title);
	}
	else if (menuitem == priv->popup_new_test_word_pool_menu_item)
	{
		CanvasLessonTestWordPool* test = canvas_lesson_test_word_pool_new ();
		canvas_lesson_add_lesson_element (CANVAS_LESSON (object_popup),
		                                  CANVAS_LESSON_ELEMENT (test));

		title = g_strdup_printf (_("Lesson Test (Word Pool): \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, test,
		                    -1);
		g_free (title);
	}
	else if (menuitem == priv->popup_new_test_order_menu_item)
	{
		CanvasLessonTestOrder* test = canvas_lesson_test_order_new ();
		canvas_lesson_add_lesson_element (CANVAS_LESSON (object_popup),
		                                  CANVAS_LESSON_ELEMENT (test));

		title = g_strdup_printf (_("Lesson Test (Order): \"%s\""), _("Untitled"));
		gtk_tree_store_append (priv->store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, test,
		                    -1);
		g_free (title);
	}
	else
		return;

	canvas_editor_window_set_modified (window, TRUE);
}

G_DEFINE_TYPE (CanvasEditorWindow, canvas_editor_window, GTK_TYPE_WINDOW);

static void
canvas_editor_window_init (CanvasEditorWindow *object)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (object);

	priv->store = NULL;
	priv->object_editor = NULL;

	priv->parser = NULL;
	priv->project = NULL;
	priv->project_file = NULL;
	priv->modified = FALSE;

	GError* error = NULL;

	GtkActionEntry actions[] = 
	{
		{"file", NULL, _("_File")},
		{"file-new", GTK_STOCK_NEW, NULL, "<Control>N", NULL, G_CALLBACK (file_new_action)},
		{"file-open", GTK_STOCK_OPEN, NULL, "<Control>O", NULL, G_CALLBACK (file_open_action)},
		{"file-save", GTK_STOCK_SAVE, NULL, "<Control>S", NULL, G_CALLBACK (file_save_action)},
		{"file-saveas", GTK_STOCK_SAVE_AS, NULL, "<Control><Shift>S", NULL, G_CALLBACK (file_saveas_action)},
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
		"      <menuitem name=\"Open\" action=\"file-open\" />"
		"      <separator/>"
		"      <menuitem name=\"Save\" action=\"file-save\" />"
		"      <menuitem name=\"SaveAs\" action=\"file-saveas\" />"
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
		"      <toolitem name=\"Open\" action=\"file-open\" />"
		"      <toolitem name=\"Save\" action=\"file-save\" />"
		"      <separator/>"
		"    </placeholder>"
		"  </toolbar>"
		"</ui>";


	gtk_window_set_title (GTK_WINDOW (object), _("Canvas Editor"));
	gtk_window_set_default_size (GTK_WINDOW (object), 800, 600);

	priv->main_vbox = gtk_vbox_new (FALSE, 3);
	gtk_container_add (GTK_CONTAINER (object), priv->main_vbox);


	priv->manager = gtk_ui_manager_new ();
	gtk_window_add_accel_group (GTK_WINDOW (object), gtk_ui_manager_get_accel_group (priv->manager));

	GtkActionGroup* action_group = gtk_action_group_new ("canvas-editor-window");
	gtk_action_group_add_actions (action_group, actions, G_N_ELEMENTS (actions), object);

	gtk_ui_manager_insert_action_group (priv->manager, action_group, 0);
	gtk_ui_manager_add_ui_from_string (priv->manager, ui, -1, &error);

	if (error)
	{
		g_critical ("%s", error->message);
		g_error_free (error);
	}

	GtkWidget* main_menu = gtk_ui_manager_get_widget (priv->manager, "/menubar");
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), main_menu, FALSE, TRUE, 0);

	GtkWidget* toolbar = gtk_ui_manager_get_widget (priv->manager, "/toolbar");
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), toolbar, FALSE, TRUE, 0);


	priv->infobar = gtk_info_bar_new_with_buttons (GTK_STOCK_OK, GTK_RESPONSE_OK,
	                                               NULL);
	gtk_widget_set_no_show_all (priv->infobar, TRUE);
	g_signal_connect (priv->infobar, "response", G_CALLBACK (info_bar_response), object);
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), priv->infobar, FALSE, TRUE, 0);
	gtk_info_bar_set_default_response (GTK_INFO_BAR (priv->infobar), GTK_RESPONSE_OK);

	priv->infobarlabel = gtk_label_new (NULL);
	gtk_box_pack_start (GTK_BOX (gtk_info_bar_get_content_area (GTK_INFO_BAR (priv->infobar))),
	                    priv->infobarlabel, FALSE, TRUE, 0);


	priv->hpaned = gtk_hpaned_new ();
	gtk_paned_set_position (GTK_PANED (priv->hpaned), 400);
	gtk_box_pack_start (GTK_BOX (priv->main_vbox), priv->hpaned, TRUE, TRUE, 0);

	priv->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (priv->scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack1 (GTK_PANED (priv->hpaned), priv->scrolled_window, FALSE, TRUE);

	priv->store = gtk_tree_store_new (N_COLUMNS,
	                                  G_TYPE_STRING,
	                                  G_TYPE_POINTER);

	priv->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (priv->tree_view), FALSE);
	g_signal_connect (priv->tree_view, "realize", G_CALLBACK (tree_view_realized), object);
	g_signal_connect (priv->tree_view, "button-press-event", G_CALLBACK (tree_view_press_event), object);
	g_signal_connect (priv->tree_view, "key-press-event", G_CALLBACK (tree_view_press_event), object);
	g_signal_connect (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->tree_view)), "changed", G_CALLBACK (tree_view_selection_changed), object);
	gtk_container_add (GTK_CONTAINER (priv->scrolled_window), priv->tree_view);

	priv->column = gtk_tree_view_column_new_with_attributes (_("Project Tree"), gtk_cell_renderer_text_new (),
	                                                         "text", 0,
	                                                         NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->tree_view), priv->column);

	priv->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (priv->scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack2 (GTK_PANED (priv->hpaned), priv->scrolled_window, TRUE, TRUE);


	priv->popup_menu = gtk_menu_new ();

	priv->popup_new_category_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Category"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_category_menu_item);
	g_signal_connect (priv->popup_new_category_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	priv->popup_new_lesson_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Lesson"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_lesson_menu_item);
	g_signal_connect (priv->popup_new_lesson_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	priv->popup_new_discussion_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Discussion"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_discussion_menu_item);
	g_signal_connect (priv->popup_new_discussion_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	priv->popup_new_reading_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Reading"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_reading_menu_item);
	g_signal_connect (priv->popup_new_reading_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	priv->popup_new_test_multi_choice_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Multiple Choice Test"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_test_multi_choice_menu_item);
	g_signal_connect (priv->popup_new_test_multi_choice_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	priv->popup_new_test_word_pool_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Word Pool Test"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_test_word_pool_menu_item);
	g_signal_connect (priv->popup_new_test_word_pool_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	priv->popup_new_test_order_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Order Test"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_test_order_menu_item);
	g_signal_connect (priv->popup_new_test_order_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);


	priv->parser = canvas_parser_new ();


	g_signal_connect (object, "delete-event", G_CALLBACK (window_delete_event), NULL);

	g_log_set_handler (G_LOG_DOMAIN,
	                   G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_WARNING | G_LOG_LEVEL_MESSAGE | G_LOG_LEVEL_INFO | G_LOG_LEVEL_DEBUG,
	                    canvas_editor_window_log, object);
}

static void
canvas_editor_window_finalize (GObject *object)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (object);

	if (priv->store)
		g_object_unref (priv->store);

	if (priv->parser)
		g_object_unref (priv->parser);

	gtk_widget_destroy (priv->popup_menu);

	G_OBJECT_CLASS (canvas_editor_window_parent_class)->finalize (object);
}

static void
canvas_editor_window_class_init (CanvasEditorWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasEditorWindowPrivate));

	object_class->finalize = canvas_editor_window_finalize;
}


GtkWidget*
canvas_editor_window_new (void)
{
	return g_object_new(CANVAS_TYPE_EDITOR_WINDOW, NULL);
}

const gchar*
canvas_editor_window_get_filename (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	return priv->project_file;
}

static void
canvas_editor_window_set_filename (CanvasEditorWindow* window, const gchar* file)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	if (priv->project_file)
		g_free (priv->project_file);
	priv->project_file = g_strdup (file);
}

gboolean
canvas_editor_window_get_modified (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	return priv->modified;
}

void
canvas_editor_window_set_modified (CanvasEditorWindow* window, gboolean modified)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	gchar* title;

	if (priv->project)
	{
		gchar* file;
		if (priv->project_file)
			file = g_path_get_basename (priv->project_file);
		else
			file = g_strdup ("New File");

		title = g_strdup_printf ("%s - %s%s", _("Canvas Editor"), file, modified?"*":"");
		g_free (file);
	}
	else
		title = g_strdup (_("Canvas Editor"));

	gtk_window_set_title (GTK_WINDOW (window), title);

	g_free (title);

	priv->modified = modified;
}

void
canvas_editor_window_new_file (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	if (priv->project)
	{
		if (canvas_editor_window_close_current_file (window) == FALSE)
			return;
	}

	priv->project = canvas_project_new ();

	canvas_editor_window_set_filename (window, NULL);

	canvas_editor_window_set_modified (window, TRUE);

	canvas_editor_window_update_tree_store (window);
}

void
canvas_editor_window_open_file (CanvasEditorWindow* window, const gchar* file)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	GError* error = NULL;

	if (priv->project)
	{
		if (canvas_editor_window_close_current_file (window) == FALSE)
			return;
	}

	priv->project = canvas_parser_open (priv->parser, file, &error);
	if (error)
	{
		g_critical (_("Parsing file error: %s"), error->message);
		g_error_free (error);
		return;
	}

	canvas_editor_window_set_filename (window, file);
	canvas_editor_window_set_modified (window, FALSE);

	canvas_editor_window_update_tree_store (window);
}

gboolean
canvas_editor_window_close_current_file (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	if (priv->project == NULL)
		return FALSE;

	gboolean save = TRUE;
	if (canvas_editor_window_get_modified (window))
	{
		GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
		                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		                                            GTK_MESSAGE_ERROR,
		                                            GTK_BUTTONS_NONE,
		                                            _("Save project?"));
		gtk_dialog_add_buttons (GTK_DIALOG (dialog),
		                        GTK_STOCK_YES, GTK_RESPONSE_YES,
		                        GTK_STOCK_NO, GTK_RESPONSE_NO,
		                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		                        NULL);

		gint response = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);

		if (response == GTK_RESPONSE_NO)
			save = FALSE;
		else if (response == GTK_RESPONSE_CANCEL)
			return FALSE;
	}

	if (save)
	{
		if (canvas_editor_window_save_file (window) == FALSE)
			return FALSE;
	}

	if (priv->project)
		g_object_unref (priv->project);
	priv->project = NULL;

	canvas_editor_window_set_filename (window, NULL);
	canvas_editor_window_set_modified (window, FALSE);

	canvas_editor_window_update_tree_store (window); 

	return TRUE;
}

gboolean
canvas_editor_window_save_file (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	if (priv->project == NULL)
		return FALSE;

	gchar* filename = NULL;

	if (priv->project_file == NULL)
	{
		if (canvas_editor_window_get_filename (window) == NULL)
		{
			GtkWidget *dialog;
			dialog = gtk_file_chooser_dialog_new (_("Save File"),
			                                      GTK_WINDOW (window),
			                                      GTK_FILE_CHOOSER_ACTION_SAVE,
			                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			                                      NULL);
			gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

			gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), _("Untitled.xml"));

			gint response = gtk_dialog_run (GTK_DIALOG (dialog));

			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

			gtk_widget_destroy (dialog);

			if (response != GTK_RESPONSE_ACCEPT)
				return FALSE;
		}
	}
	else
		filename = g_strdup (priv->project_file);

	GError* error = NULL;

	canvas_parser_save (priv->parser, priv->project, filename, &error);

	if (error)
	{
		g_critical (_("Saving file error: %s"), error->message);
		g_error_free (error);
	}
	else
	{
		if (priv->project_file == NULL)
			canvas_editor_window_set_filename (window, filename);
		canvas_editor_window_set_modified (window, FALSE);
	}

	g_free (filename);

	if (error)
		return FALSE;
	else
		return TRUE;
}

gboolean
canvas_editor_window_save_file_as (CanvasEditorWindow* window, const gchar* file)
{
	canvas_editor_window_set_filename (window, file);

	return canvas_editor_window_save_file (window);
}

void
canvas_editor_window_update_tree_store (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	gtk_tree_store_clear (priv->store);

	if (priv->project == NULL)
		return;

	GtkTreeIter iterProject, iterCategory, iterLesson, iterLessonElement;
	gchar* title;

	CanvasProject* project = priv->project;

	title = g_strdup_printf (_("Project: \"%s\""), canvas_project_get_title (project));
	gtk_tree_store_append (priv->store, &iterProject, NULL);
	gtk_tree_store_set (priv->store, &iterProject,
	                    TITLE_COLUMN, title,
	                    DATA_COLUMN, project,
	                    -1);
	g_free (title);

	GList *categories = canvas_project_get_categories (priv->project);
	GList *curr_categories = categories;

	while (curr_categories)
	{
		CanvasCategory* category = CANVAS_CATEGORY (curr_categories->data);

		title = g_strdup_printf (_("Category: \"%s\""), canvas_category_get_title (category));
		gtk_tree_store_append (priv->store, &iterCategory, &iterProject);
		gtk_tree_store_set (priv->store, &iterCategory,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, category,
		                    -1);
		g_free (title);

		GList *lessons = canvas_category_get_lessons (category);
		GList *curr_lessons = lessons;

		while (curr_lessons)
		{
			CanvasLesson* lesson = CANVAS_LESSON (curr_lessons->data);

			title = g_strdup_printf (_("Lesson: \"%s\""), canvas_lesson_get_title (lesson));
			gtk_tree_store_append (priv->store, &iterLesson, &iterCategory);
			gtk_tree_store_set (priv->store, &iterLesson,
			                    TITLE_COLUMN, title,
			                    DATA_COLUMN, lesson,
			                    -1);
			g_free (title);

			GList *lesson_elements = canvas_lesson_get_lesson_elements (lesson);
			GList *curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				CanvasLessonElement* lesson_element = CANVAS_LESSON_ELEMENT (curr_lesson_elements->data);

				if (CANVAS_IS_LESSON_DISCUSSION (lesson_element))
				{
					/*CanvasLessonDiscussion* lesson_discussion = CANVAS_LESSON_DISCUSSION (lesson_element);*/

					title = g_strdup_printf (_("Lesson Discussion: \"%s\""), canvas_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (CANVAS_IS_LESSON_READING (lesson_element))
				{
					/*CanvasLessonReading* lesson_reading = CANVAS_LESSON_READING (lesson_element);*/

					title = g_strdup_printf (_("Lesson Reading: \"%s\""), canvas_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (lesson_element))
				{
					/*CanvasLessonTestMultiChoice* lesson_test_multi_choice = CANVAS_LESSON_TEST_MULTI_CHOICE (lesson_element);*/

					title = g_strdup_printf (_("Lesson Test (Multi-Choice): \"%s\""), canvas_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (CANVAS_IS_LESSON_TEST_WORD_POOL (lesson_element))
				{
					/*CanvasLessonTestWordPool* lesson_test_word_pool = CANVAS_LESSON_TEST_WORD_POOL (lesson_element);*/

					title = g_strdup_printf (_("Lesson Test (Word Pool): \"%s\""), canvas_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (CANVAS_IS_LESSON_TEST_ORDER (lesson_element))
				{
					/*CanvasLessonTestOrder* lesson_test_order = CANVAS_LESSON_TEST_ORDER (lesson_element);*/

					title = g_strdup_printf (_("Lesson Test (Order): \"%s\""), canvas_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}

				curr_lesson_elements = curr_lesson_elements->next;
			}

			g_list_free (lesson_elements);

			curr_lessons = curr_lessons->next;
		}

		g_list_free (lessons);

		curr_categories = curr_categories->next;
	}

	g_list_free (categories);

	gtk_tree_view_expand_all (GTK_TREE_VIEW (priv->tree_view));
}

gboolean
canvas_editor_window_quit (CanvasEditorWindow* window)
{
	CanvasEditorWindowPrivate* priv = CANVAS_EDITOR_WINDOW_PRIVATE (window);

	if (priv->project)
	{
		if (canvas_editor_window_close_current_file (window) == FALSE)
			return FALSE;
	}

	return TRUE;
}
