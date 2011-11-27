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
#include "gpinstruct/gpinstruct-private.h"
#include "gpinstruct/gpinstruct-marshalers.h"
#include "gpinstruct-view/gpinstruct-view.h"

#include "gtktextbuffermarkup.h"

struct _GPInstructLessonViewPrivate
{
	GPInstructLesson* lesson;
	GPInstructMessagePool* pool;
	GPInstructLog* log;

	gint current_page;
	GPInstructLessonViewPage* current_page_object;

	GtkWidget* next;
	GtkWidget* back;

	GtkWidget* contents;
	GList* pages;
	GtkSizeGroup* pages_size_group;

	GtkWidget* infobar;
	GtkWidget* infobar_label;
	GtkWidget* explain_button;

	gchar* explanation;
};

#define GPINSTRUCT_LESSON_VIEW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_VIEW, GPInstructLessonViewPrivate))

enum
{
	NEXT,
	BACK,

	LAST_SIGNAL
};


static guint lesson_view_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GPInstructLessonView, gpinstruct_lesson_view, GTK_TYPE_DIALOG);


void
show_page (GPInstructLessonView *view,
           guint page_num)
{
	guint num_pages = g_list_length (view->priv->pages);

	if (page_num < num_pages)
	{
		GPInstructLessonViewPage* page = g_list_nth_data (view->priv->pages, page_num);
		view->priv->current_page = page_num;
		view->priv->current_page_object = page;

		gpinstruct_lesson_view_page_show_current (page);
		gtk_notebook_set_current_page (GTK_NOTEBOOK (view->priv->contents), page_num);

		gchar* title = g_strdup_printf ("%s - %s",
		                                gpinstruct_lesson_get_title (view->priv->lesson),
		                                gpinstruct_lesson_view_page_get_title (page));
		gtk_window_set_title (GTK_WINDOW (view), title);
		g_free (title);

		if (page_num != 0 && gpinstruct_lesson_view_page_get_show_back_button (page))
			gtk_widget_set_sensitive (view->priv->back, TRUE);
		else
			gtk_widget_set_sensitive (view->priv->back, FALSE);

		if (page_num != num_pages-1 && gpinstruct_lesson_view_page_get_show_next_button (page))
			gtk_widget_set_sensitive (view->priv->next, TRUE);
		else
			gtk_widget_set_sensitive (view->priv->next, FALSE);
	}
}

void
next_button_clicked (GtkButton *button,
                     gpointer   user_data)
{
	gboolean clicked = FALSE;
	g_signal_emit (user_data, lesson_view_signals[NEXT], 0, &clicked);
}

void
back_button_clicked (GtkButton *button,
                     gpointer   user_data)
{
	gboolean clicked = FALSE;
	g_signal_emit (user_data, lesson_view_signals[BACK], 0, &clicked);
}

void
view_show (GtkWidget* widget,
           gpointer   user_data)
{
	GPInstructLessonView* view = GPINSTRUCT_LESSON_VIEW (widget);

	GList* current_pages = view->priv->pages;

	while (current_pages)
	{
		gpinstruct_lesson_view_page_reset (GPINSTRUCT_LESSON_VIEW_PAGE (current_pages->data));

		current_pages = current_pages->next;
	}

	gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_INFO);
	gtk_label_set_text (GTK_LABEL (view->priv->infobar_label), _("Welcome! Click Next to continue."));
	gtk_widget_hide (view->priv->explain_button);

	show_page (view, 0);
}

