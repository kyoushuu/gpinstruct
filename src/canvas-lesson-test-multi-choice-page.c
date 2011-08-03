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

#include "canvas.h"
#include "gtktextbuffermarkup.h"

typedef struct _CanvasLessonTestMultiChoicePagePrivate CanvasLessonTestMultiChoicePagePrivate;
struct _CanvasLessonTestMultiChoicePagePrivate
{
	CanvasLessonTestMultiChoice* test;
	CanvasLessonScore* score;

	guint curr_question;
	GList* choice_buttons;

	GtkWidget* vbox;
	GtkWidget* question_textview;
	GtkWidget* choices_vbox;
};

#define CANVAS_LESSON_TEST_MULTI_CHOICE_PAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_PAGE, CanvasLessonTestMultiChoicePagePrivate))



G_DEFINE_TYPE (CanvasLessonTestMultiChoicePage, canvas_lesson_test_multi_choice_page, CANVAS_TYPE_LESSON_VIEW_PAGE);

static void
canvas_lesson_test_multi_choice_page_init (CanvasLessonTestMultiChoicePage *object)
{
	CanvasLessonTestMultiChoicePagePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PAGE_PRIVATE (object);

	priv->test = NULL;
	priv->score = NULL;
	priv->curr_question = 0;
	priv->choice_buttons = NULL;
	priv->vbox = NULL;
	priv->question_textview = NULL;
	priv->choices_vbox = NULL;
}

static void
canvas_lesson_test_multi_choice_page_finalize (GObject *object)
{
	CanvasLessonTestMultiChoicePagePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PAGE_PRIVATE (object);

	if (priv->choice_buttons)
		g_list_free (priv->choice_buttons);

	G_OBJECT_CLASS (canvas_lesson_test_multi_choice_page_parent_class)->finalize (object);
}

static void
canvas_lesson_test_multi_choice_page_class_init (CanvasLessonTestMultiChoicePageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonViewPageClass* parent_class = CANVAS_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestMultiChoicePagePrivate));

	object_class->finalize = canvas_lesson_test_multi_choice_page_finalize;
}


void
multi_choice_show_question (CanvasLessonTestMultiChoicePage* page, guint question_num)
{
	CanvasLessonTestMultiChoicePagePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PAGE_PRIVATE (page);

	GList* questions = canvas_lesson_test_multi_choice_get_questions (priv->test);
	if (question_num < g_list_length (questions))
	{
		priv->curr_question = question_num;

		gchar* text;
		int i;

		CanvasLessonTestMultiChoiceQuestion* question = g_list_nth_data (questions, question_num);

		text = g_strdup_printf ("%d. %s", 1+question_num, canvas_lesson_test_multi_choice_question_get_text (question));
		gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->question_textview)), text);
		g_free (text);

		if (priv->choices_vbox)
			gtk_widget_destroy (priv->choices_vbox);

		priv->choices_vbox = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start (GTK_BOX (priv->vbox), priv->choices_vbox, FALSE, TRUE, 3);

		GtkWidget* choice_radio_button;
		GtkWidget* last_radio_button = NULL;

		if (priv->choice_buttons)
		{
			g_list_free (priv->choice_buttons);
			priv->choice_buttons = NULL;
		}

		GList* choices = canvas_lesson_test_multi_choice_question_get_choices (question);
		GList* curr_choices = choices;

		for (i = 0; curr_choices; curr_choices = curr_choices->next, i++)
		{
			text = g_strdup_printf ("_%c. %s", 'a'+i, (gchar*)curr_choices->data);
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

gboolean
multi_choice_page_show_next (CanvasLessonTestMultiChoicePage* page, gpointer user_data)
{
	CanvasLessonTestMultiChoicePagePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PAGE_PRIVATE (page);

	canvas_lesson_score_increase_total (priv->score);

	GList* questions = canvas_lesson_test_multi_choice_get_questions (priv->test);

	CanvasLessonTestMultiChoiceQuestion* question = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION (g_list_nth_data (questions, priv->curr_question));

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_list_nth_data (priv->choice_buttons, canvas_lesson_test_multi_choice_question_get_answer (question)))))
		canvas_lesson_score_increase_score (priv->score);

	if (priv->curr_question+1 < g_list_length (questions))
	{
		multi_choice_show_question (page, priv->curr_question+1);
		g_list_free (questions);
		return TRUE;
	}
	else
	{
		g_list_free (questions);
		return FALSE;
	}
}


CanvasLessonTestMultiChoicePage*
canvas_lesson_test_multi_choice_page_new (CanvasLessonTestMultiChoice* test,
                                          CanvasLessonScore* score)
{
	CanvasLessonTestMultiChoicePage* page = g_object_new (CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_PAGE, NULL);
	CanvasLessonTestMultiChoicePagePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PAGE_PRIVATE (page);

	canvas_lesson_view_page_set_title (CANVAS_LESSON_VIEW_PAGE (page),
	                                   canvas_lesson_element_get_title (CANVAS_LESSON_ELEMENT (test)));
	canvas_lesson_view_page_set_show_back_button (CANVAS_LESSON_VIEW_PAGE (page), FALSE);

	g_signal_connect (page, "show-next", G_CALLBACK (multi_choice_page_show_next), NULL);

	priv->test = test;
	priv->score = score;

	priv->vbox = gtk_vbox_new (FALSE, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), priv->vbox);

	priv->question_textview = gtk_text_view_new ();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->question_textview), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (priv->question_textview), FALSE);
	gtk_box_pack_start (GTK_BOX (priv->vbox), priv->question_textview, TRUE, TRUE, 3);

	multi_choice_show_question (page, 0);

	return page;
}
