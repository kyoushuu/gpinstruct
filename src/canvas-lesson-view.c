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

#include "canvas.h"
#include "canvas-private.h"
#include "canvas-marshalers.h"

typedef struct _CanvasLessonViewPrivate CanvasLessonViewPrivate;
struct _CanvasLessonViewPrivate
{
	CanvasLesson* lesson;

	gint current_page;
	CanvasLessonViewPage* current_page_object;

	GtkWidget* next;
	GtkWidget* back;
	GtkWidget* contents;
	GList* pages;
	GtkSizeGroup* pages_size_group;
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
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (view);

	guint num_pages = g_list_length (priv->pages);

	if (page_num < num_pages)
	{
		CanvasLessonViewPage* page = g_list_nth_data (priv->pages, page_num);
		priv->current_page = page_num;
		priv->current_page_object = page;

		canvas_lesson_view_page_show_current (page);
		gtk_notebook_set_current_page (GTK_NOTEBOOK (priv->contents), page_num);

		gtk_window_set_title (GTK_WINDOW (view), canvas_lesson_view_page_get_title (page));

		if (page_num != 0 && canvas_lesson_view_page_get_show_back_button (page))
			gtk_widget_set_sensitive (priv->back, TRUE);
		else
			gtk_widget_set_sensitive (priv->back, FALSE);

		if (page_num != num_pages-1 && canvas_lesson_view_page_get_show_next_button (page))
			gtk_widget_set_sensitive (priv->next, TRUE);
		else
			gtk_widget_set_sensitive (priv->next, FALSE);
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
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (view);

	GList* current_pages = priv->pages;

	while (current_pages)
	{
		canvas_lesson_view_page_reset (CANVAS_LESSON_VIEW_PAGE (current_pages->data));

		current_pages = current_pages->next;
	}

	show_page (view, 0);
}


static void
canvas_lesson_view_init (CanvasLessonView *object)
{
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (object);

	priv->current_page = 0;
	priv->current_page_object = NULL;
	priv->pages = NULL;

	GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (object));
	GtkWidget* action_area = gtk_dialog_get_action_area (GTK_DIALOG (object));

	priv->contents = gtk_notebook_new ();
	gtk_notebook_set_show_tabs (GTK_NOTEBOOK (priv->contents), FALSE);
	gtk_widget_show_all (priv->contents);
	gtk_box_pack_start (GTK_BOX (content_area), priv->contents, TRUE, TRUE, 3);

	priv->back = gtk_button_new_with_mnemonic (_("_Back"));
	g_signal_connect (priv->back, "clicked", G_CALLBACK (back_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (action_area), priv->back, FALSE, TRUE, 3);
	gtk_widget_show (priv->back);

	priv->next = gtk_button_new_with_mnemonic (_("_Next"));
	g_signal_connect (priv->next, "clicked", G_CALLBACK (next_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (action_area), priv->next, FALSE, TRUE, 3);
	gtk_widget_show (priv->next);

	gtk_dialog_add_buttons (GTK_DIALOG (object),
	                        _("C_lose"), 0,
	                        NULL);

	gtk_window_set_default_size (GTK_WINDOW (object), 400, 300);
	gtk_widget_set_size_request (GTK_WIDGET (object), 400, 300);

	g_signal_connect (object, "show", G_CALLBACK (view_show), NULL);

	priv->pages_size_group = gtk_size_group_new (GTK_SIZE_GROUP_BOTH);
}

static void
canvas_lesson_view_finalize (GObject *object)
{
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (object);

	if (priv->pages)
		g_list_free (priv->pages);

	G_OBJECT_CLASS (canvas_lesson_view_parent_class)->finalize (object);
}

static gboolean
canvas_lesson_view_next (CanvasLessonView* view, gpointer user_data)
{
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (view);

	if (!canvas_lesson_view_page_show_next (priv->current_page_object))
		show_page (CANVAS_LESSON_VIEW (view), priv->current_page+1);

	return TRUE;
}

static gboolean
canvas_lesson_view_back (CanvasLessonView* view, gpointer user_data)
{
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (view);

	if (!canvas_lesson_view_page_show_previous (priv->current_page_object))
		show_page (CANVAS_LESSON_VIEW (view), priv->current_page-1);

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
canvas_lesson_view_new (CanvasLesson* lesson)
{
	CanvasLessonView* view = g_object_new(CANVAS_TYPE_LESSON_VIEW, NULL);

	CanvasLessonElement* curr_element;
	CanvasLessonDiscussion* curr_discussion;
	CanvasLessonReading* curr_reading;
	CanvasLessonTestMultiChoice* curr_test_multi_choice;
	CanvasLessonTestWordPool* curr_test_word_pool;
	CanvasLessonTestOrder* curr_test_order;
	CanvasLessonScore* curr_score;
	CanvasLessonScore* curr_lesson_score;

	gboolean single_score;

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
	CanvasLessonViewPrivate* priv = CANVAS_LESSON_VIEW_PRIVATE (view);

	guint page_num = g_list_length (priv->pages) -1;

	priv->pages = g_list_insert (priv->pages, page, page_num);

	gtk_size_group_add_widget (priv->pages_size_group, GTK_WIDGET (page));

	gtk_widget_show_all (GTK_WIDGET (page));

	gtk_notebook_insert_page (GTK_NOTEBOOK (priv->contents), GTK_WIDGET (page), NULL, page_num);
}

guint
canvas_lesson_view_get_current_page (CanvasLessonView* view)
{
	return CANVAS_LESSON_VIEW_PRIVATE (view)->current_page;
}

void
canvas_lesson_view_set_current_page (CanvasLessonView* view, guint page)
{
	show_page (view, page);
}

CanvasLessonViewPage*
canvas_lesson_view_get_current_page_object (CanvasLessonView* view)
{
	return CANVAS_LESSON_VIEW_PRIVATE (view)->current_page_object;
}
