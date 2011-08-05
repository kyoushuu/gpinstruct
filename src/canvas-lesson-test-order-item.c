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

#include "canvas.h"

typedef struct _CanvasLessonTestOrderItemPrivate CanvasLessonTestOrderItemPrivate;
struct _CanvasLessonTestOrderItemPrivate
{
	gchar* text;
	guint answer;
};

#define CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_ORDER_ITEM, CanvasLessonTestOrderItemPrivate))



G_DEFINE_TYPE (CanvasLessonTestOrderItem, canvas_lesson_test_order_item, CANVAS_TYPE_OBJECT);

static void
canvas_lesson_test_order_item_init (CanvasLessonTestOrderItem *object)
{
	CanvasLessonTestOrderItemPrivate* priv = CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(object);
	priv->text = g_strdup ("");
	priv->answer = 0;
}

static void
canvas_lesson_test_order_item_finalize (GObject *object)
{
	CanvasLessonTestOrderItemPrivate* priv = CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(object);

	if (priv->text)
		g_free (priv->text);

	G_OBJECT_CLASS (canvas_lesson_test_order_item_parent_class)->finalize (object);
}

static void
canvas_lesson_test_order_item_class_init (CanvasLessonTestOrderItemClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestOrderItemPrivate));

	object_class->finalize = canvas_lesson_test_order_item_finalize;
}


CanvasLessonTestOrderItem*
canvas_lesson_test_order_item_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST_ORDER_ITEM, NULL);
}

const gchar*
canvas_lesson_test_order_item_get_text (CanvasLessonTestOrderItem* item)
{
	return CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(item)->text;
}

void
canvas_lesson_test_order_item_set_text (CanvasLessonTestOrderItem* item, const gchar* text)
{
	CanvasLessonTestOrderItemPrivate* priv = CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(item);

	if (priv->text)
		g_free (priv->text);
	priv->text = g_strdup (text);
}

guint
canvas_lesson_test_order_item_get_answer (CanvasLessonTestOrderItem* item)
{
	return CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(item)->answer;
}

void
canvas_lesson_test_order_item_set_answer (CanvasLessonTestOrderItem* item, guint answer)
{
	CanvasLessonTestOrderItemPrivate* priv = CANVAS_LESSON_TEST_ORDER_ITEM_PRIVATE(item);

	priv->answer = answer;
}
