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

struct _GPInstructLessonPrivate
{
	gchar* title;
	GList* lesson_elements;
	gboolean single_score;
};

#define GPINSTRUCT_LESSON_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON, GPInstructLessonPrivate))



G_DEFINE_TYPE (GPInstructLesson, gpinstruct_lesson, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_init (GPInstructLesson *object)
{
	object->priv = GPINSTRUCT_LESSON_PRIVATE (object);

	object->priv->title = g_strdup ("");
	object->priv->lesson_elements = NULL;
	object->priv->single_score = FALSE;
}

static void
gpinstruct_lesson_finalize (GObject *object)
{
	GPInstructLesson* lesson = GPINSTRUCT_LESSON (object);

	if (lesson->priv->title)
		g_free (lesson->priv->title);
	if (lesson->priv->lesson_elements)
		g_list_free_full (lesson->priv->lesson_elements, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_lesson_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_class_init (GPInstructLessonClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass* parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonPrivate));

	object_class->finalize = gpinstruct_lesson_finalize;
}


GPInstructLesson*
gpinstruct_lesson_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON, NULL);
}

const gchar*
gpinstruct_lesson_get_title (GPInstructLesson* lesson)
{
	return lesson->priv->title;
}

void
gpinstruct_lesson_set_title (GPInstructLesson* lesson,
                             const gchar* title)
{
	if (lesson->priv->title)
		g_free (lesson->priv->title);
	lesson->priv->title = g_strdup (title);
}

guint
gpinstruct_lesson_get_lesson_elements_length (GPInstructLesson* lesson)
{
	return g_list_length (lesson->priv->lesson_elements);
}

void
gpinstruct_lesson_add_lesson_element (GPInstructLesson* lesson,
                                      GPInstructLessonElement* element)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_ELEMENT (element));

	lesson->priv->lesson_elements = g_list_append (lesson->priv->lesson_elements, element);
}

void
gpinstruct_lesson_remove_lesson_element (GPInstructLesson* lesson,
                                         guint lesson_element)
{
	GList* nth_link = g_list_nth (lesson->priv->lesson_elements, lesson_element);
	g_object_unref (nth_link->data);
	lesson->priv->lesson_elements = g_list_delete_link (lesson->priv->lesson_elements, nth_link);
}

GList*
gpinstruct_lesson_get_lesson_elements (GPInstructLesson* lesson)
{
	return g_list_copy (lesson->priv->lesson_elements);
}

gboolean gpinstruct_lesson_get_single_score (GPInstructLesson* lesson)
{
	return lesson->priv->single_score;
}

void gpinstruct_lesson_set_single_score (GPInstructLesson* lesson,
                                         gboolean single_score)
{
	lesson->priv->single_score = single_score;
}
