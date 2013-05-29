/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2013  Arnel A. Borja <kyoushuu@yahoo.com>
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

#include "config.h"
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"



G_DEFINE_TYPE (GPInstructLessonObjectivePage, gpinstruct_lesson_objective_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_objective_page_init (GPInstructLessonObjectivePage *object)
{
	/* TODO: Add initialization code here */
}

static void
gpinstruct_lesson_objective_page_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gpinstruct_lesson_objective_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_objective_page_class_init (GPInstructLessonObjectivePageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass *parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	object_class->finalize = gpinstruct_lesson_objective_page_finalize;
}


GPInstructLessonObjectivePage *
gpinstruct_lesson_objective_page_new (const gchar *title,
                                       const gchar *text)
{
	GPInstructLessonObjectivePage *page = g_object_new (GPINSTRUCT_TYPE_LESSON_OBJECTIVE_PAGE, NULL);
	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page), title);
	gpinstruct_lesson_view_page_set_show_back_button (GPINSTRUCT_LESSON_VIEW_PAGE (page), FALSE);

	GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), vbox);

	GtkWidget *title_label = gtk_label_new (NULL);
	gchar *title_text = g_strdup_printf ("<b><big>%s</big></b>", title);
	gtk_label_set_markup (GTK_LABEL (title_label), title_text);
	g_free (title_text);
	gtk_misc_set_alignment (GTK_MISC (title_label), 0.0, 0.5);
	gtk_box_pack_start (GTK_BOX (vbox), title_label, FALSE, TRUE, 3);

	GtkWidget *objective_label_top = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (objective_label_top), _("<b>Objective:</b>"));
	gtk_misc_set_alignment (GTK_MISC (objective_label_top), 0.0, 0.5);
	gtk_box_pack_start (GTK_BOX (vbox), objective_label_top, FALSE, TRUE, 3);

	GtkWidget *objective_label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (objective_label), text);
	gtk_label_set_line_wrap (GTK_LABEL (objective_label), TRUE);
	gtk_label_set_selectable (GTK_LABEL (objective_label), TRUE);
	gtk_misc_set_alignment (GTK_MISC (objective_label), 0.0, 0.0);
	gtk_misc_set_padding (GTK_MISC (objective_label), 5, 5);
	gtk_box_pack_start (GTK_BOX (vbox), objective_label, TRUE, TRUE, 3);

	return page;
}
