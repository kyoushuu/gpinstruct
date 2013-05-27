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

	GtkWidget *title_label;
	GtkWidget *title_entry;
};

#define GPINSTRUCT_PROJECT_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_PROJECT_EDITOR, GPInstructProjectEditorPrivate))



G_DEFINE_TYPE (GPInstructProjectEditor, gpinstruct_project_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_project_editor_init (GPInstructProjectEditor *object)
{
	object->priv = GPINSTRUCT_PROJECT_EDITOR_GET_PRIVATE (object);
	GPInstructProjectEditorPrivate *priv = object->priv;

	priv->title_label = gtk_label_new (_("Title:"));
	gtk_container_add (GTK_CONTAINER (object), priv->title_label);

	priv->title_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (priv->title_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->title_entry,
	                         priv->title_label, GTK_POS_RIGHT, 1, 1);
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

	return editor;
}
