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
#include "gpinstruct-editor/gpinstruct-editor.h"

#include "compat/compat-gtk.h"

struct _GPInstructLessonTestWordPoolEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructLessonTestWordPool *test;

	GtkWidget *title_label;
	GtkWidget *title_entry;

	GtkWidget *directions_label;
	GtkWidget *directions_view;

	GtkWidget *explain_label;
	GtkWidget *explain_switch;

	GtkWidget *questions_tree_view;
	GtkListStore *questions_store;

	GtkWidget *choices_tree_view;
	GtkListStore *choices_store;
};

#define GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_EDITOR, GPInstructLessonTestWordPoolEditorPrivate))


enum
{
	TITLE_COLUMN,
	DATA_COLUMN,
	N_COLUMNS
};

static void
update_questions_tree_view (GPInstructLessonTestWordPoolEditor *editor)
{
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	gtk_list_store_clear (priv->questions_store);

	GtkTreeIter iterQuestion;
	const gchar *text;

	GList *questions = gpinstruct_lesson_test_word_pool_get_questions (priv->test);
	GList *curr_questions = questions;

	while (curr_questions)
	{
		GPInstructLessonTestWordPoolQuestion *question = GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION (curr_questions->data);

		text = gpinstruct_lesson_test_word_pool_question_get_text (question);
		gtk_list_store_append (priv->questions_store, &iterQuestion);
		gtk_list_store_set (priv->questions_store, &iterQuestion,
		                    TITLE_COLUMN, (text != NULL && *text != '\0')? text:_("(Empty Question)"),
		                    DATA_COLUMN, question,
		                    -1);

		curr_questions = curr_questions->next;
	}

	g_list_free (questions);

	gtk_tree_view_expand_all (GTK_TREE_VIEW (priv->questions_tree_view));
}


static void
questions_tree_view_row_activated (GtkTreeView       *tree_view,
                                   GtkTreePath       *path,
                                   GtkTreeViewColumn *column,
                                   gpointer           user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GPInstructObject *object;
	GtkTreeIter iter;
	GtkWidget *scrolled_window, *text_view, *explanation_view, *answer_combobox;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->questions_store), &iter, path))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->questions_store), &iter,
		                    DATA_COLUMN, &object,
		                    -1);
		if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL_QUESTION (object))
		{
			GPInstructLessonTestWordPoolQuestion *question = GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION (object);
			guint choices_num = gpinstruct_lesson_test_word_pool_get_choices_length (priv->test);

			GtkWidget *dialog = gtk_dialog_new_with_buttons (_("Question Properties"),
			                                                 GTK_WINDOW (priv->window),
			                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
			                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			                                                 NULL);
			GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
			gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 300);

			scrolled_window = gtk_scrolled_window_new (NULL, NULL);
			gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
			text_view = gtk_text_view_new ();
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD_CHAR);
			gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
			                          gpinstruct_lesson_test_word_pool_question_get_text (question),
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
			                          gpinstruct_lesson_test_word_pool_question_get_explanation (question),
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
				answer_combobox = gtk_combo_box_new_with_model (GTK_TREE_MODEL (priv->choices_store));
				GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
				gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (answer_combobox), renderer, TRUE);
				gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (answer_combobox), renderer,
				                                "text", TITLE_COLUMN,
				                                NULL);
				gtk_combo_box_set_active (GTK_COMBO_BOX (answer_combobox),
				                          gpinstruct_lesson_test_word_pool_question_get_answer (question));
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

				gtk_text_buffer_get_bounds (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
				                            &start, &end);
				gchar *text = gtk_text_iter_get_text (&start, &end);
				gpinstruct_lesson_test_word_pool_question_set_text (question, text);
				g_free (text);

				gtk_text_buffer_get_bounds (gtk_text_view_get_buffer (GTK_TEXT_VIEW (explanation_view)),
				                            &start, &end);
				text = gtk_text_iter_get_text (&start, &end);
				gpinstruct_lesson_test_word_pool_question_set_explanation (question, text);
				g_free (text);

				if (choices_num)
					gpinstruct_lesson_test_word_pool_question_set_answer (question,
					                                                      gtk_combo_box_get_active (GTK_COMBO_BOX (answer_combobox)));

				update_questions_tree_view (GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data));
				gpinstruct_editor_window_set_modified (priv->window, TRUE);

				gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (priv->questions_store),
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
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GPInstructLessonTestWordPoolQuestion *question;
	GtkTreeIter iter;

	question = gpinstruct_lesson_test_word_pool_question_new ();
	gpinstruct_lesson_test_word_pool_add_question (priv->test, question);

	gtk_list_store_append (priv->questions_store, &iter);
	gtk_list_store_set (priv->questions_store, &iter,
	                    TITLE_COLUMN, _("(Empty Question)"),
	                    DATA_COLUMN, question,
	                    -1);

	gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->questions_tree_view)),
	                                &iter);

	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}


