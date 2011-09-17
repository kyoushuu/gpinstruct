/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
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

#ifndef _GPINSTRUCT_LESSON_ELEMENT_GROUP_H_
#define _GPINSTRUCT_LESSON_ELEMENT_GROUP_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP             (gpinstruct_lesson_element_group_get_type ())
#define GPINSTRUCT_LESSON_ELEMENT_GROUP(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP, GPInstructLessonElementGroup))
#define GPINSTRUCT_LESSON_ELEMENT_GROUP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP, GPInstructLessonElementGroupClass))
#define GPINSTRUCT_IS_LESSON_ELEMENT_GROUP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP))
#define GPINSTRUCT_IS_LESSON_ELEMENT_GROUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP))
#define GPINSTRUCT_LESSON_ELEMENT_GROUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP, GPInstructLessonElementGroupClass))

typedef struct _GPInstructLessonElementGroupPrivate GPInstructLessonElementGroupPrivate;
typedef struct _GPInstructLessonElementGroupClass GPInstructLessonElementGroupClass;
typedef struct _GPInstructLessonElementGroup GPInstructLessonElementGroup;

struct _GPInstructLessonElementGroupClass
{
	GPInstructLessonElementClass parent_class;
};

struct _GPInstructLessonElementGroup
{
	GPInstructLessonElement parent_instance;

	GPInstructLessonElementGroupPrivate* priv;
};

GType gpinstruct_lesson_element_group_get_type (void) G_GNUC_CONST;
GPInstructLessonElementGroup* gpinstruct_lesson_element_group_new (void);
const gchar* gpinstruct_lesson_element_group_get_directions (GPInstructLessonElementGroup* group);
void gpinstruct_lesson_element_group_set_directions (GPInstructLessonElementGroup* group, const gchar* directions);
gboolean gpinstruct_lesson_element_group_get_single_score (GPInstructLessonElementGroup* group);
void gpinstruct_lesson_element_group_set_single_score (GPInstructLessonElementGroup* group, gboolean single_score);
gboolean gpinstruct_lesson_element_group_get_single_directions (GPInstructLessonElementGroup* group);
void gpinstruct_lesson_element_group_set_single_directions (GPInstructLessonElementGroup* group, gboolean single_directions);
void gpinstruct_lesson_element_group_add_lesson_element (GPInstructLessonElementGroup* group, GPInstructLessonElement* lesson_element);
void gpinstruct_lesson_element_group_remove_lesson_element (GPInstructLessonElementGroup* group, guint lesson_element);
GList* gpinstruct_lesson_element_group_get_lesson_elements (GPInstructLessonElementGroup* group);
guint gpinstruct_lesson_element_group_get_lesson_elements_length (GPInstructLessonElementGroup* group);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_H_ */
