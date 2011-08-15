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
#ifndef GDK_KEY_Left
#define GDK_KEY_Left GDK_Left
#endif
#ifndef GDK_KEY_Right
#define GDK_KEY_Right GDK_Right
#endif

#include "canvas.h"
#include "canvas-editor.h"

typedef struct _CanvasLessonTestWordPoolEditorPrivate CanvasLessonTestWordPoolEditorPrivate;
struct _CanvasLessonTestWordPoolEditorPrivate
{
	CanvasEditorWindow* window;
	CanvasLessonTestWordPool* test;

	GtkWidget* title_label;
	GtkWidget* title_entry;

	GtkWidget* directions_label;
	GtkWidget* directions_view;

	GtkWidget* explain_label;
	GtkWidget* explain_switch;

	GtkWidget* questions_tree_view;
	GtkTreeStore* questions_store;

	GtkWidget* choices_tree_view;
	GtkTreeStore* choices_store;

	GtkWidget* popup_menu;

	GtkWidget* popup_new_question_menu_item;
	GtkWidget* popup_new_choice_menu_item;
	GtkWidget* popup_remove_question_menu_item;
	GtkWidget* popup_remove_choice_menu_item;

	GtkTreeIter iter_popup;
};

#define CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_WORD_POOL_EDITOR, CanvasLessonTestWordPoolEditorPrivate))


enum
{
	TITLE_COLUMN,
	DATA_COLUMN,
	N_COLUMNS
};

static void
update_questions_tree_view (CanvasLessonTestWordPoolEditor* editor)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (editor);

	gtk_tree_store_clear (priv->questions_store);

	GtkTreeIter iterTest, iterQuestion;

	CanvasLessonTestWordPool* test = priv->test;
	const gchar* text;

	gtk_tree_store_append (priv->questions_store, &iterTest, NULL);
	gtk_tree_store_set (priv->questions_store, &iterTest,
	                    TITLE_COLUMN, _("Test"),
	                    DATA_COLUMN, test,
	                    -1);

	GList* questions = canvas_lesson_test_word_pool_get_questions (test);
	GList* curr_questions = questions;

	while (curr_questions)
	{
		CanvasLessonTestWordPoolQuestion* question = CANVAS_LESSON_TEST_WORD_POOL_QUESTION (curr_questions->data);

		text = canvas_lesson_test_word_pool_question_get_text (question);
		gtk_tree_store_append (priv->questions_store, &iterQuestion, &iterTest);
		gtk_tree_store_set (priv->questions_store, &iterQuestion,
		                    TITLE_COLUMN, text,
		                    DATA_COLUMN, question,
		                    -1);

		curr_questions = curr_questions->next;
	}

	g_list_free (questions);

	gtk_tree_view_expand_all (GTK_TREE_VIEW (priv->questions_tree_view));
}


