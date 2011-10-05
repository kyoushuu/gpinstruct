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
#include "gpinstruct-editor/gpinstruct-editor.h"
#include "gpinstruct-view/gpinstruct-view.h"

#include "compat/compat-gtk.h"

struct _GPInstructEditorWindowPrivate
{
	GtkWidget* main_vbox;

	GtkUIManager* manager;
	GtkActionGroup* action_group;

	GtkWidget* infobar;
	GtkWidget* infobarlabel;


	GtkWidget* hpaned;

	GtkWidget *tree_view;
	GtkTreeStore* store;
	GtkTreeViewColumn *column;

	GtkWidget *scrolled_window;
	GtkWidget* object_editor;


	GtkWidget* statusbar;


	GtkWidget* popup_menu;

	GtkWidget* popup_new_category_menu_item;
	GtkWidget* popup_new_lesson_menu_item;
	GtkWidget* popup_new_group_menu_item;
	GtkWidget* popup_new_discussion_menu_item;
	GtkWidget* popup_new_reading_menu_item;
	GtkWidget* popup_new_test_multi_choice_menu_item;
	GtkWidget* popup_new_test_word_pool_menu_item;
	GtkWidget* popup_new_test_order_menu_item;

	GtkWidget* popup_remove_menu_item;


	GPInstructParser* parser;
	GPInstructProject *project;
	gchar* project_file;
	gboolean modified;

	GtkTreeIter iter_popup;
};

#define GPINSTRUCT_EDITOR_WINDOW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_EDITOR_WINDOW, GPInstructEditorWindowPrivate))



enum
{
	TITLE_COLUMN,
	DATA_COLUMN,
	N_COLUMNS
};

static void gpinstruct_editor_window_log (const gchar *log_domain,
                                          GLogLevelFlags log_level,
                                          const gchar *message,
                                          gpointer user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

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

	gtk_info_bar_set_message_type (GTK_INFO_BAR (window->priv->infobar), type);
	gtk_label_set_text (GTK_LABEL (window->priv->infobarlabel), message);

	gtk_widget_show (window->priv->infobarlabel);
	gtk_widget_show (window->priv->infobar);
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
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (widget);

	if (gpinstruct_editor_window_quit (window))
		return FALSE;
	else
		return TRUE;
}


static gboolean
tree_view_press_event (GtkWidget *widget,
                       GdkEvent *event,
                       gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);
	GdkEventButton *event_button = NULL;
	GdkEventKey *event_key;

	GtkTreePath* path = NULL;
	GtkTreeIter iter;

	GPInstructObject* object_popup;

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
			gtk_tree_view_get_cursor (GTK_TREE_VIEW (window->priv->tree_view), &path, NULL);
			if (path)
			{
				if (gtk_tree_model_get_iter (GTK_TREE_MODEL (window->priv->store),
				                             &window->priv->iter_popup, path))
				{
					show_popup = TRUE;
					gtk_tree_path_free (path);
				}
			}
		}
	}
	else if (event->type == GDK_KEY_PRESS)
	{
		event_key = (GdkEventKey *) event;
		if (event_key->state == 0 && event_key->keyval == GDK_KEY_Menu)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->priv->tree_view)),
			                                     NULL, &window->priv->iter_popup))
				show_popup = TRUE;
		}
		else if (event_key->state == 0 && event_key->keyval == GDK_KEY_Left)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->priv->tree_view)),
			                                     NULL, &iter))
			{
				path = gtk_tree_model_get_path (GTK_TREE_MODEL (window->priv->store),
				                                &iter);
				if (path)
				{
					gtk_tree_view_collapse_row (GTK_TREE_VIEW (window->priv->tree_view), path);
					gtk_tree_path_free (path);
					return TRUE;
				}
			}
		}
		else if (event_key->state == 0 && event_key->keyval == GDK_KEY_Right)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->priv->tree_view)),
			                                     NULL, &iter))
			{
				path = gtk_tree_model_get_path (GTK_TREE_MODEL (window->priv->store),
				                                &iter);
				if (path)
				{
					gtk_tree_view_expand_row (GTK_TREE_VIEW (window->priv->tree_view), path, FALSE);
					gtk_tree_path_free (path);
					return TRUE;
				}
			}
		}
	}

	if (show_popup)
	{
		gtk_tree_model_get (GTK_TREE_MODEL (window->priv->store), &window->priv->iter_popup,
		                    DATA_COLUMN, &object_popup,
		                    -1);

		gtk_widget_hide (window->priv->popup_new_category_menu_item);
		gtk_widget_hide (window->priv->popup_new_lesson_menu_item);
		gtk_widget_hide (window->priv->popup_new_group_menu_item);
		gtk_widget_hide (window->priv->popup_new_discussion_menu_item);
		gtk_widget_hide (window->priv->popup_new_reading_menu_item);
		gtk_widget_hide (window->priv->popup_new_test_multi_choice_menu_item);
		gtk_widget_hide (window->priv->popup_new_test_word_pool_menu_item);
		gtk_widget_hide (window->priv->popup_new_test_order_menu_item);
		gtk_widget_hide (window->priv->popup_remove_menu_item);

		if (GPINSTRUCT_IS_PROJECT (object_popup))
		{
			gtk_widget_show (window->priv->popup_new_category_menu_item);
		}
		else if (GPINSTRUCT_IS_CATEGORY (object_popup))
		{
			gtk_widget_show (window->priv->popup_new_lesson_menu_item);
			gtk_widget_show (window->priv->popup_remove_menu_item);
		}
		else if (GPINSTRUCT_IS_LESSON (object_popup))
		{
			gtk_widget_show (window->priv->popup_new_group_menu_item);
			gtk_widget_show (window->priv->popup_new_discussion_menu_item);
			gtk_widget_show (window->priv->popup_new_reading_menu_item);
			gtk_widget_show (window->priv->popup_new_test_multi_choice_menu_item);
			gtk_widget_show (window->priv->popup_new_test_word_pool_menu_item);
			gtk_widget_show (window->priv->popup_new_test_order_menu_item);
			gtk_widget_show (window->priv->popup_remove_menu_item);
		}
		else if (GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (object_popup))
		{
			gtk_widget_show (window->priv->popup_new_discussion_menu_item);
			gtk_widget_show (window->priv->popup_new_reading_menu_item);
			gtk_widget_show (window->priv->popup_new_test_multi_choice_menu_item);
			gtk_widget_show (window->priv->popup_new_test_word_pool_menu_item);
			gtk_widget_show (window->priv->popup_new_test_order_menu_item);
			gtk_widget_show (window->priv->popup_remove_menu_item);
		}
		else if (GPINSTRUCT_IS_LESSON_ELEMENT (object_popup))
		{
			gtk_widget_show (window->priv->popup_remove_menu_item);
		}
		else
			return FALSE;

		gtk_menu_popup (GTK_MENU (window->priv->popup_menu), NULL, NULL, NULL, NULL,
		                button, (event_button)? event_button->time : event_key->time);
		return TRUE;
	}

	return FALSE;
}


