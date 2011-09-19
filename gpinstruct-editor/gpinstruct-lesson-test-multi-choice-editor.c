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
#include "gpinstruct-editor/gpinstruct-editor.h"

struct _GPInstructLessonTestMultiChoiceEditorPrivate
{
	GPInstructEditorWindow* window;
	GPInstructLessonTestMultiChoice* test;

	GtkWidget* title_label;
	GtkWidget* title_entry;

	GtkWidget* directions_label;
	GtkWidget* directions_view;

	GtkWidget* explain_label;
	GtkWidget* explain_switch;

	GtkWidget* questions_tree_view;
	GtkListStore* questions_store;

	GtkWidget* choices_tree_view;
	GtkListStore* choices_store;
};

#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, GPInstructLessonTestMultiChoiceEditorPrivate))


enum
{
	TITLE_COLUMN,
	DATA_COLUMN,
	N_COLUMNS
};

static void
update_questions_tree_view (GPInstructLessonTestMultiChoiceEditor* editor)
{
	gtk_list_store_clear (editor->priv->questions_store);

	GtkTreeIter iterQuestion;
	const gchar* text;

	GList* questions = gpinstruct_lesson_test_multi_choice_get_questions (editor->priv->test);
	GList* curr_questions = questions;

	while (curr_questions)
	{
		GPInstructLessonTestMultiChoiceQuestion* question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (curr_questions->data);

		text = gpinstruct_lesson_test_multi_choice_question_get_text (question);
		gtk_list_store_append (editor->priv->questions_store, &iterQuestion);
		gtk_list_store_set (editor->priv->questions_store, &iterQuestion,
		                    TITLE_COLUMN, (text != NULL && *text != '\0')? text:_("(Empty Question)"),
		                    DATA_COLUMN, question,
		                    -1);

		curr_questions = curr_questions->next;
	}

	g_list_free (questions);
}


static void
questions_tree_view_row_activated (GtkTreeView       *tree_view,
                                   GtkTreePath       *path,
                                   GtkTreeViewColumn *column,
                                   gpointer           user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GPInstructObject* object;
	GtkTreeIter iter;
	GtkWidget *scrolled_window, *text_view, *explanation_view, *answer_combobox;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (editor->priv->questions_store), &iter, path))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (editor->priv->questions_store), &iter,
		                    DATA_COLUMN, &object,
		                    -1);
		if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (object))
		{
			GPInstructLessonTestMultiChoiceQuestion* question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (object);
			guint choices_num = gpinstruct_lesson_test_multi_choice_question_get_choices_length (question);

			GtkWidget* dialog = gtk_dialog_new_with_buttons (_("Question Properties"),
			                                                 GTK_WINDOW (editor->priv->window),
			                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
			                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			                                                 NULL);
			GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
			gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 300);

			scrolled_window = gtk_scrolled_window_new (NULL, NULL);
			gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
			text_view = gtk_text_view_new ();
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD_CHAR);
			gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
			                          gpinstruct_lesson_test_multi_choice_question_get_text (question),
			                          -1);
			gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Text:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    scrolled_window,
			                    TRUE, TRUE, 0);

			scrolled_window = gtk_scrolled_window_new (NULL, NULL);
			gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
			explanation_view = gtk_text_view_new ();
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (explanation_view), GTK_WRAP_WORD_CHAR);
			gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (explanation_view)),
			                          gpinstruct_lesson_test_multi_choice_question_get_explanation (question),
			                          -1);
			gtk_container_add (GTK_CONTAINER (scrolled_window), explanation_view);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Explanation:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    scrolled_window,
			                    TRUE, TRUE, 0);

			if (choices_num)
			{
				answer_combobox = gtk_combo_box_new_with_model (GTK_TREE_MODEL (editor->priv->choices_store));
				GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
				gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (answer_combobox), renderer, TRUE);
				gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (answer_combobox), renderer,
				                                "text", TITLE_COLUMN,
				                                NULL);
				gtk_combo_box_set_active (GTK_COMBO_BOX (answer_combobox),
				                          gpinstruct_lesson_test_multi_choice_question_get_answer (question));
				gtk_box_pack_start (GTK_BOX (content_area),
				                    gtk_label_new (_("Answer:")),
				                    FALSE, TRUE, 0);
				gtk_box_pack_start (GTK_BOX (content_area),
				                    answer_combobox,
				                    FALSE, TRUE, 0);
			}

			gtk_widget_show_all (content_area);
			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				GtkTextIter start, end;
				gchar* text;

				gtk_text_buffer_get_bounds (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
				                            &start, &end);
				text = gtk_text_iter_get_text (&start, &end);
				gpinstruct_lesson_test_multi_choice_question_set_text (question, text);
				g_free (text);

				gtk_text_buffer_get_bounds (gtk_text_view_get_buffer (GTK_TEXT_VIEW (explanation_view)),
				                            &start, &end);
				text = gtk_text_iter_get_text (&start, &end);
				gpinstruct_lesson_test_multi_choice_question_set_explanation (question, text);
				g_free (text);

				if (choices_num)
					gpinstruct_lesson_test_multi_choice_question_set_answer (question,
					                                                         gtk_combo_box_get_active (GTK_COMBO_BOX (answer_combobox)));

				update_questions_tree_view (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data));
				gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
				
				gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (editor->priv->questions_store),
				                               &iter, NULL, gtk_tree_path_get_indices (path)[0]);
				gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view)), &iter);
			}

			gtk_widget_destroy (dialog);
		}
	}
}


