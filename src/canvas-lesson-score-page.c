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

typedef struct _CanvasLessonScorePagePrivate CanvasLessonScorePagePrivate;
struct _CanvasLessonScorePagePrivate
{
	CanvasLessonScore* score;

	GtkWidget* score_label;
	GtkWidget* percentage_label;
};

#define CANVAS_LESSON_SCORE_PAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_SCORE_PAGE, CanvasLessonScorePagePrivate))



G_DEFINE_TYPE (CanvasLessonScorePage, canvas_lesson_score_page, CANVAS_TYPE_LESSON_VIEW_PAGE);

static void
canvas_lesson_score_page_init (CanvasLessonScorePage *object)
{
	CANVAS_LESSON_SCORE_PAGE_PRIVATE (object)->score = NULL;
}

static void
canvas_lesson_score_page_finalize (GObject *object)
{
	CanvasLessonScorePagePrivate* priv = CANVAS_LESSON_SCORE_PAGE_PRIVATE (object);

	if (priv->score)
		g_object_unref (priv->score);

	G_OBJECT_CLASS (canvas_lesson_score_page_parent_class)->finalize (object);
}

static void
canvas_lesson_score_page_class_init (CanvasLessonScorePageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonViewPageClass* parent_class = CANVAS_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonScorePagePrivate));

	object_class->finalize = canvas_lesson_score_page_finalize;
}


void
page_show (CanvasLessonScorePage* page, gpointer user_data)
{
	CanvasLessonScorePagePrivate* priv = CANVAS_LESSON_SCORE_PAGE_PRIVATE (page);

	guint score_value = canvas_lesson_score_get_score (priv->score);
	guint total_value = canvas_lesson_score_get_total (priv->score);

	gchar* score_text = g_strdup_printf ("<span size='xx-large'><big>%d</big>/%d</span>",
	                                     score_value,
	                                     total_value);
	gtk_label_set_markup (GTK_LABEL (priv->score_label), score_text);
	g_free (score_text);

	gchar* percentage_text = g_strdup_printf ("<span size='xx-large'>%.2f%%</span>",
	                                          total_value? (double)100 * score_value / total_value : 0);
	gtk_label_set_markup (GTK_LABEL (priv->percentage_label), percentage_text);
	g_free (percentage_text);
}

CanvasLessonScorePage*
canvas_lesson_score_page_new (CanvasLessonScore* score)
{
	CanvasLessonScorePage* page = g_object_new (CANVAS_TYPE_LESSON_SCORE_PAGE, NULL);
	CanvasLessonScorePagePrivate* priv = CANVAS_LESSON_SCORE_PAGE_PRIVATE (page);

	priv->score = score;

	guint score_value = canvas_lesson_score_get_score (score);
	guint total_value = canvas_lesson_score_get_total (score);

	g_signal_connect (CANVAS_LESSON_VIEW_PAGE (page), "show-current", G_CALLBACK (page_show), NULL);

	canvas_lesson_view_page_set_title (CANVAS_LESSON_VIEW_PAGE (page), canvas_lesson_element_get_title (CANVAS_LESSON_ELEMENT (score)));
	canvas_lesson_view_page_set_show_back_button (CANVAS_LESSON_VIEW_PAGE (page), FALSE);

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
	priv->score_label = score_label_value;

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
	priv->percentage_label = percentage_label_value;

	return page;
}
