/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-editor/gpinstruct-editor.h"



G_DEFINE_TYPE (GPInstructObjectEditor, gpinstruct_object_editor, GTK_TYPE_TABLE);

static void
gpinstruct_object_editor_init (GPInstructObjectEditor *object)
{
	/* TODO: Add initialization code here */
}

static void
gpinstruct_object_editor_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gpinstruct_object_editor_parent_class)->finalize (object);
}

static void
gpinstruct_object_editor_class_init (GPInstructObjectEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkVBoxClass* parent_class = GTK_TABLE_CLASS (klass);*/

	object_class->finalize = gpinstruct_object_editor_finalize;
}


GPInstructObjectEditor*
gpinstruct_object_editor_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_OBJECT_EDITOR, NULL);
}
