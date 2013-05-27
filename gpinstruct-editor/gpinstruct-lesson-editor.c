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

struct _GPInstructLessonEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructLesson *lesson;

	GtkWidget *title_entry;
	GtkWidget *single_score_switch;
};

#define GPINSTRUCT_LESSON_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_EDITOR, GPInstructLessonEditorPrivate))



G_DEFINE_TYPE (GPInstructLessonEditor, gpinstruct_lesson_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_editor_init (GPInstructLessonEditor *object)
{
	GtkStyleContext *context;

	object->priv = GPINSTRUCT_LESSON_EDITOR_GET_PRIVATE (object);
	GPInstructLessonEditorPrivate *priv = object->priv;

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
}

static void
gpinstruct_lesson_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_lesson_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_editor_class_init (GPInstructLessonEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonEditorPrivate));

	object_class->finalize = gpinstruct_lesson_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonEditor *editor = GPINSTRUCT_LESSON_EDITOR (user_data);
	GPInstructLessonEditorPrivate *priv = editor->priv;

	gpinstruct_lesson_set_title (priv->lesson,
	                             gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->lesson);
}

static void
single_score_activate (GObject    *gobject,
                       GParamSpec *pspec,
                       gpointer    user_data)
{
	GPInstructLessonEditor *editor = GPINSTRUCT_LESSON_EDITOR (user_data);
	GPInstructLessonEditorPrivate *priv = editor->priv;

	gboolean active = gtk_switch_get_active (GTK_SWITCH (priv->single_score_switch));

	if (active != gpinstruct_lesson_get_single_score (priv->lesson))
	{
		gpinstruct_lesson_set_single_score (priv->lesson,
		                                    active);
		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}


GPInstructLessonEditor *
gpinstruct_lesson_editor_new (GPInstructEditorWindow *window,
                              GPInstructLesson *lesson)
{
	GPInstructLessonEditor *editor = g_object_new (GPINSTRUCT_TYPE_LESSON_EDITOR, NULL);
	GPInstructLessonEditorPrivate *priv = editor->priv;

	priv->window = window;
	priv->lesson = lesson;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_lesson_get_title (lesson));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	gtk_switch_set_active (GTK_SWITCH (priv->single_score_switch),
	                       gpinstruct_lesson_get_single_score (lesson));
	g_signal_connect (priv->single_score_switch, "notify::active",
	                  G_CALLBACK (single_score_activate), editor);

	return editor;
}
