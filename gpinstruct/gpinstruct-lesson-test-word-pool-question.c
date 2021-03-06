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

struct _GPInstructLessonTestWordPoolQuestionPrivate
{
	gchar *text;
	gchar *explanation;
	guint answer;
};

#define GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION, GPInstructLessonTestWordPoolQuestionPrivate))



G_DEFINE_TYPE (GPInstructLessonTestWordPoolQuestion, gpinstruct_lesson_test_word_pool_question, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_test_word_pool_question_init (GPInstructLessonTestWordPoolQuestion *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_GET_PRIVATE (object);
	GPInstructLessonTestWordPoolQuestionPrivate *priv = object->priv;

	priv->text = g_strdup ("");
	priv->explanation = g_strdup ("");
	priv->answer = 0;
}

static void
gpinstruct_lesson_test_word_pool_question_finalize (GObject *object)
{
	GPInstructLessonTestWordPoolQuestion *question = GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION (object);
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	g_free (priv->text);
	g_free (priv->explanation);

	G_OBJECT_CLASS (gpinstruct_lesson_test_word_pool_question_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_word_pool_question_class_init (GPInstructLessonTestWordPoolQuestionClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass *parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestWordPoolQuestionPrivate));

	object_class->finalize = gpinstruct_lesson_test_word_pool_question_finalize;
}


GPInstructLessonTestWordPoolQuestion *
gpinstruct_lesson_test_word_pool_question_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION, NULL);
}

const gchar *
gpinstruct_lesson_test_word_pool_question_get_text (GPInstructLessonTestWordPoolQuestion *question)
{
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	return priv->text;
}

void
gpinstruct_lesson_test_word_pool_question_set_text (GPInstructLessonTestWordPoolQuestion *question,
                                                    const gchar *text)
{
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	g_free (priv->text);
	priv->text = g_strdup (text);
}

guint
gpinstruct_lesson_test_word_pool_question_get_answer (GPInstructLessonTestWordPoolQuestion *question)
{
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	return priv->answer;
}

void
gpinstruct_lesson_test_word_pool_question_set_answer (GPInstructLessonTestWordPoolQuestion *question,
                                                      guint answer)
{
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	priv->answer = answer;
}

const gchar *
gpinstruct_lesson_test_word_pool_question_get_explanation (GPInstructLessonTestWordPoolQuestion *question)
{
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	return priv->explanation;
}

void
gpinstruct_lesson_test_word_pool_question_set_explanation (GPInstructLessonTestWordPoolQuestion *question,
                                                           const gchar *explanation)
{
	GPInstructLessonTestWordPoolQuestionPrivate *priv = question->priv;

	g_free (priv->explanation);
	priv->explanation = g_strdup (explanation);
}
