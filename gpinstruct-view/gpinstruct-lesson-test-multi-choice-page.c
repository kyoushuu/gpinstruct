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

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-view-private.h"
#include "gtktextbuffermarkup.h"

struct _GPInstructLessonTestMultiChoicePagePrivate
{
	GPInstructLessonTestMultiChoice *test;
	GPInstructLessonScore *score;
	GPInstructLog *log;

	guint curr_question;
	guint *questions;
	guint *choices;
	GList *choice_buttons;

	GtkWidget *vbox;
	GtkWidget *question_textview;
	GtkWidget *choices_vbox;
};

#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_PAGE_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_PAGE, GPInstructLessonTestMultiChoicePagePrivate))



static void
multi_choice_show_question (GPInstructLessonTestMultiChoicePage *page,
                            guint question_num)
{
	GPInstructLessonTestMultiChoicePagePrivate *priv = page->priv;

	GList *questions = gpinstruct_lesson_test_multi_choice_get_questions (priv->test);
	if (question_num < g_list_length (questions))
	{
		priv->curr_question = question_num;

		gchar *text;
		int i;

		GPInstructLessonTestMultiChoiceQuestion *question = g_list_nth_data (questions, priv->questions[question_num]);

		text = g_strdup_printf ("%d. %s", 1+question_num, gpinstruct_lesson_test_multi_choice_question_get_text (question));
		gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->question_textview)), text);
		g_free (text);

		if (priv->choices_vbox)
			gtk_widget_destroy (priv->choices_vbox);

		priv->choices_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
		gtk_box_pack_start (GTK_BOX (priv->vbox), priv->choices_vbox, FALSE, TRUE, 3);

		GtkWidget *choice_radio_button;
		GtkWidget *last_radio_button = NULL;

		if (priv->choice_buttons)
		{
			g_list_free (priv->choice_buttons);
			priv->choice_buttons = NULL;
		}

		GList *choices = gpinstruct_lesson_test_multi_choice_question_get_choices (question);
		guint length = g_list_length (choices);

		g_free (priv->choices);
		priv->choices = random_array (gpinstruct_lesson_test_multi_choice_question_get_choices_length (question));

		for (i = 0; i<length; i++)
		{
			text = g_strdup_printf ("_%c. %s", 'a'+i, (gchar*) g_list_nth_data (choices, priv->choices[i]));
			choice_radio_button = gtk_radio_button_new_with_mnemonic_from_widget (GTK_RADIO_BUTTON (last_radio_button),
			                                                                      text);
			g_free (text);
			gtk_box_pack_start (GTK_BOX (priv->choices_vbox), choice_radio_button, FALSE, TRUE, 3);

			priv->choice_buttons = g_list_append (priv->choice_buttons, choice_radio_button);

			last_radio_button = choice_radio_button;
		}

		if (priv->choice_buttons)
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->choice_buttons->data), TRUE);

		g_list_free (choices);

		gtk_widget_show_all (priv->choices_vbox);
	}

	g_list_free (questions);

}

static void
page_reset (GPInstructLessonTestMultiChoicePage *page,
            gpointer user_data)
{
	GPInstructLessonTestMultiChoicePagePrivate *priv = page->priv;

	if (priv->log)
		gpinstruct_log_close_test (priv->log);

	g_free (priv->questions);

	guint questions_num = gpinstruct_lesson_test_multi_choice_get_questions_length (priv->test);
	priv->questions = random_array (questions_num);
	if (questions_num)
		multi_choice_show_question (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_PAGE (page), 0);
}



