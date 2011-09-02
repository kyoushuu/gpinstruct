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

#include <gtk/gtk.h>

#include "canvas.h"
#include "canvas-editor.h"



G_DEFINE_TYPE (CanvasObjectEditor, canvas_object_editor, GTK_TYPE_TABLE);

static void
canvas_object_editor_init (CanvasObjectEditor *object)
{
	/* TODO: Add initialization code here */
}

static void
canvas_object_editor_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (canvas_object_editor_parent_class)->finalize (object);
}

static void
canvas_object_editor_class_init (CanvasObjectEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkVBoxClass* parent_class = GTK_TABLE_CLASS (klass);*/

	object_class->finalize = canvas_object_editor_finalize;
}


CanvasObjectEditor*
canvas_object_editor_new (void)
{
	return g_object_new(CANVAS_TYPE_OBJECT_EDITOR, NULL);
}
