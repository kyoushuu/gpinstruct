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

struct _CanvasLessonTestMultiChoicePrivate
{
	GList* questions;
};

#define CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE, CanvasLessonTestMultiChoicePrivate))



static gchar*
get_item (CanvasLessonTest* test, guint item)
{
	CanvasLessonTestMultiChoiceQuestion* question =
		canvas_lesson_test_multi_choice_get_question (CANVAS_LESSON_TEST_MULTI_CHOICE (test),
		                                              item);

	if (question)
		return g_strdup (canvas_lesson_test_multi_choice_question_get_text (question));
	else
		return NULL;
}


static gchar*
get_choice (CanvasLessonTest* test, guint item, guint choice)
{
	CanvasLessonTestMultiChoiceQuestion* question =
		canvas_lesson_test_multi_choice_get_question (CANVAS_LESSON_TEST_MULTI_CHOICE (test),
		                                              item);

	if (question)
		return g_strdup (canvas_lesson_test_multi_choice_question_get_choice (question, choice));
	else
		return NULL;
}


static guint
get_items_length (CanvasLessonTest* test)
{
	return canvas_lesson_test_multi_choice_get_questions_length (CANVAS_LESSON_TEST_MULTI_CHOICE (test));
}


static guint
get_choices_length (CanvasLessonTest* test, guint item)
{
	CanvasLessonTestMultiChoiceQuestion* question =
		canvas_lesson_test_multi_choice_get_question (CANVAS_LESSON_TEST_MULTI_CHOICE (test),
		                                              item);

	if (question)
		return canvas_lesson_test_multi_choice_question_get_choices_length (question);
	else
		return 0;
}


static guint
get_item_correct_choice (CanvasLessonTest* test, guint item)
{
	CanvasLessonTestMultiChoiceQuestion* question =
		canvas_lesson_test_multi_choice_get_question (CANVAS_LESSON_TEST_MULTI_CHOICE (test), item);
	if (question)
		return canvas_lesson_test_multi_choice_question_get_answer (question);
	else
		return 0;
}



G_DEFINE_TYPE (CanvasLessonTestMultiChoice, canvas_lesson_test_multi_choice, CANVAS_TYPE_LESSON_TEST);

static void
canvas_lesson_test_multi_choice_init (CanvasLessonTestMultiChoice *object)
{
	object->priv = CANVAS_LESSON_TEST_MULTI_CHOICE_PRIVATE (object);

	object->priv->questions = NULL;
}

static void
canvas_lesson_test_multi_choice_finalize (GObject *object)
{
	CanvasLessonTestMultiChoice* test = CANVAS_LESSON_TEST_MULTI_CHOICE (object);

	if (test->priv->questions)
		g_list_free_full (test->priv->questions, g_object_unref);

	G_OBJECT_CLASS (canvas_lesson_test_multi_choice_parent_class)->finalize (object);
}

static void
canvas_lesson_test_multi_choice_class_init (CanvasLessonTestMultiChoiceClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	CanvasLessonTestClass* parent_class = CANVAS_LESSON_TEST_CLASS (klass);

	parent_class->get_item = get_item;
	parent_class->get_choice = get_choice;
	parent_class->get_items_length = get_items_length;
	parent_class->get_choices_length = get_choices_length;
	parent_class->get_item_correct_choice = get_item_correct_choice;

	g_type_class_add_private (klass, sizeof (CanvasLessonTestMultiChoicePrivate));

	object_class->finalize = canvas_lesson_test_multi_choice_finalize;
}


CanvasLessonTestMultiChoice*
canvas_lesson_test_multi_choice_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE, NULL);
}

void
canvas_lesson_test_multi_choice_add_question (CanvasLessonTestMultiChoice* test,
                                              CanvasLessonTestMultiChoiceQuestion* question)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (test));

	test->priv->questions = g_list_append (test->priv->questions, question);
}

void
canvas_lesson_test_multi_choice_remove_question (CanvasLessonTestMultiChoice* test,
                                                 guint question)
{
	GList* nth_link = g_list_nth (test->priv->questions, question);
	g_object_unref (nth_link->data);
	test->priv->questions = g_list_delete_link (test->priv->questions, nth_link);
}

CanvasLessonTestMultiChoiceQuestion*
canvas_lesson_test_multi_choice_get_question (CanvasLessonTestMultiChoice* test,
                                              guint question)
{
	return g_list_nth_data (test->priv->questions, question);
}

GList*
canvas_lesson_test_multi_choice_get_questions (CanvasLessonTestMultiChoice* test)
{
	return g_list_copy (test->priv->questions);
}

guint
canvas_lesson_test_multi_choice_get_questions_length (CanvasLessonTestMultiChoice* test)
{
	return g_list_length (test->priv->questions);
}

gboolean
canvas_lesson_test_multi_choice_is_correct (CanvasLessonTestMultiChoice* test, guint item, guint answer)
{
	CanvasLessonTestMultiChoiceQuestion* question = canvas_lesson_test_multi_choice_get_question (test, item);

	if (question == NULL)
		return FALSE;

	if (answer == canvas_lesson_test_multi_choice_question_get_answer (question))
		return TRUE;
	else
		return FALSE;
}
