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

#include "canvas.h"
#include "canvas-editor.h"

typedef struct _CanvasProjectEditorPrivate CanvasProjectEditorPrivate;
struct _CanvasProjectEditorPrivate
{
	CanvasEditorWindow* window;
	CanvasProject* project;

	GtkWidget* title_label;
	GtkWidget* title_entry;
};

#define CANVAS_PROJECT_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_PROJECT_EDITOR, CanvasProjectEditorPrivate))



G_DEFINE_TYPE (CanvasProjectEditor, canvas_project_editor, CANVAS_TYPE_OBJECT_EDITOR);

static void
canvas_project_editor_init (CanvasProjectEditor *object)
{
	CanvasProjectEditorPrivate* priv = CANVAS_PROJECT_EDITOR_PRIVATE (object);

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
}

static void
canvas_project_editor_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (canvas_project_editor_parent_class)->finalize (object);
}

static void
canvas_project_editor_class_init (CanvasProjectEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectEditorClass* parent_class = CANVAS_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasProjectEditorPrivate));

	object_class->finalize = canvas_project_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	CanvasProjectEditor* editor = CANVAS_PROJECT_EDITOR (user_data);
	CanvasProjectEditorPrivate* priv = CANVAS_PROJECT_EDITOR_PRIVATE (editor);

	canvas_project_set_title (priv->project,
	                          gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	canvas_editor_window_set_modified (priv->window, TRUE);
	canvas_editor_window_update_tree_store (priv->window);
}


CanvasProjectEditor*
canvas_project_editor_new (CanvasEditorWindow* window,
                           CanvasProject* project)
{
	CanvasProjectEditor* editor = g_object_new (CANVAS_TYPE_PROJECT_EDITOR, NULL);
	CanvasProjectEditorPrivate* priv = CANVAS_PROJECT_EDITOR_PRIVATE (editor);

	priv->window = window;
	priv->project = project;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    canvas_project_get_title (project));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	return editor;
}
