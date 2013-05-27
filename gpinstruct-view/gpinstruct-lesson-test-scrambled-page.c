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

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-view-private.h"
#include "gtktextbuffermarkup.h"

struct _GPInstructLessonTestScrambledPagePrivate
{
	GPInstructLessonTestScrambled *test;
	GPInstructLessonScore *score;
	GPInstructLog *log;

	guint curr_question;
	guint *questions;

	GtkWidget *vbox;
	GtkWidget *question_textview;
	GtkWidget *scrambled_entry;
	GtkWidget *answer_entry;
};

#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE, GPInstructLessonTestScrambledPagePrivate))



static void
scrambled_show_question (GPInstructLessonTestScrambledPage *page,
                         guint question_num)
{
	GPInstructLessonTestScrambledPagePrivate *priv = page->priv;

	GList *questions = gpinstruct_lesson_test_scrambled_get_questions (priv->test);
	if (question_num < g_list_length (questions))
	{
		priv->curr_question = question_num;

		gtk_entry_set_text (GTK_ENTRY (priv->answer_entry), "");

		GPInstructLessonTestScrambledQuestion *question = g_list_nth_data (questions, priv->questions[question_num]);

		gchar *text = g_strdup_printf ("%d. %s", 1+question_num, gpinstruct_lesson_test_scrambled_question_get_text (question));
		gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->question_textview)), text);
		g_free (text);

		gchar *curr_answer = g_strdup (gpinstruct_lesson_test_scrambled_question_get_answer (question));
		randomize_string (curr_answer);
		gtk_entry_set_text (GTK_ENTRY (priv->scrambled_entry), curr_answer);
		g_free (curr_answer);
	}

	g_list_free (questions);

}

static void
page_reset (GPInstructLessonTestScrambledPage *page,
            gpointer user_data)
{
	GPInstructLessonTestScrambledPagePrivate *priv = page->priv;

	if (priv->log)
		gpinstruct_log_close_test (priv->log);

	g_free (priv->questions);

	guint questions_num = gpinstruct_lesson_test_scrambled_get_questions_length (priv->test);
	priv->questions = random_array (questions_num);

	if (questions_num)
		scrambled_show_question (GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE (page), 0);
}


G_DEFINE_TYPE (GPInstructLessonTestScrambledPage, gpinstruct_lesson_test_scrambled_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_test_scrambled_page_init (GPInstructLessonTestScrambledPage *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_GET_PRIVATE (object);
	GPInstructLessonTestScrambledPagePrivate *priv = object->priv;

	priv->test = NULL;
	priv->score = NULL;
	priv->log = NULL;
	priv->curr_question = 0;
	priv->questions = NULL;
	priv->vbox = NULL;
	priv->question_textview = NULL;
	priv->answer_entry = NULL;
}

static void
gpinstruct_lesson_test_scrambled_page_finalize (GObject *object)
{
	GPInstructLessonTestScrambledPage *page = GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE (object);
	GPInstructLessonTestScrambledPagePrivate *priv = page->priv;

	g_free (priv->questions);

	G_OBJECT_CLASS (gpinstruct_lesson_test_scrambled_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_scrambled_page_class_init (GPInstructLessonTestScrambledPageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass *parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestScrambledPagePrivate));

	object_class->finalize = gpinstruct_lesson_test_scrambled_page_finalize;
}


static gboolean
page_show_next (GPInstructLessonTestScrambledPage *page,
                gpointer user_data)
{
	GPInstructLessonTestScrambledPagePrivate *priv = page->priv;

	GList *questions = gpinstruct_lesson_test_scrambled_get_questions (priv->test);
	guint questions_num = g_list_length (questions);

	if (questions_num == 0)
		return FALSE;

	guint question_id = priv->questions[priv->curr_question];
	GPInstructLessonTestScrambledQuestion *question = GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION (g_list_nth_data (questions, question_id));

	g_list_free (questions);

	gpinstruct_lesson_score_increase_total (priv->score);

	const gchar *answer = gtk_entry_get_text (GTK_ENTRY (priv->answer_entry));
	const gchar *correct_answer =
		gpinstruct_lesson_test_scrambled_question_get_answer (question);

	gboolean explain = gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (priv->test));

	if (priv->log)
		gpinstruct_log_add_string (priv->log,
		                           GPINSTRUCT_LESSON_TEST (priv->test),
		                           question_id, answer);

	if (g_ascii_strcasecmp (answer, correct_answer) == 0)
	{
		gpinstruct_lesson_score_increase_score (priv->score);
		if (explain)
			gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
			                                         GPINSTRUCT_MESSAGE_TYPE_CORRECT);
	}
	else if (explain)
	{
		gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
		                                         GPINSTRUCT_MESSAGE_TYPE_WRONG);

		const gchar *explanation = gpinstruct_lesson_test_scrambled_question_get_explanation (question);

		if (explanation && *explanation != '\0')
			gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
				                                             explanation);
		else
		{
			gchar *explanation = g_strdup_printf ("%s\n\nAnswer: %s",
			                                      gpinstruct_lesson_test_scrambled_question_get_text (question),
			                                      correct_answer);
			gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
				                                         explanation);
			g_free (explanation);
		}
	}

	if (priv->curr_question+1 < questions_num)
	{
		scrambled_show_question (page, priv->curr_question+1);
		return TRUE;
	}
	else
		return FALSE;
}


