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

#include <config.h>

#include "gpinstruct/gpinstruct.h"

struct _GPInstructLessonTestOrderItemPrivate
{
	gchar* text;
	guint answer;
};

#define GPINSTRUCT_LESSON_TEST_ORDER_ITEM_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM, GPInstructLessonTestOrderItemPrivate))



G_DEFINE_TYPE (GPInstructLessonTestOrderItem, gpinstruct_lesson_test_order_item, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_test_order_item_init (GPInstructLessonTestOrderItem *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_ORDER_ITEM_PRIVATE (object);

	object->priv->text = g_strdup ("");
	object->priv->answer = 0;
}

static void
gpinstruct_lesson_test_order_item_finalize (GObject *object)
{
	GPInstructLessonTestOrderItem* item = GPINSTRUCT_LESSON_TEST_ORDER_ITEM (object);

	if (item->priv->text)
		g_free (item->priv->text);

	G_OBJECT_CLASS (gpinstruct_lesson_test_order_item_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_order_item_class_init (GPInstructLessonTestOrderItemClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass* parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestOrderItemPrivate));

	object_class->finalize = gpinstruct_lesson_test_order_item_finalize;
}


GPInstructLessonTestOrderItem*
gpinstruct_lesson_test_order_item_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_ORDER_ITEM, NULL);
}

const gchar*
gpinstruct_lesson_test_order_item_get_text (GPInstructLessonTestOrderItem* item)
{
	return item->priv->text;
}

void
gpinstruct_lesson_test_order_item_set_text (GPInstructLessonTestOrderItem* item,
                                            const gchar* text)
{
	if (item->priv->text)
		g_free (item->priv->text);
	item->priv->text = g_strdup (text);
}

guint
gpinstruct_lesson_test_order_item_get_answer (GPInstructLessonTestOrderItem* item)
{
	return item->priv->answer;
}

void
gpinstruct_lesson_test_order_item_set_answer (GPInstructLessonTestOrderItem* item,
                                              guint answer)
{
	item->priv->answer = answer;
}
