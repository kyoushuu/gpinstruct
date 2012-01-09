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
#include "gpinstruct/gpinstruct-private.h"
#include "gpinstruct/gpinstruct-marshalers.h"
#include "gpinstruct-view/gpinstruct-view.h"

struct _GPInstructLessonViewPagePrivate
{
	gchar *title;

	gboolean next;
	gboolean back;

	GPInstructMessageType message;
	gchar *explanation;
};

#define GPINSTRUCT_LESSON_VIEW_PAGE_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_VIEW_PAGE, GPInstructLessonViewPagePrivate))

enum
{
	SHOW_NEXT,
	SHOW_PREVIOUS,
	SHOW_CURRENT,
	RESET,

	LAST_SIGNAL
};


static guint lesson_view_page_signals[LAST_SIGNAL] = { 0 };


static void
page_reset (GPInstructLessonViewPage *view,
            gpointer user_data)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW (view);

	gtk_range_set_fill_level (GTK_RANGE (gtk_scrolled_window_get_hscrollbar (scrolled_window)),
	                          0);
	gtk_range_set_fill_level (GTK_RANGE (gtk_scrolled_window_get_vscrollbar (scrolled_window)),
	                          0);
}


G_DEFINE_TYPE (GPInstructLessonViewPage, gpinstruct_lesson_view_page, GTK_TYPE_SCROLLED_WINDOW);

static void
gpinstruct_lesson_view_page_init (GPInstructLessonViewPage *object)
{
	object->priv = GPINSTRUCT_LESSON_VIEW_PAGE_GET_PRIVATE (object);
	GPInstructLessonViewPagePrivate *priv = object->priv;

	priv->title = NULL;

	priv->next = TRUE;
	priv->back = TRUE;

	priv->message = GTK_MESSAGE_INFO;
	priv->explanation = g_strdup ("");

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (object),
	                                GTK_POLICY_AUTOMATIC,
	                                GTK_POLICY_AUTOMATIC);
}

static void
gpinstruct_lesson_view_page_finalize (GObject *object)
{
	GPInstructLessonViewPage *page = GPINSTRUCT_LESSON_VIEW_PAGE (object);
	GPInstructLessonViewPagePrivate *priv = page->priv;

	g_free (priv->title);
	g_free (priv->explanation);

	G_OBJECT_CLASS (gpinstruct_lesson_view_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_view_page_class_init (GPInstructLessonViewPageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GtkScrolledWindowClass *parent_class = GTK_SCROLLED_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonViewPagePrivate));

	object_class->finalize = gpinstruct_lesson_view_page_finalize;

	klass->show_next = NULL;
	klass->show_previous = NULL;
	klass->show_current = NULL;
	klass->reset = page_reset;

	lesson_view_page_signals[SHOW_NEXT] =
		g_signal_new ("show-next",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructLessonViewPageClass, show_next),
		              _gpinstruct_boolean_handled_accumulator, NULL,
		              _gpinstruct_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);

	lesson_view_page_signals[SHOW_PREVIOUS] =
		g_signal_new ("show-previous",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructLessonViewPageClass, show_previous),
		              _gpinstruct_boolean_handled_accumulator, NULL,
		              _gpinstruct_marshal_BOOLEAN__VOID,
		              G_TYPE_BOOLEAN, 0);

	lesson_view_page_signals[SHOW_CURRENT] =
		g_signal_new ("show-current",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructLessonViewPageClass, show_current),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);

	lesson_view_page_signals[RESET] =
		g_signal_new ("reset",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructLessonViewPageClass, reset),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);
}


GPInstructLessonViewPage *
gpinstruct_lesson_view_page_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_VIEW_PAGE, NULL);
}

const gchar *
gpinstruct_lesson_view_page_get_title (GPInstructLessonViewPage *page)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	return priv->title;
}

void
gpinstruct_lesson_view_page_set_title (GPInstructLessonViewPage *page,
                                       const gchar *title)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	g_free (priv->title);
	priv->title = g_strdup (title);
}

gboolean
gpinstruct_lesson_view_page_get_show_next_button (GPInstructLessonViewPage *page)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	return priv->next;
}

void
gpinstruct_lesson_view_page_set_show_next_button (GPInstructLessonViewPage *page,
                                                  gboolean show)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	priv->next = show;
}

gboolean
gpinstruct_lesson_view_page_get_show_back_button (GPInstructLessonViewPage *page)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	return priv->back;
}

void
gpinstruct_lesson_view_page_set_show_back_button (GPInstructLessonViewPage *page,
                                                  gboolean show)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	priv->back = show;
}

gboolean
gpinstruct_lesson_view_page_show_next (GPInstructLessonViewPage *page)
{
	gboolean clicked = FALSE;
	g_signal_emit (page, lesson_view_page_signals[SHOW_NEXT], 0, &clicked);
	return clicked;
}

gboolean
gpinstruct_lesson_view_page_show_previous (GPInstructLessonViewPage *page)
{
	gboolean clicked = FALSE;
	g_signal_emit (page, lesson_view_page_signals[SHOW_PREVIOUS], 0, &clicked);
	return clicked;
}

void
gpinstruct_lesson_view_page_show_current (GPInstructLessonViewPage *page)
{
	g_signal_emit (page, lesson_view_page_signals[SHOW_CURRENT], 0);
}

void
gpinstruct_lesson_view_page_reset (GPInstructLessonViewPage *page)
{
	g_signal_emit (page, lesson_view_page_signals[RESET], 0);
}

void
gpinstruct_lesson_view_page_set_message (GPInstructLessonViewPage *page,
                                         GPInstructMessageType message)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	priv->message = message;
}

GPInstructMessageType
gpinstruct_lesson_view_page_get_message (GPInstructLessonViewPage *page)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	return priv->message;
}

void
gpinstruct_lesson_view_page_set_explanation (GPInstructLessonViewPage *page,
                                             const gchar *explanation)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	g_free (priv->explanation);
	priv->explanation = g_strdup (explanation);
}

const gchar *
gpinstruct_lesson_view_page_get_explanation (GPInstructLessonViewPage *page)
{
	GPInstructLessonViewPagePrivate *priv = page->priv;

	return priv->explanation;
}