void
explanation_show (GtkInfoBar *info_bar,
                  gint        response_id,
                  gpointer    user_data)
{
	GPInstructLessonView* view = GPINSTRUCT_LESSON_VIEW (user_data);

	GtkWidget* dialog = gtk_dialog_new_with_buttons (_("Explanation"),
	                                                 GTK_WINDOW (view),
	                                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                                 GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE,
	                                                 NULL);
	gtk_window_set_default_size (GTK_WINDOW (dialog), 800, 600);
	gtk_widget_set_size_request (GTK_WIDGET (dialog), 600, 400);

	GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	GtkWidget* explanation_label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (explanation_label),
	                      _("<b><big>Explanation:</big></b>"));
	gtk_box_pack_start (GTK_BOX (content_area),
	                    explanation_label, FALSE, TRUE, 3);

	GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (content_area),
	                    scrolled_window, TRUE, TRUE, 3);

	GtkWidget* explanation = gtk_text_view_new ();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (explanation), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (explanation), FALSE);
	gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (explanation)),
	                            view->priv->explanation);
	gtk_container_add (GTK_CONTAINER (scrolled_window), explanation);

	gtk_widget_show_all (dialog);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}


static void
gpinstruct_lesson_view_init (GPInstructLessonView *object)
{
	object->priv = GPINSTRUCT_LESSON_VIEW_PRIVATE (object);

	object->priv->current_page = 0;
	object->priv->current_page_object = NULL;
	object->priv->pages = NULL;
	object->priv->explanation = g_strdup ("");

	GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (object));
	GtkWidget* action_area = gtk_dialog_get_action_area (GTK_DIALOG (object));

	object->priv->contents = gtk_notebook_new ();
	gtk_notebook_set_show_tabs (GTK_NOTEBOOK (object->priv->contents), FALSE);
	gtk_widget_show_all (object->priv->contents);
	gtk_box_pack_start (GTK_BOX (content_area), object->priv->contents, TRUE, TRUE, 3);

	object->priv->infobar = gtk_info_bar_new ();
	object->priv->infobar_label = gtk_label_new (NULL);
	gtk_box_pack_start (GTK_BOX (gtk_info_bar_get_content_area (GTK_INFO_BAR (object->priv->infobar))),
	                    object->priv->infobar_label, FALSE, TRUE, 3);
	g_signal_connect (object->priv->infobar, "response", G_CALLBACK (explanation_show), object);
	gtk_widget_show_all (object->priv->infobar);
	object->priv->explain_button = gtk_info_bar_add_button (GTK_INFO_BAR (object->priv->infobar), _("Explanation"), GTK_RESPONSE_OK);
	gtk_widget_hide (object->priv->explain_button);
	gtk_box_pack_start (GTK_BOX (content_area), object->priv->infobar, FALSE, FALSE, 3);

	object->priv->back = gtk_button_new_with_mnemonic (_("_Back"));
	g_signal_connect (object->priv->back, "clicked", G_CALLBACK (back_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (action_area), object->priv->back, FALSE, TRUE, 3);
	gtk_widget_show (object->priv->back);

	object->priv->next = gtk_button_new_with_mnemonic (_("_Next"));
	g_signal_connect (object->priv->next, "clicked", G_CALLBACK (next_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (action_area), object->priv->next, FALSE, TRUE, 3);
	gtk_widget_show (object->priv->next);

	gtk_dialog_add_buttons (GTK_DIALOG (object),
	                        _("C_lose"), 0,
	                        NULL);

	gtk_window_set_default_size (GTK_WINDOW (object), 800, 600);
	gtk_widget_set_size_request (GTK_WIDGET (object), 600, 400);

	g_signal_connect (object, "show", G_CALLBACK (view_show), NULL);

	object->priv->pages_size_group = gtk_size_group_new (GTK_SIZE_GROUP_BOTH);
}

static void
gpinstruct_lesson_view_finalize (GObject *object)
{
	GPInstructLessonView* view = GPINSTRUCT_LESSON_VIEW (object);

	if (view->priv->pages)
		g_list_free (view->priv->pages);

	if (view->priv->explanation)
		g_free (view->priv->explanation);

	if (view->priv->lesson)
		g_object_unref (view->priv->lesson);

	if (view->priv->pool)
		g_object_unref (view->priv->pool);

	if (view->priv->log)
		g_object_unref (view->priv->log);	

	G_OBJECT_CLASS (gpinstruct_lesson_view_parent_class)->finalize (object);
}

static gboolean
gpinstruct_lesson_view_next (GPInstructLessonView* view,
                             gpointer user_data)
{
	GPInstructLessonViewPage* current_page = view->priv->current_page_object;

	if (view->priv->log)
		gpinstruct_log_timer_stop (view->priv->log);

	gboolean show_next = !gpinstruct_lesson_view_page_show_next (view->priv->current_page_object);

	if (view->priv->log)
		gpinstruct_log_timer_start (view->priv->log);

	GPInstructMessageType message = gpinstruct_lesson_view_page_get_message (current_page);

	switch (message)
	{
		case GPINSTRUCT_MESSAGE_TYPE_CORRECT:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_QUESTION);
			break;
		case GPINSTRUCT_MESSAGE_TYPE_CORRECT_ALL:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_QUESTION);
			break;
		case GPINSTRUCT_MESSAGE_TYPE_WRONG:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_ERROR);
			break;
		case GPINSTRUCT_MESSAGE_TYPE_WRONG_SOME:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_WARNING);
			break;
		case GPINSTRUCT_MESSAGE_TYPE_WRONG_ALL:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_ERROR);
			break;
		case GPINSTRUCT_MESSAGE_TYPE_PASS:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_INFO);
			break;
		case GPINSTRUCT_MESSAGE_TYPE_FAIL:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_ERROR);
			break;
		default:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_OTHER);
			break;
	}

	gtk_label_set_text (GTK_LABEL (view->priv->infobar_label),
	                    gpinstruct_message_pool_get_random (view->priv->pool, message));

	if (message == GPINSTRUCT_MESSAGE_TYPE_WRONG ||
	    message == GPINSTRUCT_MESSAGE_TYPE_WRONG_SOME ||
	    message == GPINSTRUCT_MESSAGE_TYPE_WRONG_ALL)
	{
		if (view->priv->explanation)
			g_free (view->priv->explanation);
		view->priv->explanation = g_strdup (gpinstruct_lesson_view_page_get_explanation (current_page));

		if (view->priv->explanation == NULL ||
		    g_strcmp0 (view->priv->explanation, "") == 0)
		{
			g_free (view->priv->explanation);
			view->priv->explanation = g_strdup (_("No explanation available."));
		}

		gtk_widget_show (view->priv->explain_button);
	}
	else
		gtk_widget_hide (view->priv->explain_button);

	gpinstruct_message_pool_play_sound_random (view->priv->pool, message);

	if (show_next)
		show_page (GPINSTRUCT_LESSON_VIEW (view), view->priv->current_page+1);

	return TRUE;
}

