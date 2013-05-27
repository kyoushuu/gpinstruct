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
#include "gpinstruct-editor/gpinstruct-editor.h"

struct _GPInstructLessonElementGroupEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructLessonElementGroup *group;

	GtkWidget *title_entry;
	GtkWidget *single_score_switch;
	GtkWidget *single_directions_switch;
	GtkWidget *directions_view;
};

#define GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP_EDITOR, GPInstructLessonElementGroupEditorPrivate))

G_DEFINE_TYPE (GPInstructLessonElementGroupEditor, gpinstruct_lesson_element_group_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_element_group_editor_init (GPInstructLessonElementGroupEditor *object)
{
	GtkStyleContext *context;

	object->priv = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR_GET_PRIVATE (object);
	GPInstructLessonElementGroupEditorPrivate *priv = object->priv;

	GtkWidget *title_label = gtk_label_new (_("Title"));
	context = gtk_widget_get_style_context (title_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (title_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), title_label);

	priv->title_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (priv->title_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->title_entry,
	                         title_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *single_score_label = gtk_label_new (_("Single Score"));
	context = gtk_widget_get_style_context (single_score_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (single_score_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), single_score_label);

	priv->single_score_switch = gtk_switch_new ();
	gtk_widget_set_halign (priv->single_score_switch, GTK_ALIGN_START);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->single_score_switch,
	                         single_score_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *single_directions_label = gtk_label_new (_("Single Directions"));
	context = gtk_widget_get_style_context (single_directions_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (single_directions_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), single_directions_label);

	priv->single_directions_switch = gtk_switch_new ();
	gtk_widget_set_halign (priv->single_directions_switch, GTK_ALIGN_START);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->single_directions_switch,
	                         single_directions_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *directions_label = gtk_label_new (_("Directions"));
	context = gtk_widget_get_style_context (directions_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (directions_label, GTK_ALIGN_END);
	gtk_widget_set_valign (directions_label, GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (object), directions_label);

	GtkWidget *directions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (directions_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	priv->directions_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (directions_view_scrolled_window), priv->directions_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->directions_view), GTK_WRAP_WORD_CHAR);
	gtk_widget_set_hexpand (priv->directions_view, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), directions_view_scrolled_window,
	                         directions_label, GTK_POS_RIGHT, 1, 3);

	g_object_bind_property (priv->single_directions_switch, "active",
	                        priv->directions_view, "sensitive",
	                        G_BINDING_SYNC_CREATE);
}

static void
gpinstruct_lesson_element_group_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_lesson_element_group_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_element_group_editor_class_init (GPInstructLessonElementGroupEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonElementGroupEditorPrivate));

	object_class->finalize = gpinstruct_lesson_element_group_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);
	GPInstructLessonElementGroupEditorPrivate *priv = editor->priv;

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (priv->group),
	                                     gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->group);
}

static void
single_score_activate (GObject    *gobject,
                       GParamSpec *pspec,
                       gpointer    user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);
	GPInstructLessonElementGroupEditorPrivate *priv = editor->priv;

	gboolean active = gtk_switch_get_active (GTK_SWITCH (priv->single_score_switch));

	if (active != gpinstruct_lesson_element_group_get_single_score (priv->group))
	{
		gpinstruct_lesson_element_group_set_single_score (priv->group,
		                                                  active);
		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}

static void
single_directions_activate (GObject    *gobject,
                            GParamSpec *pspec,
                            gpointer    user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);
	GPInstructLessonElementGroupEditorPrivate *priv = editor->priv;

	gboolean active = gtk_switch_get_active (GTK_SWITCH (priv->single_directions_switch));

	if (active != gpinstruct_lesson_element_group_get_single_directions (priv->group))
	{
		gpinstruct_lesson_element_group_set_single_directions (priv->group,
		                                                       active);
		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}

static void
directions_buffer_changed (GtkTextBuffer *textbuffer,
                           gpointer       user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);
	GPInstructLessonElementGroupEditorPrivate *priv = editor->priv;

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_element_group_set_directions (priv->group,
	                                                text);
	g_free (text);
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}

GPInstructLessonElementGroupEditor *
gpinstruct_lesson_element_group_editor_new (GPInstructEditorWindow *window,
                                            GPInstructLessonElementGroup *group)
{
	GPInstructLessonElementGroupEditor *editor = g_object_new (GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP_EDITOR, NULL);
	GPInstructLessonElementGroupEditorPrivate *priv = editor->priv;

	GtkTextBuffer *buffer;

	priv->window = window;
	priv->group = group;

	gtk_switch_set_active (GTK_SWITCH (priv->single_score_switch),
	                       gpinstruct_lesson_element_group_get_single_score (group));
	g_signal_connect (priv->single_score_switch, "notify::active",
	                  G_CALLBACK (single_score_activate), editor);

	gtk_switch_set_active (GTK_SWITCH (priv->single_directions_switch),
	                       gpinstruct_lesson_element_group_get_single_directions (group));
	g_signal_connect (priv->single_directions_switch, "notify::active",
	                  G_CALLBACK (single_directions_activate), editor);

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (group)));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->directions_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_element_group_get_directions (group), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (directions_buffer_changed), editor);

	return editor;
}
