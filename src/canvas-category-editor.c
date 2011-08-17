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

typedef struct _CanvasCategoryEditorPrivate CanvasCategoryEditorPrivate;
struct _CanvasCategoryEditorPrivate
{
	CanvasEditorWindow* window;
	CanvasCategory* category;

	GtkWidget* title_label;
	GtkWidget* title_entry;
};

#define CANVAS_CATEGORY_EDITOR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_CATEGORY_EDITOR, CanvasCategoryEditorPrivate))



G_DEFINE_TYPE (CanvasCategoryEditor, canvas_category_editor, CANVAS_TYPE_OBJECT_EDITOR);

static void
canvas_category_editor_init (CanvasCategoryEditor *object)
{
	CanvasCategoryEditorPrivate* priv = CANVAS_CATEGORY_EDITOR_PRIVATE (object);

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
canvas_category_editor_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (canvas_category_editor_parent_class)->finalize (object);
}

static void
canvas_category_editor_class_init (CanvasCategoryEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectEditorClass* parent_class = CANVAS_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasCategoryEditorPrivate));

	object_class->finalize = canvas_category_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	CanvasCategoryEditor* editor = CANVAS_CATEGORY_EDITOR (user_data);
	CanvasCategoryEditorPrivate* priv = CANVAS_CATEGORY_EDITOR_PRIVATE (editor);

	canvas_category_set_title (priv->category,
	                           gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	canvas_editor_window_set_modified (priv->window, TRUE);
	canvas_editor_window_update_tree_store (priv->window, (gpointer)priv->category);
}


CanvasCategoryEditor*
canvas_category_editor_new (CanvasEditorWindow* window,
                            CanvasCategory* category)
{
	CanvasCategoryEditor* editor = g_object_new (CANVAS_TYPE_CATEGORY_EDITOR, NULL);
	CanvasCategoryEditorPrivate* priv = CANVAS_CATEGORY_EDITOR_PRIVATE (editor);

	priv->window = window;
	priv->category = category;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    canvas_category_get_title (category));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	return editor;
}