static gboolean
questions_tree_view_press_event (GtkWidget *widget,
                                 GdkEvent *event,
                                 gpointer   user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	GdkEventButton *event_button = NULL;
	GdkEventKey *event_key;

	GtkTreePath* path = NULL;
	GtkTreeIter iter;

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
			gtk_tree_view_get_cursor (GTK_TREE_VIEW (priv->questions_tree_view), &path, NULL);
			if (path)
			{
				if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->questions_store),
				                             &priv->iter_popup, path))
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
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->questions_tree_view)),
			                                     NULL, &priv->iter_popup))
				show_popup = TRUE;
		}
		else if (event_key->state == 0 && event_key->keyval == GDK_KEY_Left)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->questions_tree_view)),
			                                     NULL, &iter))
			{
				path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->questions_store),
				                                &iter);
				if (path)
				{
					gtk_tree_view_collapse_row (GTK_TREE_VIEW (priv->questions_tree_view), path);
					gtk_tree_path_free (path);
					return TRUE;
				}
			}
		}
		else if (event_key->state == 0 && event_key->keyval == GDK_KEY_Right)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->questions_tree_view)),
			                                     NULL, &iter))
			{
				path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->questions_store),
				                                &iter);
				if (path)
				{
					gtk_tree_view_expand_row (GTK_TREE_VIEW (priv->questions_tree_view), path, FALSE);
					gtk_tree_path_free (path);
					return TRUE;
				}
			}
		}
	}

	if (show_popup)
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->questions_store), &priv->iter_popup,
		                    DATA_COLUMN, &object_popup,
		                    -1);

		gtk_widget_hide (priv->popup_new_question_menu_item);
		gtk_widget_hide (priv->popup_new_choice_menu_item);
		gtk_widget_hide (priv->popup_remove_question_menu_item);
		gtk_widget_hide (priv->popup_remove_choice_menu_item);

		if (CANVAS_IS_LESSON_TEST_WORD_POOL (object_popup))
		{
			gtk_widget_show (priv->popup_new_question_menu_item);
		}
		else if (CANVAS_IS_LESSON_TEST_WORD_POOL_QUESTION (object_popup))
		{
			gtk_widget_show (priv->popup_remove_question_menu_item);
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
questions_tree_view_row_activated (GtkTreeView       *tree_view,
                                   GtkTreePath       *path,
                                   GtkTreeViewColumn *column,
                                   gpointer           user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	CanvasObject* object;
	GtkTreeIter iter;
	GtkWidget *text_entry, *answer_spin;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->questions_store), &iter, path))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->questions_store), &iter,
		                    DATA_COLUMN, &object,
		                    -1);
		if (CANVAS_IS_LESSON_TEST_WORD_POOL_QUESTION (object))
		{
			CanvasLessonTestWordPoolQuestion* question = CANVAS_LESSON_TEST_WORD_POOL_QUESTION (object);
			guint choices_num = canvas_lesson_test_word_pool_get_choices_length (priv->test);

			GtkWidget* dialog = gtk_dialog_new_with_buttons (_("Question Properties"),
			                                                 GTK_WINDOW (priv->window),
			                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
			                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			                                                 NULL);
			GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

			text_entry = gtk_entry_new ();
			gtk_entry_set_text (GTK_ENTRY (text_entry),
			                    canvas_lesson_test_word_pool_question_get_text (question));
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Text:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    text_entry,
			                    FALSE, TRUE, 0);

			if (choices_num)
			{
				answer_spin = gtk_spin_button_new_with_range (1, choices_num, 1);
				gtk_spin_button_set_value (GTK_SPIN_BUTTON (answer_spin),
				                           canvas_lesson_test_word_pool_question_get_answer (question)+1);
				gtk_box_pack_start (GTK_BOX (content_area),
				                    gtk_label_new (_("Answer:")),
				                    FALSE, TRUE, 0);
				gtk_box_pack_start (GTK_BOX (content_area),
				                    answer_spin,
				                    FALSE, TRUE, 0);
			}

			gtk_widget_show_all (content_area);
			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				canvas_lesson_test_word_pool_question_set_text (question,
				                                                gtk_entry_get_text (GTK_ENTRY (text_entry)));
				if (choices_num)
					canvas_lesson_test_word_pool_question_set_answer (question,
					                                                  gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (answer_spin))-1);

				update_questions_tree_view (CANVAS_LESSON_TEST_WORD_POOL_EDITOR (user_data));
				canvas_editor_window_set_modified (priv->window, TRUE);
			}

			gtk_widget_destroy (dialog);
		}
	}
}


static void
questions_new_object_activate (GtkWidget *menuitem,
                               gpointer   user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	CanvasObject* object_popup;
	GtkTreeIter iter;
	gchar* title;

	gtk_tree_model_get (GTK_TREE_MODEL (priv->questions_store), &priv->iter_popup,
	                    DATA_COLUMN, &object_popup,
	                    -1);

	if (menuitem == priv->popup_new_question_menu_item)
	{
		title = _("Empty Question");
		CanvasLessonTestWordPoolQuestion* question = canvas_lesson_test_word_pool_question_new ();
		canvas_lesson_test_word_pool_question_set_text (question, title);
		canvas_lesson_test_word_pool_add_question (CANVAS_LESSON_TEST_WORD_POOL (priv->test), question);

		gtk_tree_store_append (priv->questions_store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->questions_store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, question,
		                    -1);
	}
	else
		return;

	canvas_editor_window_set_modified (priv->window, TRUE);
}