static void
tree_view_selection_changed (GtkTreeSelection *treeselection,
                             gpointer          user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);
	GtkTreeIter iter;
	GtkTreeModel *model;
	GPInstructObject *object;

	if (window->priv->object_editor)
	{
		gtk_widget_destroy (window->priv->object_editor);
		window->priv->object_editor = NULL;
	}

	if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->priv->tree_view)),
	                                     &model, &iter))
	{
		gtk_tree_model_get (model, &iter,
		                    DATA_COLUMN, &object,
		                    -1);

		if (GPINSTRUCT_IS_PROJECT (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_project_editor_new (window,
			                                                                         GPINSTRUCT_PROJECT (object)));
		}
		else if (GPINSTRUCT_IS_CATEGORY (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_category_editor_new (window,
			                                                                          GPINSTRUCT_CATEGORY (object)));
		}
		else if (GPINSTRUCT_IS_LESSON (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_editor_new (window,
			                                                                        GPINSTRUCT_LESSON (object)));
		}
		else if (GPINSTRUCT_IS_LESSON_DISCUSSION (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_discussion_editor_new (window,
			                                                                                   GPINSTRUCT_LESSON_DISCUSSION (object)));
		}
		else if (GPINSTRUCT_IS_LESSON_READING (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_reading_editor_new (window,
			                                                                                GPINSTRUCT_LESSON_READING (object)));
		}
		else if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_test_multi_choice_editor_new (window,
			                                                                                          GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (object)));
		}
		else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_test_word_pool_editor_new (window,
			                                                                                       GPINSTRUCT_LESSON_TEST_WORD_POOL (object)));
		}
		else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_test_order_editor_new (window,
			                                                                                   GPINSTRUCT_LESSON_TEST_ORDER (object)));
		}
		else if (GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (object))
		{
			window->priv->object_editor = GTK_WIDGET (gpinstruct_lesson_element_group_editor_new (window,
			                                                                                      GPINSTRUCT_LESSON_ELEMENT_GROUP (object)));
		}

		if (window->priv->object_editor)
		{
			gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (window->priv->scrolled_window),
			                                       window->priv->object_editor);
			gtk_widget_show_all (window->priv->scrolled_window);
		}
	}
}


typedef struct {
	GPInstructEditorWindow* window;
	gpointer data;
} TreeViewSetCursorObjectData;

static gboolean
tree_view_set_cursor_object (GtkTreeModel *model,
                             GtkTreePath *path,
                             GtkTreeIter *iter,
                             gpointer data)
{
	GPInstructEditorWindow* window = ((TreeViewSetCursorObjectData*)data)->window;

	gpointer current_data;
	gtk_tree_model_get (model, iter,
	                    DATA_COLUMN, &current_data,
	                    -1);

	if (current_data == ((TreeViewSetCursorObjectData*)data)->data)
	{
		gtk_tree_view_set_cursor (GTK_TREE_VIEW (window->priv->tree_view),
		                          path, NULL, FALSE);
		return TRUE;
	}

	return FALSE;
}


static void
file_new_action (GtkAction *action,
                 gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

	gpinstruct_editor_window_new_file (window);
}


static void
file_open_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new (_("Open File"),
	                                      GTK_WINDOW (window),
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
		gchar *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		gpinstruct_editor_window_open_file (window, filename);
		g_free (filename);
	}
	gtk_widget_destroy (dialog);
}


static void
file_save_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

	gpinstruct_editor_window_save_file (window);
}


