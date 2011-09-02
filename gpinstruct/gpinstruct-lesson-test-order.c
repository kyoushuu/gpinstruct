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

#include "gpinstruct/gpinstruct.h"

struct _GPInstructLessonTestOrderPrivate
{
	gchar* explanation;
	GList* items;
};

#define GPINSTRUCT_LESSON_TEST_ORDER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_ORDER, GPInstructLessonTestOrderPrivate))



static gchar*
get_item (GPInstructLessonTest* test,
          guint item)
{
	GPInstructLessonTestOrderItem* oitem =
		gpinstruct_lesson_test_order_get_item (GPINSTRUCT_LESSON_TEST_ORDER (test),
		                                       item);

	if (oitem)
		return g_strdup (gpinstruct_lesson_test_order_item_get_text (oitem));
	else
		return NULL;
}


static gchar*
get_choice (GPInstructLessonTest* test,
            guint item,
            guint choice)
{
	if (choice < gpinstruct_lesson_test_order_get_items_length (GPINSTRUCT_LESSON_TEST_ORDER (test)))
		return g_strdup_printf ("%d", choice);
	else
		return NULL;
}


static guint
get_items_length (GPInstructLessonTest* test)
{
	return gpinstruct_lesson_test_order_get_items_length (GPINSTRUCT_LESSON_TEST_ORDER (test));
}


static guint
get_choices_length (GPInstructLessonTest* test,
                    guint item)
{
	return gpinstruct_lesson_test_order_get_items_length (GPINSTRUCT_LESSON_TEST_ORDER (test));
}


static guint
get_item_correct_choice (GPInstructLessonTest* test,
                         guint item)
{
	GPInstructLessonTestOrderItem* oitem =
		gpinstruct_lesson_test_order_get_item (GPINSTRUCT_LESSON_TEST_ORDER (test), item);
	if (oitem)
		return gpinstruct_lesson_test_order_item_get_answer (oitem);
	else
		return 0;
}



G_DEFINE_TYPE (GPInstructLessonTestOrder, gpinstruct_lesson_test_order, GPINSTRUCT_TYPE_LESSON_TEST);

static void
gpinstruct_lesson_test_order_init (GPInstructLessonTestOrder *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_ORDER_PRIVATE (object);

	object->priv->explanation = g_strdup ("");
	object->priv->items = NULL;
}

static void
gpinstruct_lesson_test_order_finalize (GObject *object)
{
	GPInstructLessonTestOrder* test = GPINSTRUCT_LESSON_TEST_ORDER (object);

	if (test->priv->explanation)
		g_free (test->priv->explanation);

	if (test->priv->items)
		g_list_free_full (test->priv->items, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_lesson_test_order_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_order_class_init (GPInstructLessonTestOrderClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GPInstructLessonTestClass* parent_class = GPINSTRUCT_LESSON_TEST_CLASS (klass);

	parent_class->get_item = get_item;
	parent_class->get_choice = get_choice;
	parent_class->get_items_length = get_items_length;
	parent_class->get_choices_length = get_choices_length;
	parent_class->get_item_correct_choice = get_item_correct_choice;

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestOrderPrivate));

	object_class->finalize = gpinstruct_lesson_test_order_finalize;
}


GPInstructLessonTestOrder*
gpinstruct_lesson_test_order_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_ORDER, NULL);
}

void
gpinstruct_lesson_test_order_add_item (GPInstructLessonTestOrder* test,
                                       GPInstructLessonTestOrderItem* item)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_ORDER (test));

	test->priv->items = g_list_append (test->priv->items, item);
}

void
gpinstruct_lesson_test_order_remove_item (GPInstructLessonTestOrder* test,
                                          guint item)
{
	GList* nth_link = g_list_nth (test->priv->items, item);
	g_object_unref (nth_link->data);
	test->priv->items = g_list_delete_link (test->priv->items, nth_link);
}

GPInstructLessonTestOrderItem*
gpinstruct_lesson_test_order_get_item (GPInstructLessonTestOrder* test,
                                       guint item)
{
	return g_list_nth_data (test->priv->items, item);
}

GList*
gpinstruct_lesson_test_order_get_items (GPInstructLessonTestOrder* test)
{
	return g_list_copy (test->priv->items);
}

guint
gpinstruct_lesson_test_order_get_items_length (GPInstructLessonTestOrder* test)
{
	return g_list_length (test->priv->items);
}

const gchar*
gpinstruct_lesson_test_order_get_explanation (GPInstructLessonTestOrder* test)
{
	return test->priv->explanation;
}

void
gpinstruct_lesson_test_order_set_explanation (GPInstructLessonTestOrder* test,
                                              const gchar* explanation)
{
	if (test->priv->explanation)
		g_free (test->priv->explanation);
	test->priv->explanation = g_strdup (explanation);
}

gboolean
gpinstruct_lesson_test_order_is_correct (GPInstructLessonTestOrder* test,
                                         guint item,
                                         guint answer)
{
	GPInstructLessonTestOrderItem* order_item = gpinstruct_lesson_test_order_get_item (test, item);

	if (order_item == NULL)
		return FALSE;

	if (answer == gpinstruct_lesson_test_order_item_get_answer (order_item))
		return TRUE;
	else
		return FALSE;
}