static void
questions_remove_button_clicked (GtkButton *button,
                                 gpointer   user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GtkTreeSelection *selection;
	GtkTreeIter iter, iterSel;
	GtkTreePath *path;
	gboolean select = FALSE;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->questions_tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		iterSel = iter;
		if (gtk_tree_model_iter_next (GTK_TREE_MODEL (priv->questions_store), &iterSel))
			select = TRUE;
		else
		{
			iterSel = iter;
			if (gtk_tree_model_iter_previous (GTK_TREE_MODEL (priv->questions_store), &iterSel))
				select = TRUE;
		}

		if (select)
			gtk_tree_selection_select_iter (selection, &iterSel);

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->questions_store), &iter);

		gpinstruct_lesson_test_word_pool_remove_question (priv->test, gtk_tree_path_get_indices (path)[0]);

		gtk_list_store_remove (priv->questions_store, &iter);

		gtk_tree_path_free (path);

		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}


static void
update_choices_tree_view (GPInstructLessonTestWordPoolEditor *editor)
{
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	gtk_list_store_clear (priv->choices_store);

	GtkTreeIter iterChoices;
	const gchar *text;

	GList *choices = gpinstruct_lesson_test_word_pool_get_choices (priv->test);
	GList *curr_choices = choices;

	while (curr_choices)
	{
		text = curr_choices->data;
		gtk_list_store_append (priv->choices_store, &iterChoices);
		gtk_list_store_set (priv->choices_store, &iterChoices,
		                    TITLE_COLUMN, (text != NULL && *text != '\0')? text:_("(Empty Choice)"),
		                    DATA_COLUMN, NULL,
		                    -1);

		curr_choices = curr_choices->next;
	}

	g_list_free (choices);

	gtk_tree_view_expand_all (GTK_TREE_VIEW (priv->choices_tree_view));
}


static void
choices_tree_view_row_activated (GtkTreeView       *tree_view,
                                 GtkTreePath       *path,
                                 GtkTreeViewColumn *column,
                                 gpointer           user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GtkTreeIter iter;
	GtkWidget *scrolled_window, *text_view;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->choices_store), &iter, path))
	{
		guint choice = gtk_tree_path_get_indices (path)[0];

		GtkWidget *dialog = gtk_dialog_new_with_buttons (_("Choice Properties"),
		                                                 GTK_WINDOW (priv->window),
		                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
		                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
		                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
		                                                 NULL);
		GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
		gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 300);

		scrolled_window = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		text_view = gtk_text_view_new ();
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD_CHAR);
		gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
		                          gpinstruct_lesson_test_word_pool_get_choice (priv->test, choice),
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
			gchar *text = gtk_text_iter_get_text (&start, &end);
			gpinstruct_lesson_test_word_pool_set_choice (priv->test, choice, text);
			g_free (text);

			update_choices_tree_view (GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data));
			gpinstruct_editor_window_set_modified (priv->window, TRUE);

			gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (priv->choices_store),
			                               &iter, NULL, choice);
			gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view)),
			                                &iter);
		}

		gtk_widget_destroy (dialog);
	}
}


static void
choices_add_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GtkTreeIter iter;

	gpinstruct_lesson_test_word_pool_add_choice (priv->test, "");

	gtk_list_store_append (priv->choices_store, &iter);
	gtk_list_store_set (priv->choices_store, &iter,
	                    TITLE_COLUMN, _("(Empty Choice)"),
	                    DATA_COLUMN, NULL,
	                    -1);

	gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->choices_tree_view)),
	                                &iter);

	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}


static void
choices_remove_button_clicked (GtkButton *button,
                               gpointer   user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GtkTreeSelection *selection;
	GtkTreeIter iter, iterSel;
	GtkTreePath *path;
	gboolean select = FALSE;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->choices_tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		iterSel = iter;
		if (gtk_tree_model_iter_next (GTK_TREE_MODEL (priv->choices_store), &iterSel))
			select = TRUE;
		else
		{
			iterSel = iter;
			if (gtk_tree_model_iter_previous (GTK_TREE_MODEL (priv->choices_store), &iterSel))
				select = TRUE;
		}

		if (select)
			gtk_tree_selection_select_iter (selection, &iterSel);

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->choices_store), &iter);

		gpinstruct_lesson_test_word_pool_remove_choice (priv->test, gtk_tree_path_get_indices (path)[0]);

		gtk_list_store_remove (priv->choices_store, &iter);

		gtk_tree_path_free (path);

		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}


