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

#include "canvas/canvas.h"

struct _CanvasLessonPrivate
{
	gchar* title;
	GList* lesson_elements;
	gboolean single_score;
};

#define CANVAS_LESSON_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON, CanvasLessonPrivate))



G_DEFINE_TYPE (CanvasLesson, canvas_lesson, CANVAS_TYPE_OBJECT);

static void
canvas_lesson_init (CanvasLesson *object)
{
	object->priv = CANVAS_LESSON_PRIVATE (object);

	object->priv->title = g_strdup ("");
	object->priv->lesson_elements = NULL;
	object->priv->single_score = FALSE;
}

static void
canvas_lesson_finalize (GObject *object)
{
	CanvasLesson* lesson = CANVAS_LESSON (object);

	if (lesson->priv->title)
		g_free (lesson->priv->title);
	if (lesson->priv->lesson_elements)
		g_list_free_full (lesson->priv->lesson_elements, g_object_unref);

	G_OBJECT_CLASS (canvas_lesson_parent_class)->finalize (object);
}

static void
canvas_lesson_class_init (CanvasLessonClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonPrivate));

	object_class->finalize = canvas_lesson_finalize;
}


CanvasLesson*
canvas_lesson_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON, NULL);
}

const gchar*
canvas_lesson_get_title (CanvasLesson* lesson)
{
	return lesson->priv->title;
}

void
canvas_lesson_set_title (CanvasLesson* lesson,
                         const gchar* title)
{
	if (lesson->priv->title)
		g_free (lesson->priv->title);
	lesson->priv->title = g_strdup (title);
}

guint
canvas_lesson_get_lesson_elements_length (CanvasLesson* lesson)
{
	return g_list_length (lesson->priv->lesson_elements);
}

void
canvas_lesson_add_lesson_element (CanvasLesson* lesson,
                                  CanvasLessonElement* element)
{
	g_return_if_fail (CANVAS_IS_LESSON_ELEMENT (element));

	lesson->priv->lesson_elements = g_list_append (lesson->priv->lesson_elements, element);
}

void
canvas_lesson_remove_lesson_element (CanvasLesson* lesson,
                                     guint lesson_element)
{
	GList* nth_link = g_list_nth (lesson->priv->lesson_elements, lesson_element);
	g_object_unref (nth_link->data);
	lesson->priv->lesson_elements = g_list_delete_link (lesson->priv->lesson_elements, nth_link);
}

GList*
canvas_lesson_get_lesson_elements (CanvasLesson* lesson)
{
	return g_list_copy (lesson->priv->lesson_elements);
}

gboolean canvas_lesson_get_single_score (CanvasLesson* lesson)
{
	return lesson->priv->single_score;
}

void canvas_lesson_set_single_score (CanvasLesson* lesson, gboolean single_score)
{
	lesson->priv->single_score = single_score;
}
