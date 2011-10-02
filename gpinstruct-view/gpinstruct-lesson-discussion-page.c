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
#include "gtktextbuffermarkup.h"



G_DEFINE_TYPE (GPInstructLessonDiscussionPage, gpinstruct_lesson_discussion_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_discussion_page_init (GPInstructLessonDiscussionPage *object)
{
	/* TODO: Add initialization code here */
}

static void
gpinstruct_lesson_discussion_page_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gpinstruct_lesson_discussion_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_discussion_page_class_init (GPInstructLessonDiscussionPageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass* parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	object_class->finalize = gpinstruct_lesson_discussion_page_finalize;
}


GPInstructLessonDiscussionPage*
gpinstruct_lesson_discussion_page_new (GPInstructLessonDiscussion *discussion)
{
	GPInstructLessonDiscussionPage* page = g_object_new (GPINSTRUCT_TYPE_LESSON_DISCUSSION_PAGE, NULL);
	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page), gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (discussion)));

	GtkWidget* discussion_textview = gtk_text_view_new ();
	gtk_text_buffer_set_markup (gtk_text_view_get_buffer (GTK_TEXT_VIEW (discussion_textview)),
	                            gpinstruct_lesson_discussion_get_text (discussion));
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (discussion_textview), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (discussion_textview), FALSE);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), discussion_textview);

	return page;
}