static void
questions_add_button_clicked (GtkButton *button,
                              gpointer   user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GPInstructLessonTestMultiChoiceQuestion* question;
	GtkTreeIter iter;

	question = gpinstruct_lesson_test_multi_choice_question_new ();
	gpinstruct_lesson_test_multi_choice_add_question (editor->priv->test, question);

	gtk_list_store_append (editor->priv->questions_store, &iter);
	gtk_list_store_set (editor->priv->questions_store, &iter,
	                    TITLE_COLUMN, _("(Empty Question)"),
	                    DATA_COLUMN, question,
	                    -1);

	gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->questions_tree_view)),
	                                &iter);

	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
}


static void
questions_remove_button_clicked (GtkButton *button,
                                 gpointer   user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GtkTreeSelection *selection;
	GtkTreeIter iter, iterSel;
	GtkTreePath *path;
	gboolean select = FALSE;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->questions_tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		iterSel = iter;
		if (gtk_tree_model_iter_next (GTK_TREE_MODEL (editor->priv->questions_store), &iterSel))
			select = TRUE;
		else
		{
			iterSel = iter;
			if (gtk_tree_model_iter_previous (GTK_TREE_MODEL (editor->priv->questions_store), &iterSel))
				select = TRUE;
		}

		if (select)
			gtk_tree_selection_select_iter (selection, &iterSel);

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (editor->priv->questions_store), &iter);

		gpinstruct_lesson_test_multi_choice_remove_question (editor->priv->test, gtk_tree_path_get_indices (path)[0]);

		gtk_list_store_remove (editor->priv->questions_store, &iter);

		gtk_tree_path_free (path);

		gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	}
}


static gpointer
questions_tree_view_get_current_data (GPInstructLessonTestMultiChoiceEditor* editor)
{
	GPInstructObject* object;
	GtkTreeSelection *selection;
	GtkTreeIter iter;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->questions_tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (editor->priv->questions_store), &iter,
		                    DATA_COLUMN, &object,
		                    -1);
		return object;
	}

	return NULL;
}


