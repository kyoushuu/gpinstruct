/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011  Arnel A. Borja <kyoushuu@yahoo.com>
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

struct _GPInstructLessonTestScrambledQuestionPrivate
{
	gchar *text;
	gchar *explanation;
	gchar *answer;
};

#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION, GPInstructLessonTestScrambledQuestionPrivate))



G_DEFINE_TYPE (GPInstructLessonTestScrambledQuestion, gpinstruct_lesson_test_scrambled_question, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_test_scrambled_question_init (GPInstructLessonTestScrambledQuestion *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_PRIVATE (object);

	object->priv->text = g_strdup ("");
	object->priv->explanation = g_strdup ("");
	object->priv->answer = g_strdup ("");
}

static void
gpinstruct_lesson_test_scrambled_question_finalize (GObject *object)
{
	GPInstructLessonTestScrambledQuestion *question = GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION (object);

	g_free (question->priv->text);
	g_free (question->priv->explanation);
	g_free (question->priv->answer);

	G_OBJECT_CLASS (gpinstruct_lesson_test_scrambled_question_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_scrambled_question_class_init (GPInstructLessonTestScrambledQuestionClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass *parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestScrambledQuestionPrivate));

	object_class->finalize = gpinstruct_lesson_test_scrambled_question_finalize;
}


GPInstructLessonTestScrambledQuestion *
gpinstruct_lesson_test_scrambled_question_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION, NULL);
}

const gchar *
gpinstruct_lesson_test_scrambled_question_get_text (GPInstructLessonTestScrambledQuestion *question)
{
	return question->priv->text;
}

void
gpinstruct_lesson_test_scrambled_question_set_text (GPInstructLessonTestScrambledQuestion *question,
                                                    const gchar *text)
{
	g_free (question->priv->text);
	question->priv->text = g_strdup (text);
}

const gchar *
gpinstruct_lesson_test_scrambled_question_get_answer (GPInstructLessonTestScrambledQuestion *question)
{
	return question->priv->answer;
}

void
gpinstruct_lesson_test_scrambled_question_set_answer (GPInstructLessonTestScrambledQuestion *question,
                                                      const gchar *answer)
{
	g_free (question->priv->answer);
	question->priv->answer = g_strdup (answer);
}

const gchar *
gpinstruct_lesson_test_scrambled_question_get_explanation (GPInstructLessonTestScrambledQuestion *question)
{
	return question->priv->explanation;
}

void
gpinstruct_lesson_test_scrambled_question_set_explanation (GPInstructLessonTestScrambledQuestion *question,
                                                           const gchar *explanation)
{
	g_free (question->priv->explanation);
	question->priv->explanation = g_strdup (explanation);
}
