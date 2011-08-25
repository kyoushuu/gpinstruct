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

struct _CanvasLessonTestWordPoolPrivate
{
	GList* questions;
	GList* choices;
};

#define CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_WORD_POOL, CanvasLessonTestWordPoolPrivate))



static gchar*
get_item (CanvasLessonTest* test, guint item)
{
	CanvasLessonTestWordPoolQuestion* question =
		canvas_lesson_test_word_pool_get_question (CANVAS_LESSON_TEST_WORD_POOL (test),
		                                           item);

	if (question)
		return g_strdup (canvas_lesson_test_word_pool_question_get_text (question));
	else
		return NULL;
}


static gchar*
get_choice (CanvasLessonTest* test, guint item, guint choice)
{
	return g_strdup (canvas_lesson_test_word_pool_get_choice (CANVAS_LESSON_TEST_WORD_POOL (test),
	                                                          choice));
}


static guint
get_items_length (CanvasLessonTest* test)
{
	return canvas_lesson_test_word_pool_get_questions_length (CANVAS_LESSON_TEST_WORD_POOL (test));
}


static guint
get_choices_length (CanvasLessonTest* test, guint item)
{
	return canvas_lesson_test_word_pool_get_choices_length (CANVAS_LESSON_TEST_WORD_POOL (test));
}


static guint
get_item_correct_choice (CanvasLessonTest* test, guint item)
{
	CanvasLessonTestWordPoolQuestion* question =
		canvas_lesson_test_word_pool_get_question (CANVAS_LESSON_TEST_WORD_POOL (test), item);
	if (question)
		return canvas_lesson_test_word_pool_question_get_answer (question);
	else
		return 0;
}



G_DEFINE_TYPE (CanvasLessonTestWordPool, canvas_lesson_test_word_pool, CANVAS_TYPE_LESSON_TEST);

static void
canvas_lesson_test_word_pool_init (CanvasLessonTestWordPool *object)
{
	object->priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE (object);

	object->priv->questions = NULL;
	object->priv->choices = NULL;
}

static void
canvas_lesson_test_word_pool_finalize (GObject *object)
{
	CanvasLessonTestWordPool* test = CANVAS_LESSON_TEST_WORD_POOL (object);

	if (test->priv->questions)
		g_list_free_full (test->priv->questions, g_object_unref);
	if (test->priv->choices)
		g_list_free_full (test->priv->choices, g_free);

	G_OBJECT_CLASS (canvas_lesson_test_word_pool_parent_class)->finalize (object);
}

static void
canvas_lesson_test_word_pool_class_init (CanvasLessonTestWordPoolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	CanvasLessonTestClass* parent_class = CANVAS_LESSON_TEST_CLASS (klass);

	parent_class->get_item = get_item;
	parent_class->get_choice = get_choice;
	parent_class->get_items_length = get_items_length;
	parent_class->get_choices_length = get_choices_length;
	parent_class->get_item_correct_choice = get_item_correct_choice;

	g_type_class_add_private (klass, sizeof (CanvasLessonTestWordPoolPrivate));

	object_class->finalize = canvas_lesson_test_word_pool_finalize;
}


CanvasLessonTestWordPool*
canvas_lesson_test_word_pool_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_TEST_WORD_POOL, NULL);
}

void
canvas_lesson_test_word_pool_add_question (CanvasLessonTestWordPool* test,
                                           CanvasLessonTestWordPoolQuestion* question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	test->priv->questions = g_list_append (test->priv->questions, question);
}

void
canvas_lesson_test_word_pool_remove_question (CanvasLessonTestWordPool* test,
                                              guint question)
{
	GList* nth_link = g_list_nth (test->priv->questions, question);
	g_object_unref (nth_link->data);
	test->priv->questions = g_list_delete_link (test->priv->questions, nth_link);
}

CanvasLessonTestWordPoolQuestion*
canvas_lesson_test_word_pool_get_question (CanvasLessonTestWordPool* test,
                                           guint question)
{
	return g_list_nth_data (test->priv->questions, question);
}

GList*
canvas_lesson_test_word_pool_get_questions (CanvasLessonTestWordPool* test)
{
	return g_list_copy (test->priv->questions);
}

guint
canvas_lesson_test_word_pool_get_questions_length (CanvasLessonTestWordPool* test)
{
	return g_list_length (test->priv->questions);
}

void
canvas_lesson_test_word_pool_add_choice (CanvasLessonTestWordPool* test,
                                         const gchar* choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	test->priv->choices = g_list_append (test->priv->choices, g_strdup (choice));
}

void
canvas_lesson_test_word_pool_remove_choice (CanvasLessonTestWordPool* test,
                                            guint choice)
{
	GList* nth_link = g_list_nth (test->priv->choices, choice);
	g_free (nth_link->data);
	test->priv->choices = g_list_delete_link (test->priv->choices, nth_link);
}

const gchar*
canvas_lesson_test_word_pool_get_choice (CanvasLessonTestWordPool* test, guint choice)
{
	return g_list_nth_data (test->priv->choices, choice);
}

void
canvas_lesson_test_word_pool_set_choice (CanvasLessonTestWordPool* test, guint choice, const gchar* text)
{
	GList* nth_node = g_list_nth (test->priv->choices, choice);

	if (nth_node->data)
		g_free (nth_node->data);
	nth_node->data = g_strdup (text);
}

GList*
canvas_lesson_test_word_pool_get_choices (CanvasLessonTestWordPool* test)
{
	return g_list_copy (test->priv->choices);
}

guint
canvas_lesson_test_word_pool_get_choices_length (CanvasLessonTestWordPool* test)
{
	return g_list_length (test->priv->choices);
}

gboolean
canvas_lesson_test_word_pool_is_correct (CanvasLessonTestWordPool* test, guint item, guint answer)
{
	CanvasLessonTestWordPoolQuestion* question = canvas_lesson_test_word_pool_get_question (test, item);

	if (question == NULL)
		return FALSE;

	if (answer == canvas_lesson_test_word_pool_question_get_answer (question))
		return TRUE;
	else
		return FALSE;
}
