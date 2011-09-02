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
	gchar* directions;
	gboolean explain;
};

#define CANVAS_LESSON_TEST_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST, CanvasLessonTestPrivate))



G_DEFINE_TYPE (CanvasLessonTest, canvas_lesson_test, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_test_init (CanvasLessonTest *object)
{
	CanvasLessonTestPrivate* priv = CANVAS_LESSON_TEST_PRIVATE(object);
	priv->directions = g_strdup ("");
	priv->explain = FALSE;
}

static void
canvas_lesson_test_finalize (GObject *object)
{
	CanvasLessonTestPrivate* priv = CANVAS_LESSON_TEST_PRIVATE(object);

	if (priv->directions)
		g_free (priv->directions);

	G_OBJECT_CLASS (canvas_lesson_test_parent_class)->finalize (object);
}

static void
canvas_lesson_test_class_init (CanvasLessonTestClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestPrivate));

	object_class->finalize = canvas_lesson_test_finalize;
}


CanvasLessonTest*
canvas_lesson_test_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST, NULL);
}

const gchar*
canvas_lesson_test_get_directions (CanvasLessonTest* test)
{
	return CANVAS_LESSON_TEST_PRIVATE(test)->directions;
}

void
canvas_lesson_test_set_directions (CanvasLessonTest* test, const gchar* directions)
{
	CanvasLessonTestPrivate* priv = CANVAS_LESSON_TEST_PRIVATE(test);

	if (priv->directions)
		g_free (priv->directions);
	priv->directions = g_strdup (directions);
}

gboolean
canvas_lesson_test_get_explain (CanvasLessonTest* test)
{
	return CANVAS_LESSON_TEST_PRIVATE(test)->explain;
}

void
canvas_lesson_test_set_explain (CanvasLessonTest* test, gboolean explain)
{
	CANVAS_LESSON_TEST_PRIVATE(test)->explain = explain;
}
