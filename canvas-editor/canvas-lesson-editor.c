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

#include <gtk/gtk.h>

#include "canvas/canvas.h"
#include "canvas-editor/canvas-editor.h"

struct _CanvasLessonEditorPrivate
{
	CanvasEditorWindow* window;
	CanvasLesson* lesson;

	GtkWidget* title_label;
	GtkWidget* title_entry;

	GtkWidget* singlescore_label;
	GtkWidget* singlescore_switch;
};

#define CANVAS_LESSON_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_EDITOR, CanvasLessonEditorPrivate))



G_DEFINE_TYPE (CanvasLessonEditor, canvas_lesson_editor, CANVAS_TYPE_OBJECT_EDITOR);

static void
canvas_lesson_editor_init (CanvasLessonEditor *object)
{
	object->priv = CANVAS_LESSON_EDITOR_PRIVATE (object);

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
#if GTK_MAJOR_VERSION >= 3
	object->priv->singlescore_switch = gtk_switch_new ();
#else
	object->priv->singlescore_switch = gtk_toggle_button_new_with_label (GTK_STOCK_NO);
	gtk_button_set_use_stock (GTK_BUTTON (object->priv->singlescore_switch), TRUE);
#endif
	gtk_table_attach (GTK_TABLE (object), object->priv->singlescore_switch,
	                  1, 2, 1, 2,
	                  GTK_SHRINK, GTK_SHRINK | GTK_FILL,
	                  3, 3);
}

static void
canvas_lesson_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (canvas_lesson_editor_parent_class)->finalize (object);
}

static void
canvas_lesson_editor_class_init (CanvasLessonEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectEditorClass* parent_class = CANVAS_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonEditorPrivate));

	object_class->finalize = canvas_lesson_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	CanvasLessonEditor* editor = CANVAS_LESSON_EDITOR (user_data);

	canvas_lesson_set_title (editor->priv->lesson,
	                         gtk_entry_get_text (GTK_ENTRY (editor->priv->title_entry)));
	canvas_editor_window_set_modified (editor->priv->window, TRUE);
	canvas_editor_window_update_tree_store (editor->priv->window, (gpointer)editor->priv->lesson);
}

#if GTK_MAJOR_VERSION >= 3
static void
singlescore_activate (GObject    *gobject,
                      GParamSpec *pspec,
                      gpointer    user_data)
#else
static void
singlescore_activate (GtkToggleButton *togglebutton,
                      gpointer         user_data)
#endif
{
	CanvasLessonEditor* editor = CANVAS_LESSON_EDITOR (user_data);

#if GTK_MAJOR_VERSION >= 3
	gboolean active = gtk_switch_get_active (GTK_SWITCH (editor->priv->singlescore_switch));
#else
	gboolean active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (editor->priv->singlescore_switch));
	gtk_button_set_label (GTK_BUTTON (editor->priv->singlescore_switch), active? GTK_STOCK_YES:GTK_STOCK_NO);
#endif

	if (active != canvas_lesson_get_single_score (editor->priv->lesson))
	{
		canvas_lesson_set_single_score (editor->priv->lesson,
		                                active);
		canvas_editor_window_set_modified (editor->priv->window, TRUE);
	}
}


CanvasLessonEditor*
canvas_lesson_editor_new (CanvasEditorWindow* window,
                          CanvasLesson* lesson)
{
	CanvasLessonEditor* editor = g_object_new (CANVAS_TYPE_LESSON_EDITOR, NULL);

	editor->priv->window = window;
	editor->priv->lesson = lesson;

	gtk_entry_set_text (GTK_ENTRY (editor->priv->title_entry),
	                    canvas_lesson_get_title (lesson));
	g_signal_connect (editor->priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

#if GTK_MAJOR_VERSION >= 3
	gtk_switch_set_active (GTK_SWITCH (editor->priv->singlescore_switch),
	                       canvas_lesson_get_single_score (lesson));
	g_signal_connect (editor->priv->singlescore_switch, "notify::active",
	                  G_CALLBACK (singlescore_activate), editor);
#else
	g_signal_connect (editor->priv->singlescore_switch, "toggled",
	                  G_CALLBACK (singlescore_activate), editor);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (editor->priv->singlescore_switch),
	                              canvas_lesson_get_single_score (lesson));
#endif

	return editor;
}
