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

#ifndef _GPINSTRUCT_LESSON_ELEMENT_H_
#define _GPINSTRUCT_LESSON_ELEMENT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_ELEMENT             (gpinstruct_lesson_element_get_type ())
#define GPINSTRUCT_LESSON_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_ELEMENT, GPInstructLessonElement))
#define GPINSTRUCT_LESSON_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_ELEMENT, GPInstructLessonElementClass))
#define GPINSTRUCT_IS_LESSON_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_ELEMENT))
#define GPINSTRUCT_IS_LESSON_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_ELEMENT))
#define GPINSTRUCT_LESSON_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_ELEMENT, GPInstructLessonElementClass))

typedef struct _GPInstructLessonElementPrivate GPInstructLessonElementPrivate;
typedef struct _GPInstructLessonElementClass GPInstructLessonElementClass;
typedef struct _GPInstructLessonElement GPInstructLessonElement;

struct _GPInstructLessonElementClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLessonElement
{
	GPInstructObject parent_instance;

	GPInstructLessonElementPrivate *priv;
};

GType gpinstruct_lesson_element_get_type (void) G_GNUC_CONST;
GPInstructLessonElement *gpinstruct_lesson_element_new (void);
const gchar *gpinstruct_lesson_element_get_title (GPInstructLessonElement *element);
void gpinstruct_lesson_element_set_title (GPInstructLessonElement *element, const gchar *title);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_ELEMENT_H_ */
