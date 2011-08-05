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

typedef struct _CanvasLessonTestMultiChoicePrivate CanvasLessonTestMultiChoicePrivate;
struct _CanvasLessonTestMultiChoicePrivate
{
	GList* questions;
};

#define CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE, CanvasLessonTestMultiChoicePrivate))



G_DEFINE_TYPE (CanvasLessonTestMultiChoice, canvas_lesson_test_multi_choice, CANVAS_TYPE_LESSON_TEST);

static void
canvas_lesson_test_multi_choice_init (CanvasLessonTestMultiChoice *object)
{
	CanvasLessonTestMultiChoicePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(object);
	priv->questions = NULL;
}

static void
canvas_lesson_test_multi_choice_finalize (GObject *object)
{
	CanvasLessonTestMultiChoicePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(object);

	if (priv->questions)
		g_list_free_full (priv->questions, g_object_unref);

	G_OBJECT_CLASS (canvas_lesson_test_multi_choice_parent_class)->finalize (object);
}

static void
canvas_lesson_test_multi_choice_class_init (CanvasLessonTestMultiChoiceClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonTestClass* parent_class = CANVAS_LESSON_TEST_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestMultiChoicePrivate));

	object_class->finalize = canvas_lesson_test_multi_choice_finalize;
}


CanvasLessonTestMultiChoice*
canvas_lesson_test_multi_choice_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE, NULL);
}

void
canvas_lesson_test_multi_choice_add_question (CanvasLessonTestMultiChoice* test,
                                              CanvasLessonTestMultiChoiceQuestion* question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (test));

	CanvasLessonTestMultiChoicePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(test);
	priv->questions = g_list_append (priv->questions, question);
}

void
canvas_lesson_test_multi_choice_remove_question (CanvasLessonTestMultiChoice* test,
                                                 guint question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (test));

	CanvasLessonTestMultiChoicePrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(test);
	priv->questions = g_list_delete_link (priv->questions, g_list_nth (priv->questions, question));
}

GList*
canvas_lesson_test_multi_choice_get_questions (CanvasLessonTestMultiChoice* test)
{
	return g_list_copy (CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(test)->questions);
}
