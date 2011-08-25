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

struct _CanvasLessonTestPrivate
{
	gchar* directions;
	gboolean explain;
	gchar* id;
};

#define CANVAS_LESSON_TEST_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST, CanvasLessonTestPrivate))



gchar*
create_id ()
{
	static const gint length = 8;
	static const char lookup_table[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static const gint lookup_table_elements = G_N_ELEMENTS (lookup_table);

	gchar* id = g_new (gchar, length+1);
	id[length] = 0;

    int i;
    for (i=0; i<length; i++)
		id[i] = lookup_table[g_random_int_range (0, lookup_table_elements)];

	return id;
}



G_DEFINE_TYPE (CanvasLessonTest, canvas_lesson_test, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_test_init (CanvasLessonTest *object)
{
	object->priv = CANVAS_LESSON_TEST_PRIVATE (object);

	object->priv->directions = g_strdup ("");
	object->priv->explain = FALSE;
	object->priv->id = NULL;
}

static void
canvas_lesson_test_finalize (GObject *object)
{
	CanvasLessonTest* test = CANVAS_LESSON_TEST (object);

	if (test->priv->directions)
		g_free (test->priv->directions);

	if (test->priv->id)
		g_free (test->priv->id);

	G_OBJECT_CLASS (canvas_lesson_test_parent_class)->finalize (object);
}

static void
canvas_lesson_test_class_init (CanvasLessonTestClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);*/

	klass->get_item = NULL;
	klass->get_choice = NULL;
	klass->get_items_length = NULL;
	klass->get_choices_length = NULL;
	klass->get_item_correct_choice = NULL;

	g_type_class_add_private (klass, sizeof (CanvasLessonTestPrivate));

	object_class->finalize = canvas_lesson_test_finalize;
}


CanvasLessonTest*
canvas_lesson_test_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_TEST, NULL);
}

const gchar*
canvas_lesson_test_get_directions (CanvasLessonTest* test)
{
	return test->priv->directions;
}

void
canvas_lesson_test_set_directions (CanvasLessonTest* test, const gchar* directions)
{
	if (test->priv->directions)
		g_free (test->priv->directions);
	test->priv->directions = g_strdup (directions);
}

gboolean
canvas_lesson_test_get_explain (CanvasLessonTest* test)
{
	return test->priv->explain;
}

void
canvas_lesson_test_set_explain (CanvasLessonTest* test, gboolean explain)
{
	test->priv->explain = explain;
}

const gchar*
canvas_lesson_test_get_id (CanvasLessonTest* test)
{
	if (test->priv->id == NULL)
		test->priv->id = create_id ();
	return test->priv->id;
}

void
canvas_lesson_test_set_id (CanvasLessonTest* test, const gchar* id)
{
	if (test->priv->id)
		g_free (test->priv->id);
	test->priv->id = g_strdup (id);
}

gchar*
canvas_lesson_test_get_item (CanvasLessonTest* test, guint item)
{
	gchar*(* get_item) (CanvasLessonTest* test, guint item) =
		CANVAS_LESSON_TEST_GET_CLASS (test)->get_item;
	if (get_item)
		return get_item (test, item);
	else
		return NULL;
}

gchar*
canvas_lesson_test_get_choice (CanvasLessonTest* test, guint item, guint choice)
{
	gchar*(* get_choice) (CanvasLessonTest* test, guint item, guint choice) =
		CANVAS_LESSON_TEST_GET_CLASS (test)->get_choice;
	if (get_choice)
		return get_choice (test, item, choice);
	else
		return NULL;
}

guint
canvas_lesson_test_get_items_length (CanvasLessonTest* test)
{
	guint(* get_items_length) (CanvasLessonTest* test) =
		CANVAS_LESSON_TEST_GET_CLASS (test)->get_items_length;
	if (get_items_length)
		return get_items_length (test);
	else
		return FALSE;
}

guint
canvas_lesson_test_get_choices_length (CanvasLessonTest* test, guint item)
{
	guint(* get_choices_length) (CanvasLessonTest* test, guint item) =
		CANVAS_LESSON_TEST_GET_CLASS (test)->get_choices_length;
	if (get_choices_length)
		return get_choices_length (test, item);
	else
		return 0;
}

guint
canvas_lesson_test_get_item_correct_choice (CanvasLessonTest* test, guint item)
{
	guint(* get_item_correct_choice) (CanvasLessonTest* test, guint item) =
		CANVAS_LESSON_TEST_GET_CLASS (test)->get_item_correct_choice;
	if (get_item_correct_choice)
		return get_item_correct_choice (test, item);
	else
		return 0;
}
