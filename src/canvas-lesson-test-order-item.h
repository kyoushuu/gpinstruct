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

#ifndef _CANVAS_LESSON_TEST_ORDER_ITEM_H_
#define _CANVAS_LESSON_TEST_ORDER_ITEM_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_TEST_ORDER_ITEM             (canvas_lesson_test_order_item_get_type ())
#define CANVAS_LESSON_TEST_ORDER_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_TEST_ORDER_ITEM, CanvasLessonTestOrderItem))
#define CANVAS_LESSON_TEST_ORDER_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_TEST_ORDER_ITEM, CanvasLessonTestOrderItemClass))
#define CANVAS_IS_LESSON_TEST_ORDER_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_TEST_ORDER_ITEM))
#define CANVAS_IS_LESSON_TEST_ORDER_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_TEST_ORDER_ITEM))
#define CANVAS_LESSON_TEST_ORDER_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_TEST_ORDER_ITEM, CanvasLessonTestOrderItemClass))

typedef struct _CanvasLessonTestOrderItemClass CanvasLessonTestOrderItemClass;
typedef struct _CanvasLessonTestOrderItem CanvasLessonTestOrderItem;

struct _CanvasLessonTestOrderItemClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasLessonTestOrderItem
{
	CanvasObject parent_instance;
};

GType canvas_lesson_test_order_item_get_type (void) G_GNUC_CONST;
CanvasLessonTestOrderItem* canvas_lesson_test_order_item_new (void);
const gchar* canvas_lesson_test_order_item_get_text (CanvasLessonTestOrderItem* item);
void canvas_lesson_test_order_item_set_text (CanvasLessonTestOrderItem* item, const gchar* text);
guint canvas_lesson_test_order_item_get_answer (CanvasLessonTestOrderItem* item);
void canvas_lesson_test_order_item_set_answer (CanvasLessonTestOrderItem* item, guint answer);

G_END_DECLS

#endif /* _CANVAS_LESSON_TEST_ORDER_ITEM_H_ */
