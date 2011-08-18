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

#include "canvas.h"
#include "canvas-view.h"
#include "canvas-private.h"
#include "canvas-marshalers.h"

#include "gtktextbuffermarkup.h"

struct _CanvasLessonViewPrivate
{
	CanvasLesson* lesson;
	CanvasMessagePool* pool;

	gint current_page;
	CanvasLessonViewPage* current_page_object;

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

#define CANVAS_LESSON_VIEW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_VIEW, CanvasLessonViewPrivate))

enum
{
	NEXT,
	BACK,

	LAST_SIGNAL
};


static guint lesson_view_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (CanvasLessonView, canvas_lesson_view, GTK_TYPE_DIALOG);


void
show_page (CanvasLessonView *view,
           guint page_num)
{
	guint num_pages = g_list_length (view->priv->pages);

	if (page_num < num_pages)
	{
		CanvasLessonViewPage* page = g_list_nth_data (view->priv->pages, page_num);
		view->priv->current_page = page_num;
		view->priv->current_page_object = page;

		canvas_lesson_view_page_show_current (page);
		gtk_notebook_set_current_page (GTK_NOTEBOOK (view->priv->contents), page_num);

		gtk_window_set_title (GTK_WINDOW (view), canvas_lesson_view_page_get_title (page));

		if (page_num != 0 && canvas_lesson_view_page_get_show_back_button (page))
			gtk_widget_set_sensitive (view->priv->back, TRUE);
		else
			gtk_widget_set_sensitive (view->priv->back, FALSE);

		if (page_num != num_pages-1 && canvas_lesson_view_page_get_show_next_button (page))
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
	CanvasLessonView* view = CANVAS_LESSON_VIEW (widget);

	GList* current_pages = view->priv->pages;

	while (current_pages)
	{
		canvas_lesson_view_page_reset (CANVAS_LESSON_VIEW_PAGE (current_pages->data));

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
	CanvasLessonView* view = CANVAS_LESSON_VIEW (user_data);

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
canvas_lesson_view_init (CanvasLessonView *object)
{
	object->priv = CANVAS_LESSON_VIEW_PRIVATE (object);

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
canvas_lesson_view_finalize (GObject *object)
{
	CanvasLessonView* view = CANVAS_LESSON_VIEW (object);

	if (view->priv->pages)
		g_list_free (view->priv->pages);

	if (view->priv->explanation)
		g_free (view->priv->explanation);

	if (view->priv->lesson)
		g_object_unref (view->priv->lesson);

	if (view->priv->pool)
		g_object_unref (view->priv->pool);	

	G_OBJECT_CLASS (canvas_lesson_view_parent_class)->finalize (object);
}

static gboolean
canvas_lesson_view_next (CanvasLessonView* view, gpointer user_data)
{
	CanvasLessonViewPage* current_page = view->priv->current_page_object;

	gboolean show_next = !canvas_lesson_view_page_show_next (view->priv->current_page_object);

	CanvasMessageType message = canvas_lesson_view_page_get_message (current_page);

	switch (message)
	{
		case CANVAS_MESSAGE_TYPE_CORRECT:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_QUESTION);
			break;
		case CANVAS_MESSAGE_TYPE_CORRECT_ALL:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_QUESTION);
			break;
		case CANVAS_MESSAGE_TYPE_WRONG:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_ERROR);
			break;
		case CANVAS_MESSAGE_TYPE_WRONG_SOME:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_WARNING);
			break;
		case CANVAS_MESSAGE_TYPE_WRONG_ALL:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_ERROR);
			break;
		case CANVAS_MESSAGE_TYPE_PASS:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_INFO);
			break;
		case CANVAS_MESSAGE_TYPE_FAIL:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_ERROR);
			break;
		default:
			gtk_info_bar_set_message_type (GTK_INFO_BAR (view->priv->infobar), GTK_MESSAGE_OTHER);
			break;
	}

	gtk_label_set_text (GTK_LABEL (view->priv->infobar_label),
	                    canvas_message_pool_get_random (view->priv->pool, message));

	if (message == CANVAS_MESSAGE_TYPE_WRONG ||
	    message == CANVAS_MESSAGE_TYPE_WRONG_SOME ||
	    message == CANVAS_MESSAGE_TYPE_WRONG_ALL)
	{
		if (view->priv->explanation)
			g_free (view->priv->explanation);
		view->priv->explanation = g_strdup (canvas_lesson_view_page_get_explanation (current_page));

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

	if (show_next)
		show_page (CANVAS_LESSON_VIEW (view), view->priv->current_page+1);

	return TRUE;
}

