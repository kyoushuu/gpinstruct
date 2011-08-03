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

typedef struct _CanvasLessonTestMultiChoiceQuestionPrivate CanvasLessonTestMultiChoiceQuestionPrivate;
struct _CanvasLessonTestMultiChoiceQuestionPrivate
{
	gchar* text;
	guint answer;
	GList* choices;
};

#define CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, CanvasLessonTestMultiChoiceQuestionPrivate))



G_DEFINE_TYPE (CanvasLessonTestMultiChoiceQuestion, canvas_lesson_test_multi_choice_question, CANVAS_TYPE_OBJECT);

static void
canvas_lesson_test_multi_choice_question_init (CanvasLessonTestMultiChoiceQuestion *object)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* private_data = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(object);
	private_data->text = g_strdup ("");
	private_data->answer = 0;
	private_data->choices = NULL;
}

static void
canvas_lesson_test_multi_choice_question_finalize (GObject *object)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* private_data = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(object);

	if (private_data->text)
		g_free (private_data->text);

	if (private_data->choices)
		g_list_free_full (private_data->choices, g_free);

	G_OBJECT_CLASS (canvas_lesson_test_multi_choice_question_parent_class)->finalize (object);
}

static void
canvas_lesson_test_multi_choice_question_class_init (CanvasLessonTestMultiChoiceQuestionClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestMultiChoiceQuestionPrivate));

	object_class->finalize = canvas_lesson_test_multi_choice_question_finalize;
}


CanvasLessonTestMultiChoiceQuestion*
canvas_lesson_test_multi_choice_question_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, NULL);
}

const gchar*
canvas_lesson_test_multi_choice_question_get_text (CanvasLessonTestMultiChoiceQuestion* question)
{
	return CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question)->text;
}

void
canvas_lesson_test_multi_choice_question_set_text (CanvasLessonTestMultiChoiceQuestion* question,
                                                   const gchar* text)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* private_data = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question);

	if (private_data->text)
		g_free (private_data->text);
	private_data->text = g_strdup (text);
}

guint
canvas_lesson_test_multi_choice_question_get_answer (CanvasLessonTestMultiChoiceQuestion* question)
{
	return CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question)->answer;
}

void
canvas_lesson_test_multi_choice_question_set_answer (CanvasLessonTestMultiChoiceQuestion* question,
                                                     guint answer)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* private_data = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question);

	private_data->answer = answer;
}

void
canvas_lesson_test_multi_choice_question_add_choice (CanvasLessonTestMultiChoiceQuestion* question,
                                                     const gchar* choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (question));

	CanvasLessonTestMultiChoiceQuestionPrivate* private_data = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE (question);
	private_data->choices = g_list_append (private_data->choices, g_strdup (choice));
}

void
canvas_lesson_test_multi_choice_question_remove_choice (CanvasLessonTestMultiChoiceQuestion* question,
                                                        guint choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (question));

	CanvasLessonTestMultiChoiceQuestionPrivate* private_data = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE (question);
	private_data->choices = g_list_delete_link (private_data->choices, g_list_nth (private_data->choices, choice));
}

GList*
canvas_lesson_test_multi_choice_question_get_choices (CanvasLessonTestMultiChoiceQuestion* question)
{
	return g_list_copy (CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question)->choices);
}
