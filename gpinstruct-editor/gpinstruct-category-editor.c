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

struct _GPInstructCategoryEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructCategory *category;

	GtkWidget *title_label;
	GtkWidget *title_entry;
};

#define GPINSTRUCT_CATEGORY_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_CATEGORY_EDITOR, GPInstructCategoryEditorPrivate))



G_DEFINE_TYPE (GPInstructCategoryEditor, gpinstruct_category_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_category_editor_init (GPInstructCategoryEditor *object)
{
	object->priv = GPINSTRUCT_CATEGORY_EDITOR_GET_PRIVATE (object);
	GPInstructCategoryEditorPrivate *priv = object->priv;

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
gpinstruct_category_editor_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_category_editor_parent_class)->finalize (object);
}

static void
gpinstruct_category_editor_class_init (GPInstructCategoryEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructCategoryEditorPrivate));

	object_class->finalize = gpinstruct_category_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructCategoryEditor *editor = GPINSTRUCT_CATEGORY_EDITOR (user_data);
	GPInstructCategoryEditorPrivate *priv = editor->priv;

	gpinstruct_category_set_title (priv->category,
	                               gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->category);
}


GPInstructCategoryEditor *
gpinstruct_category_editor_new (GPInstructEditorWindow *window,
                                GPInstructCategory *category)
{
	GPInstructCategoryEditor *editor = g_object_new (GPINSTRUCT_TYPE_CATEGORY_EDITOR, NULL);
	GPInstructCategoryEditorPrivate *priv = editor->priv;

	priv->window = window;
	priv->category = category;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_category_get_title (category));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	return editor;
}