static void
questions_remove_object_activate (GtkWidget *menuitem,
                     gpointer   user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	CanvasObject* object_popup;
	GtkTreeIter iter;

	if (gtk_tree_model_iter_parent (GTK_TREE_MODEL (priv->questions_store),
	                                &iter, &priv->iter_popup))
	{
		GtkTreePath* path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->questions_store),
		                                &priv->iter_popup);
		gint* indices = gtk_tree_path_get_indices (path);

		gtk_tree_model_get (GTK_TREE_MODEL (priv->questions_store), &iter,
			                DATA_COLUMN, &object_popup,
			                -1);

		gboolean modified = TRUE;

		if (CANVAS_LESSON_TEST_WORD_POOL (object_popup))
		{
			canvas_lesson_test_word_pool_remove_question (CANVAS_LESSON_TEST_WORD_POOL (object_popup),
			                                            indices[1]);
		}
		else
			modified = FALSE;

		if (modified)
		{
			canvas_editor_window_set_modified (priv->window, TRUE);
			gtk_tree_store_remove (GTK_TREE_STORE (priv->questions_store),
			                       &priv->iter_popup);
		}

		gtk_tree_path_free (path);
	}
}


static void
update_choices_tree_view (CanvasLessonTestWordPoolEditor* editor)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (editor);

	gtk_tree_store_clear (priv->choices_store);

	GtkTreeIter iterTest, iterChoices;

	CanvasLessonTestWordPool* test = priv->test;

	gtk_tree_store_append (priv->choices_store, &iterTest, NULL);
	gtk_tree_store_set (priv->choices_store, &iterTest,
	                    TITLE_COLUMN, _("Test"),
	                    DATA_COLUMN, test,
	                    -1);

	GList* choices = canvas_lesson_test_word_pool_get_choices (test);
	GList* curr_choices = choices;

	while (curr_choices)
	{
		gtk_tree_store_append (priv->choices_store, &iterChoices, &iterTest);
		gtk_tree_store_set (priv->choices_store, &iterChoices,
		                    TITLE_COLUMN, curr_choices->data,
		                    DATA_COLUMN, NULL,
		                    -1);

		curr_choices = curr_choices->next;
	}

	g_list_free (choices);

	gtk_tree_view_expand_all (GTK_TREE_VIEW (priv->choices_tree_view));
}


