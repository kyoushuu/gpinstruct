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

#ifndef _GPINSTRUCT_LESSON_H_
#define _GPINSTRUCT_LESSON_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON             (gpinstruct_lesson_get_type ())
#define GPINSTRUCT_LESSON(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON, GPInstructLesson))
#define GPINSTRUCT_LESSON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON, GPInstructLessonClass))
#define GPINSTRUCT_IS_LESSON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON))
#define GPINSTRUCT_IS_LESSON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON))
#define GPINSTRUCT_LESSON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON, GPInstructLessonClass))

typedef struct _GPInstructLessonPrivate GPInstructLessonPrivate;
typedef struct _GPInstructLessonClass GPInstructLessonClass;
typedef struct _GPInstructLesson GPInstructLesson;

struct _GPInstructLessonClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLesson
{
	GPInstructObject parent_instance;

	GPInstructLessonPrivate* priv;
};

GType gpinstruct_lesson_get_type (void) G_GNUC_CONST;
GPInstructLesson* gpinstruct_lesson_new (void);
const gchar* gpinstruct_lesson_get_title (GPInstructLesson* lesson);
void gpinstruct_lesson_set_title (GPInstructLesson* lesson, const gchar* title);
guint gpinstruct_lesson_get_lesson_elements_length (GPInstructLesson* lesson);
void gpinstruct_lesson_add_lesson_element (GPInstructLesson* lesson, GPInstructLessonElement* element);
void gpinstruct_lesson_remove_lesson_element (GPInstructLesson* lesson, guint lesson_element);
GList* gpinstruct_lesson_get_lesson_elements (GPInstructLesson* lesson);
gboolean gpinstruct_lesson_get_single_score (GPInstructLesson* lesson);
void gpinstruct_lesson_set_single_score (GPInstructLesson* lesson, gboolean single_score);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_H_ */