static void
file_saveas_action (GtkAction *action,
                    gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);
	if (window->priv->project == NULL)
		return;

	GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Save As File"),
	                                                 GTK_WINDOW (window),
	                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("GPInstruct project file"));
	gtk_file_filter_add_pattern (filter, "*.gpinstruct-project");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), _("Untitled.gpinstruct-project"));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar* filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		gpinstruct_editor_window_save_file_as (window, filename);
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
}


static void
file_close_action (GtkAction *action,
                   gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

	gpinstruct_editor_window_close_current_file (window);
}


static void
file_quit_action (GtkAction *action,
                  gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

	if (gpinstruct_editor_window_quit (window))
		gtk_widget_destroy (GTK_WIDGET (window));
}


static void
view_execute_action (GtkAction *action,
                     gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);
	GPInstructMessagePool* message_pool = gpinstruct_message_pool_new ();

	if (message_pool)
	{
#ifdef G_OS_WIN32
		gchar* prefix = g_win32_get_package_installation_directory_of_module (NULL);
		gchar* datadir = g_build_filename (prefix, "share", PACKAGE, NULL);
#else
		gchar* datadir = g_strdup (PACKAGE_DATA_DIR);
#endif

		gchar* message_file = g_build_filename (datadir, "messages.ini", NULL);
		gpinstruct_message_pool_load_from_file (message_pool, message_file);
		g_free (message_file);

#ifdef G_OS_WIN32
		g_free (prefix);
#endif
		g_free (datadir);
	}

	GtkWidget* preview_window = gpinstruct_project_view_new (window->priv->project, message_pool, NULL);

	g_object_unref (message_pool);

	if (preview_window)
	{
		gtk_window_set_transient_for (GTK_WINDOW (preview_window), GTK_WINDOW (window));
		gtk_window_set_modal (GTK_WINDOW (preview_window), TRUE);
		gtk_widget_show_all (preview_window);
	}
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
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);

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
	                                          "program-name", _("GPInstruct Editor"),
	                                          "version", PACKAGE_VERSION,
	                                          "title", _("About GPInstruct Editor"),
	                                          "comments", _("GPInstruct Project Editor"),
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


