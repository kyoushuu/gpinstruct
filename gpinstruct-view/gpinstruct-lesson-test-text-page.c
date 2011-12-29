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

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-view-private.h"
#include "gtktextbuffermarkup.h"

struct _GPInstructLessonTestTextPagePrivate
{
	GPInstructLessonTestText* test;
	GPInstructLessonScore* score;
	GPInstructLog* log;

	guint curr_question;
	guint* questions;

	GtkWidget* vbox;
	GtkWidget* question_textview;
	GtkWidget* answer_entry;
};

#define GPINSTRUCT_LESSON_TEST_TEXT_PAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_PAGE, GPInstructLessonTestTextPagePrivate))



void
text_show_question (GPInstructLessonTestTextPage* page,
                         guint question_num)
{
	GList* questions = gpinstruct_lesson_test_text_get_questions (page->priv->test);
	if (question_num < g_list_length (questions))
	{
		page->priv->curr_question = question_num;

		gtk_entry_set_text (GTK_ENTRY (page->priv->answer_entry), "");

		GPInstructLessonTestTextQuestion* question = g_list_nth_data (questions, page->priv->questions[question_num]);

		gchar* text = g_strdup_printf ("%d. %s", 1+question_num, gpinstruct_lesson_test_text_question_get_text (question));
		gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (page->priv->question_textview)), text);
		g_free (text);
	}

	g_list_free (questions);

}

static void
page_reset (GPInstructLessonTestTextPage* page,
            gpointer user_data)
{
	if (page->priv->log)
		gpinstruct_log_close_test (page->priv->log);

	g_free (page->priv->questions);

	guint questions_num = gpinstruct_lesson_test_text_get_questions_length (page->priv->test);
	page->priv->questions = random_array (questions_num);

	if (questions_num)
		text_show_question (GPINSTRUCT_LESSON_TEST_TEXT_PAGE (page), 0);
}


G_DEFINE_TYPE (GPInstructLessonTestTextPage, gpinstruct_lesson_test_text_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_test_text_page_init (GPInstructLessonTestTextPage *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_TEXT_PAGE_PRIVATE (object);

	object->priv->test = NULL;
	object->priv->score = NULL;
	object->priv->log = NULL;
	object->priv->curr_question = 0;
	object->priv->questions = NULL;
	object->priv->vbox = NULL;
	object->priv->question_textview = NULL;
	object->priv->answer_entry = NULL;
}

static void
gpinstruct_lesson_test_text_page_finalize (GObject *object)
{
	GPInstructLessonTestTextPage* page = GPINSTRUCT_LESSON_TEST_TEXT_PAGE (object);

	g_free (page->priv->questions);

	G_OBJECT_CLASS (gpinstruct_lesson_test_text_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_text_page_class_init (GPInstructLessonTestTextPageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass* parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestTextPagePrivate));

	object_class->finalize = gpinstruct_lesson_test_text_page_finalize;
}


static gboolean
page_show_next (GPInstructLessonTestTextPage* page,
                gpointer user_data)
{
	GList* questions = gpinstruct_lesson_test_text_get_questions (page->priv->test);
	guint questions_num = g_list_length (questions);

	if (questions_num == 0)
		return FALSE;

	guint question_id = page->priv->questions[page->priv->curr_question];
	GPInstructLessonTestTextQuestion* question = GPINSTRUCT_LESSON_TEST_TEXT_QUESTION (g_list_nth_data (questions, question_id));

	g_list_free (questions);

	gpinstruct_lesson_score_increase_total (page->priv->score);

	const gchar* answer = gtk_entry_get_text (GTK_ENTRY (page->priv->answer_entry));
	const gchar* correct_answer =
		gpinstruct_lesson_test_text_question_get_answer (question);

	gboolean explain = gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (page->priv->test));

	if (page->priv->log)
		gpinstruct_log_add_string (page->priv->log,
		                           GPINSTRUCT_LESSON_TEST (page->priv->test),
		                           question_id, answer);

	if (g_ascii_strcasecmp (answer, correct_answer) == 0)
	{
		gpinstruct_lesson_score_increase_score (page->priv->score);
		if (explain)
			gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
			                                         GPINSTRUCT_MESSAGE_TYPE_CORRECT);
	}
	else if (explain)
	{
		gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
		                                         GPINSTRUCT_MESSAGE_TYPE_WRONG);

		const gchar *explanation = gpinstruct_lesson_test_text_question_get_explanation (question);

		if (explanation && *explanation != '\0')
			gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
				                                             explanation);
		else
		{
			gchar *explanation = g_strdup_printf ("%s\n\nAnswer: %s",
			                                      gpinstruct_lesson_test_text_question_get_text (question),
			                                      correct_answer);
			gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
				                                         explanation);
			g_free (explanation);
		}
	}

	if (page->priv->curr_question+1 < questions_num)
	{
		text_show_question (page, page->priv->curr_question+1);
		return TRUE;
	}
	else
		return FALSE;
}


GPInstructLessonTestTextPage*
gpinstruct_lesson_test_text_page_new (GPInstructLessonTestText* test,
                                           GPInstructLessonScore* score,
                                           GPInstructLog* log)
{
	GPInstructLessonTestTextPage* page = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_TEXT_PAGE, NULL);

	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page),
	                                       gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	gpinstruct_lesson_view_page_set_show_back_button (GPINSTRUCT_LESSON_VIEW_PAGE (page), FALSE);

	g_signal_connect (page, "show-next", G_CALLBACK (page_show_next), NULL);
	g_signal_connect (page, "reset", G_CALLBACK (page_reset), NULL);

	page->priv->test = test;
	page->priv->score = score;
	page->priv->log = log;

	page->priv->vbox = gtk_vbox_new (FALSE, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), page->priv->vbox);

	page->priv->question_textview = gtk_text_view_new ();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (page->priv->question_textview), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (page->priv->question_textview), FALSE);
	gtk_box_pack_start (GTK_BOX (page->priv->vbox), page->priv->question_textview, TRUE, TRUE, 3);

	page->priv->answer_entry = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (page->priv->vbox), page->priv->answer_entry, FALSE, TRUE, 3);

	return page;
}
