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

typedef struct _CanvasLessonTestWordPoolPrivate CanvasLessonTestWordPoolPrivate;
struct _CanvasLessonTestWordPoolPrivate
{
	GList* questions;
	GList* choices;
};

#define CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_WORD_POOL, CanvasLessonTestWordPoolPrivate))



G_DEFINE_TYPE (CanvasLessonTestWordPool, canvas_lesson_test_word_pool, CANVAS_TYPE_LESSON_TEST);

static void
canvas_lesson_test_word_pool_init (CanvasLessonTestWordPool *object)
{
	CanvasLessonTestWordPoolPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(object);
	priv->questions = NULL;
	priv->choices = NULL;
}

static void
canvas_lesson_test_word_pool_finalize (GObject *object)
{
	CanvasLessonTestWordPoolPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(object);

	if (priv->questions)
		g_list_free_full (priv->questions, g_object_unref);
	if (priv->choices)
		g_list_free_full (priv->choices, g_free);

	G_OBJECT_CLASS (canvas_lesson_test_word_pool_parent_class)->finalize (object);
}

static void
canvas_lesson_test_word_pool_class_init (CanvasLessonTestWordPoolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonTestClass* parent_class = CANVAS_LESSON_TEST_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestWordPoolPrivate));

	object_class->finalize = canvas_lesson_test_word_pool_finalize;
}


CanvasLessonTestWordPool*
canvas_lesson_test_word_pool_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST_WORD_POOL, NULL);
}

void
canvas_lesson_test_word_pool_add_question (CanvasLessonTestWordPool* test,
                                           CanvasLessonTestWordPoolQuestion* question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	CanvasLessonTestWordPoolPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test);
	priv->questions = g_list_append (priv->questions, question);
}

void
canvas_lesson_test_word_pool_remove_question (CanvasLessonTestWordPool* test,
                                              guint question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	CanvasLessonTestWordPoolPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test);
	priv->questions = g_list_remove (priv->questions, g_list_nth (priv->questions, question));
}

GList*
canvas_lesson_test_word_pool_get_questions (CanvasLessonTestWordPool* test)
{
	return g_list_copy (CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test)->questions);
}

void
canvas_lesson_test_word_pool_add_choice (CanvasLessonTestWordPool* test,
                                         const gchar* choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	CanvasLessonTestWordPoolPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test);
	priv->choices = g_list_append (priv->choices, g_strdup (choice));
}

void
canvas_lesson_test_word_pool_remove_choice (CanvasLessonTestWordPool* test,
                                            guint choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	CanvasLessonTestWordPoolPrivate* priv = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test);
	
	priv->questions = g_list_delete_link (priv->choices, g_list_nth (priv->choices, choice));
}

GList*
canvas_lesson_test_word_pool_get_choices (CanvasLessonTestWordPool* test)
{
	return g_list_copy (CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test)->choices);
}
