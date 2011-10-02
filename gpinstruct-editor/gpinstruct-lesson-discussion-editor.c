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

struct _GPInstructLessonDiscussionEditorPrivate
{
	GPInstructEditorWindow* window;
	GPInstructLessonDiscussion* discussion;

	GtkWidget* title_label;
	GtkWidget* title_entry;

	GtkWidget* text_label;
	GtkWidget* text_view;
};

#define GPINSTRUCT_LESSON_DISCUSSION_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR, GPInstructLessonDiscussionEditorPrivate))



G_DEFINE_TYPE (GPInstructLessonDiscussionEditor, gpinstruct_lesson_discussion_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_discussion_editor_init (GPInstructLessonDiscussionEditor *object)
{
	object->priv = GPINSTRUCT_LESSON_DISCUSSION_EDITOR_PRIVATE (object);

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

	object->priv->text_label = gtk_label_new (_("Text:"));
	gtk_table_attach (GTK_TABLE (object), object->priv->text_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL,
	                  3, 3);
	GtkWidget* text_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (text_view_scrolled_window),
	                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	object->priv->text_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (text_view_scrolled_window), object->priv->text_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (object->priv->text_view), GTK_WRAP_WORD_CHAR);
	gtk_table_attach (GTK_TABLE (object), text_view_scrolled_window,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
	                  3, 3);
}

static void
gpinstruct_lesson_discussion_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_lesson_discussion_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_discussion_editor_class_init (GPInstructLessonDiscussionEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass* parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonDiscussionEditorPrivate));

	object_class->finalize = gpinstruct_lesson_discussion_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonDiscussionEditor* editor = GPINSTRUCT_LESSON_DISCUSSION_EDITOR (user_data);

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (editor->priv->discussion),
	                                     gtk_entry_get_text (GTK_ENTRY (editor->priv->title_entry)));
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (editor->priv->window, (gpointer)editor->priv->discussion);
}

static void
text_buffer_changed (GtkTextBuffer *textbuffer,
                     gpointer       user_data)
{
	GPInstructLessonDiscussionEditor* editor = GPINSTRUCT_LESSON_DISCUSSION_EDITOR (user_data);

	GtkTextIter start, end;
	gchar* text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_discussion_set_text (editor->priv->discussion,
	                                       text);
	g_free (text);
	gpinstruct_editor_window_set_modified (editor->priv->window, TRUE);
}


GPInstructLessonDiscussionEditor*
gpinstruct_lesson_discussion_editor_new (GPInstructEditorWindow* window,
                                         GPInstructLessonDiscussion *discussion)
{
	GPInstructLessonDiscussionEditor* editor = g_object_new (GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR, NULL);

	editor->priv->window = window;
	editor->priv->discussion = discussion;

	gtk_entry_set_text (GTK_ENTRY (editor->priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (discussion)));
	g_signal_connect (editor->priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	GtkTextBuffer* buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->priv->text_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_discussion_get_text (discussion), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (text_buffer_changed), editor);

	return editor;
}
