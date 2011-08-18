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

#include <gtk/gtk.h>

#include "canvas.h"
#include "canvas-view.h"
#include "canvas-view-private.h"
#include "gtktextbuffermarkup.h"

struct _CanvasLessonTestWordPoolPagePrivate
{
	CanvasLessonTestWordPool* test;
	CanvasLessonScore* score;

	guint curr_question;
	guint* questions;
	guint* choices;

	GtkWidget* vbox;
	GtkListStore* store;
	GtkWidget* question_textview;
	GtkWidget* choices_treeview;
};

#define CANVAS_LESSON_TEST_WORD_POOL_PAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_WORD_POOL_PAGE, CanvasLessonTestWordPoolPagePrivate))



void
word_pool_show_question (CanvasLessonTestWordPoolPage* page, guint question_num)
{
	GList* questions = canvas_lesson_test_word_pool_get_questions (page->priv->test);
	if (question_num < g_list_length (questions))
	{
		page->priv->curr_question = question_num;

		GtkTreePath *path = gtk_tree_path_new_from_string ("0");
		gtk_tree_selection_select_path (gtk_tree_view_get_selection (GTK_TREE_VIEW (page->priv->choices_treeview)), path);
		gtk_tree_path_free (path);

		CanvasLessonTestWordPoolQuestion* question = g_list_nth_data (questions, page->priv->questions[question_num]);

		gchar* text = g_strdup_printf ("%d. %s", 1+question_num, canvas_lesson_test_word_pool_question_get_text (question));
		gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (page->priv->question_textview)), text);
		g_free (text);
	}

	g_list_free (questions);

}

static void
page_reset (CanvasLessonTestWordPoolPage* page, gpointer user_data)
{
	if (page->priv->questions)
		g_free (page->priv->questions);
	page->priv->questions = random_array (canvas_lesson_test_word_pool_get_questions_length (page->priv->test));

	GList* choices = canvas_lesson_test_word_pool_get_choices (page->priv->test);
	guint length = g_list_length (choices);

	if (page->priv->choices)
		g_free (page->priv->choices);
	page->priv->choices = random_array (length);

	gtk_list_store_clear (page->priv->store);

	gchar* text;
	int i;
	for (i = 0; i<length; i++)
	{
		text = g_strdup_printf ("%c. %s", 'a'+i, (gchar*)g_list_nth_data (choices, page->priv->choices[i]));
		gtk_list_store_insert_with_values (page->priv->store, NULL, -1, 0, text, -1);
		g_free (text);
	}

	g_list_free (choices);

	word_pool_show_question (CANVAS_LESSON_TEST_WORD_POOL_PAGE (page), 0);
}


G_DEFINE_TYPE (CanvasLessonTestWordPoolPage, canvas_lesson_test_word_pool_page, CANVAS_TYPE_LESSON_VIEW_PAGE);

static void
canvas_lesson_test_word_pool_page_init (CanvasLessonTestWordPoolPage *object)
{
	object->priv = CANVAS_LESSON_TEST_WORD_POOL_PAGE_PRIVATE (object);

	object->priv->test = NULL;
	object->priv->score = NULL;
	object->priv->curr_question = 0;
	object->priv->questions = NULL;
	object->priv->choices = NULL;
	object->priv->vbox = NULL;
	object->priv->store = gtk_list_store_new (1, G_TYPE_STRING);
	object->priv->question_textview = NULL;
	object->priv->choices_treeview = NULL;
}

static void
canvas_lesson_test_word_pool_page_finalize (GObject *object)
{
	CanvasLessonTestWordPoolPage* page = CANVAS_LESSON_TEST_WORD_POOL_PAGE (object);

	if (page->priv->questions)
		g_free (page->priv->questions);

	if (page->priv->choices)
		g_free (page->priv->choices);

	if (page->priv->store)
		g_object_unref (page->priv->store);

	G_OBJECT_CLASS (canvas_lesson_test_word_pool_page_parent_class)->finalize (object);
}

static void
canvas_lesson_test_word_pool_page_class_init (CanvasLessonTestWordPoolPageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonViewPageClass* parent_class = CANVAS_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestWordPoolPagePrivate));

	object_class->finalize = canvas_lesson_test_word_pool_page_finalize;
}


