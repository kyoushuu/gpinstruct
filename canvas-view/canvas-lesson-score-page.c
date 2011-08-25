/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"

struct _GPInstructLessonScorePagePrivate
{
	GPInstructLessonScore* score;

	GtkWidget* score_label;
	GtkWidget* percentage_label;
};

#define GPINSTRUCT_LESSON_SCORE_PAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_SCORE_PAGE, GPInstructLessonScorePagePrivate))



void
score_reset (GPInstructLessonViewPage* view,
             gpointer user_data)
{
	gpinstruct_lesson_score_clear (GPINSTRUCT_LESSON_SCORE_PAGE (view)->priv->score);
}


G_DEFINE_TYPE (GPInstructLessonScorePage, gpinstruct_lesson_score_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_score_page_init (GPInstructLessonScorePage *object)
{
	object->priv = GPINSTRUCT_LESSON_SCORE_PAGE_PRIVATE (object);

	object->priv->score = NULL;
}

static void
gpinstruct_lesson_score_page_finalize (GObject *object)
{
	GPInstructLessonScorePage* page = GPINSTRUCT_LESSON_SCORE_PAGE (object);

	if (page->priv->score)
		g_object_unref (page->priv->score);

	G_OBJECT_CLASS (gpinstruct_lesson_score_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_score_page_class_init (GPInstructLessonScorePageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GPInstructLessonViewPageClass* parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GPInstructLessonScorePagePrivate));

	object_class->finalize = gpinstruct_lesson_score_page_finalize;
	parent_class->reset = score_reset;
}


void
page_show (GPInstructLessonScorePage* page,
           gpointer user_data)
{
	guint score_value = gpinstruct_lesson_score_get_score (page->priv->score);
	guint total_value = gpinstruct_lesson_score_get_total (page->priv->score);

	gchar* score_text = g_strdup_printf ("<span size='xx-large'><big>%d</big>/%d</span>",
	                                     score_value,
	                                     total_value);
	gtk_label_set_markup (GTK_LABEL (page->priv->score_label), score_text);
	g_free (score_text);

	gchar* percentage_text = g_strdup_printf ("<span size='xx-large'>%.2f%%</span>",
	                                          total_value? (double)100 * score_value / total_value : 0);
	gtk_label_set_markup (GTK_LABEL (page->priv->percentage_label), percentage_text);
	g_free (percentage_text);
}

GPInstructLessonScorePage*
gpinstruct_lesson_score_page_new (GPInstructLessonScore* score)
{
	GPInstructLessonScorePage* page = g_object_new (GPINSTRUCT_TYPE_LESSON_SCORE_PAGE, NULL);

	page->priv->score = score;

	guint score_value = gpinstruct_lesson_score_get_score (score);
	guint total_value = gpinstruct_lesson_score_get_total (score);

	g_signal_connect (GPINSTRUCT_LESSON_VIEW_PAGE (page), "show-current", G_CALLBACK (page_show), NULL);

	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page), gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (score)));
	gpinstruct_lesson_view_page_set_show_back_button (GPINSTRUCT_LESSON_VIEW_PAGE (page), FALSE);

	GtkWidget* vbox = gtk_vbox_new (FALSE, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), vbox);

	GtkWidget* score_label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (score_label), _("<b>Score:</b>"));
	gtk_misc_set_alignment (GTK_MISC (score_label), 0.0, 0.0);
	gtk_box_pack_start (GTK_BOX (vbox), score_label, FALSE, TRUE, 3);

	GtkWidget* score_label_value = gtk_label_new (NULL);
	gchar* score_text = g_strdup_printf ("<span size='xx-large'><big>%d</big>/%d</span>",
	                                     score_value,
	                                     total_value);
	gtk_label_set_markup (GTK_LABEL (score_label_value), score_text);
	g_free (score_text);
	gtk_label_set_selectable (GTK_LABEL (score_label_value), TRUE);
	gtk_misc_set_alignment (GTK_MISC (score_label_value), 0.5, 0.5);
	gtk_box_pack_start (GTK_BOX (vbox), score_label_value, TRUE, TRUE, 3);
	page->priv->score_label = score_label_value;

	GtkWidget* percentage_label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (percentage_label), _("<b>Percentage:</b>"));
	gtk_misc_set_alignment (GTK_MISC (percentage_label), 0.0, 0.0);
	gtk_box_pack_start (GTK_BOX (vbox), percentage_label, FALSE, TRUE, 3);

	GtkWidget* percentage_label_value = gtk_label_new (NULL);
	gchar* percentage_text = g_strdup_printf ("<span size='xx-large'>%.2f%%</span>",
	                                          total_value? (double)100 * score_value / total_value : 0);
	gtk_label_set_markup (GTK_LABEL (percentage_label_value), percentage_text);
	g_free (percentage_text);
	gtk_label_set_selectable (GTK_LABEL (percentage_label_value), TRUE);
	gtk_misc_set_alignment (GTK_MISC (percentage_label_value), 0.5, 0.5);
	gtk_box_pack_start (GTK_BOX (vbox), percentage_label_value, TRUE, TRUE, 3);
	page->priv->percentage_label = percentage_label_value;

	return page;
}
