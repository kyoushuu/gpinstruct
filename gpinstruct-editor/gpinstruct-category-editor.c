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

	GtkWidget *title_entry;
};

#define GPINSTRUCT_CATEGORY_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_CATEGORY_EDITOR, GPInstructCategoryEditorPrivate))



G_DEFINE_TYPE (GPInstructCategoryEditor, gpinstruct_category_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_category_editor_init (GPInstructCategoryEditor *object)
{
	GtkStyleContext *context;

	object->priv = GPINSTRUCT_CATEGORY_EDITOR_GET_PRIVATE (object);
	GPInstructCategoryEditorPrivate *priv = object->priv;

	GtkWidget *title_label = gtk_label_new (_("Title"));
	context = gtk_widget_get_style_context (title_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (title_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), title_label);

	priv->title_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (priv->title_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->title_entry,
	                         title_label, GTK_POS_RIGHT, 1, 1);
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
