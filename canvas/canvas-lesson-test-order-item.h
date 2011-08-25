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

#ifndef _GPINSTRUCT_LESSON_TEST_ORDER_ITEM_H_
#define _GPINSTRUCT_LESSON_TEST_ORDER_ITEM_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM             (gpinstruct_lesson_test_order_item_get_type ())
#define GPINSTRUCT_LESSON_TEST_ORDER_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM, GPInstructLessonTestOrderItem))
#define GPINSTRUCT_LESSON_TEST_ORDER_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM, GPInstructLessonTestOrderItemClass))
#define GPINSTRUCT_IS_LESSON_TEST_ORDER_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM))
#define GPINSTRUCT_IS_LESSON_TEST_ORDER_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM))
#define GPINSTRUCT_LESSON_TEST_ORDER_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM, GPInstructLessonTestOrderItemClass))

typedef struct _GPInstructLessonTestOrderItemPrivate GPInstructLessonTestOrderItemPrivate;
typedef struct _GPInstructLessonTestOrderItemClass GPInstructLessonTestOrderItemClass;
typedef struct _GPInstructLessonTestOrderItem GPInstructLessonTestOrderItem;

struct _GPInstructLessonTestOrderItemClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLessonTestOrderItem
{
	GPInstructObject parent_instance;

	GPInstructLessonTestOrderItemPrivate* priv;
};

GType gpinstruct_lesson_test_order_item_get_type (void) G_GNUC_CONST;
GPInstructLessonTestOrderItem* gpinstruct_lesson_test_order_item_new (void);
const gchar* gpinstruct_lesson_test_order_item_get_text (GPInstructLessonTestOrderItem* item);
void gpinstruct_lesson_test_order_item_set_text (GPInstructLessonTestOrderItem* item, const gchar* text);
guint gpinstruct_lesson_test_order_item_get_answer (GPInstructLessonTestOrderItem* item);
void gpinstruct_lesson_test_order_item_set_answer (GPInstructLessonTestOrderItem* item, guint answer);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_ORDER_ITEM_H_ */
