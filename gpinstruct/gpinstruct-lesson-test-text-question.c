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

struct _GPInstructLessonTestTextQuestionPrivate
{
	gchar *text;
	gchar *explanation;
	gchar *answer;
};

#define GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION, GPInstructLessonTestTextQuestionPrivate))



G_DEFINE_TYPE (GPInstructLessonTestTextQuestion, gpinstruct_lesson_test_text_question, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_test_text_question_init (GPInstructLessonTestTextQuestion *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_GET_PRIVATE (object);

	object->priv->text = g_strdup ("");
	object->priv->explanation = g_strdup ("");
	object->priv->answer = g_strdup ("");
}

static void
gpinstruct_lesson_test_text_question_finalize (GObject *object)
{
	GPInstructLessonTestTextQuestion *question = GPINSTRUCT_LESSON_TEST_TEXT_QUESTION (object);

	g_free (question->priv->text);
	g_free (question->priv->explanation);
	g_free (question->priv->answer);

	G_OBJECT_CLASS (gpinstruct_lesson_test_text_question_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_text_question_class_init (GPInstructLessonTestTextQuestionClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass *parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestTextQuestionPrivate));

	object_class->finalize = gpinstruct_lesson_test_text_question_finalize;
}


GPInstructLessonTestTextQuestion *
gpinstruct_lesson_test_text_question_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION, NULL);
}

const gchar *
gpinstruct_lesson_test_text_question_get_text (GPInstructLessonTestTextQuestion *question)
{
	return question->priv->text;
}

void
gpinstruct_lesson_test_text_question_set_text (GPInstructLessonTestTextQuestion *question,
                                               const gchar *text)
{
	g_free (question->priv->text);
	question->priv->text = g_strdup (text);
}

const gchar *
gpinstruct_lesson_test_text_question_get_answer (GPInstructLessonTestTextQuestion *question)
{
	return question->priv->answer;
}

void
gpinstruct_lesson_test_text_question_set_answer (GPInstructLessonTestTextQuestion *question,
                                                 const gchar *answer)
{
	g_free (question->priv->answer);
	question->priv->answer = g_strdup (answer);
}

const gchar *
gpinstruct_lesson_test_text_question_get_explanation (GPInstructLessonTestTextQuestion *question)
{
	return question->priv->explanation;
}

void
gpinstruct_lesson_test_text_question_set_explanation (GPInstructLessonTestTextQuestion *question,
                                                      const gchar *explanation)
{
	g_free (question->priv->explanation);
	question->priv->explanation = g_strdup (explanation);
}
