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

#ifndef _GPINSTRUCT_PROJECT_EDITOR_H_
#define _GPINSTRUCT_PROJECT_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_PROJECT_EDITOR             (gpinstruct_project_editor_get_type ())
#define GPINSTRUCT_PROJECT_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_PROJECT_EDITOR, GPInstructProjectEditor))
#define GPINSTRUCT_PROJECT_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_PROJECT_EDITOR, GPInstructProjectEditorClass))
#define GPINSTRUCT_IS_PROJECT_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_PROJECT_EDITOR))
#define GPINSTRUCT_IS_PROJECT_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_PROJECT_EDITOR))
#define GPINSTRUCT_PROJECT_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_PROJECT_EDITOR, GPInstructProjectEditorClass))

typedef struct _GPInstructProjectEditorPrivate GPInstructProjectEditorPrivate;
typedef struct _GPInstructProjectEditorClass GPInstructProjectEditorClass;
typedef struct _GPInstructProjectEditor GPInstructProjectEditor;

struct _GPInstructProjectEditorClass
{
	GPInstructObjectEditorClass parent_class;
};

struct _GPInstructProjectEditor
{
	GPInstructObjectEditor parent_instance;

	GPInstructProjectEditorPrivate* priv;
};

GType gpinstruct_project_editor_get_type (void) G_GNUC_CONST;
GPInstructProjectEditor* gpinstruct_project_editor_new (GPInstructEditorWindow* window, GPInstructProject* project);

G_END_DECLS

#endif /* _GPINSTRUCT_PROJECT_EDITOR_H_ */
