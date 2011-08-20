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

#ifndef _CANVAS_LESSON_READING_EDITOR_H_
#define _CANVAS_LESSON_READING_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_READING_EDITOR             (canvas_lesson_reading_editor_get_type ())
#define CANVAS_LESSON_READING_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_READING_EDITOR, CanvasLessonReadingEditor))
#define CANVAS_LESSON_READING_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_READING_EDITOR, CanvasLessonReadingEditorClass))
#define CANVAS_IS_LESSON_READING_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_READING_EDITOR))
#define CANVAS_IS_LESSON_READING_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_READING_EDITOR))
#define CANVAS_LESSON_READING_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_READING_EDITOR, CanvasLessonReadingEditorClass))

typedef struct _CanvasLessonReadingEditorPrivate CanvasLessonReadingEditorPrivate;
typedef struct _CanvasLessonReadingEditorClass CanvasLessonReadingEditorClass;
typedef struct _CanvasLessonReadingEditor CanvasLessonReadingEditor;

struct _CanvasLessonReadingEditorClass
{
	CanvasObjectEditorClass parent_class;
};

struct _CanvasLessonReadingEditor
{
	CanvasObjectEditor parent_instance;

	CanvasLessonReadingEditorPrivate* priv;
};

GType canvas_lesson_reading_editor_get_type (void) G_GNUC_CONST;
CanvasLessonReadingEditor* canvas_lesson_reading_editor_new (CanvasEditorWindow* window, CanvasLessonReading *reading);

G_END_DECLS

#endif /* _CANVAS_LESSON_READING_EDITOR_H_ */