static gboolean
choices_tree_view_press_event (GtkWidget *widget,
                               GdkEvent *event,
                               gpointer   user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	GdkEventButton *event_button = NULL;
	GdkEventKey *event_key;

	GtkTreePath* path = NULL;
	GtkTreeIter iter;

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
			gtk_tree_view_get_cursor (GTK_TREE_VIEW (priv->choices_tree_view), &path, NULL);
			if (path)
			{
				if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->choices_store),
				                             &priv->iter_popup, path))
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
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->choices_tree_view)),
			                                     NULL, &priv->iter_popup))
				show_popup = TRUE;
		}
		else if (event_key->state == 0 && event_key->keyval == GDK_KEY_Left)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->choices_tree_view)),
			                                     NULL, &iter))
			{
				path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->choices_store),
				                                &iter);
				if (path)
				{
					gtk_tree_view_collapse_row (GTK_TREE_VIEW (priv->choices_tree_view), path);
					gtk_tree_path_free (path);
					return TRUE;
				}
			}
		}
		else if (event_key->state == 0 && event_key->keyval == GDK_KEY_Right)
		{
			if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->choices_tree_view)),
			                                     NULL, &iter))
			{
				path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->choices_store),
				                                &iter);
				if (path)
				{
					gtk_tree_view_expand_row (GTK_TREE_VIEW (priv->choices_tree_view), path, FALSE);
					gtk_tree_path_free (path);
					return TRUE;
				}
			}
		}
	}

	if (show_popup)
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->choices_store), &priv->iter_popup,
		                    DATA_COLUMN, &object_popup,
		                    -1);

		gtk_widget_hide (priv->popup_new_question_menu_item);
		gtk_widget_hide (priv->popup_new_choice_menu_item);
		gtk_widget_hide (priv->popup_remove_question_menu_item);
		gtk_widget_hide (priv->popup_remove_choice_menu_item);

		if (CANVAS_IS_LESSON_TEST_WORD_POOL (object_popup))
		{
			gtk_widget_show (priv->popup_new_choice_menu_item);
		}
		else if (object_popup == NULL)
		{
			gtk_widget_show (priv->popup_remove_choice_menu_item);
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
choices_tree_view_row_activated (GtkTreeView       *tree_view,
                                 GtkTreePath       *path,
                                 GtkTreeViewColumn *column,
                                 gpointer           user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	CanvasObject* object;
	GtkTreeIter iter;
	GtkWidget *text_entry;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->choices_store), &iter, path))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->choices_store), &iter,
		                    DATA_COLUMN, &object,
		                    -1);
		if (object == NULL)
		{
			guint choice = gtk_tree_path_get_indices(path)[1];

			GtkWidget* dialog = gtk_dialog_new_with_buttons (_("Question Properties"),
			                                                 GTK_WINDOW (priv->window),
			                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
			                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			                                                 NULL);
			GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

			text_entry = gtk_entry_new ();
			gtk_entry_set_text (GTK_ENTRY (text_entry),
			                    canvas_lesson_test_word_pool_get_choice (priv->test, choice));
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Text:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    text_entry,
			                    FALSE, TRUE, 0);

			gtk_widget_show_all (content_area);
			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				canvas_lesson_test_word_pool_set_choice (priv->test, choice,
				                                         gtk_entry_get_text (GTK_ENTRY (text_entry)));

				update_choices_tree_view (CANVAS_LESSON_TEST_WORD_POOL_EDITOR (user_data));
				canvas_editor_window_set_modified (priv->window, TRUE);
			}

			gtk_widget_destroy (dialog);
		}
	}
}


static void
choices_new_object_activate (GtkWidget *menuitem,
                             gpointer   user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	CanvasObject* object_popup;
	GtkTreeIter iter;
	gchar* title;

	gtk_tree_model_get (GTK_TREE_MODEL (priv->choices_store), &priv->iter_popup,
	                    DATA_COLUMN, &object_popup,
	                    -1);

	if (menuitem == priv->popup_new_choice_menu_item)
	{
		title = _("Empty Choice");
		canvas_lesson_test_word_pool_add_choice (CANVAS_LESSON_TEST_WORD_POOL (priv->test), title);

		gtk_tree_store_append (priv->choices_store, &iter, &priv->iter_popup);
		gtk_tree_store_set (priv->choices_store, &iter,
		                    TITLE_COLUMN, title,
		                    DATA_COLUMN, NULL,
		                    -1);
	}
	else
		return;

	canvas_editor_window_set_modified (priv->window, TRUE);
}


static void
choices_remove_object_activate (GtkWidget *menuitem,
                     gpointer   user_data)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (user_data);

	CanvasObject* object_popup;
	GtkTreeIter iter;

	if (gtk_tree_model_iter_parent (GTK_TREE_MODEL (priv->choices_store),
	                                &iter, &priv->iter_popup))
	{
		GtkTreePath* path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->choices_store),
		                                &priv->iter_popup);
		gint* indices = gtk_tree_path_get_indices (path);

		gtk_tree_model_get (GTK_TREE_MODEL (priv->choices_store), &iter,
			                DATA_COLUMN, &object_popup,
			                -1);

		gboolean modified = TRUE;

		if (CANVAS_LESSON_TEST_WORD_POOL (object_popup))
		{
			canvas_lesson_test_word_pool_remove_choice (CANVAS_LESSON_TEST_WORD_POOL (object_popup),
			                                            indices[1]);
		}
		else
			modified = FALSE;

		if (modified)
		{
			canvas_editor_window_set_modified (priv->window, TRUE);
			gtk_tree_store_remove (GTK_TREE_STORE (priv->choices_store),
			                       &priv->iter_popup);
		}

		gtk_tree_path_free (path);
	}
}


