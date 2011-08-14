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
	gchar* explanation;
	guint answer;
	GList* choices;
};

#define CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, CanvasLessonTestMultiChoiceQuestionPrivate))



G_DEFINE_TYPE (CanvasLessonTestMultiChoiceQuestion, canvas_lesson_test_multi_choice_question, CANVAS_TYPE_OBJECT);

static void
canvas_lesson_test_multi_choice_question_init (CanvasLessonTestMultiChoiceQuestion *object)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(object);
	priv->text = g_strdup ("");
	priv->explanation = g_strdup ("");
	priv->answer = 0;
	priv->choices = NULL;
}

static void
canvas_lesson_test_multi_choice_question_finalize (GObject *object)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(object);

	if (priv->text)
		g_free (priv->text);

	if (priv->explanation)
		g_free (priv->explanation);

	if (priv->choices)
		g_list_free_full (priv->choices, g_free);

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
	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question);

	if (priv->text)
		g_free (priv->text);
	priv->text = g_strdup (text);
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
	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question);

	priv->answer = answer;
}

void
canvas_lesson_test_multi_choice_question_add_choice (CanvasLessonTestMultiChoiceQuestion* question,
                                                     const gchar* choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (question));

	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE (question);
	priv->choices = g_list_append (priv->choices, g_strdup (choice));
}

void
canvas_lesson_test_multi_choice_question_remove_choice (CanvasLessonTestMultiChoiceQuestion* question,
                                                        guint choice)
{
	g_return_if_fail (CANVAS_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (question));

	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE (question);
	priv->choices = g_list_delete_link (priv->choices, g_list_nth (priv->choices, choice));
}

GList*
canvas_lesson_test_multi_choice_question_get_choices (CanvasLessonTestMultiChoiceQuestion* question)
{
	return g_list_copy (CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question)->choices);
}

guint
canvas_lesson_test_multi_choice_question_get_choices_length (CanvasLessonTestMultiChoiceQuestion* question)
{
	return g_list_length (CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question)->choices);
}

const gchar*
canvas_lesson_test_multi_choice_question_get_explanation (CanvasLessonTestMultiChoiceQuestion* question)
{
	return CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question)->explanation;
}

void
canvas_lesson_test_multi_choice_question_set_explanation (CanvasLessonTestMultiChoiceQuestion* question,
                                                          const gchar* explanation)
{
	CanvasLessonTestMultiChoiceQuestionPrivate* priv = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION_PRIVATE(question);

	if (priv->explanation)
		g_free (priv->explanation);
	priv->explanation = g_strdup (explanation);
}
