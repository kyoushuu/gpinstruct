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

struct _GPInstructProjectEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructProject *project;

	GtkWidget *title_entry;
	GtkWidget *instructions_view;
};

#define GPINSTRUCT_PROJECT_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_PROJECT_EDITOR, GPInstructProjectEditorPrivate))



G_DEFINE_TYPE (GPInstructProjectEditor, gpinstruct_project_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_project_editor_init (GPInstructProjectEditor *object)
{
	GtkStyleContext *context;

	object->priv = GPINSTRUCT_PROJECT_EDITOR_GET_PRIVATE (object);
	GPInstructProjectEditorPrivate *priv = object->priv;

	GtkWidget *title_label = gtk_label_new (_("Title"));
	context = gtk_widget_get_style_context (title_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (title_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), title_label);

	priv->title_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (priv->title_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->title_entry,
	                         title_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *instructions_label = gtk_label_new (_("Instructions"));
	context = gtk_widget_get_style_context (instructions_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (instructions_label, GTK_ALIGN_END);
	gtk_widget_set_valign (instructions_label, GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (object), instructions_label);

	GtkWidget *instructions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (instructions_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	priv->instructions_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (instructions_view_scrolled_window), priv->instructions_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->instructions_view), GTK_WRAP_WORD_CHAR);
	gtk_widget_set_hexpand (priv->instructions_view, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), instructions_view_scrolled_window,
	                         instructions_label, GTK_POS_RIGHT, 1, 3);
}

static void
gpinstruct_project_editor_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gpinstruct_project_editor_parent_class)->finalize (object);
}

static void
gpinstruct_project_editor_class_init (GPInstructProjectEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructProjectEditorPrivate));

	object_class->finalize = gpinstruct_project_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructProjectEditor *editor = GPINSTRUCT_PROJECT_EDITOR (user_data);
	GPInstructProjectEditorPrivate *priv = editor->priv;

	gpinstruct_project_set_title (priv->project,
	                              gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->project);
}

static void
instructions_buffer_changed (GtkTextBuffer *textbuffer,
                             gpointer       user_data)
{
	GPInstructProjectEditor *editor = GPINSTRUCT_PROJECT_EDITOR (user_data);
	GPInstructProjectEditorPrivate *priv = editor->priv;

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_project_set_instructions (priv->project,
	                                     text);
	g_free (text);
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}


GPInstructProjectEditor *
gpinstruct_project_editor_new (GPInstructEditorWindow *window,
                               GPInstructProject *project)
{
	GPInstructProjectEditor *editor = g_object_new (GPINSTRUCT_TYPE_PROJECT_EDITOR, NULL);
	GPInstructProjectEditorPrivate *priv = editor->priv;

	priv->window = window;
	priv->project = project;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_project_get_title (project));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->instructions_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_project_get_instructions (project), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (instructions_buffer_changed), editor);

	return editor;
}
