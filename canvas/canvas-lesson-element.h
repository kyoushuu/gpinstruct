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

#ifndef _CANVAS_LESSON_ELEMENT_H_
#define _CANVAS_LESSON_ELEMENT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_ELEMENT             (canvas_lesson_element_get_type ())
#define CANVAS_LESSON_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_ELEMENT, CanvasLessonElement))
#define CANVAS_LESSON_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_ELEMENT, CanvasLessonElementClass))
#define CANVAS_IS_LESSON_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_ELEMENT))
#define CANVAS_IS_LESSON_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_ELEMENT))
#define CANVAS_LESSON_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_ELEMENT, CanvasLessonElementClass))

typedef struct _CanvasLessonElementPrivate CanvasLessonElementPrivate;
typedef struct _CanvasLessonElementClass CanvasLessonElementClass;
typedef struct _CanvasLessonElement CanvasLessonElement;

struct _CanvasLessonElementClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasLessonElement
{
	CanvasObject parent_instance;

	CanvasLessonElementPrivate* priv;
};

GType canvas_lesson_element_get_type (void) G_GNUC_CONST;
CanvasLessonElement* canvas_lesson_element_new (void);
const gchar* canvas_lesson_element_get_title (CanvasLessonElement* element);
void canvas_lesson_element_set_title (CanvasLessonElement* element, const gchar *title);

G_END_DECLS

#endif /* _CANVAS_LESSON_ELEMENT_H_ */
