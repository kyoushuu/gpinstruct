/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja <kyoushuu@yahoo.com>
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

struct _GPInstructLessonTestPrivate
{
	gchar *directions;
	gboolean explain;
	gchar *id;
};

#define GPINSTRUCT_LESSON_TEST_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST, GPInstructLessonTestPrivate))



static gchar *
create_id ()
{
	static const gint length = 8;
	static const char lookup_table[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static const gint lookup_table_elements = G_N_ELEMENTS (lookup_table)-1;

	gchar *id = g_new (gchar, length+1);
	id[length] = 0;

	int i;
	for (i=0; i<length; i++)
		id[i] = lookup_table[g_random_int_range (0, lookup_table_elements)];

	return id;
}



G_DEFINE_TYPE (GPInstructLessonTest, gpinstruct_lesson_test, GPINSTRUCT_TYPE_LESSON_ELEMENT);

static void
gpinstruct_lesson_test_init (GPInstructLessonTest *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_GET_PRIVATE (object);
	GPInstructLessonTestPrivate *priv = object->priv;

	priv->directions = g_strdup ("");
	priv->explain = FALSE;
	priv->id = NULL;
}

static void
gpinstruct_lesson_test_finalize (GObject *object)
{
	GPInstructLessonTest *test = GPINSTRUCT_LESSON_TEST (object);
	GPInstructLessonTestPrivate *priv = test->priv;

	g_free (priv->directions);
	g_free (priv->id);

	G_OBJECT_CLASS (gpinstruct_lesson_test_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_class_init (GPInstructLessonTestClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonElementClass *parent_class = GPINSTRUCT_LESSON_ELEMENT_CLASS (klass);*/

	klass->answer_is_string = FALSE;
	klass->get_item = NULL;
	klass->get_choice = NULL;
	klass->get_items_length = NULL;
	klass->get_choices_length = NULL;
	klass->get_item_correct_choice = NULL;
	klass->get_item_correct_string = NULL;

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestPrivate));

	object_class->finalize = gpinstruct_lesson_test_finalize;
}


GPInstructLessonTest *
gpinstruct_lesson_test_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST, NULL);
}

const gchar *
gpinstruct_lesson_test_get_directions (GPInstructLessonTest *test)
{
	GPInstructLessonTestPrivate *priv = test->priv;

	return priv->directions;
}

void
gpinstruct_lesson_test_set_directions (GPInstructLessonTest *test,
                                       const gchar *directions)
{
	GPInstructLessonTestPrivate *priv = test->priv;

	g_free (priv->directions);
	priv->directions = g_strdup (directions);
}

gboolean
gpinstruct_lesson_test_get_explain (GPInstructLessonTest *test)
{
	GPInstructLessonTestPrivate *priv = test->priv;

	return priv->explain;
}

void
gpinstruct_lesson_test_set_explain (GPInstructLessonTest *test,
                                    gboolean explain)
{
	GPInstructLessonTestPrivate *priv = test->priv;

	priv->explain = explain;
}

const gchar *
gpinstruct_lesson_test_get_id (GPInstructLessonTest *test)
{
	GPInstructLessonTestPrivate *priv = test->priv;

	if (priv->id == NULL)
		priv->id = create_id ();
	return priv->id;
}

void
gpinstruct_lesson_test_set_id (GPInstructLessonTest *test,
                               const gchar *id)
{
	GPInstructLessonTestPrivate *priv = test->priv;

	g_free (priv->id);
	priv->id = g_strdup (id);
}

gchar *
gpinstruct_lesson_test_get_item (GPInstructLessonTest *test,
                                 guint item)
{
	gchar*(* get_item) (GPInstructLessonTest *test, guint item) =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->get_item;
	if (get_item)
		return get_item (test, item);
	else
		return NULL;
}

gchar *
gpinstruct_lesson_test_get_choice (GPInstructLessonTest *test,
                                   guint item,
                                   guint choice)
{
	gchar*(* get_choice) (GPInstructLessonTest *test, guint item, guint choice) =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->get_choice;
	if (get_choice)
		return get_choice (test, item, choice);
	else
		return NULL;
}

guint
gpinstruct_lesson_test_get_items_length (GPInstructLessonTest *test)
{
	guint(* get_items_length) (GPInstructLessonTest *test) =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->get_items_length;
	if (get_items_length)
		return get_items_length (test);
	else
		return FALSE;
}

gboolean
gpinstruct_lesson_test_get_answer_is_string (GPInstructLessonTest *test)
{
	gboolean answer_is_string =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->answer_is_string;
	return answer_is_string;
}

guint
gpinstruct_lesson_test_get_choices_length (GPInstructLessonTest *test,
                                           guint item)
{
	guint(* get_choices_length) (GPInstructLessonTest *test, guint item) =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->get_choices_length;
	if (get_choices_length)
		return get_choices_length (test, item);
	else
		return 0;
}

guint
gpinstruct_lesson_test_get_item_correct_choice (GPInstructLessonTest *test,
                                                guint item)
{
	guint(* get_item_correct_choice) (GPInstructLessonTest *test, guint item) =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->get_item_correct_choice;
	if (get_item_correct_choice)
		return get_item_correct_choice (test, item);
	else
		return 0;
}

gchar *
gpinstruct_lesson_test_get_item_correct_string (GPInstructLessonTest *test,
                                                guint item)
{
	gchar*(* get_item_correct_string) (GPInstructLessonTest *test, guint item) =
		GPINSTRUCT_LESSON_TEST_GET_CLASS (test)->get_item_correct_string;
	if (get_item_correct_string)
		return get_item_correct_string (test, item);
	else
		return NULL;
}