G_DEFINE_TYPE (CanvasLessonTestWordPoolEditor, canvas_lesson_test_word_pool_editor, CANVAS_TYPE_OBJECT_EDITOR);

static void
canvas_lesson_test_word_pool_editor_init (CanvasLessonTestWordPoolEditor *object)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (object);

	priv->questions_store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
	priv->choices_store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);

	priv->title_label = gtk_label_new (_("Title:"));
	gtk_table_attach (GTK_TABLE (object), priv->title_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	priv->title_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (object), priv->title_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	priv->directions_label = gtk_label_new (_("Directions:"));
	gtk_table_attach (GTK_TABLE (object), priv->directions_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	GtkWidget* directions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (directions_view_scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	priv->directions_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (directions_view_scrolled_window), priv->directions_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->directions_view), GTK_WRAP_WORD_CHAR);
	gtk_table_attach (GTK_TABLE (object), directions_view_scrolled_window,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);

	priv->explain_label = gtk_label_new (_("Explain:"));
	gtk_table_attach (GTK_TABLE (object), priv->explain_label,
	                  0, 1, 2, 3,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
#if GTK_MAJOR_VERSION >= 3
	priv->explain_switch = gtk_switch_new ();
#else
	priv->explain_switch = gtk_toggle_button_new_with_label (GTK_STOCK_NO);
	gtk_button_set_use_stock (GTK_BUTTON (priv->explain_switch), TRUE);
#endif
	gtk_table_attach (GTK_TABLE (object), priv->explain_switch,
	                  1, 2, 2, 3,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	GtkWidget* questions_tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (questions_tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_table_attach (GTK_TABLE (object), questions_tree_view_scrolled_window,
	                  0, 2, 3, 4,
	                  GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);
	priv->questions_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->questions_store));
	gtk_widget_set_events (priv->questions_tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (priv->questions_tree_view, "button-press-event", G_CALLBACK (questions_tree_view_press_event), object);
	g_signal_connect (priv->questions_tree_view, "key-press-event", G_CALLBACK (questions_tree_view_press_event), object);
	g_signal_connect (priv->questions_tree_view, "row-activated", G_CALLBACK (questions_tree_view_row_activated), object);
	gtk_container_add (GTK_CONTAINER (questions_tree_view_scrolled_window), priv->questions_tree_view);

	GtkTreeViewColumn* questions_column = gtk_tree_view_column_new_with_attributes (_("Questions:"), gtk_cell_renderer_text_new (),
	                                                                                "text", 0,
	                                                                                NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->questions_tree_view), questions_column);


	GtkWidget* choices_tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (choices_tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_table_attach (GTK_TABLE (object), choices_tree_view_scrolled_window,
	                  0, 2, 4, 5,
	                  GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);
	priv->choices_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->choices_store));
	gtk_widget_set_events (priv->choices_tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (priv->choices_tree_view, "button-press-event", G_CALLBACK (choices_tree_view_press_event), object);
	g_signal_connect (priv->choices_tree_view, "key-press-event", G_CALLBACK (choices_tree_view_press_event), object);
	g_signal_connect (priv->choices_tree_view, "row-activated", G_CALLBACK (choices_tree_view_row_activated), object);
	gtk_container_add (GTK_CONTAINER (choices_tree_view_scrolled_window), priv->choices_tree_view);

	GtkTreeViewColumn* choices_column = gtk_tree_view_column_new_with_attributes (_("Choices:"), gtk_cell_renderer_text_new (),
	                                                                              "text", 0,
	                                                                              NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->choices_tree_view), choices_column);


	priv->popup_menu = gtk_menu_new ();

	priv->popup_new_question_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Question"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_question_menu_item);
	g_signal_connect (priv->popup_new_question_menu_item, "activate",
	                  G_CALLBACK (questions_new_object_activate), object);

	priv->popup_new_choice_menu_item = gtk_menu_item_new_with_mnemonic (_("New _Choice"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_new_choice_menu_item);
	g_signal_connect (priv->popup_new_choice_menu_item, "activate",
	                  G_CALLBACK (choices_new_object_activate), object);

	priv->popup_remove_question_menu_item = gtk_menu_item_new_with_mnemonic (_("_Remove"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_remove_question_menu_item);
	g_signal_connect (priv->popup_remove_question_menu_item, "activate",
	                  G_CALLBACK (questions_remove_object_activate), object);

	priv->popup_remove_choice_menu_item = gtk_menu_item_new_with_mnemonic (_("_Remove"));
	gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), priv->popup_remove_choice_menu_item);
	g_signal_connect (priv->popup_remove_choice_menu_item, "activate",
	                  G_CALLBACK (choices_remove_object_activate), object);
}