static gboolean
canvas_lesson_view_back (CanvasLessonView* view, gpointer user_data)
{
	if (!canvas_lesson_view_page_show_previous (view->priv->current_page_object))
		show_page (CANVAS_LESSON_VIEW (view), view->priv->current_page-1);

	return TRUE;
}

static void
canvas_lesson_view_class_init (CanvasLessonViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkDialogClass* parent_class = GTK_DIALOG_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonViewPrivate));

	object_class->finalize = canvas_lesson_view_finalize;

	klass->next = canvas_lesson_view_next;
	klass->back = canvas_lesson_view_back;

	lesson_view_signals[NEXT] =
		g_signal_new ("next",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CanvasLessonViewClass, next),
		              _canvas_boolean_handled_accumulator, NULL,
		              _canvas_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);

	lesson_view_signals[BACK] =
		g_signal_new ("back",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CanvasLessonViewClass, back),
		              _canvas_boolean_handled_accumulator, NULL,
		              _canvas_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);
}


CanvasLessonView*
canvas_lesson_view_new (CanvasLesson* lesson, CanvasMessagePool* pool)
{
	CanvasLessonView* view = g_object_new (CANVAS_TYPE_LESSON_VIEW, NULL);

	CanvasLessonElement* curr_element;
	CanvasLessonDiscussion* curr_discussion;
	CanvasLessonReading* curr_reading;
	CanvasLessonTestMultiChoice* curr_test_multi_choice;
	CanvasLessonTestWordPool* curr_test_word_pool;
	CanvasLessonTestOrder* curr_test_order;
	CanvasLessonScore* curr_score;
	CanvasLessonScore* curr_lesson_score;

	gboolean single_score;

	view->priv->lesson = g_object_ref (lesson);
	view->priv->pool = g_object_ref (pool);

	gchar* title;
	const gchar* lesson_title = canvas_lesson_get_title (lesson);

	CanvasLessonViewPage* finish_page = canvas_lesson_view_page_new ();
	canvas_lesson_view_page_set_title (finish_page, lesson_title);

	GtkWidget* finish_label = gtk_label_new (NULL);
	title = g_strdup_printf ("End of\n<b><big>%s</big></b>", lesson_title);
	gtk_label_set_markup (GTK_LABEL (finish_label), title);
	g_free (title);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (finish_page), finish_label);

	canvas_lesson_view_append_page (view, finish_page);

	CanvasLessonViewPage* welcome_page = canvas_lesson_view_page_new ();
	canvas_lesson_view_page_set_title (welcome_page, lesson_title);

	GtkWidget* welcome_label = gtk_label_new (NULL);
	title = g_strdup_printf ("Welcome to\n<b><big>%s</big></b>", lesson_title);
	gtk_label_set_markup (GTK_LABEL (welcome_label), title);
	g_free (title);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (welcome_page), welcome_label);

	canvas_lesson_view_append_page (view, welcome_page);

	single_score = canvas_lesson_get_single_score (lesson);

	if (single_score)
		curr_lesson_score = canvas_lesson_score_new ();

	GList* lesson_elements = canvas_lesson_get_lesson_elements (lesson);
	GList* curr_lesson_elements = lesson_elements;

	while (curr_lesson_elements)
	{
		curr_element = curr_lesson_elements->data;

		if (CANVAS_IS_LESSON_DISCUSSION (curr_element))
		{
			curr_discussion = CANVAS_LESSON_DISCUSSION (curr_element);

			CanvasLessonDiscussionPage* page = canvas_lesson_discussion_page_new (curr_discussion);
			canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (page));
		}
		else if (CANVAS_IS_LESSON_READING (curr_element))
		{
			curr_reading = CANVAS_LESSON_READING (curr_element);

			CanvasLessonReadingPage* page = canvas_lesson_reading_page_new (curr_reading);
			canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (page));
		}
		else if (CANVAS_IS_LESSON_TEST (curr_element))
		{
			if (!single_score)
				curr_score = canvas_lesson_score_new ();

			if (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (curr_element))
			{
				curr_test_multi_choice = CANVAS_LESSON_TEST_MULTI_CHOICE (curr_element);

				CanvasLessonDirectionsPage* directions_page =
					canvas_lesson_directions_page_new (canvas_lesson_element_get_title (curr_element),
					                                   canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_element)));
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (directions_page));

				CanvasLessonTestMultiChoicePage* question_page = canvas_lesson_test_multi_choice_page_new (curr_test_multi_choice, curr_score);
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (question_page));
			}
			else if (CANVAS_IS_LESSON_TEST_WORD_POOL (curr_element))
			{
				curr_test_word_pool = CANVAS_LESSON_TEST_WORD_POOL (curr_element);

				CanvasLessonDirectionsPage* directions_page =
					canvas_lesson_directions_page_new (canvas_lesson_element_get_title (curr_element),
					                                   canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_element)));
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (directions_page));

				CanvasLessonTestWordPoolPage* question_page = canvas_lesson_test_word_pool_page_new (curr_test_word_pool, curr_score);
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (question_page));
			}
			else if (CANVAS_IS_LESSON_TEST_ORDER (curr_element))
			{
				curr_test_order = CANVAS_LESSON_TEST_ORDER (curr_element);

				CanvasLessonDirectionsPage* directions_page =
					canvas_lesson_directions_page_new (canvas_lesson_element_get_title (curr_element),
					                                   canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_element)));
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (directions_page));

				CanvasLessonTestOrderPage* question_page = canvas_lesson_test_order_page_new (curr_test_order, curr_score);
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (question_page));
			}

			if (!single_score)
			{
				CanvasLessonScorePage* score_page = canvas_lesson_score_page_new (curr_score);
				canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (score_page));
			}
		}

		curr_lesson_elements = curr_lesson_elements->next;
	}

	if (single_score)
	{
		CanvasLessonScorePage* score_page = canvas_lesson_score_page_new (curr_lesson_score);
		canvas_lesson_view_append_page (view, CANVAS_LESSON_VIEW_PAGE (score_page));
	}

	g_list_free (lesson_elements);

	return view;
}

void
canvas_lesson_view_append_page (CanvasLessonView* view, CanvasLessonViewPage* page)
{
	guint page_num = g_list_length (view->priv->pages) -1;

	view->priv->pages = g_list_insert (view->priv->pages, page, page_num);

	gtk_size_group_add_widget (view->priv->pages_size_group, GTK_WIDGET (page));

	gtk_widget_show_all (GTK_WIDGET (page));

	gtk_notebook_insert_page (GTK_NOTEBOOK (view->priv->contents), GTK_WIDGET (page), NULL, page_num);
}

guint
canvas_lesson_view_get_current_page (CanvasLessonView* view)
{
	return view->priv->current_page;
}

void
canvas_lesson_view_set_current_page (CanvasLessonView* view, guint page)
{
	show_page (view, page);
}

CanvasLessonViewPage*
canvas_lesson_view_get_current_page_object (CanvasLessonView* view)
{
	return view->priv->current_page_object;
}
