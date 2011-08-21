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

#include "canvas/canvas.h"
#include "canvas/canvas-private.h"
#include "canvas/canvas-marshalers.h"
#include "canvas-view/canvas-view.h"

struct _CanvasLessonViewPagePrivate
{
	gchar* title;

	gboolean next;
	gboolean back;

	CanvasMessageType message;
	gchar* explanation;
};

#define CANVAS_LESSON_VIEW_PAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_VIEW_PAGE, CanvasLessonViewPagePrivate))

enum
{
	SHOW_NEXT,
	SHOW_PREVIOUS,
	SHOW_CURRENT,
	RESET,

	LAST_SIGNAL
};


static guint lesson_view_page_signals[LAST_SIGNAL] = { 0 };


void
page_reset (CanvasLessonViewPage* view, gpointer user_data)
{
	GtkScrolledWindow* scrolled_window = GTK_SCROLLED_WINDOW (view);

	gtk_range_set_fill_level (GTK_RANGE (gtk_scrolled_window_get_hscrollbar (scrolled_window)),
	                          0);
	gtk_range_set_fill_level (GTK_RANGE (gtk_scrolled_window_get_vscrollbar (scrolled_window)),
	                          0);
}


G_DEFINE_TYPE (CanvasLessonViewPage, canvas_lesson_view_page, GTK_TYPE_SCROLLED_WINDOW);

static void
canvas_lesson_view_page_init (CanvasLessonViewPage *object)
{
	object->priv = CANVAS_LESSON_VIEW_PAGE_PRIVATE (object);

	object->priv->title = NULL;

	object->priv->next = TRUE;
	object->priv->back = TRUE;

	object->priv->message = GTK_MESSAGE_INFO;
	object->priv->explanation = g_strdup ("");

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (object),
	                                GTK_POLICY_AUTOMATIC,
	                                GTK_POLICY_AUTOMATIC);
}

static void
canvas_lesson_view_page_finalize (GObject *object)
{
	CanvasLessonViewPage* page = CANVAS_LESSON_VIEW_PAGE (object);

	if (page->priv->title)
		g_free (page->priv->title);

	if (page->priv->explanation)
		g_free (page->priv->explanation);

	G_OBJECT_CLASS (canvas_lesson_view_page_parent_class)->finalize (object);
}

static void
canvas_lesson_view_page_class_init (CanvasLessonViewPageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkScrolledWindowClass* parent_class = GTK_SCROLLED_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonViewPagePrivate));

	object_class->finalize = canvas_lesson_view_page_finalize;

	klass->show_next = NULL;
	klass->show_previous = NULL;
	klass->show_current = NULL;
	klass->reset = page_reset;

	lesson_view_page_signals[SHOW_NEXT] =
		g_signal_new ("show-next",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CanvasLessonViewPageClass, show_next),
		              _canvas_boolean_handled_accumulator, NULL,
		              _canvas_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);

	lesson_view_page_signals[SHOW_PREVIOUS] =
		g_signal_new ("show-previous",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CanvasLessonViewPageClass, show_previous),
		              _canvas_boolean_handled_accumulator, NULL,
		              _canvas_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);

	lesson_view_page_signals[SHOW_CURRENT] =
		g_signal_new ("show-current",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CanvasLessonViewPageClass, show_current),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);

	lesson_view_page_signals[RESET] =
		g_signal_new ("reset",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CanvasLessonViewPageClass, reset),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);
}


CanvasLessonViewPage*
canvas_lesson_view_page_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_VIEW_PAGE, NULL);
}

const gchar*
canvas_lesson_view_page_get_title (CanvasLessonViewPage* page)
{
	return page->priv->title;
}

void
canvas_lesson_view_page_set_title (CanvasLessonViewPage* page,
                                   const gchar* title)
{
	if (page->priv->title)
		g_free (page->priv->title);
	page->priv->title = g_strdup (title);
}

gboolean
canvas_lesson_view_page_get_show_next_button (CanvasLessonViewPage* page)
{
	return page->priv->next;
}

void
canvas_lesson_view_page_set_show_next_button (CanvasLessonViewPage* page,
                                              gboolean show)
{
	page->priv->next = show;
}

gboolean
canvas_lesson_view_page_get_show_back_button (CanvasLessonViewPage* page)
{
	return page->priv->back;
}

void
canvas_lesson_view_page_set_show_back_button (CanvasLessonViewPage* page,
                                              gboolean show)
{
	page->priv->back = show;
}

gboolean
canvas_lesson_view_page_show_next (CanvasLessonViewPage* page)
{
	gboolean clicked = FALSE;
	g_signal_emit (page, lesson_view_page_signals[SHOW_NEXT], 0, &clicked);
	return clicked;
}

gboolean
canvas_lesson_view_page_show_previous (CanvasLessonViewPage* page)
{
	gboolean clicked = FALSE;
	g_signal_emit (page, lesson_view_page_signals[SHOW_PREVIOUS], 0, &clicked);
	return clicked;
}

void
canvas_lesson_view_page_show_current (CanvasLessonViewPage* page)
{
	g_signal_emit (page, lesson_view_page_signals[SHOW_CURRENT], 0);
}

void
canvas_lesson_view_page_reset (CanvasLessonViewPage* page)
{
	g_signal_emit (page, lesson_view_page_signals[RESET], 0);
}

void
canvas_lesson_view_page_set_message (CanvasLessonViewPage* page, CanvasMessageType message)
{
	page->priv->message = message;
}

CanvasMessageType
canvas_lesson_view_page_get_message (CanvasLessonViewPage* page)
{
	return page->priv->message;
}

void
canvas_lesson_view_page_set_explanation (CanvasLessonViewPage* page, const gchar* explanation)
{
	if (page->priv->explanation)
		g_free (page->priv->explanation);
	page->priv->explanation = g_strdup (explanation);
}

const gchar*
canvas_lesson_view_page_get_explanation (CanvasLessonViewPage* page)
{
	return page->priv->explanation;
}