static gboolean
gpinstruct_lesson_view_back (GPInstructLessonView* view,
                             gpointer user_data)
{
	if (!gpinstruct_lesson_view_page_show_previous (view->priv->current_page_object))
		show_page (GPINSTRUCT_LESSON_VIEW (view), view->priv->current_page-1);

	return TRUE;
}

static void
gpinstruct_lesson_view_class_init (GPInstructLessonViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkDialogClass* parent_class = GTK_DIALOG_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonViewPrivate));

	object_class->finalize = gpinstruct_lesson_view_finalize;

	klass->next = gpinstruct_lesson_view_next;
	klass->back = gpinstruct_lesson_view_back;

	lesson_view_signals[NEXT] =
		g_signal_new ("next",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructLessonViewClass, next),
		              _gpinstruct_boolean_handled_accumulator, NULL,
		              _gpinstruct_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);

	lesson_view_signals[BACK] =
		g_signal_new ("back",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructLessonViewClass, back),
		              _gpinstruct_boolean_handled_accumulator, NULL,
		              _gpinstruct_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);
}


GPInstructLessonView*
gpinstruct_lesson_view_new (GPInstructLesson* lesson,
                            GPInstructMessagePool* pool,
                            GPInstructLog* log)
{
	GPInstructLessonView* view = g_object_new (GPINSTRUCT_TYPE_LESSON_VIEW, NULL);

	GPInstructLessonElementGroup* curr_element_group;
	GPInstructLessonElement* curr_group_element;
	GPInstructLessonElement* curr_element;
	GPInstructLessonDiscussion* curr_discussion;
	GPInstructLessonReading* curr_reading;
	GPInstructLessonTestMultiChoice* curr_test_multi_choice;
	GPInstructLessonTestWordPool* curr_test_word_pool;
	GPInstructLessonTestOrder* curr_test_order;
	GPInstructLessonTestText* curr_test_text;
	GPInstructLessonScore* curr_score;

	gboolean single_score, group_single_score, single_directions;

	view->priv->lesson = g_object_ref (lesson);
	if (pool)
		view->priv->pool = g_object_ref (pool);
	if (log)
		view->priv->log = g_object_ref (log);

	gchar* title;
	const gchar* lesson_title = gpinstruct_lesson_get_title (lesson);

	GPInstructLessonViewPage* finish_page = gpinstruct_lesson_view_page_new ();
	gpinstruct_lesson_view_page_set_title (finish_page, _("End"));

	GtkWidget* finish_label = gtk_label_new (NULL);
	title = g_strdup_printf (_("End of\n<b><big>%s</big></b>"), lesson_title);
	gtk_label_set_markup (GTK_LABEL (finish_label), title);
	g_free (title);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (finish_page), finish_label);

	gpinstruct_lesson_view_append_page (view, finish_page);

	GPInstructLessonViewPage* welcome_page = gpinstruct_lesson_view_page_new ();
	gpinstruct_lesson_view_page_set_title (welcome_page, _("Welcome"));

	GtkWidget* welcome_label = gtk_label_new (NULL);
	title = g_strdup_printf (_("Welcome to\n<b><big>%s</big></b>"), lesson_title);
	gtk_label_set_markup (GTK_LABEL (welcome_label), title);
	g_free (title);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (welcome_page), welcome_label);

	gpinstruct_lesson_view_append_page (view, welcome_page);

	single_score = gpinstruct_lesson_get_single_score (lesson);

	if (single_score)
		curr_score = gpinstruct_lesson_score_new ();

	GList* lesson_elements = gpinstruct_lesson_get_lesson_elements (lesson);
	GList* curr_lesson_elements = lesson_elements;

	while (curr_lesson_elements)
	{
		curr_element = curr_lesson_elements->data;

		if (GPINSTRUCT_IS_LESSON_DISCUSSION (curr_element))
		{
			curr_discussion = GPINSTRUCT_LESSON_DISCUSSION (curr_element);

			GPInstructLessonDiscussionPage* page = gpinstruct_lesson_discussion_page_new (curr_discussion);
			gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (page));
		}
		else if (GPINSTRUCT_IS_LESSON_READING (curr_element))
		{
			curr_reading = GPINSTRUCT_LESSON_READING (curr_element);

			GPInstructLessonReadingPage* page = gpinstruct_lesson_reading_page_new (curr_reading);
			gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (page));
		}
		else if (GPINSTRUCT_IS_LESSON_TEST (curr_element))
		{
			if (!single_score)
				curr_score = gpinstruct_lesson_score_new ();

			if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (curr_element))
			{
				curr_test_multi_choice = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (curr_element);

				GPInstructLessonDirectionsPage* directions_page =
					gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_element),
					                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_element)));
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));

				GPInstructLessonTestMultiChoicePage* question_page = gpinstruct_lesson_test_multi_choice_page_new (curr_test_multi_choice, curr_score, view->priv->log);
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
			}
			else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (curr_element))
			{
				curr_test_word_pool = GPINSTRUCT_LESSON_TEST_WORD_POOL (curr_element);

				GPInstructLessonDirectionsPage* directions_page =
					gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_element),
					                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_element)));
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));

				GPInstructLessonTestWordPoolPage* question_page = gpinstruct_lesson_test_word_pool_page_new (curr_test_word_pool, curr_score, view->priv->log);
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
			}
			else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (curr_element))
			{
				curr_test_order = GPINSTRUCT_LESSON_TEST_ORDER (curr_element);

				GPInstructLessonDirectionsPage* directions_page =
					gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_element),
					                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_element)));
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));

				GPInstructLessonTestOrderPage* question_page = gpinstruct_lesson_test_order_page_new (curr_test_order, curr_score, view->priv->log);
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
			}
			else if (GPINSTRUCT_IS_LESSON_TEST_TEXT (curr_element))
			{
				curr_test_text = GPINSTRUCT_LESSON_TEST_TEXT (curr_element);

				GPInstructLessonDirectionsPage* directions_page =
					gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_element),
					                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_element)));
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));

				GPInstructLessonTestTextPage* question_page = gpinstruct_lesson_test_text_page_new (curr_test_text, curr_score, view->priv->log);
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
			}

			if (!single_score)
			{
				GPInstructLessonScorePage* score_page = gpinstruct_lesson_score_page_new (curr_score);
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (score_page));
				title = g_strdup_printf ("%s - %s",
				                         gpinstruct_lesson_element_get_title (curr_element),
				                         _("Score"));
				gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (score_page), title);
				g_free (title);
			}
		}
		else if (GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (curr_element))
		{
			curr_element_group = GPINSTRUCT_LESSON_ELEMENT_GROUP (curr_element);

			group_single_score = gpinstruct_lesson_element_group_get_single_score (curr_element_group);
			single_directions = gpinstruct_lesson_element_group_get_single_directions (curr_element_group);

			if (single_directions)
			{
				GPInstructLessonDirectionsPage* directions_page =
					gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_element),
					                                       gpinstruct_lesson_element_group_get_directions (curr_element_group));
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));
			}

			if (!single_score && group_single_score)
				curr_score = gpinstruct_lesson_score_new ();

			GList* lesson_elements_group = gpinstruct_lesson_element_group_get_lesson_elements (curr_element_group);
			GList* curr_lesson_elements_group = lesson_elements_group;

			while (curr_lesson_elements_group)
			{
				curr_group_element = curr_lesson_elements_group->data;

				if (GPINSTRUCT_IS_LESSON_DISCUSSION (curr_group_element))
				{
					curr_discussion = GPINSTRUCT_LESSON_DISCUSSION (curr_group_element);

					GPInstructLessonDiscussionPage* page = gpinstruct_lesson_discussion_page_new (curr_discussion);
					gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (page));
				}
				else if (GPINSTRUCT_IS_LESSON_READING (curr_group_element))
				{
					curr_reading = GPINSTRUCT_LESSON_READING (curr_group_element);

					GPInstructLessonReadingPage* page = gpinstruct_lesson_reading_page_new (curr_reading);
					gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (page));
				}
				else if (GPINSTRUCT_IS_LESSON_TEST (curr_group_element))
				{
					if (!single_score && !group_single_score)
						curr_score = gpinstruct_lesson_score_new ();

					if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (curr_group_element))
					{
						curr_test_multi_choice = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (curr_group_element);

						if (!single_directions)
						{
							GPInstructLessonDirectionsPage* directions_page =
								gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_group_element),
								                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_group_element)));
							gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));
						}

						GPInstructLessonTestMultiChoicePage* question_page = gpinstruct_lesson_test_multi_choice_page_new (curr_test_multi_choice, curr_score, view->priv->log);
						gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
					}
					else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (curr_group_element))
					{
						curr_test_word_pool = GPINSTRUCT_LESSON_TEST_WORD_POOL (curr_group_element);

						if (!single_directions)
						{
							GPInstructLessonDirectionsPage* directions_page =
								gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_group_element),
								                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_group_element)));
							gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));
						}

						GPInstructLessonTestWordPoolPage* question_page = gpinstruct_lesson_test_word_pool_page_new (curr_test_word_pool, curr_score, view->priv->log);
						gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
					}
					else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (curr_group_element))
					{
						curr_test_order = GPINSTRUCT_LESSON_TEST_ORDER (curr_group_element);

						if (!single_directions)
						{
							GPInstructLessonDirectionsPage* directions_page =
								gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_group_element),
								                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_group_element)));
							gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));
						}

						GPInstructLessonTestOrderPage* question_page = gpinstruct_lesson_test_order_page_new (curr_test_order, curr_score, view->priv->log);
						gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
					}
					else if (GPINSTRUCT_IS_LESSON_TEST_TEXT (curr_group_element))
					{
						curr_test_text = GPINSTRUCT_LESSON_TEST_TEXT (curr_group_element);

						if (!single_directions)
						{
							GPInstructLessonDirectionsPage* directions_page =
								gpinstruct_lesson_directions_page_new (gpinstruct_lesson_element_get_title (curr_group_element),
								                                       gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_group_element)));
							gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (directions_page));
						}

						GPInstructLessonTestTextPage* question_page = gpinstruct_lesson_test_text_page_new (curr_test_text, curr_score, view->priv->log);
						gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (question_page));
					}

					if (!single_score && !group_single_score)
					{
						GPInstructLessonScorePage* score_page = gpinstruct_lesson_score_page_new (curr_score);
						gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (score_page));
						title = g_strdup_printf ("%s - %s",
						                         gpinstruct_lesson_element_get_title (curr_group_element),
						                         _("Score"));
						gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (score_page), title);
						g_free (title);
					}
				}

				curr_lesson_elements_group = curr_lesson_elements_group->next;
			}

			if (!single_score && group_single_score)
			{
				GPInstructLessonScorePage* score_page = gpinstruct_lesson_score_page_new (curr_score);
				gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (score_page));
				title = g_strdup_printf ("%s - %s",
				                         gpinstruct_lesson_element_get_title (curr_element),
				                         _("Score"));
				gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (score_page), title);
				g_free (title);
			}

			g_list_free (lesson_elements_group);
		}

		curr_lesson_elements = curr_lesson_elements->next;
	}

	if (single_score)
	{
		GPInstructLessonScorePage* score_page = gpinstruct_lesson_score_page_new (curr_score);
		gpinstruct_lesson_view_append_page (view, GPINSTRUCT_LESSON_VIEW_PAGE (score_page));
		gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (score_page), _("Score"));
	}

	g_list_free (lesson_elements);

	return view;
}

void
gpinstruct_lesson_view_append_page (GPInstructLessonView* view,
                                    GPInstructLessonViewPage* page)
{
	guint page_num = g_list_length (view->priv->pages) -1;

	view->priv->pages = g_list_insert (view->priv->pages, page, page_num);

	gtk_size_group_add_widget (view->priv->pages_size_group, GTK_WIDGET (page));

	gtk_widget_show_all (GTK_WIDGET (page));

	gtk_notebook_insert_page (GTK_NOTEBOOK (view->priv->contents), GTK_WIDGET (page), NULL, page_num);
}

guint
gpinstruct_lesson_view_get_current_page (GPInstructLessonView* view)
{
	return view->priv->current_page;
}

void
gpinstruct_lesson_view_set_current_page (GPInstructLessonView* view,
                                         guint page)
{
	show_page (view, page);
}

GPInstructLessonViewPage*
gpinstruct_lesson_view_get_current_page_object (GPInstructLessonView* view)
{
	return view->priv->current_page_object;
}
