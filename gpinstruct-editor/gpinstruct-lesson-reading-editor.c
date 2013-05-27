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

struct _GPInstructLessonReadingEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructLessonReading *reading;

	GtkWidget *title_entry;
	GtkWidget *text_view;
};

#define GPINSTRUCT_LESSON_READING_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_READING_EDITOR, GPInstructLessonReadingEditorPrivate))



G_DEFINE_TYPE (GPInstructLessonReadingEditor, gpinstruct_lesson_reading_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_reading_editor_init (GPInstructLessonReadingEditor *object)
{
	GtkStyleContext *context;

	object->priv = GPINSTRUCT_LESSON_READING_EDITOR_GET_PRIVATE (object);
	GPInstructLessonReadingEditorPrivate *priv = object->priv;

	GtkWidget *title_label = gtk_label_new (_("Title"));
	context = gtk_widget_get_style_context (title_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (title_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), title_label);

	priv->title_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (priv->title_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->title_entry,
	                         title_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *text_label = gtk_label_new (_("Text"));
	context = gtk_widget_get_style_context (text_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (text_label, GTK_ALIGN_END);
	gtk_widget_set_valign (text_label, GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (object), text_label);

	GtkWidget *text_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (text_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	priv->text_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (text_view_scrolled_window), priv->text_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->text_view), GTK_WRAP_WORD_CHAR);
	gtk_widget_set_hexpand (priv->text_view, TRUE);
	gtk_widget_set_vexpand (priv->text_view, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), text_view_scrolled_window,
	                         text_label, GTK_POS_RIGHT, 1, 5);
}

static void
gpinstruct_lesson_reading_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_lesson_reading_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_reading_editor_class_init (GPInstructLessonReadingEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonReadingEditorPrivate));

	object_class->finalize = gpinstruct_lesson_reading_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonReadingEditor *editor = GPINSTRUCT_LESSON_READING_EDITOR (user_data);
	GPInstructLessonReadingEditorPrivate *priv = editor->priv;

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (priv->reading),
	                                     gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->reading);
}

static void
text_buffer_changed (GtkTextBuffer *textbuffer,
                     gpointer       user_data)
{
	GPInstructLessonReadingEditor *editor = GPINSTRUCT_LESSON_READING_EDITOR (user_data);
	GPInstructLessonReadingEditorPrivate *priv = editor->priv;

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_reading_set_text (priv->reading,
	                                    text);
	g_free (text);
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}


GPInstructLessonReadingEditor *
gpinstruct_lesson_reading_editor_new (GPInstructEditorWindow *window,
                                      GPInstructLessonReading *reading)
{
	GPInstructLessonReadingEditor *editor = g_object_new (GPINSTRUCT_TYPE_LESSON_READING_EDITOR, NULL);
	GPInstructLessonReadingEditorPrivate *priv = editor->priv;

	priv->window = window;
	priv->reading = reading;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (reading)));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->text_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_reading_get_text (reading), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (text_buffer_changed), editor);

	return editor;
}
