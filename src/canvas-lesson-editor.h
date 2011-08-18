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

#ifndef _CANVAS_LESSON_EDITOR_H_
#define _CANVAS_LESSON_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_EDITOR             (canvas_lesson_editor_get_type ())
#define CANVAS_LESSON_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_EDITOR, CanvasLessonEditor))
#define CANVAS_LESSON_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_EDITOR, CanvasLessonEditorClass))
#define CANVAS_IS_LESSON_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_EDITOR))
#define CANVAS_IS_LESSON_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_EDITOR))
#define CANVAS_LESSON_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_EDITOR, CanvasLessonEditorClass))

typedef struct _CanvasLessonEditorPrivate CanvasLessonEditorPrivate;
typedef struct _CanvasLessonEditorClass CanvasLessonEditorClass;
typedef struct _CanvasLessonEditor CanvasLessonEditor;

struct _CanvasLessonEditorClass
{
	CanvasObjectEditorClass parent_class;
};

struct _CanvasLessonEditor
{
	CanvasObjectEditor parent_instance;

	CanvasLessonEditorPrivate* priv;
};

GType canvas_lesson_editor_get_type (void) G_GNUC_CONST;
CanvasLessonEditor* canvas_lesson_editor_new (CanvasEditorWindow* window, CanvasLesson* lesson);

G_END_DECLS

#endif /* _CANVAS_LESSON_EDITOR_H_ */
