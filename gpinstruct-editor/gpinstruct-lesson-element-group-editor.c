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
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-editor/gpinstruct-editor.h"

#include "compat/compat-gtk.h"

struct _GPInstructLessonElementGroupEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructLessonElementGroup *group;

	GtkWidget *title_label;
	GtkWidget *title_entry;

	GtkWidget *single_score_label;
	GtkWidget *single_score_switch;

	GtkWidget *single_directions_label;
	GtkWidget *single_directions_switch;

	GtkWidget *directions_label;
	GtkWidget *directions_view;
};

#define GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP_EDITOR, GPInstructLessonElementGroupEditorPrivate))

G_DEFINE_TYPE (GPInstructLessonElementGroupEditor, gpinstruct_lesson_element_group_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_element_group_editor_init (GPInstructLessonElementGroupEditor *object)
{
	object->priv = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR_GET_PRIVATE (object);

	object->priv->title_label = gtk_label_new (_("Title:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->title_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	object->priv->title_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (object), object->priv->title_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	object->priv->single_score_label = gtk_label_new (_("Single Score:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->single_score_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	object->priv->single_score_switch = gtk_switch_new ();
	gtk_table_attach (GTK_TABLE (object), object->priv->single_score_switch,
	                  1, 2, 1, 2,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	object->priv->single_directions_label = gtk_label_new (_("Single Directions:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->single_directions_label,
	                  0, 1, 2, 3,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	object->priv->single_directions_switch = gtk_switch_new ();
	gtk_table_attach (GTK_TABLE (object), object->priv->single_directions_switch,
	                  1, 2, 2, 3,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	object->priv->directions_label = gtk_label_new (_("Directions:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->directions_label,
	                  0, 1, 3, 4,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	GtkWidget *directions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (directions_view_scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	object->priv->directions_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (directions_view_scrolled_window), object->priv->directions_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (object->priv->directions_view), GTK_WRAP_WORD_CHAR);
	gtk_table_attach (GTK_TABLE (object), directions_view_scrolled_window,
	                  1, 2, 3, 4,
	                  GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);
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

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (editor->priv->group),
	                                     gtk_entry_get_text (GTK_ENTRY (editor->priv->title_entry)));
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (editor->priv->window, (gpointer)editor->priv->group);
}

static void
single_score_activate (GObject    *gobject,
                       GParamSpec *pspec,
                       gpointer    user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);

	gboolean active = gtk_switch_get_active (GTK_SWITCH (editor->priv->single_score_switch));

	if (active != gpinstruct_lesson_element_group_get_single_score (editor->priv->group))
	{
		gpinstruct_lesson_element_group_set_single_score (editor->priv->group,
		                                                  active);
		gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	}
}

static void
single_directions_activate (GObject    *gobject,
                            GParamSpec *pspec,
                            gpointer    user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);

	gboolean active = gtk_switch_get_active (GTK_SWITCH (editor->priv->single_directions_switch));

	if (active != gpinstruct_lesson_element_group_get_single_directions (editor->priv->group))
	{
		gpinstruct_lesson_element_group_set_single_directions (editor->priv->group,
		                                                       active);
		gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	}
}

static void
directions_buffer_changed (GtkTextBuffer *textbuffer,
                           gpointer       user_data)
{
	GPInstructLessonElementGroupEditor *editor = GPINSTRUCT_LESSON_ELEMENT_GROUP_EDITOR (user_data);

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_element_group_set_directions (editor->priv->group,
	                                                text);
	g_free (text);
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
}

GPInstructLessonElementGroupEditor *
gpinstruct_lesson_element_group_editor_new (GPInstructEditorWindow *window,
                                            GPInstructLessonElementGroup *group)
{
	GPInstructLessonElementGroupEditor *editor = g_object_new (GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP_EDITOR, NULL);

	GtkTextBuffer *buffer;

	editor->priv->window = window;
	editor->priv->group = group;

	gtk_switch_set_active (GTK_SWITCH (editor->priv->single_score_switch),
	                       gpinstruct_lesson_element_group_get_single_score (group));
	g_signal_connect (editor->priv->single_score_switch, "notify::active",
	                  G_CALLBACK (single_score_activate), editor);

	gtk_switch_set_active (GTK_SWITCH (editor->priv->single_directions_switch),
	                       gpinstruct_lesson_element_group_get_single_directions (group));
	g_signal_connect (editor->priv->single_directions_switch, "notify::active",
	                  G_CALLBACK (single_directions_activate), editor);

	gtk_entry_set_text (GTK_ENTRY (editor->priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (group)));
	g_signal_connect (editor->priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->priv->directions_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_element_group_get_directions (group), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (directions_buffer_changed), editor);

	return editor;
}
