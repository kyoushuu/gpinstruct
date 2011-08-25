/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja
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

#ifndef _GPINSTRUCT_LESSON_READING_H_
#define _GPINSTRUCT_LESSON_READING_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_READING             (gpinstruct_lesson_reading_get_type ())
#define GPINSTRUCT_LESSON_READING(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_READING, GPInstructLessonReading))
#define GPINSTRUCT_LESSON_READING_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_READING, GPInstructLessonReadingClass))
#define GPINSTRUCT_IS_LESSON_READING(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_READING))
#define GPINSTRUCT_IS_LESSON_READING_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_READING))
#define GPINSTRUCT_LESSON_READING_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_READING, GPInstructLessonReadingClass))

typedef struct _GPInstructLessonReadingPrivate GPInstructLessonReadingPrivate;
typedef struct _GPInstructLessonReadingClass GPInstructLessonReadingClass;
typedef struct _GPInstructLessonReading GPInstructLessonReading;

struct _GPInstructLessonReadingClass
{
	GPInstructLessonElementClass parent_class;
};

struct _GPInstructLessonReading
{
	GPInstructLessonElement parent_instance;

	GPInstructLessonReadingPrivate* priv;
};

GType gpinstruct_lesson_reading_get_type (void) G_GNUC_CONST;
GPInstructLessonReading* gpinstruct_lesson_reading_new (void);
const gchar* gpinstruct_lesson_reading_get_text (GPInstructLessonReading* reading);
void gpinstruct_lesson_reading_set_text (GPInstructLessonReading* reading, const gchar* text);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_READING_H_ */
