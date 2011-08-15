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

typedef struct _CanvasLessonTestOrderPrivate CanvasLessonTestOrderPrivate;
struct _CanvasLessonTestOrderPrivate
{
	gchar* explanation;
	GList* items;
};

#define CANVAS_LESSON_TEST_ORDER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_ORDER, CanvasLessonTestOrderPrivate))



G_DEFINE_TYPE (CanvasLessonTestOrder, canvas_lesson_test_order, CANVAS_TYPE_LESSON_TEST);

static void
canvas_lesson_test_order_init (CanvasLessonTestOrder *object)
{
	CanvasLessonTestOrderPrivate* priv = CANVAS_LESSON_TEST_ORDER_PRIVATE(object);

	priv->explanation = g_strdup ("");
	priv->items = NULL;
}

static void
canvas_lesson_test_order_finalize (GObject *object)
{
	CanvasLessonTestOrderPrivate* priv = CANVAS_LESSON_TEST_ORDER_PRIVATE(object);

	if (priv->explanation)
		g_free (priv->explanation);

	if (priv->items)
		g_list_free_full (priv->items, g_object_unref);

	G_OBJECT_CLASS (canvas_lesson_test_order_parent_class)->finalize (object);
}

static void
canvas_lesson_test_order_class_init (CanvasLessonTestOrderClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonTestClass* parent_class = CANVAS_LESSON_TEST_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestOrderPrivate));

	object_class->finalize = canvas_lesson_test_order_finalize;
}


CanvasLessonTestOrder*
canvas_lesson_test_order_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST_ORDER, NULL);
}

void
canvas_lesson_test_order_add_item (CanvasLessonTestOrder* test, CanvasLessonTestOrderItem* item)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_ORDER (test));

	CanvasLessonTestOrderPrivate* priv = CANVAS_LESSON_TEST_ORDER_PRIVATE(test);
	priv->items = g_list_append (priv->items, item);
}

void
canvas_lesson_test_order_remove_item (CanvasLessonTestOrder* test, guint item)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_ORDER (test));

	CanvasLessonTestOrderPrivate* priv = CANVAS_LESSON_TEST_ORDER_PRIVATE(test);
	priv->items = g_list_delete_link (priv->items, g_list_nth (priv->items, item));
}

CanvasLessonTestOrderItem*
canvas_lesson_test_order_get_item (CanvasLessonTestOrder* test, guint item)
{
	return g_list_nth_data (CANVAS_LESSON_TEST_ORDER_PRIVATE(test)->items, item);
}

GList*
canvas_lesson_test_order_get_items (CanvasLessonTestOrder* test)
{
	return g_list_copy (CANVAS_LESSON_TEST_ORDER_PRIVATE(test)->items);
}

guint
canvas_lesson_test_order_get_items_length (CanvasLessonTestOrder* test)
{
	return g_list_length (CANVAS_LESSON_TEST_ORDER_PRIVATE(test)->items);
}

const gchar*
canvas_lesson_test_order_get_explanation (CanvasLessonTestOrder* test)
{
	return CANVAS_LESSON_TEST_ORDER_PRIVATE(test)->explanation;
}

void
canvas_lesson_test_order_set_explanation (CanvasLessonTestOrder* test,
                                          const gchar* explanation)
{
	CanvasLessonTestOrderPrivate* priv = CANVAS_LESSON_TEST_ORDER_PRIVATE(test);

	if (priv->explanation)
		g_free (priv->explanation);
	priv->explanation = g_strdup (explanation);
}