static void
canvas_lesson_test_word_pool_editor_finalize (GObject *object)
{
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (object);

	if (priv->questions_store)
		g_object_unref (priv->questions_store);

	gtk_widget_destroy (priv->popup_menu);

	G_OBJECT_CLASS (canvas_lesson_test_word_pool_editor_parent_class)->finalize (object);
}

static void
canvas_lesson_test_word_pool_editor_class_init (CanvasLessonTestWordPoolEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectEditorClass* parent_class = CANVAS_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestWordPoolEditorPrivate));

	object_class->finalize = canvas_lesson_test_word_pool_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	CanvasLessonTestWordPoolEditor* editor = CANVAS_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (editor);

	canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (priv->test),
	                                 gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	canvas_editor_window_set_modified (priv->window, TRUE);
	canvas_editor_window_update_tree_store (priv->window);
}

static void
directions_buffer_changed (GtkTextBuffer *textbuffer,
                           gpointer       user_data)
{
	CanvasLessonTestWordPoolEditor* editor = CANVAS_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (editor);

	GtkTextIter start, end;
	gchar* text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (priv->test),
	                                   text);
	g_free (text);
	canvas_editor_window_set_modified (priv->window, TRUE);
}

#if GTK_MAJOR_VERSION >= 3
static void
explain_activate (GObject    *gobject,
                  GParamSpec *pspec,
                  gpointer    user_data)
#else
static void
explain_activate (GtkToggleButton *togglebutton,
                  gpointer         user_data)
#endif
{
	CanvasLessonTestWordPoolEditor* editor = CANVAS_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (editor);

#if GTK_MAJOR_VERSION >= 3
	gboolean active = gtk_switch_get_active (GTK_SWITCH (priv->explain_switch));
#else
	gboolean active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (priv->explain_switch));
	gtk_button_set_label (GTK_BUTTON (priv->explain_switch), active? GTK_STOCK_YES:GTK_STOCK_NO);
#endif

	if (active != canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (priv->test)))
	{
		canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (priv->test),
		                                active);
		canvas_editor_window_set_modified (priv->window, TRUE);
	}
}


CanvasLessonTestWordPoolEditor*
canvas_lesson_test_word_pool_editor_new (CanvasEditorWindow* window, CanvasLessonTestWordPool *test)
{
	CanvasLessonTestWordPoolEditor* editor = g_object_new (CANVAS_TYPE_LESSON_TEST_WORD_POOL_EDITOR, NULL);
	CanvasLessonTestWordPoolEditorPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_EDITOR_PRIVATE (editor);

	priv->window = window;
	priv->test = test;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    canvas_lesson_element_get_title (CANVAS_LESSON_ELEMENT (test)));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	GtkTextBuffer* buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->directions_view));
	gtk_text_buffer_set_text (buffer,
	                          canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (test)), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (directions_buffer_changed), editor);

#if GTK_MAJOR_VERSION >= 3
	gtk_switch_set_active (GTK_SWITCH (priv->explain_switch),
	                       canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (test)));
	g_signal_connect (priv->explain_switch, "notify::active",
	                  G_CALLBACK (explain_activate), editor);
#else
	g_signal_connect (priv->explain_switch, "toggled",
	                  G_CALLBACK (explain_activate), editor);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->explain_switch),
	                              canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (test)));
#endif

	update_questions_tree_view (editor);
	update_choices_tree_view (editor);

	return editor;
}