G_DEFINE_TYPE (GPInstructLessonTestMultiChoicePage, gpinstruct_lesson_test_multi_choice_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_test_multi_choice_page_init (GPInstructLessonTestMultiChoicePage *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_PAGE_GET_PRIVATE (object);
	GPInstructLessonTestMultiChoicePagePrivate *priv = object->priv;

	priv->test = NULL;
	priv->score = NULL;
	priv->log = NULL;
	priv->curr_question = 0;
	priv->questions = NULL;
	priv->choices = NULL;
	priv->choice_buttons = NULL;
	priv->vbox = NULL;
	priv->question_textview = NULL;
	priv->choices_vbox = NULL;
}

static void
gpinstruct_lesson_test_multi_choice_page_finalize (GObject *object)
{
	GPInstructLessonTestMultiChoicePage *page = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_PAGE (object);
	GPInstructLessonTestMultiChoicePagePrivate *priv = page->priv;

	g_free (priv->questions);
	g_free (priv->choices);

	if (priv->choice_buttons)
		g_list_free (priv->choice_buttons);

	G_OBJECT_CLASS (gpinstruct_lesson_test_multi_choice_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_multi_choice_page_class_init (GPInstructLessonTestMultiChoicePageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass *parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestMultiChoicePagePrivate));

	object_class->finalize = gpinstruct_lesson_test_multi_choice_page_finalize;
}


static gboolean
page_show_next (GPInstructLessonTestMultiChoicePage *page,
                gpointer user_data)
{
	GPInstructLessonTestMultiChoicePagePrivate *priv = page->priv;

	GList *questions = gpinstruct_lesson_test_multi_choice_get_questions (priv->test);
	guint questions_num = g_list_length (questions);

	if (questions_num == 0)
		return FALSE;

	guint question_id = priv->questions[priv->curr_question];
	GPInstructLessonTestMultiChoiceQuestion *question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (g_list_nth_data (questions, question_id));

	g_list_free (questions);

	gpinstruct_lesson_score_increase_total (priv->score);

	guint correct_choice = -1;
	guint answer = gpinstruct_lesson_test_multi_choice_question_get_answer (question);
	guint choices = gpinstruct_lesson_test_multi_choice_question_get_choices_length (question);
	int i;
	for (i=0; i<choices; i++)
	{
		if (priv->choices[i] == answer)
		{
			correct_choice = i;
			break;
		}
	}

	GList *choice_buttons = priv->choice_buttons;
	guint choice = -1;

	for (i=0; choice_buttons; i++, choice_buttons = choice_buttons->next)
	{
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (choice_buttons->data)))
		{
			choice = i;
			break;
		}
	}

	if (correct_choice >= 0)
	{
		gboolean explain = gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (priv->test));

		if (priv->log)
			gpinstruct_log_add_choice (priv->log,
			                           GPINSTRUCT_LESSON_TEST (priv->test),
			                           question_id, priv->choices[choice]);

		if (choice == correct_choice)
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

			const gchar *explanation = gpinstruct_lesson_test_multi_choice_question_get_explanation (question);
			if (explanation && *explanation != '\0')
				gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
				                                             explanation);
			else
			{
				GString *explanation_answer = g_string_new (gpinstruct_lesson_test_multi_choice_question_get_text (question));

				GList *choices = gpinstruct_lesson_test_multi_choice_question_get_choices (question);
				guint length = g_list_length (choices);
				for (i = 0; i<length; i++)
				{
					g_string_append_printf (explanation_answer,
							                correct_choice==i? "\n<i>%c. %s</i>":"\n%c. %s",
							                'a'+i,
							                (gchar*) g_list_nth_data (choices, priv->choices[i]));
				}
				g_list_free (choices);

				gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
						                                     explanation_answer->str);

				g_string_free (explanation_answer, TRUE);
			}
		}
	}
	else
		g_critical ("Error: Answer not in choices.");

	if (priv->curr_question+1 < questions_num)
	{
		multi_choice_show_question (page, priv->curr_question+1);
		return TRUE;
	}
	else
		return FALSE;
}


GPInstructLessonTestMultiChoicePage *
gpinstruct_lesson_test_multi_choice_page_new (GPInstructLessonTestMultiChoice *test,
                                              GPInstructLessonScore *score,
                                              GPInstructLog *log)
{
	GPInstructLessonTestMultiChoicePage *page = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_PAGE, NULL);
	GPInstructLessonTestMultiChoicePagePrivate *priv = page->priv;

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

	GtkWidget *question_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (question_scrolledwindow),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (priv->vbox), question_scrolledwindow, TRUE, TRUE, 0);

	priv->question_textview = gtk_text_view_new ();
	gtk_style_context_add_class (gtk_widget_get_style_context (priv->question_textview),
	                             "test-question");
	gtk_text_view_set_justification (GTK_TEXT_VIEW (priv->question_textview), GTK_JUSTIFY_CENTER);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->question_textview), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (priv->question_textview), FALSE);
	gtk_container_add (GTK_CONTAINER (question_scrolledwindow), priv->question_textview);

	return page;
}
