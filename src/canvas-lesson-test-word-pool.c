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
	CanvasLessonTestWordPoolPrivate* private_data = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(object);
	private_data->questions = NULL;
	private_data->choices = NULL;
}

static void
canvas_lesson_test_word_pool_finalize (GObject *object)
{
	CanvasLessonTestWordPoolPrivate* private_data = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(object);

	if (private_data->questions)
		g_list_free_full (private_data->questions, g_free);
	if (private_data->choices)
		g_list_free_full (private_data->choices, g_free);

	G_OBJECT_CLASS (canvas_lesson_test_word_pool_parent_class)->finalize (object);
}

static void
canvas_lesson_test_word_pool_class_init (CanvasLessonTestWordPoolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	CanvasLessonTestClass* parent_class = CANVAS_LESSON_TEST_CLASS (klass);

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

	CanvasLessonTestWordPoolPrivate* private_data = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test);
	private_data->questions = g_list_append (private_data->questions, question);
}

void
canvas_lesson_test_word_pool_remove_question (CanvasLessonTestWordPool* test,
                                                 CanvasLessonTestWordPoolQuestion* question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_WORD_POOL (test));

	CanvasLessonTestWordPoolPrivate* private_data = CANVAS_LESSON_TEST_WORD_POOL_PRIVATE(test);
	private_data->questions = g_list_remove (private_data->questions, question);
}