G_DEFINE_TYPE (GPInstructLessonTestWordPoolEditor, gpinstruct_lesson_test_word_pool_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_test_word_pool_editor_init (GPInstructLessonTestWordPoolEditor *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR_GET_PRIVATE (object);
	GPInstructLessonTestWordPoolEditorPrivate *priv = object->priv;

	priv->questions_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
	priv->choices_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);

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
	GtkWidget *directions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
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
	priv->explain_switch = gtk_switch_new ();
	gtk_table_attach (GTK_TABLE (object), priv->explain_switch,
	                  1, 2, 2, 3,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	GtkWidget *questions_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_table_attach (GTK_TABLE (object), questions_hbox,
	                  0, 2, 3, 4,
	                  GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);

	GtkWidget *questions_tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (questions_tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (questions_hbox), questions_tree_view_scrolled_window, TRUE, TRUE, 0);

	priv->questions_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->questions_store));
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (priv->questions_tree_view), TRUE);
	gtk_widget_set_events (priv->questions_tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (priv->questions_tree_view, "row-activated", G_CALLBACK (questions_tree_view_row_activated), object);
	gtk_container_add (GTK_CONTAINER (questions_tree_view_scrolled_window), priv->questions_tree_view);

	GtkTreeViewColumn *questions_column = gtk_tree_view_column_new_with_attributes (_("Questions:"), gtk_cell_renderer_text_new (),
	                                                                                "text", 0,
	                                                                                NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->questions_tree_view), questions_column);

	GtkWidget *questions_buttonbox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (questions_buttonbox), GTK_BUTTONBOX_START);
	gtk_box_pack_start (GTK_BOX (questions_hbox), questions_buttonbox, FALSE, TRUE, 0);

	GtkWidget *questions_add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	g_signal_connect (questions_add_button, "clicked", G_CALLBACK (questions_add_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (questions_buttonbox), questions_add_button, FALSE, TRUE, 0);

	GtkWidget *questions_remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	g_signal_connect (questions_remove_button, "clicked", G_CALLBACK (questions_remove_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (questions_buttonbox), questions_remove_button, FALSE, TRUE, 0);


	GtkWidget *choices_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_table_attach (GTK_TABLE (object), choices_hbox,
	                  0, 2, 4, 5,
	                  GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);

	GtkWidget *choices_tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (choices_tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (choices_hbox), choices_tree_view_scrolled_window, TRUE, TRUE, 0);

	priv->choices_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->choices_store));
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (priv->choices_tree_view), TRUE);
	gtk_widget_set_events (priv->choices_tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (priv->choices_tree_view, "row-activated", G_CALLBACK (choices_tree_view_row_activated), object);
	gtk_container_add (GTK_CONTAINER (choices_tree_view_scrolled_window), priv->choices_tree_view);

	GtkTreeViewColumn *choices_column = gtk_tree_view_column_new_with_attributes (_("Choices:"), gtk_cell_renderer_text_new (),
	                                                                              "text", 0,
	                                                                              NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->choices_tree_view), choices_column);

	GtkWidget *choices_buttonbox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (choices_buttonbox), GTK_BUTTONBOX_START);
	gtk_box_pack_start (GTK_BOX (choices_hbox), choices_buttonbox, FALSE, TRUE, 0);

	GtkWidget *choices_add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	g_signal_connect (choices_add_button, "clicked", G_CALLBACK (choices_add_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (choices_buttonbox), choices_add_button, FALSE, TRUE, 0);

	GtkWidget *choices_remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	g_signal_connect (choices_remove_button, "clicked", G_CALLBACK (choices_remove_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (choices_buttonbox), choices_remove_button, FALSE, TRUE, 0);
}

static void
gpinstruct_lesson_test_word_pool_editor_finalize (GObject *object)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (object);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	if (priv->questions_store)
		g_object_unref (priv->questions_store);

	if (priv->choices_store)
		g_object_unref (priv->choices_store);

	G_OBJECT_CLASS (gpinstruct_lesson_test_word_pool_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_word_pool_editor_class_init (GPInstructLessonTestWordPoolEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestWordPoolEditorPrivate));

	object_class->finalize = gpinstruct_lesson_test_word_pool_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (priv->test),
	                                     gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->test);
}

static void
directions_buffer_changed (GtkTextBuffer *textbuffer,
                           gpointer       user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_test_set_directions (GPINSTRUCT_LESSON_TEST (priv->test),
	                                       text);
	g_free (text);
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}

static void
explain_activate (GObject    *gobject,
                  GParamSpec *pspec,
                  gpointer    user_data)
{
	GPInstructLessonTestWordPoolEditor *editor = GPINSTRUCT_LESSON_TEST_WORD_POOL_EDITOR (user_data);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	gboolean active = gtk_switch_get_active (GTK_SWITCH (priv->explain_switch));

	if (active != gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (priv->test)))
	{
		gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (priv->test),
		                                    active);
		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}


GPInstructLessonTestWordPoolEditor *
gpinstruct_lesson_test_word_pool_editor_new (GPInstructEditorWindow *window,
                                             GPInstructLessonTestWordPool *test)
{
	GPInstructLessonTestWordPoolEditor *editor = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_EDITOR, NULL);
	GPInstructLessonTestWordPoolEditorPrivate *priv = editor->priv;

	priv->window = window;
	priv->test = test;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->directions_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (test)), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (directions_buffer_changed), editor);

	gtk_switch_set_active (GTK_SWITCH (priv->explain_switch),
	                       gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test)));
	g_signal_connect (priv->explain_switch, "notify::active",
	                  G_CALLBACK (explain_activate), editor);

	update_questions_tree_view (editor);
	update_choices_tree_view (editor);

	return editor;
}
