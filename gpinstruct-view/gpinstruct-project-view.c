/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja <kyoushuu@yahoo.com>
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
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"

struct _GPInstructProjectViewPrivate
{
	GPInstructProject *project;
	GPInstructMessagePool *pool;
	GPInstructLog *log;

	GHashTable *hashtable;
};

#define GPINSTRUCT_PROJECT_VIEW_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_PROJECT_VIEW, GPInstructProjectViewPrivate))



static void
lesson_button_clicked (GtkButton *button,
                       gpointer   user_data)
{
	GPInstructProjectView *view = GPINSTRUCT_PROJECT_VIEW (user_data);
	GPInstructProjectViewPrivate *priv = view->priv;

	GPInstructLessonView *lesson_view = g_hash_table_lookup (priv->hashtable, button);

	if (lesson_view)
	{
		gtk_dialog_run (GTK_DIALOG (lesson_view));
		gtk_widget_hide (GTK_WIDGET (lesson_view));
	}
}

static void
apply_css (GtkWidget *widget, GtkStyleProvider *provider)
{
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
	if (GTK_IS_CONTAINER (widget))
		gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
}



G_DEFINE_TYPE (GPInstructProjectView, gpinstruct_project_view, GTK_TYPE_WINDOW);

static void
gpinstruct_project_view_init (GPInstructProjectView *object)
{
	object->priv = GPINSTRUCT_PROJECT_VIEW_GET_PRIVATE (object);
	GPInstructProjectViewPrivate *priv = object->priv;

	g_object_set (object, "icon-name", "gpinstruct-view", NULL);

	priv->hashtable = g_hash_table_new (NULL, NULL);

	gtk_widget_set_size_request (GTK_WIDGET (object), 600, 400);
}

static void
gpinstruct_project_view_finalize (GObject *object)
{
	GPInstructProjectView *view = GPINSTRUCT_PROJECT_VIEW (object);
	GPInstructProjectViewPrivate *priv = view->priv;

	if (priv->hashtable)
	{
		GList *views = g_hash_table_get_values (priv->hashtable);
		GList *current_views = views;

		while (current_views)
		{
			GtkWidget *view = GTK_WIDGET (current_views->data);
			gtk_widget_destroy (view);
			current_views = current_views->next;
		}

		g_list_free (views);

		g_hash_table_destroy (priv->hashtable);
	}

	if (priv->project)
		g_object_unref (priv->project);

	if (priv->pool)
		g_object_unref (priv->pool);

	if (priv->log)
		g_object_unref (priv->log);

	G_OBJECT_CLASS (gpinstruct_project_view_parent_class)->finalize (object);
}

static void
gpinstruct_project_view_class_init (GPInstructProjectViewClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass *parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructProjectViewPrivate));

	object_class->finalize = gpinstruct_project_view_finalize;
}


GtkWidget *
gpinstruct_project_view_new (GPInstructProject *project,
                             GPInstructMessagePool *pool,
                             GPInstructLog *log)
{
	g_return_val_if_fail (project != NULL, NULL);

	GPInstructProjectView *view = g_object_new (GPINSTRUCT_TYPE_PROJECT_VIEW, NULL);
	GPInstructProjectViewPrivate *priv = view->priv;

	if (pool)
		priv->pool = g_object_ref (pool);
	else
	{
		priv->pool = gpinstruct_message_pool_new ();
		gpinstruct_message_pool_add_multiple (priv->pool,
		                                      GPINSTRUCT_MESSAGE_TYPE_CORRECT,		_("Your last answer is correct."),
		                                      GPINSTRUCT_MESSAGE_TYPE_CORRECT_ALL,	_("All of your answers are correct."),
		                                      GPINSTRUCT_MESSAGE_TYPE_WRONG,		_("Your last answer is wrong."),
		                                      GPINSTRUCT_MESSAGE_TYPE_WRONG_SOME,	_("Some of your answers are wrong."),
		                                      GPINSTRUCT_MESSAGE_TYPE_WRONG_MOST,	_("Most of your answers are wrong."),
		                                      GPINSTRUCT_MESSAGE_TYPE_WRONG_ALL,	_("All of your answers are wrong."),
		                                      GPINSTRUCT_MESSAGE_TYPE_PASS,			_("You passed the test."),
		                                      GPINSTRUCT_MESSAGE_TYPE_FAIL,			_("You failed the test."),
		                                      GPINSTRUCT_MESSAGE_TYPE_NONE);
	}

	if (log)
		priv->log = g_object_ref (log);

	priv->project = g_object_ref (project);
	gtk_window_set_default_size (GTK_WINDOW (view), 600, 400);
	gtk_window_set_title (GTK_WINDOW (view), gpinstruct_project_get_title (project));

	GtkCssProvider *provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_data (provider, gpinstruct_project_get_stylesheet (project), -1, NULL);

	apply_css (GTK_WIDGET (view), GTK_STYLE_PROVIDER (provider));

	GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
	                                GTK_POLICY_AUTOMATIC,
	                                GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (view), scrolled_window);

	GtkWidget *main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 3);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), main_vbox);

	GtkWidget *category_frame, *category_label, *category_vbox, *lesson_button;

	GList *categories, *curr_categories, *lessons, *curr_lessons;

	GPInstructCategory *category;
	GPInstructLesson *lesson;

	gchar *title;

	categories = gpinstruct_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		category = GPINSTRUCT_CATEGORY (curr_categories->data);

		category_frame = gtk_frame_new (NULL);

		category_label = gtk_label_new (NULL);
		title = g_strdup_printf ("<b><big>%s</big></b>", gpinstruct_category_get_title (category));
		gtk_label_set_markup (GTK_LABEL (category_label), title);
		g_free (title);
		gtk_frame_set_label_widget (GTK_FRAME (category_frame), category_label);

		category_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
		gtk_container_add (GTK_CONTAINER (category_frame), category_vbox);

		gtk_box_pack_start (GTK_BOX (main_vbox), category_frame, FALSE, TRUE, 3);

		lessons = gpinstruct_category_get_lessons (category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			lesson = GPINSTRUCT_LESSON (curr_lessons->data);

			lesson_button = gtk_button_new ();
			gtk_button_set_label (GTK_BUTTON (lesson_button), gpinstruct_lesson_get_title (lesson));
			gtk_button_set_relief (GTK_BUTTON (lesson_button), GTK_RELIEF_NONE);
			gtk_box_pack_start (GTK_BOX (category_vbox), lesson_button, FALSE, FALSE, 0);
			g_signal_connect (lesson_button, "clicked", G_CALLBACK (lesson_button_clicked), view);

			GPInstructLessonView *lesson_view = gpinstruct_lesson_view_new (lesson, priv->pool, priv->log);
			apply_css (GTK_WIDGET (lesson_view), GTK_STYLE_PROVIDER (provider));

			g_hash_table_insert (priv->hashtable, lesson_button, lesson_view);

			curr_lessons = curr_lessons->next;
		}

		g_list_free (lessons);

		curr_categories = curr_categories->next;
	}

	g_list_free (categories);

	g_object_unref (provider);

	return GTK_WIDGET (view);
}
