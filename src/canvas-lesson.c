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

typedef struct _CanvasLessonPrivate CanvasLessonPrivate;
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
	CanvasLessonPrivate* priv = CANVAS_LESSON_PRIVATE(object);

	priv->title = g_strdup ("");
	priv->lesson_elements = NULL;
	priv->single_score = FALSE;
}

static void
canvas_lesson_finalize (GObject *object)
{
	CanvasLessonPrivate* priv = CANVAS_LESSON_PRIVATE(object);

	if (priv->title)
		g_free (priv->title);
	if (priv->lesson_elements)
		g_list_free_full (priv->lesson_elements, g_object_unref);

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
	return CANVAS_LESSON_PRIVATE (lesson)->title;
}

void
canvas_lesson_set_title (CanvasLesson* lesson,
                         const gchar* title)
{
	CanvasLessonPrivate* priv = CANVAS_LESSON_PRIVATE(lesson);

	if (priv->title)
		g_free (priv->title);
	priv->title = g_strdup (title);
}

guint
canvas_lesson_get_lesson_elements_length (CanvasLesson* lesson)
{
	return g_list_length (CANVAS_LESSON_PRIVATE(lesson)->lesson_elements);
}

void
canvas_lesson_add_lesson_element (CanvasLesson* lesson,
                                  CanvasLessonElement* element)
{
	g_return_if_fail (CANVAS_IS_LESSON_ELEMENT (element));

	CanvasLessonPrivate* priv = CANVAS_LESSON_PRIVATE(lesson);
	priv->lesson_elements = g_list_append (priv->lesson_elements, element);
}

void
canvas_lesson_remove_lesson_element (CanvasLesson* lesson,
                                     CanvasLessonElement* element)
{
	g_return_if_fail (CANVAS_IS_LESSON_ELEMENT (element));

	CanvasLessonPrivate* priv = CANVAS_LESSON_PRIVATE(lesson);
	priv->lesson_elements = g_list_append (priv->lesson_elements, element);
}

GList*
canvas_lesson_get_lesson_elements (CanvasLesson* lesson)
{
	return g_list_copy (CANVAS_LESSON_PRIVATE(lesson)->lesson_elements);
}

gboolean canvas_lesson_get_single_score (CanvasLesson* lesson)
{
	return CANVAS_LESSON_PRIVATE(lesson)->single_score;
}

void canvas_lesson_set_single_score (CanvasLesson* lesson, gboolean single_score)
{
	CANVAS_LESSON_PRIVATE(lesson)->single_score = single_score;
}
