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
#include "canvas-view/canvas-view.h"



G_DEFINE_TYPE (CanvasLessonReadingPage, canvas_lesson_reading_page, CANVAS_TYPE_LESSON_VIEW_PAGE);

static void
canvas_lesson_reading_page_init (CanvasLessonReadingPage *object)
{
	/* TODO: Add initialization code here */
}

static void
canvas_lesson_reading_page_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (canvas_lesson_reading_page_parent_class)->finalize (object);
}

static void
canvas_lesson_reading_page_class_init (CanvasLessonReadingPageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonViewPageClass* parent_class = CANVAS_LESSON_VIEW_PAGE_CLASS (klass);*/

	object_class->finalize = canvas_lesson_reading_page_finalize;
}


CanvasLessonReadingPage*
canvas_lesson_reading_page_new (CanvasLessonReading* reading)
{
	CanvasLessonReadingPage* page = g_object_new (CANVAS_TYPE_LESSON_READING_PAGE, NULL);
	canvas_lesson_view_page_set_title (CANVAS_LESSON_VIEW_PAGE (page), canvas_lesson_element_get_title (CANVAS_LESSON_ELEMENT (reading)));

	GtkWidget* reading_label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (reading_label), canvas_lesson_reading_get_text (reading));
	gtk_label_set_line_wrap (GTK_LABEL (reading_label), TRUE);
	gtk_label_set_selectable (GTK_LABEL (reading_label), TRUE);
	gtk_label_set_justify (GTK_LABEL (reading_label), GTK_JUSTIFY_FILL);
	gtk_misc_set_alignment (GTK_MISC (reading_label), 0.0, 0.0);
	gtk_misc_set_padding (GTK_MISC (reading_label), 5, 5);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), reading_label);

	return page;
}
