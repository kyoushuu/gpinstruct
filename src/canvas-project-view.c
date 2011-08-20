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

#include "canvas.h"
#include "canvas-view.h"

struct _CanvasProjectViewPrivate
{
	CanvasProject* project;
	CanvasMessagePool* pool;
	CanvasLog* log;

	GHashTable* hashtable;
};

#define CANVAS_PROJECT_VIEW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_PROJECT_VIEW, CanvasProjectViewPrivate))



void
lesson_button_clicked (GtkButton *button,
                       gpointer   user_data)
{
	CanvasProjectView* view = CANVAS_PROJECT_VIEW (user_data);

	CanvasLessonView* lesson_view = g_hash_table_lookup (view->priv->hashtable, button);

	if (lesson_view)
	{
		gtk_dialog_run (GTK_DIALOG (lesson_view));
		gtk_widget_hide (GTK_WIDGET (lesson_view));
	}
}



G_DEFINE_TYPE (CanvasProjectView, canvas_project_view, GTK_TYPE_WINDOW);

static void
canvas_project_view_init (CanvasProjectView *object)
{
	object->priv = CANVAS_PROJECT_VIEW_PRIVATE (object);

	object->priv->hashtable = g_hash_table_new (NULL, NULL);

	gtk_widget_set_size_request (GTK_WIDGET (object), 600, 400);
}

static void
canvas_project_view_finalize (GObject *object)
{
	CanvasProjectView* view = CANVAS_PROJECT_VIEW (object);

	if (view->priv->hashtable)
	{
		GList* views = g_hash_table_get_values (view->priv->hashtable);
		GList* current_views = views;

		while (current_views)
		{
			GtkWidget* view = GTK_WIDGET (current_views->data);
			gtk_widget_destroy (view);
			current_views = current_views->next;
		}

		g_list_free (views);

		g_hash_table_destroy (view->priv->hashtable);
	}

	if (view->priv->project)
		g_object_unref (view->priv->project);

	if (view->priv->pool)
		g_object_unref (view->priv->pool);

	if (view->priv->log)
		g_object_unref (view->priv->log);

	G_OBJECT_CLASS (canvas_project_view_parent_class)->finalize (object);
}

static void
canvas_project_view_class_init (CanvasProjectViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasProjectViewPrivate));

	object_class->finalize = canvas_project_view_finalize;
}


GtkWidget*
canvas_project_view_new (CanvasProject* project, CanvasMessagePool* pool, CanvasLog* log)
{
	g_return_val_if_fail (project != NULL, NULL);

	CanvasProjectView* view = g_object_new (CANVAS_TYPE_PROJECT_VIEW, NULL);

	if (pool)
		view->priv->pool = g_object_ref (pool);
	else
	{
		view->priv->pool = canvas_message_pool_new ();
		canvas_message_pool_add_multiple (view->priv->pool,
		                                  CANVAS_MESSAGE_TYPE_CORRECT,		"Your last answer is correct.",
		                                  CANVAS_MESSAGE_TYPE_CORRECT_ALL,	"All of your answers are correct.",
		                                  CANVAS_MESSAGE_TYPE_WRONG,		"Your last answer is wrong.",
		                                  CANVAS_MESSAGE_TYPE_WRONG_SOME,	"Some of your answers are wrong.",
		                                  CANVAS_MESSAGE_TYPE_WRONG_ALL,	"All of your answers are wrong.",
		                                  CANVAS_MESSAGE_TYPE_PASS,			"You passed the test.",
		                                  CANVAS_MESSAGE_TYPE_FAIL,			"You failed the test.",
		                                  CANVAS_MESSAGE_TYPE_NONE);
	}

	view->priv->log = g_object_ref (log);

	view->priv->project = g_object_ref (project);
	gtk_window_set_default_size (GTK_WINDOW (view), 600, 400);
	gtk_window_set_title (GTK_WINDOW (view), canvas_project_get_title (project));

	GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
	                                GTK_POLICY_AUTOMATIC,
	                                GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (view), scrolled_window);

	GtkWidget* main_vbox = gtk_hbox_new (TRUE, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), main_vbox);

	GtkWidget *category_frame, *category_label, *category_vbox, *lesson_button;

	GList *categories, *curr_categories, *lessons, *curr_lessons;

	CanvasCategory* category;
	CanvasLesson* lesson;

	gchar* title;

	categories = canvas_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		category = CANVAS_CATEGORY (curr_categories->data);

		category_frame = gtk_frame_new (NULL);

		category_label = gtk_label_new (NULL);
		title = g_strdup_printf ("<b><big>%s</big></b>", canvas_category_get_title (category));
		gtk_label_set_markup (GTK_LABEL (category_label), title);
		g_free (title);
		gtk_frame_set_label_widget (GTK_FRAME (category_frame), category_label);

		category_vbox = gtk_vbox_new (FALSE, 3);
		gtk_container_add (GTK_CONTAINER (category_frame), category_vbox);

		gtk_box_pack_start (GTK_BOX (main_vbox), category_frame, FALSE, TRUE, 3);

		lessons = canvas_category_get_lessons (category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			lesson = CANVAS_LESSON (curr_lessons->data);

			lesson_button = gtk_button_new ();
			gtk_button_set_label (GTK_BUTTON (lesson_button), canvas_lesson_get_title (lesson));
			gtk_box_pack_start (GTK_BOX (category_vbox), lesson_button, FALSE, FALSE, 3);
			g_signal_connect (lesson_button, "clicked", G_CALLBACK (lesson_button_clicked), view);

			CanvasLessonView* lesson_view = canvas_lesson_view_new (lesson, view->priv->pool, view->priv->log);

			g_hash_table_insert (view->priv->hashtable, lesson_button, lesson_view);

			curr_lessons = curr_lessons->next;
		}

		g_list_free (lessons);

		curr_categories = curr_categories->next;
	}

	g_list_free (categories);

	return (GtkWidget*)view;
}
