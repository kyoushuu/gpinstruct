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

typedef struct _CanvasLessonTestPrivate CanvasLessonTestPrivate;
struct _CanvasLessonTestPrivate
{
	gchar* direction;
};

#define CANVAS_LESSON_TEST_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST, CanvasLessonTestPrivate))



G_DEFINE_TYPE (CanvasLessonTest, canvas_lesson_test, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_test_init (CanvasLessonTest *object)
{
	CanvasLessonTestPrivate* private_data = CANVAS_LESSON_TEST_PRIVATE(object);
	private_data->direction = g_strdup ("");
}

static void
canvas_lesson_test_finalize (GObject *object)
{
	CanvasLessonTestPrivate* private_data = CANVAS_LESSON_TEST_PRIVATE(object);

	if (private_data->direction)
		g_free (private_data->direction);

	G_OBJECT_CLASS (canvas_lesson_test_parent_class)->finalize (object);
}

static void
canvas_lesson_test_class_init (CanvasLessonTestClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (CanvasLessonTestPrivate));

	object_class->finalize = canvas_lesson_test_finalize;
}


CanvasLessonTest*
canvas_lesson_test_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST, NULL);
}

const gchar*
canvas_lesson_test_get_direction (CanvasLessonTest* test)
{
	return CANVAS_LESSON_TEST_PRIVATE(test)->direction;
}

void
canvas_lesson_test_set_direction (CanvasLessonTest* test, const gchar* direction)
{
	CanvasLessonTestPrivate* private_data = CANVAS_LESSON_TEST_PRIVATE(test);

	if (private_data->direction)
		g_free (private_data->direction);
	private_data->direction = g_strdup (direction);
}
