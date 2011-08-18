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

#ifndef _CANVAS_CATEGORY_EDITOR_H_
#define _CANVAS_CATEGORY_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_CATEGORY_EDITOR             (canvas_category_editor_get_type ())
#define CANVAS_CATEGORY_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_CATEGORY_EDITOR, CanvasCategoryEditor))
#define CANVAS_CATEGORY_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_CATEGORY_EDITOR, CanvasCategoryEditorClass))
#define CANVAS_IS_CATEGORY_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_CATEGORY_EDITOR))
#define CANVAS_IS_CATEGORY_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_CATEGORY_EDITOR))
#define CANVAS_CATEGORY_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_CATEGORY_EDITOR, CanvasCategoryEditorClass))

typedef struct _CanvasCategoryEditorPrivate CanvasCategoryEditorPrivate;
typedef struct _CanvasCategoryEditorClass CanvasCategoryEditorClass;
typedef struct _CanvasCategoryEditor CanvasCategoryEditor;

struct _CanvasCategoryEditorClass
{
	CanvasObjectEditorClass parent_class;
};

struct _CanvasCategoryEditor
{
	CanvasObjectEditor parent_instance;

	CanvasCategoryEditorPrivate* priv;
};

GType canvas_category_editor_get_type (void) G_GNUC_CONST;
CanvasCategoryEditor* canvas_category_editor_new (CanvasEditorWindow* window, CanvasCategory* category);

G_END_DECLS

#endif /* _CANVAS_CATEGORY_EDITOR_H_ */
