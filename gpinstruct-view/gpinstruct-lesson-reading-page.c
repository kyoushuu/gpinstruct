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

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"



G_DEFINE_TYPE (GPInstructLessonReadingPage, gpinstruct_lesson_reading_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_reading_page_init (GPInstructLessonReadingPage *object)
{
	/* TODO: Add initialization code here */
}

static void
gpinstruct_lesson_reading_page_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gpinstruct_lesson_reading_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_reading_page_class_init (GPInstructLessonReadingPageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass* parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	object_class->finalize = gpinstruct_lesson_reading_page_finalize;
}


GPInstructLessonReadingPage*
gpinstruct_lesson_reading_page_new (GPInstructLessonReading* reading)
{
	GPInstructLessonReadingPage* page = g_object_new (GPINSTRUCT_TYPE_LESSON_READING_PAGE, NULL);
	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page), gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (reading)));

	GtkWidget* reading_label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (reading_label), gpinstruct_lesson_reading_get_text (reading));
	gtk_label_set_line_wrap (GTK_LABEL (reading_label), TRUE);
	gtk_label_set_selectable (GTK_LABEL (reading_label), TRUE);
	gtk_label_set_justify (GTK_LABEL (reading_label), GTK_JUSTIFY_FILL);
	gtk_misc_set_alignment (GTK_MISC (reading_label), 0.0, 0.0);
	gtk_misc_set_padding (GTK_MISC (reading_label), 5, 5);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), reading_label);

	return page;
}