static void
update_choices_tree_view (GPInstructLessonTestMultiChoiceEditor* editor)
{
	gtk_list_store_clear (editor->priv->choices_store);

	GtkTreeIter iterChoice;
	const gchar* text;

	GPInstructLessonTestMultiChoiceQuestion* question = questions_tree_view_get_current_data (editor);

	if (question)
	{
		GList* choices = gpinstruct_lesson_test_multi_choice_question_get_choices (question);
		GList* curr_choices = choices;

		while (curr_choices)
		{
			text = curr_choices->data;

			gtk_list_store_append (editor->priv->choices_store, &iterChoice);
			gtk_list_store_set (editor->priv->choices_store, &iterChoice,
			                    TITLE_COLUMN, (text != NULL && *text != '\0')? text:_("(Empty Choice)"),
			                    DATA_COLUMN, NULL,
			                    -1);

			curr_choices = curr_choices->next;
		}

		g_list_free (choices);
	}
}


static void
choices_tree_view_row_activated (GtkTreeView       *tree_view,
                                 GtkTreePath       *path,
                                 GtkTreeViewColumn *column,
                                 gpointer           user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GPInstructObject* object;
	GtkTreeIter iter;
	GtkWidget *scrolled_window, *text_view;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (editor->priv->choices_store), &iter, path))
	{
		object = questions_tree_view_get_current_data (editor);
		if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (object))
		{
			guint choice = gtk_tree_path_get_indices (path)[0];
			GPInstructLessonTestMultiChoiceQuestion* question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (object);

			GtkWidget* dialog = gtk_dialog_new_with_buttons (_("Choice Properties"),
			                                                 GTK_WINDOW (editor->priv->window),
			                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
			                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			                                                 NULL);
			GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
			gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 300);

			scrolled_window = gtk_scrolled_window_new (NULL, NULL);
			gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
			text_view = gtk_text_view_new ();
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD_CHAR);
			gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
			                          gpinstruct_lesson_test_multi_choice_question_get_choice (question, choice),
			                          -1);
			gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Text:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    scrolled_window,
			                    TRUE, TRUE, 0);

			gtk_widget_show_all (content_area);
			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				GtkTextIter start, end;
				gtk_text_buffer_get_bounds (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
				                            &start, &end);
				gchar* text = gtk_text_iter_get_text (&start, &end);
				gpinstruct_lesson_test_multi_choice_question_set_choice (question, choice, text);
				g_free (text);

				update_choices_tree_view (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data));
				gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);

				gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (editor->priv->choices_store),
					                           &iter, NULL, choice);
				gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view)),
					                            &iter);
			}

			gtk_widget_destroy (dialog);
		}
	}
}


static void
choices_add_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GPInstructLessonTestMultiChoiceQuestion* question;
	GtkTreeSelection *selection;
	GtkTreeIter iter;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->questions_tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (editor->priv->questions_store), &iter,
		                    DATA_COLUMN, &question,
		                    -1);

		gpinstruct_lesson_test_multi_choice_question_add_choice (question, "");

		gtk_list_store_append (editor->priv->choices_store, &iter);
		gtk_list_store_set (editor->priv->choices_store, &iter,
		                    TITLE_COLUMN, _("(Empty Choice)"),
		                    DATA_COLUMN, NULL,
		                    -1);

		gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->choices_tree_view)),
			                            &iter);

		gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	}
}


static void
choices_remove_button_clicked (GtkButton *button,
                               gpointer   user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GPInstructLessonTestMultiChoiceQuestion* question;
	GtkTreeSelection *selection;
	GtkTreeIter iter, iterSel;
	GtkTreePath *path;
	gboolean select = FALSE;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->questions_tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (editor->priv->questions_store), &iter,
		                    DATA_COLUMN, &question,
		                    -1);

		selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->priv->choices_tree_view));
		if (gtk_tree_selection_get_selected (selection, NULL, &iter))
		{
			iterSel = iter;
			if (gtk_tree_model_iter_next (GTK_TREE_MODEL (editor->priv->choices_store), &iterSel))
				select = TRUE;
			else
			{
				iterSel = iter;
				if (gtk_tree_model_iter_previous (GTK_TREE_MODEL (editor->priv->choices_store), &iterSel))
					select = TRUE;
			}

			if (select)
				gtk_tree_selection_select_iter (selection, &iterSel);

			path = gtk_tree_model_get_path (GTK_TREE_MODEL (editor->priv->choices_store), &iter);

			gpinstruct_lesson_test_multi_choice_question_remove_choice (question, gtk_tree_path_get_indices (path)[0]);

			gtk_list_store_remove (editor->priv->choices_store, &iter);

			gtk_tree_path_free (path);

			gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
		}
	}
}


