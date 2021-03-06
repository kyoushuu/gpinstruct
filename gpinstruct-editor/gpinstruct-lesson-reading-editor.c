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

	GtkWidget *title_label;
	GtkWidget *title_entry;

	GtkWidget *text_label;
	GtkWidget *text_view;
};

#define GPINSTRUCT_LESSON_READING_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_READING_EDITOR, GPInstructLessonReadingEditorPrivate))



G_DEFINE_TYPE (GPInstructLessonReadingEditor, gpinstruct_lesson_reading_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_reading_editor_init (GPInstructLessonReadingEditor *object)
{
	object->priv = GPINSTRUCT_LESSON_READING_EDITOR_GET_PRIVATE (object);
	GPInstructLessonReadingEditorPrivate *priv = object->priv;

	priv->title_label = gtk_label_new (_("Title:"));
	gtk_table_attach (GTK_TABLE (object), priv->title_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	priv->title_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (object), priv->title_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);

	priv->text_label = gtk_label_new (_("Text:"));
	gtk_table_attach (GTK_TABLE (object), priv->text_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	GtkWidget *text_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (text_view_scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	priv->text_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (text_view_scrolled_window), priv->text_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->text_view), GTK_WRAP_WORD_CHAR);
	gtk_table_attach (GTK_TABLE (object), text_view_scrolled_window,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);
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
