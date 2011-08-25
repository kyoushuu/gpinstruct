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

#ifndef _GPINSTRUCT_LESSON_TEST_ORDER_H_
#define _GPINSTRUCT_LESSON_TEST_ORDER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_ORDER             (gpinstruct_lesson_test_order_get_type ())
#define GPINSTRUCT_LESSON_TEST_ORDER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_ORDER, GPInstructLessonTestOrder))
#define GPINSTRUCT_LESSON_TEST_ORDER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_ORDER, GPInstructLessonTestOrderClass))
#define GPINSTRUCT_IS_LESSON_TEST_ORDER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_ORDER))
#define GPINSTRUCT_IS_LESSON_TEST_ORDER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_ORDER))
#define GPINSTRUCT_LESSON_TEST_ORDER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_ORDER, GPInstructLessonTestOrderClass))

typedef struct _GPInstructLessonTestOrderPrivate GPInstructLessonTestOrderPrivate;
typedef struct _GPInstructLessonTestOrderClass GPInstructLessonTestOrderClass;
typedef struct _GPInstructLessonTestOrder GPInstructLessonTestOrder;

struct _GPInstructLessonTestOrderClass
{
	GPInstructLessonTestClass parent_class;
};

struct _GPInstructLessonTestOrder
{
	GPInstructLessonTest parent_instance;

	GPInstructLessonTestOrderPrivate* priv;
};

GType gpinstruct_lesson_test_order_get_type (void) G_GNUC_CONST;
GPInstructLessonTestOrder* gpinstruct_lesson_test_order_new (void);
void gpinstruct_lesson_test_order_add_item (GPInstructLessonTestOrder* test, GPInstructLessonTestOrderItem* item);
void gpinstruct_lesson_test_order_remove_item (GPInstructLessonTestOrder* test, guint item);
GPInstructLessonTestOrderItem* gpinstruct_lesson_test_order_get_item (GPInstructLessonTestOrder* test, guint item);
GList* gpinstruct_lesson_test_order_get_items (GPInstructLessonTestOrder* test);
guint gpinstruct_lesson_test_order_get_items_length (GPInstructLessonTestOrder* test);
const gchar* gpinstruct_lesson_test_order_get_explanation (GPInstructLessonTestOrder* test);
void gpinstruct_lesson_test_order_set_explanation (GPInstructLessonTestOrder* test, const gchar* explanation);
gboolean gpinstruct_lesson_test_order_is_correct (GPInstructLessonTestOrder* test, guint item, guint answer);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_ORDER_H_ */
