/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
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
#include "gpinstruct-editor/gpinstruct-editor.h"

#include "compat/compat-gtk.h"

struct _GPInstructLessonEditorPrivate
{
	GPInstructEditorWindow* window;
	GPInstructLesson* lesson;

	GtkWidget* title_label;
	GtkWidget* title_entry;

	GtkWidget* singlescore_label;
	GtkWidget* singlescore_switch;
};

#define GPINSTRUCT_LESSON_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_EDITOR, GPInstructLessonEditorPrivate))



G_DEFINE_TYPE (GPInstructLessonEditor, gpinstruct_lesson_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_editor_init (GPInstructLessonEditor *object)
{
	object->priv = GPINSTRUCT_LESSON_EDITOR_PRIVATE (object);

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

	object->priv->singlescore_label = gtk_label_new (_("Single Score:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->singlescore_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	object->priv->singlescore_switch = gtk_switch_new ();
	gtk_table_attach (GTK_TABLE (object), object->priv->singlescore_switch,
	                  1, 2, 1, 2,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);
}

static void
gpinstruct_lesson_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_lesson_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_editor_class_init (GPInstructLessonEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass* parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonEditorPrivate));

	object_class->finalize = gpinstruct_lesson_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonEditor* editor = GPINSTRUCT_LESSON_EDITOR (user_data);

	gpinstruct_lesson_set_title (editor->priv->lesson,
	                             gtk_entry_get_text (GTK_ENTRY (editor->priv->title_entry)));
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (editor->priv->window, (gpointer)editor->priv->lesson);
}

static void
singlescore_activate (GObject    *gobject,
                      GParamSpec *pspec,
                      gpointer    user_data)
{
	GPInstructLessonEditor* editor = GPINSTRUCT_LESSON_EDITOR (user_data);

	gboolean active = gtk_switch_get_active (GTK_SWITCH (editor->priv->singlescore_switch));

	if (active != gpinstruct_lesson_get_single_score (editor->priv->lesson))
	{
		gpinstruct_lesson_set_single_score (editor->priv->lesson,
		                                    active);
		gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	}
}


GPInstructLessonEditor*
gpinstruct_lesson_editor_new (GPInstructEditorWindow* window,
                              GPInstructLesson* lesson)
{
	GPInstructLessonEditor* editor = g_object_new (GPINSTRUCT_TYPE_LESSON_EDITOR, NULL);

	editor->priv->window = window;
	editor->priv->lesson = lesson;

	gtk_entry_set_text (GTK_ENTRY (editor->priv->title_entry),
	                    gpinstruct_lesson_get_title (lesson));
	g_signal_connect (editor->priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	gtk_switch_set_active (GTK_SWITCH (editor->priv->singlescore_switch),
	                       gpinstruct_lesson_get_single_score (lesson));
	g_signal_connect (editor->priv->singlescore_switch, "notify::active",
	                  G_CALLBACK (singlescore_activate), editor);

	return editor;
}