GPInstructLessonTestScrambledPage *
gpinstruct_lesson_test_scrambled_page_new (GPInstructLessonTestScrambled *test,
                                           GPInstructLessonScore *score,
                                           GPInstructLog *log)
{
	GPInstructLessonTestScrambledPage *page = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE, NULL);
	GPInstructLessonTestScrambledPagePrivate *priv = page->priv;

	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page),
	                                       gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	gpinstruct_lesson_view_page_set_show_back_button (GPINSTRUCT_LESSON_VIEW_PAGE (page), FALSE);

	g_signal_connect (page, "show-next", G_CALLBACK (page_show_next), NULL);
	g_signal_connect (page, "reset", G_CALLBACK (page_reset), NULL);

	priv->test = test;
	priv->score = score;
	priv->log = log;

	priv->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), priv->vbox);

	priv->question_textview = gtk_text_view_new ();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->question_textview), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (priv->question_textview), FALSE);
	gtk_box_pack_start (GTK_BOX (priv->vbox), priv->question_textview, TRUE, TRUE, 3);

	GtkWidget *grid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (grid), GTK_ORIENTATION_VERTICAL);
	gtk_box_pack_start (GTK_BOX (priv->vbox), grid, TRUE, TRUE, 3);

	GtkWidget *label;

	label = gtk_label_new (_("Clue:"));
	gtk_widget_set_hexpand (label, TRUE);
	gtk_widget_set_vexpand (label, TRUE);
	gtk_container_add (GTK_CONTAINER (grid), label);

	priv->scrambled_entry = gtk_entry_new ();
	gtk_entry_set_alignment (GTK_ENTRY (priv->scrambled_entry), 0.5);
	gtk_widget_set_halign (priv->scrambled_entry, GTK_ALIGN_CENTER);
	gtk_widget_set_hexpand (priv->scrambled_entry, TRUE);
	gtk_widget_set_vexpand (priv->scrambled_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (grid), priv->scrambled_entry,
	                         label, GTK_POS_RIGHT, 1, 1);

	label = gtk_label_new (_("Answer:"));
	gtk_widget_set_hexpand (label, TRUE);
	gtk_widget_set_vexpand (label, TRUE);
	gtk_container_add (GTK_CONTAINER (grid), label);

	priv->answer_entry = gtk_entry_new ();
	gtk_entry_set_alignment (GTK_ENTRY (priv->answer_entry), 0.5);
	gtk_widget_set_halign (priv->answer_entry, GTK_ALIGN_CENTER);
	gtk_widget_set_hexpand (priv->answer_entry, TRUE);
	gtk_widget_set_vexpand (priv->answer_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (grid), priv->answer_entry,
	                         label, GTK_POS_RIGHT, 1, 1);

	return page;
}