static void
new_object_activate (GtkWidget *menuitem,
                     gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);
	GPInstructObject* object_popup;
	GtkTreeIter iter;
	GtkTreePath* path;
	gchar* title;

	gtk_tree_model_get (GTK_TREE_MODEL (window->priv->store), &window->priv->iter_popup,
	                    DATA_COLUMN, &object_popup,
	                    -1);

	if (menuitem == window->priv->popup_new_category_menu_item)
	{
		GPInstructCategory* category = gpinstruct_category_new ();
		gpinstruct_project_add_category (GPINSTRUCT_PROJECT (object_popup), category);

		title = g_strdup_printf (_("Category: \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, category,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_lesson_menu_item)
	{
		GPInstructLesson* lesson = gpinstruct_lesson_new ();
		gpinstruct_category_add_lesson (GPINSTRUCT_CATEGORY (object_popup), lesson);

		title = g_strdup_printf (_("Lesson: \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, lesson,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_group_menu_item)
	{
		GPInstructLessonElementGroup* group = gpinstruct_lesson_element_group_new ();
		gpinstruct_lesson_add_lesson_element (GPINSTRUCT_LESSON (object_popup),
		                                      GPINSTRUCT_LESSON_ELEMENT (group));

		title = g_strdup_printf (_("Group: \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, group,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_discussion_menu_item)
	{
		GPInstructLessonDiscussion* discussion = gpinstruct_lesson_discussion_new ();
		if (GPINSTRUCT_IS_LESSON (object_popup))
			gpinstruct_lesson_add_lesson_element (GPINSTRUCT_LESSON (object_popup),
			                                      GPINSTRUCT_LESSON_ELEMENT (discussion));
		else
			gpinstruct_lesson_element_group_add_lesson_element (GPINSTRUCT_LESSON_ELEMENT_GROUP (object_popup),
			                                                    GPINSTRUCT_LESSON_ELEMENT (discussion));

		title = g_strdup_printf (_("Lesson Discussion: \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, discussion,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_reading_menu_item)
	{
		GPInstructLessonReading* reading = gpinstruct_lesson_reading_new ();
		if (GPINSTRUCT_IS_LESSON (object_popup))
			gpinstruct_lesson_add_lesson_element (GPINSTRUCT_LESSON (object_popup),
			                                      GPINSTRUCT_LESSON_ELEMENT (reading));
		else
			gpinstruct_lesson_element_group_add_lesson_element (GPINSTRUCT_LESSON_ELEMENT_GROUP (object_popup),
			                                                    GPINSTRUCT_LESSON_ELEMENT (reading));

		title = g_strdup_printf (_("Lesson Reading: \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, reading,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_test_multi_choice_menu_item)
	{
		GPInstructLessonTestMultiChoice* test = gpinstruct_lesson_test_multi_choice_new ();
		if (GPINSTRUCT_IS_LESSON (object_popup))
			gpinstruct_lesson_add_lesson_element (GPINSTRUCT_LESSON (object_popup),
			                                      GPINSTRUCT_LESSON_ELEMENT (test));
		else
			gpinstruct_lesson_element_group_add_lesson_element (GPINSTRUCT_LESSON_ELEMENT_GROUP (object_popup),
			                                                    GPINSTRUCT_LESSON_ELEMENT (test));

		title = g_strdup_printf (_("Lesson Test (Multi-Choice): \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, test,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_test_word_pool_menu_item)
	{
		GPInstructLessonTestWordPool* test = gpinstruct_lesson_test_word_pool_new ();
		if (GPINSTRUCT_IS_LESSON (object_popup))
			gpinstruct_lesson_add_lesson_element (GPINSTRUCT_LESSON (object_popup),
			                                      GPINSTRUCT_LESSON_ELEMENT (test));
		else
			gpinstruct_lesson_element_group_add_lesson_element (GPINSTRUCT_LESSON_ELEMENT_GROUP (object_popup),
			                                                    GPINSTRUCT_LESSON_ELEMENT (test));

		title = g_strdup_printf (_("Lesson Test (Word Pool): \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, test,
		                    -1);
		g_free (title);
	}
	else if (menuitem == window->priv->popup_new_test_order_menu_item)
	{
		GPInstructLessonTestOrder* test = gpinstruct_lesson_test_order_new ();
		if (GPINSTRUCT_IS_LESSON (object_popup))
			gpinstruct_lesson_add_lesson_element (GPINSTRUCT_LESSON (object_popup),
			                                      GPINSTRUCT_LESSON_ELEMENT (test));
		else
			gpinstruct_lesson_element_group_add_lesson_element (GPINSTRUCT_LESSON_ELEMENT_GROUP (object_popup),
			                                                    GPINSTRUCT_LESSON_ELEMENT (test));

		title = g_strdup_printf (_("Lesson Test (Order): \"%s\""), _("Untitled"));
		gtk_tree_store_append (window->priv->store, &iter, &window->priv->iter_popup);
		gtk_tree_store_set (window->priv->store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, test,
		                    -1);
		g_free (title);
	}
	else
		return;

	path = gtk_tree_model_get_path (GTK_TREE_MODEL (window->priv->store), &window->priv->iter_popup);
	gtk_tree_view_expand_row (GTK_TREE_VIEW (window->priv->tree_view),
	                          path, FALSE);
	gtk_tree_path_free (path);

	gpinstruct_editor_window_set_modified (window, TRUE);
}


static void
remove_object_activate (GtkWidget *menuitem,
                        gpointer   user_data)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (user_data);
	GPInstructObject* object_popup;
	GtkTreeIter iter, iterSel;
	gboolean select = FALSE;

	if (gtk_tree_model_iter_parent (GTK_TREE_MODEL (window->priv->store),
	                                &iter, &window->priv->iter_popup))
	{
		iterSel = window->priv->iter_popup;
		if (gtk_tree_model_iter_next (GTK_TREE_MODEL (window->priv->store), &iterSel))
			select = TRUE;
		else
		{
			iterSel = window->priv->iter_popup;
			if (gtk_tree_model_iter_previous (GTK_TREE_MODEL (window->priv->store), &iterSel))
				select = TRUE;
			else
			{
				if (gtk_tree_model_iter_parent (GTK_TREE_MODEL (window->priv->store), &iterSel, &window->priv->iter_popup))
					select = TRUE;
			}
		}

		if (select)
			gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->priv->tree_view)),
			                                &iterSel);

		GtkTreePath* path = gtk_tree_model_get_path (GTK_TREE_MODEL (window->priv->store),
		                                             &window->priv->iter_popup);
		gint* indices = gtk_tree_path_get_indices (path);

		gtk_tree_model_get (GTK_TREE_MODEL (window->priv->store), &iter,
		                    DATA_COLUMN, &object_popup,
		                    -1);

		gboolean modified = TRUE;

		if (GPINSTRUCT_IS_PROJECT (object_popup))
		{
			gpinstruct_project_remove_category (GPINSTRUCT_PROJECT (object_popup),
			                                    indices[1]);
		}
		else if (GPINSTRUCT_IS_CATEGORY (object_popup))
		{
			gpinstruct_category_remove_lesson (GPINSTRUCT_CATEGORY (object_popup),
			                                   indices[2]);
		}
		else if (GPINSTRUCT_IS_LESSON (object_popup))
		{
			gpinstruct_lesson_remove_lesson_element (GPINSTRUCT_LESSON (object_popup),
			                                         indices[3]);
		}
		else
			modified = FALSE;

		if (modified)
		{
			gpinstruct_editor_window_set_modified (window, TRUE);
			gtk_tree_store_remove (GTK_TREE_STORE (window->priv->store),
			                       &window->priv->iter_popup);
		}

		gtk_tree_path_free (path);
	}
}

G_DEFINE_TYPE (GPInstructEditorWindow, gpinstruct_editor_window, GTK_TYPE_WINDOW);

static void
gpinstruct_editor_window_init (GPInstructEditorWindow *object)
{
	object->priv = GPINSTRUCT_EDITOR_WINDOW_PRIVATE (object);

	object->priv->store = NULL;
	object->priv->object_editor = NULL;

	object->priv->parser = NULL;
	object->priv->project = NULL;
	object->priv->project_file = NULL;
	object->priv->modified = FALSE;

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
		{"view", NULL, _("_View")},
		{"view-execute", GTK_STOCK_EXECUTE, NULL, NULL, NULL, G_CALLBACK (view_execute_action)},
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
		"    <menu name=\"ViewMenu\" action=\"view\">"
		"      <placeholder name=\"ViewMenuAdditions\" />"
		"      <separator/>"
		"      <menuitem name=\"Execute\" action=\"view-execute\"/>"
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
		"      <toolitem name=\"Execute\" action=\"view-execute\" />"
		"      <separator/>"
		"    </placeholder>"
		"  </toolbar>"
		"</ui>";


	gtk_window_set_title (GTK_WINDOW (object), _("GPInstruct Editor"));
	gtk_window_set_default_size (GTK_WINDOW (object), 800, 600);

	object->priv->main_vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (object), object->priv->main_vbox);


	object->priv->manager = gtk_ui_manager_new ();
	gtk_window_add_accel_group (GTK_WINDOW (object), gtk_ui_manager_get_accel_group (object->priv->manager));

	object->priv->action_group = gtk_action_group_new ("gpinstruct-editor-window");
	gtk_action_group_add_actions (object->priv->action_group, actions, G_N_ELEMENTS (actions), object);
	gtk_action_set_sensitive (gtk_action_group_get_action (object->priv->action_group,
	                                                       "file-save"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (object->priv->action_group,
	                                                       "file-saveas"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (object->priv->action_group,
	                                                       "file-close"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (object->priv->action_group,
	                                                       "view-execute"),
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


	object->priv->infobar = gtk_info_bar_new_with_buttons (GTK_STOCK_OK, GTK_RESPONSE_OK,
	                                                       NULL);
	gtk_widget_set_no_show_all (object->priv->infobar, TRUE);
	g_signal_connect (object->priv->infobar, "response", G_CALLBACK (info_bar_response), object);
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), object->priv->infobar, FALSE, TRUE, 0);
	gtk_info_bar_set_default_response (GTK_INFO_BAR (object->priv->infobar), GTK_RESPONSE_OK);

	object->priv->infobarlabel = gtk_label_new (NULL);
	gtk_box_pack_start (GTK_BOX (gtk_info_bar_get_content_area (GTK_INFO_BAR (object->priv->infobar))),
	                    object->priv->infobarlabel, FALSE, TRUE, 0);


	object->priv->hpaned = gtk_hpaned_new ();
	gtk_paned_set_position (GTK_PANED (object->priv->hpaned), 300);
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), object->priv->hpaned, TRUE, TRUE, 3);

	object->priv->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (object->priv->scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack1 (GTK_PANED (object->priv->hpaned), object->priv->scrolled_window, FALSE, TRUE);

	object->priv->store = gtk_tree_store_new (N_COLUMNS,
	                                          G_TYPE_STRING,
	                                          G_TYPE_POINTER);

	object->priv->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (object->priv->tree_view), FALSE);
	gtk_widget_set_events (object->priv->tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (object->priv->tree_view, "button-press-event", G_CALLBACK (tree_view_press_event), object);
	g_signal_connect (object->priv->tree_view, "key-press-event", G_CALLBACK (tree_view_press_event), object);
	g_signal_connect (gtk_tree_view_get_selection (GTK_TREE_VIEW (object->priv->tree_view)), "changed", G_CALLBACK (tree_view_selection_changed), object);
	gtk_container_add (GTK_CONTAINER (object->priv->scrolled_window), object->priv->tree_view);

	object->priv->column = gtk_tree_view_column_new_with_attributes (_("Project Tree"), gtk_cell_renderer_text_new (),
	                                                                 "text", 0,
	                                                                 NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (object->priv->tree_view), object->priv->column);

	object->priv->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (object->priv->scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack2 (GTK_PANED (object->priv->hpaned), object->priv->scrolled_window, TRUE, TRUE);


	object->priv->statusbar = gtk_statusbar_new ();
	gtk_box_pack_start (GTK_BOX (object->priv->main_vbox), object->priv->statusbar, FALSE, TRUE, 0);


	object->priv->popup_menu = gtk_menu_new ();

	object->priv->popup_new_category_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Category"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_category_menu_item);
	g_signal_connect (object->priv->popup_new_category_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_lesson_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Lesson"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_lesson_menu_item);
	g_signal_connect (object->priv->popup_new_lesson_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_group_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Group"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_group_menu_item);
	g_signal_connect (object->priv->popup_new_group_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_discussion_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Discussion"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_discussion_menu_item);
	g_signal_connect (object->priv->popup_new_discussion_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_reading_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Reading"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_reading_menu_item);
	g_signal_connect (object->priv->popup_new_reading_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_test_multi_choice_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Multiple Choice Test"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_test_multi_choice_menu_item);
	g_signal_connect (object->priv->popup_new_test_multi_choice_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_test_word_pool_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Word Pool Test"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_test_word_pool_menu_item);
	g_signal_connect (object->priv->popup_new_test_word_pool_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_new_test_order_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Order Test"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_new_test_order_menu_item);
	g_signal_connect (object->priv->popup_new_test_order_menu_item, "activate",
	                  G_CALLBACK (new_object_activate), object);

	object->priv->popup_remove_menu_item = gtk_menu_item_new_with_mnemonic (_("_Remove"));
	gtk_menu_shell_append (GTK_MENU_SHELL (object->priv->popup_menu), object->priv->popup_remove_menu_item);
	g_signal_connect (object->priv->popup_remove_menu_item, "activate",
	                  G_CALLBACK (remove_object_activate), object);


	object->priv->parser = gpinstruct_parser_new ();


	g_signal_connect (object, "delete-event", G_CALLBACK (window_delete_event), NULL);

	g_log_set_handler (G_LOG_DOMAIN,
	                   G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_WARNING | G_LOG_LEVEL_MESSAGE | G_LOG_LEVEL_INFO | G_LOG_LEVEL_DEBUG,
	                   gpinstruct_editor_window_log, object);
}

static void
gpinstruct_editor_window_finalize (GObject *object)
{
	GPInstructEditorWindow* window = GPINSTRUCT_EDITOR_WINDOW (object);

	if (window->priv->action_group)
		g_object_unref (window->priv->action_group);

	if (window->priv->store)
		g_object_unref (window->priv->store);

	if (window->priv->parser)
		g_object_unref (window->priv->parser);

	gtk_widget_destroy (window->priv->popup_menu);

	G_OBJECT_CLASS (gpinstruct_editor_window_parent_class)->finalize (object);
}

static void
gpinstruct_editor_window_class_init (GPInstructEditorWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructEditorWindowPrivate));

	object_class->finalize = gpinstruct_editor_window_finalize;
}


GtkWidget*
gpinstruct_editor_window_new (void)
{
	return GTK_WIDGET (g_object_new (GPINSTRUCT_TYPE_EDITOR_WINDOW, NULL));
}

const gchar*
gpinstruct_editor_window_get_filename (GPInstructEditorWindow* window)
{
	return window->priv->project_file;
}

static void
gpinstruct_editor_window_set_filename (GPInstructEditorWindow* window,
                                       const gchar* file)
{
	if (window->priv->project_file)
		g_free (window->priv->project_file);
	window->priv->project_file = g_strdup (file);
}

gboolean
gpinstruct_editor_window_get_modified (GPInstructEditorWindow* window)
{
	return window->priv->modified;
}

void
gpinstruct_editor_window_set_modified (GPInstructEditorWindow* window,
                                       gboolean modified)
{
	gchar* title;

	if (window->priv->project)
	{
		gchar* file;
		if (window->priv->project_file)
			file = g_path_get_basename (window->priv->project_file);
		else
			file = g_strdup ("New File");

		title = g_strdup_printf ("%s - %s%s", _("GPInstruct Editor"), file, modified?"*":"");
		g_free (file);
	}
	else
		title = g_strdup (_("GPInstruct Editor"));

	gtk_window_set_title (GTK_WINDOW (window), title);

	g_free (title);

	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-save"),
	                          modified);

	window->priv->modified = modified;
}

void
gpinstruct_editor_window_new_file (GPInstructEditorWindow* window)
{
	if (window->priv->project)
	{
		if (gpinstruct_editor_window_close_current_file (window) == FALSE)
			return;
	}

	window->priv->project = gpinstruct_project_new ();

	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-saveas"),
	                          TRUE);
	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-close"),
	                          TRUE);
	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "view-execute"),
	                          TRUE);

	gpinstruct_editor_window_set_filename (window, NULL);

	gpinstruct_editor_window_set_modified (window, TRUE);

	gpinstruct_editor_window_update_tree_store (window, (gpointer)window->priv->project);
}

void
gpinstruct_editor_window_open_file (GPInstructEditorWindow* window,
                                    const gchar* file)
{
	GError* error = NULL;

	if (window->priv->project)
	{
		if (gpinstruct_editor_window_close_current_file (window) == FALSE)
			return;
	}

	window->priv->project = gpinstruct_parser_load_from_file (window->priv->parser, file, &error);
	if (error)
	{
		g_critical (_("Parsing file error: %s"), error->message);
		g_error_free (error);
		return;
	}

	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-saveas"),
	                          TRUE);
	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-close"),
	                          TRUE);
	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "view-execute"),
	                          TRUE);

	gpinstruct_editor_window_set_filename (window, file);
	gpinstruct_editor_window_set_modified (window, FALSE);

	gpinstruct_editor_window_update_tree_store (window, (gpointer)window->priv->project);
}

gboolean
gpinstruct_editor_window_close_current_file (GPInstructEditorWindow* window)
{
	if (window->priv->project == NULL)
		return FALSE;

	if (gpinstruct_editor_window_get_modified (window))
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

		if (response == GTK_RESPONSE_YES)
		{
			if (gpinstruct_editor_window_save_file (window) == FALSE)
				return FALSE;
		}
		else if (response == GTK_RESPONSE_CANCEL)
			return FALSE;
	}

	if (window->priv->project)
		g_object_unref (window->priv->project);
	window->priv->project = NULL;

	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-saveas"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "file-close"),
	                          FALSE);
	gtk_action_set_sensitive (gtk_action_group_get_action (window->priv->action_group,
	                                                       "view-execute"),
	                          FALSE);

	gpinstruct_editor_window_set_filename (window, NULL);
	gpinstruct_editor_window_set_modified (window, FALSE);

	gpinstruct_editor_window_update_tree_store (window, NULL);

	return TRUE;
}

gboolean
gpinstruct_editor_window_save_file (GPInstructEditorWindow* window)
{
	if (window->priv->project == NULL)
		return FALSE;

	gchar* filename = NULL;

	if (window->priv->project_file == NULL)
	{
		if (gpinstruct_editor_window_get_filename (window) == NULL)
		{
			GtkWidget *dialog;
			dialog = gtk_file_chooser_dialog_new (_("Save File"),
			                                      GTK_WINDOW (window),
			                                      GTK_FILE_CHOOSER_ACTION_SAVE,
			                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			                                      NULL);
			gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

			GtkFileFilter *filter = gtk_file_filter_new ();
			gtk_file_filter_set_name (filter, _("GPInstruct project file"));
			gtk_file_filter_add_pattern (filter, "*.gpinstruct-project");
			gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

			gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), _("Untitled.gpinstruct-project"));

			gint response = gtk_dialog_run (GTK_DIALOG (dialog));

			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

			gtk_widget_destroy (dialog);

			if (response != GTK_RESPONSE_ACCEPT)
				return FALSE;
		}
	}
	else
		filename = g_strdup (window->priv->project_file);

	GError* error = NULL;

	gpinstruct_parser_save_to_file (window->priv->parser, window->priv->project, filename, &error);

	if (error)
	{
		g_critical (_("Saving file error: %s"), error->message);
		g_error_free (error);
	}
	else
	{
		if (window->priv->project_file == NULL)
			gpinstruct_editor_window_set_filename (window, filename);
		gpinstruct_editor_window_set_modified (window, FALSE);
	}

	g_free (filename);

	if (error)
		return FALSE;
	else
		return TRUE;
}

gboolean
gpinstruct_editor_window_save_file_as (GPInstructEditorWindow* window,
                                       const gchar* file)
{
	gpinstruct_editor_window_set_filename (window, file);

	return gpinstruct_editor_window_save_file (window);
}

void
gpinstruct_editor_window_update_tree_store (GPInstructEditorWindow* window,
                                            GPInstructObject* object)
{
	gtk_tree_store_clear (window->priv->store);

	if (window->priv->project == NULL)
		return;

	GtkTreeIter iterProject, iterCategory, iterLesson, iterLessonElement, iterLessonElementGroup;
	gchar* title;

	GPInstructProject* project = window->priv->project;

	title = g_strdup_printf (_("Project: \"%s\""), gpinstruct_project_get_title (project));
	gtk_tree_store_append (window->priv->store, &iterProject, NULL);
	gtk_tree_store_set (window->priv->store, &iterProject,
	                    TITLE_COLUMN, title,
	                    DATA_COLUMN, project,
	                    -1);
	g_free (title);

	GList *categories = gpinstruct_project_get_categories (window->priv->project);
	GList *curr_categories = categories;

	while (curr_categories)
	{
		GPInstructCategory* category = GPINSTRUCT_CATEGORY (curr_categories->data);

		title = g_strdup_printf (_("Category: \"%s\""), gpinstruct_category_get_title (category));
		gtk_tree_store_append (window->priv->store, &iterCategory, &iterProject);
		gtk_tree_store_set (window->priv->store, &iterCategory,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, category,
		                    -1);
		g_free (title);

		GList *lessons = gpinstruct_category_get_lessons (category);
		GList *curr_lessons = lessons;

		while (curr_lessons)
		{
			GPInstructLesson* lesson = GPINSTRUCT_LESSON (curr_lessons->data);

			title = g_strdup_printf (_("Lesson: \"%s\""), gpinstruct_lesson_get_title (lesson));
			gtk_tree_store_append (window->priv->store, &iterLesson, &iterCategory);
			gtk_tree_store_set (window->priv->store, &iterLesson,
			                    TITLE_COLUMN, title,
			                    DATA_COLUMN, lesson,
			                    -1);
			g_free (title);

			GList *lesson_elements = gpinstruct_lesson_get_lesson_elements (lesson);
			GList *curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				GPInstructLessonElement* lesson_element = GPINSTRUCT_LESSON_ELEMENT (curr_lesson_elements->data);

				if (GPINSTRUCT_IS_LESSON_DISCUSSION (lesson_element))
				{
					/*GPInstructLessonDiscussion* lesson_discussion = GPINSTRUCT_LESSON_DISCUSSION (lesson_element);*/

					title = g_strdup_printf (_("Lesson Discussion: \"%s\""), gpinstruct_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (window->priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (GPINSTRUCT_IS_LESSON_READING (lesson_element))
				{
					/*GPInstructLessonReading* lesson_reading = GPINSTRUCT_LESSON_READING (lesson_element);*/

					title = g_strdup_printf (_("Lesson Reading: \"%s\""), gpinstruct_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (window->priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (lesson_element))
				{
					/*GPInstructLessonTestMultiChoice* lesson_test_multi_choice = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (lesson_element);*/

					title = g_strdup_printf (_("Lesson Test (Multi-Choice): \"%s\""), gpinstruct_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (window->priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (lesson_element))
				{
					/*GPInstructLessonTestWordPool* lesson_test_word_pool = GPINSTRUCT_LESSON_TEST_WORD_POOL (lesson_element);*/

					title = g_strdup_printf (_("Lesson Test (Word Pool): \"%s\""), gpinstruct_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (window->priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (lesson_element))
				{
					/*GPInstructLessonTestOrder* lesson_test_order = GPINSTRUCT_LESSON_TEST_ORDER (lesson_element);*/

					title = g_strdup_printf (_("Lesson Test (Order): \"%s\""), gpinstruct_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLesson);
					gtk_tree_store_set (window->priv->store, &iterLessonElement,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);
				}
				else if (GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (lesson_element))
				{
					GPInstructLessonElementGroup* lesson_element_group = GPINSTRUCT_LESSON_ELEMENT_GROUP (lesson_element);

					title = g_strdup_printf (_("Group: \"%s\""), gpinstruct_lesson_element_get_title (lesson_element));
					gtk_tree_store_append (window->priv->store, &iterLessonElementGroup, &iterLesson);
					gtk_tree_store_set (window->priv->store, &iterLessonElementGroup,
					                    TITLE_COLUMN, title,
					                    DATA_COLUMN, lesson_element,
					                    -1);
					g_free (title);

					GList *lesson_elements_group = gpinstruct_lesson_element_group_get_lesson_elements (lesson_element_group);
					GList *curr_lesson_elements_group = lesson_elements_group;

					while (curr_lesson_elements_group)
					{
						GPInstructLessonElement* lesson_element_group = GPINSTRUCT_LESSON_ELEMENT (curr_lesson_elements_group->data);

						if (GPINSTRUCT_IS_LESSON_DISCUSSION (lesson_element_group))
						{
							/*GPInstructLessonDiscussion* lesson_discussion = GPINSTRUCT_LESSON_DISCUSSION (lesson_element_group);*/

							title = g_strdup_printf (_("Lesson Discussion: \"%s\""), gpinstruct_lesson_element_get_title (lesson_element_group));
							gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLessonElementGroup);
							gtk_tree_store_set (window->priv->store, &iterLessonElement,
							                    TITLE_COLUMN, title,
							                    DATA_COLUMN, lesson_element_group,
							                    -1);
							g_free (title);
						}
						else if (GPINSTRUCT_IS_LESSON_READING (lesson_element_group))
						{
							/*GPInstructLessonReading* lesson_reading = GPINSTRUCT_LESSON_READING (lesson_element_group);*/

							title = g_strdup_printf (_("Lesson Reading: \"%s\""), gpinstruct_lesson_element_get_title (lesson_element_group));
							gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLessonElementGroup);
							gtk_tree_store_set (window->priv->store, &iterLessonElement,
							                    TITLE_COLUMN, title,
							                    DATA_COLUMN, lesson_element_group,
							                    -1);
							g_free (title);
						}
						else if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (lesson_element_group))
						{
							/*GPInstructLessonTestMultiChoice* lesson_test_multi_choice = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (lesson_element_group);*/

							title = g_strdup_printf (_("Lesson Test (Multi-Choice): \"%s\""), gpinstruct_lesson_element_get_title (lesson_element_group));
							gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLessonElementGroup);
							gtk_tree_store_set (window->priv->store, &iterLessonElement,
							                    TITLE_COLUMN, title,
							                    DATA_COLUMN, lesson_element_group,
							                    -1);
							g_free (title);
						}
						else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (lesson_element_group))
						{
							/*GPInstructLessonTestWordPool* lesson_test_word_pool = GPINSTRUCT_LESSON_TEST_WORD_POOL (lesson_element_group);*/

							title = g_strdup_printf (_("Lesson Test (Word Pool): \"%s\""), gpinstruct_lesson_element_get_title (lesson_element_group));
							gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLessonElementGroup);
							gtk_tree_store_set (window->priv->store, &iterLessonElement,
							                    TITLE_COLUMN, title,
							                    DATA_COLUMN, lesson_element_group,
							                    -1);
							g_free (title);
						}
						else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (lesson_element_group))
						{
							/*GPInstructLessonTestOrder* lesson_test_order = GPINSTRUCT_LESSON_TEST_ORDER (lesson_element_group);*/

							title = g_strdup_printf (_("Lesson Test (Order): \"%s\""), gpinstruct_lesson_element_get_title (lesson_element_group));
							gtk_tree_store_append (window->priv->store, &iterLessonElement, &iterLessonElementGroup);
							gtk_tree_store_set (window->priv->store, &iterLessonElement,
							                    TITLE_COLUMN, title,
							                    DATA_COLUMN, lesson_element_group,
							                    -1);
							g_free (title);
						}

						curr_lesson_elements_group = curr_lesson_elements_group->next;
					}

					g_list_free (lesson_elements_group);
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

	gtk_tree_view_expand_all (GTK_TREE_VIEW (window->priv->tree_view));

	if (object)
	{
		TreeViewSetCursorObjectData data = {window, object};
		gtk_tree_model_foreach (GTK_TREE_MODEL (window->priv->store),
		                        tree_view_set_cursor_object, &data);
	}
}

gboolean
gpinstruct_editor_window_quit (GPInstructEditorWindow* window)
{
	if (window->priv->project)
	{
		if (gpinstruct_editor_window_close_current_file (window) == FALSE)
			return FALSE;
	}

	return TRUE;
}
