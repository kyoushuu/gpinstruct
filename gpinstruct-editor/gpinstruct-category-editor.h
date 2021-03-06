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

#ifndef _GPINSTRUCT_CATEGORY_EDITOR_H_
#define _GPINSTRUCT_CATEGORY_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_CATEGORY_EDITOR             (gpinstruct_category_editor_get_type ())
#define GPINSTRUCT_CATEGORY_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_CATEGORY_EDITOR, GPInstructCategoryEditor))
#define GPINSTRUCT_CATEGORY_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_CATEGORY_EDITOR, GPInstructCategoryEditorClass))
#define GPINSTRUCT_IS_CATEGORY_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_CATEGORY_EDITOR))
#define GPINSTRUCT_IS_CATEGORY_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_CATEGORY_EDITOR))
#define GPINSTRUCT_CATEGORY_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_CATEGORY_EDITOR, GPInstructCategoryEditorClass))

typedef struct _GPInstructCategoryEditorPrivate GPInstructCategoryEditorPrivate;
typedef struct _GPInstructCategoryEditorClass GPInstructCategoryEditorClass;
typedef struct _GPInstructCategoryEditor GPInstructCategoryEditor;

struct _GPInstructCategoryEditorClass
{
	GPInstructObjectEditorClass parent_class;
};

struct _GPInstructCategoryEditor
{
	GPInstructObjectEditor parent_instance;

	GPInstructCategoryEditorPrivate *priv;
};

GType gpinstruct_category_editor_get_type (void) G_GNUC_CONST;
GPInstructCategoryEditor *gpinstruct_category_editor_new (GPInstructEditorWindow *window, GPInstructCategory *category);

G_END_DECLS

#endif /* _GPINSTRUCT_CATEGORY_EDITOR_H_ */
