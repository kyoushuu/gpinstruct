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

#ifndef _CANVAS_LESSON_H_
#define _CANVAS_LESSON_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON             (canvas_lesson_get_type ())
#define CANVAS_LESSON(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON, CanvasLesson))
#define CANVAS_LESSON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON, CanvasLessonClass))
#define CANVAS_IS_LESSON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON))
#define CANVAS_IS_LESSON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON))
#define CANVAS_LESSON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON, CanvasLessonClass))

typedef struct _CanvasLessonClass CanvasLessonClass;
typedef struct _CanvasLesson CanvasLesson;

struct _CanvasLessonClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasLesson
{
	CanvasObject parent_instance;
};

GType canvas_lesson_get_type (void) G_GNUC_CONST;
CanvasLesson* canvas_lesson_new (void);
const gchar* canvas_lesson_get_title (CanvasLesson* lesson);
void canvas_lesson_set_title (CanvasLesson* lesson, const gchar* title);
guint canvas_lesson_get_lesson_elements_length (CanvasLesson* lesson);
void canvas_lesson_add_lesson_element (CanvasLesson* lesson, CanvasLessonElement* element);
void canvas_lesson_remove_lesson_element (CanvasLesson* lesson, guint lesson_element);
GList* canvas_lesson_get_lesson_elements (CanvasLesson* lesson);
gboolean canvas_lesson_get_single_score (CanvasLesson* lesson);
void canvas_lesson_set_single_score (CanvasLesson* lesson, gboolean single_score);

G_END_DECLS

#endif /* _CANVAS_LESSON_H_ */
