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

struct _CanvasLessonTestWordPoolQuestionPrivate
{
	gchar* text;
	gchar* explanation;
	guint answer;
};

#define CANVAS_LESSON_TEST_WORD_POOL_QUESTION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION, CanvasLessonTestWordPoolQuestionPrivate))



G_DEFINE_TYPE (CanvasLessonTestWordPoolQuestion, canvas_lesson_test_word_pool_question, CANVAS_TYPE_OBJECT);

static void
canvas_lesson_test_word_pool_question_init (CanvasLessonTestWordPoolQuestion *object)
{
	object->priv = CANVAS_LESSON_TEST_WORD_POOL_QUESTION_PRIVATE (object);

	object->priv->text = g_strdup ("");
	object->priv->explanation = g_strdup ("");
	object->priv->answer = 0;
}

static void
canvas_lesson_test_word_pool_question_finalize (GObject *object)
{
	CanvasLessonTestWordPoolQuestion* question = CANVAS_LESSON_TEST_WORD_POOL_QUESTION (object);

	if (question->priv->text)
		g_free (question->priv->text);

	if (question->priv->explanation)
		g_free (question->priv->explanation);

	G_OBJECT_CLASS (canvas_lesson_test_word_pool_question_parent_class)->finalize (object);
}

static void
canvas_lesson_test_word_pool_question_class_init (CanvasLessonTestWordPoolQuestionClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonTestWordPoolQuestionPrivate));

	object_class->finalize = canvas_lesson_test_word_pool_question_finalize;
}


CanvasLessonTestWordPoolQuestion*
canvas_lesson_test_word_pool_question_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION, NULL);
}

const gchar*
canvas_lesson_test_word_pool_question_get_text (CanvasLessonTestWordPoolQuestion* question)
{
	return question->priv->text;
}

void
canvas_lesson_test_word_pool_question_set_text (CanvasLessonTestWordPoolQuestion* question,
                                                const gchar* text)
{
	if (question->priv->text)
		g_free (question->priv->text);
	question->priv->text = g_strdup (text);
}

guint
canvas_lesson_test_word_pool_question_get_answer (CanvasLessonTestWordPoolQuestion* question)
{
	return question->priv->answer;
}

void
canvas_lesson_test_word_pool_question_set_answer (CanvasLessonTestWordPoolQuestion* question,
                                                  guint answer)
{
	question->priv->answer = answer;
}

const gchar*
canvas_lesson_test_word_pool_question_get_explanation (CanvasLessonTestWordPoolQuestion* question)
{
	return question->priv->explanation;
}

void
canvas_lesson_test_word_pool_question_set_explanation (CanvasLessonTestWordPoolQuestion* question,
                                                       const gchar* explanation)
{
	if (question->priv->explanation)
		g_free (question->priv->explanation);
	question->priv->explanation = g_strdup (explanation);
}
