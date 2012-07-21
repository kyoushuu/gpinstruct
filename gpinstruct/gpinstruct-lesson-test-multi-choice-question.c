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

#include "compat/compat-glib.h"

struct _GPInstructLessonTestMultiChoiceQuestionPrivate
{
	gchar *text;
	gchar *explanation;
	guint answer;
	GList *choices;
};

#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, GPInstructLessonTestMultiChoiceQuestionPrivate))



G_DEFINE_TYPE (GPInstructLessonTestMultiChoiceQuestion, gpinstruct_lesson_test_multi_choice_question, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_test_multi_choice_question_init (GPInstructLessonTestMultiChoiceQuestion *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_GET_PRIVATE (object);
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = object->priv;

	priv->text = g_strdup ("");
	priv->explanation = g_strdup ("");
	priv->answer = 0;
	priv->choices = NULL;
}

static void
gpinstruct_lesson_test_multi_choice_question_finalize (GObject *object)
{
	GPInstructLessonTestMultiChoiceQuestion *question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (object);
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	g_free (priv->text);
	g_free (priv->explanation);

	if (priv->choices)
		g_list_free_full (priv->choices, g_free);

	G_OBJECT_CLASS (gpinstruct_lesson_test_multi_choice_question_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_multi_choice_question_class_init (GPInstructLessonTestMultiChoiceQuestionClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass *parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestMultiChoiceQuestionPrivate));

	object_class->finalize = gpinstruct_lesson_test_multi_choice_question_finalize;
}


GPInstructLessonTestMultiChoiceQuestion *
gpinstruct_lesson_test_multi_choice_question_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, NULL);
}

const gchar *
gpinstruct_lesson_test_multi_choice_question_get_text (GPInstructLessonTestMultiChoiceQuestion *question)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	return priv->text;
}

void
gpinstruct_lesson_test_multi_choice_question_set_text (GPInstructLessonTestMultiChoiceQuestion *question,
                                                       const gchar *text)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	g_free (priv->text);
	priv->text = g_strdup (text);
}

guint
gpinstruct_lesson_test_multi_choice_question_get_answer (GPInstructLessonTestMultiChoiceQuestion *question)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	return priv->answer;
}

void
gpinstruct_lesson_test_multi_choice_question_set_answer (GPInstructLessonTestMultiChoiceQuestion *question,
                                                         guint answer)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	priv->answer = answer;
}

void
gpinstruct_lesson_test_multi_choice_question_add_choice (GPInstructLessonTestMultiChoiceQuestion *question,
                                                         const gchar *choice)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_QUESTION (question));

	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	priv->choices = g_list_append (priv->choices, g_strdup (choice));
}

void
gpinstruct_lesson_test_multi_choice_question_remove_choice (GPInstructLessonTestMultiChoiceQuestion *question,
                                                            guint choice)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	GList *nth_link = g_list_nth (priv->choices, choice);
	g_free (nth_link->data);
	priv->choices = g_list_delete_link (priv->choices, nth_link);
}

const gchar *
gpinstruct_lesson_test_multi_choice_question_get_choice (GPInstructLessonTestMultiChoiceQuestion *question,
                                                         guint choice)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	return g_list_nth_data (priv->choices, choice);
}

void
gpinstruct_lesson_test_multi_choice_question_set_choice (GPInstructLessonTestMultiChoiceQuestion *question,
                                                         guint choice, const gchar *text)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	GList *nth_node = g_list_nth (priv->choices, choice);;

	g_free (nth_node->data);
	nth_node->data = g_strdup (text);
}

GList *
gpinstruct_lesson_test_multi_choice_question_get_choices (GPInstructLessonTestMultiChoiceQuestion *question)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	return g_list_copy (priv->choices);
}

guint
gpinstruct_lesson_test_multi_choice_question_get_choices_length (GPInstructLessonTestMultiChoiceQuestion *question)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	return g_list_length (priv->choices);
}

const gchar *
gpinstruct_lesson_test_multi_choice_question_get_explanation (GPInstructLessonTestMultiChoiceQuestion *question)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	return priv->explanation;
}

void
gpinstruct_lesson_test_multi_choice_question_set_explanation (GPInstructLessonTestMultiChoiceQuestion *question,
                                                              const gchar *explanation)
{
	GPInstructLessonTestMultiChoiceQuestionPrivate *priv = question->priv;

	g_free (priv->explanation);
	priv->explanation = g_strdup (explanation);
}
