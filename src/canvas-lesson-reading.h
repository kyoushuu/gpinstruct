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

#ifndef _CANVAS_LESSON_READING_H_
#define _CANVAS_LESSON_READING_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_READING             (canvas_lesson_reading_get_type ())
#define CANVAS_LESSON_READING(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_READING, CanvasLessonReading))
#define CANVAS_LESSON_READING_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_READING, CanvasLessonReadingClass))
#define CANVAS_IS_LESSON_READING(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_READING))
#define CANVAS_IS_LESSON_READING_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_READING))
#define CANVAS_LESSON_READING_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_READING, CanvasLessonReadingClass))

typedef struct _CanvasLessonReadingPrivate CanvasLessonReadingPrivate;
typedef struct _CanvasLessonReadingClass CanvasLessonReadingClass;
typedef struct _CanvasLessonReading CanvasLessonReading;

struct _CanvasLessonReadingClass
{
	CanvasLessonElementClass parent_class;
};

struct _CanvasLessonReading
{
	CanvasLessonElement parent_instance;

	CanvasLessonReadingPrivate* priv;
};

GType canvas_lesson_reading_get_type (void) G_GNUC_CONST;
CanvasLessonReading* canvas_lesson_reading_new (void);
const gchar* canvas_lesson_reading_get_text (CanvasLessonReading* reading);
void canvas_lesson_reading_set_text (CanvasLessonReading* reading, const gchar* text);

G_END_DECLS

#endif /* _CANVAS_LESSON_READING_H_ */