static gboolean
page_show_next (CanvasLessonTestWordPoolPage* page, gpointer user_data)
{
	canvas_lesson_score_increase_total (page->priv->score);

	GList* questions = canvas_lesson_test_word_pool_get_questions (page->priv->test);
	CanvasLessonTestWordPoolQuestion* question = CANVAS_LESSON_TEST_WORD_POOL_QUESTION (g_list_nth_data (questions, page->priv->questions[page->priv->curr_question]));
	guint questions_num = g_list_length (questions);
	g_list_free (questions);

	GtkTreeSelection* tsel = gtk_tree_view_get_selection (GTK_TREE_VIEW (page->priv->choices_treeview));
	GtkTreeIter iter;
	GtkTreeModel* tm;
	gint selected;

	if (gtk_tree_selection_get_selected (tsel, &tm, &iter))
		selected = gtk_tree_path_get_indices (gtk_tree_model_get_path (tm, &iter))[0];

	guint correct_choice = -1;
	guint answer = canvas_lesson_test_word_pool_question_get_answer (question);
	guint choices = canvas_lesson_test_word_pool_get_choices_length (page->priv->test);
	int i;
	for (i=0; i<choices; i++)
	{
		if (page->priv->choices[i] == answer)
		{
			correct_choice = i;
			break;
		}
	}

	if (correct_choice >= 0)
	{
		gboolean explain = canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (page->priv->test));

		if (selected == correct_choice)
		{
			canvas_lesson_score_increase_score (page->priv->score);
			if (explain)
				canvas_lesson_view_page_set_message (CANVAS_LESSON_VIEW_PAGE (page),
				                                     CANVAS_MESSAGE_TYPE_CORRECT);
		}
		else if (explain)
		{
			canvas_lesson_view_page_set_message (CANVAS_LESSON_VIEW_PAGE (page),
			                                     CANVAS_MESSAGE_TYPE_WRONG);
			canvas_lesson_view_page_set_explanation (CANVAS_LESSON_VIEW_PAGE (page),
			                                         canvas_lesson_test_word_pool_question_get_explanation (question));
		}
	}
	else
		g_critical ("Error: Answer not in choices.");

	if (page->priv->curr_question+1 < questions_num)
	{
		word_pool_show_question (page, page->priv->curr_question+1);
		return TRUE;
	}
	else
		return FALSE;
}


CanvasLessonTestWordPoolPage*
canvas_lesson_test_word_pool_page_new (CanvasLessonTestWordPool* test,
                                       CanvasLessonScore* score)
{
	CanvasLessonTestWordPoolPage* page = g_object_new (CANVAS_TYPE_LESSON_TEST_WORD_POOL_PAGE, NULL);

	canvas_lesson_view_page_set_title (CANVAS_LESSON_VIEW_PAGE (page),
	                                   canvas_lesson_element_get_title (CANVAS_LESSON_ELEMENT (test)));
	canvas_lesson_view_page_set_show_back_button (CANVAS_LESSON_VIEW_PAGE (page), FALSE);

	g_signal_connect (page, "show-next", G_CALLBACK (page_show_next), NULL);
	g_signal_connect (page, "reset", G_CALLBACK (page_reset), NULL);

	page->priv->test = test;
	page->priv->score = score;

	page->priv->vbox = gtk_vbox_new (FALSE, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), page->priv->vbox);

	page->priv->question_textview = gtk_text_view_new ();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (page->priv->question_textview), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (page->priv->question_textview), FALSE);
	gtk_box_pack_start (GTK_BOX (page->priv->vbox), page->priv->question_textview, TRUE, TRUE, 3);

	GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
	                                GTK_POLICY_AUTOMATIC,
	                                GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (page->priv->vbox), scrolled_window, TRUE, TRUE, 3);

	page->priv->choices_treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (page->priv->store));
	gtk_container_add (GTK_CONTAINER (scrolled_window), page->priv->choices_treeview);

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ("Choices", renderer, "text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (page->priv->choices_treeview), column);

	return page;
}