static void
questions_tree_selection_changed (GtkTreeSelection *selection,
                                  gpointer          user_data)
{
	update_choices_tree_view (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data));
}


G_DEFINE_TYPE (GPInstructLessonTestMultiChoiceEditor, gpinstruct_lesson_test_multi_choice_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_test_multi_choice_editor_init (GPInstructLessonTestMultiChoiceEditor *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_PRIVATE (object);

	object->priv->questions_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
	object->priv->choices_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);

	object->priv->title_label = gtk_label_new (_("Title:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->title_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	object->priv->title_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (object), object->priv->title_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	object->priv->directions_label = gtk_label_new (_("Directions:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->directions_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	GtkWidget* directions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (directions_view_scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	object->priv->directions_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (directions_view_scrolled_window), object->priv->directions_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (object->priv->directions_view), GTK_WRAP_WORD_CHAR);
	gtk_table_attach (GTK_TABLE (object), directions_view_scrolled_window,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);

	object->priv->explain_label = gtk_label_new (_("Explain:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->explain_label,
	                  0, 1, 2, 3,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
#if GTK_MAJOR_VERSION >= 3
	object->priv->explain_switch = gtk_switch_new ();
#else
	object->priv->explain_switch = gtk_toggle_button_new_with_label (GTK_STOCK_NO);
	gtk_button_set_use_stock (GTK_BUTTON (object->priv->explain_switch), TRUE);
#endif
	gtk_table_attach (GTK_TABLE (object), object->priv->explain_switch,
	                  1, 2, 2, 3,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);


	GtkWidget* questions_hbox = gtk_hbox_new (FALSE, 3);
	gtk_table_attach (GTK_TABLE (object), questions_hbox,
	                  0, 2, 3, 4,
	                  GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);

	GtkWidget* questions_tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (questions_tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (questions_hbox), questions_tree_view_scrolled_window, TRUE, TRUE, 0);

	object->priv->questions_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->questions_store));
	gtk_widget_set_events (object->priv->questions_tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (object->priv->questions_tree_view, "row-activated", G_CALLBACK (questions_tree_view_row_activated), object);
	g_signal_connect (gtk_tree_view_get_selection (GTK_TREE_VIEW (object->priv->questions_tree_view)), "changed", G_CALLBACK (questions_tree_selection_changed), object);
	gtk_container_add (GTK_CONTAINER (questions_tree_view_scrolled_window), object->priv->questions_tree_view);

	GtkTreeViewColumn* questions_column = gtk_tree_view_column_new_with_attributes (_("Questions:"), gtk_cell_renderer_text_new (),
	                                                                                "text", 0,
	                                                                                NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (object->priv->questions_tree_view), questions_column);

	GtkWidget* questions_buttonbox = gtk_vbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (questions_buttonbox), GTK_BUTTONBOX_START);
	gtk_box_pack_start (GTK_BOX (questions_hbox), questions_buttonbox, FALSE, TRUE, 0);

	GtkWidget* questions_add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	g_signal_connect (questions_add_button, "clicked", G_CALLBACK (questions_add_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (questions_buttonbox), questions_add_button, FALSE, TRUE, 0);

	GtkWidget* questions_remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	g_signal_connect (questions_remove_button, "clicked", G_CALLBACK (questions_remove_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (questions_buttonbox), questions_remove_button, FALSE, TRUE, 0);


	GtkWidget* choices_hbox = gtk_hbox_new (FALSE, 3);
	gtk_table_attach (GTK_TABLE (object), choices_hbox,
	                  0, 2, 4, 5,
	                  GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);

	GtkWidget* choices_tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (choices_tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (choices_hbox), choices_tree_view_scrolled_window, TRUE, TRUE, 0);

	object->priv->choices_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->choices_store));
	gtk_widget_set_events (object->priv->choices_tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (object->priv->choices_tree_view, "row-activated", G_CALLBACK (choices_tree_view_row_activated), object);
	gtk_container_add (GTK_CONTAINER (choices_tree_view_scrolled_window), object->priv->choices_tree_view);

	GtkTreeViewColumn* choices_column = gtk_tree_view_column_new_with_attributes (_("Choices:"), gtk_cell_renderer_text_new (),
	                                                                              "text", 0,
	                                                                              NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (object->priv->choices_tree_view), choices_column);

	GtkWidget* choices_buttonbox = gtk_vbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (choices_buttonbox), GTK_BUTTONBOX_START);
	gtk_box_pack_start (GTK_BOX (choices_hbox), choices_buttonbox, FALSE, TRUE, 0);

	GtkWidget* choices_add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	g_signal_connect (choices_add_button, "clicked", G_CALLBACK (choices_add_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (choices_buttonbox), choices_add_button, FALSE, TRUE, 0);

	GtkWidget* choices_remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	g_signal_connect (choices_remove_button, "clicked", G_CALLBACK (choices_remove_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (choices_buttonbox), choices_remove_button, FALSE, TRUE, 0);
}

static void
gpinstruct_lesson_test_multi_choice_editor_finalize (GObject *object)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (object);

	if (editor->priv->questions_store)
		g_object_unref (editor->priv->questions_store);

	if (editor->priv->choices_store)
		g_object_unref (editor->priv->choices_store);

	G_OBJECT_CLASS (gpinstruct_lesson_test_multi_choice_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_multi_choice_editor_class_init (GPInstructLessonTestMultiChoiceEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass* parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestMultiChoiceEditorPrivate));

	object_class->finalize = gpinstruct_lesson_test_multi_choice_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (editor->priv->test),
	                                     gtk_entry_get_text (GTK_ENTRY (editor->priv->title_entry)));
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (editor->priv->window, (gpointer)editor->priv->test);
}

static void
directions_buffer_changed (GtkTextBuffer *textbuffer,
                           gpointer       user_data)
{
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

	GtkTextIter start, end;
	gchar* text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_test_set_directions (GPINSTRUCT_LESSON_TEST (editor->priv->test),
	                                       text);
	g_free (text);
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
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
	GPInstructLessonTestMultiChoiceEditor* editor = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR (user_data);

#if GTK_MAJOR_VERSION >= 3
	gboolean active = gtk_switch_get_active (GTK_SWITCH (editor->priv->explain_switch));
#else
	gboolean active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (editor->priv->explain_switch));
	gtk_button_set_label (GTK_BUTTON (editor->priv->explain_switch), active? GTK_STOCK_YES:GTK_STOCK_NO);
#endif

	if (active != gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (editor->priv->test)))
	{
		gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (editor->priv->test),
		                                    active);
		gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	}
}


GPInstructLessonTestMultiChoiceEditor*
gpinstruct_lesson_test_multi_choice_editor_new (GPInstructEditorWindow* window,
                                                GPInstructLessonTestMultiChoice *test)
{
	GPInstructLessonTestMultiChoiceEditor* editor = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, NULL);

	editor->priv->window = window;
	editor->priv->test = test;

	gtk_entry_set_text (GTK_ENTRY (editor->priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	g_signal_connect (editor->priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	GtkTextBuffer* buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->priv->directions_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (test)), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (directions_buffer_changed), editor);

#if GTK_MAJOR_VERSION >= 3
	gtk_switch_set_active (GTK_SWITCH (editor->priv->explain_switch),
	                       gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test)));
	g_signal_connect (editor->priv->explain_switch, "notify::active",
	                  G_CALLBACK (explain_activate), editor);
#else
	g_signal_connect (editor->priv->explain_switch, "toggled",
	                  G_CALLBACK (explain_activate), editor);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (editor->priv->explain_switch),
	                              gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test)));
#endif

	update_questions_tree_view (editor);

	return editor;
}
