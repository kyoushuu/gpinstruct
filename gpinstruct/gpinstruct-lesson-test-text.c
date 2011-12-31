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

#include "compat/compat-glib.h"

struct _GPInstructLessonTestTextPrivate
{
	GList *questions;
};

#define GPINSTRUCT_LESSON_TEST_TEXT_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_TEXT, GPInstructLessonTestTextPrivate))



static gchar *
get_item (GPInstructLessonTest *test,
          guint item)
{
	GPInstructLessonTestTextQuestion *question =
		gpinstruct_lesson_test_text_get_question (GPINSTRUCT_LESSON_TEST_TEXT (test),
		                                          item);

	if (question)
		return g_strdup (gpinstruct_lesson_test_text_question_get_text (question));
	else
		return NULL;
}


static guint
get_items_length (GPInstructLessonTest *test)
{
	return gpinstruct_lesson_test_text_get_questions_length (GPINSTRUCT_LESSON_TEST_TEXT (test));
}


static gchar *
get_item_correct_string (GPInstructLessonTest *test,
                         guint item)
{
	GPInstructLessonTestTextQuestion *question =
		gpinstruct_lesson_test_text_get_question (GPINSTRUCT_LESSON_TEST_TEXT (test), item);
	if (question)
		return g_strdup (gpinstruct_lesson_test_text_question_get_answer (question));
	else
		return NULL;
}



G_DEFINE_TYPE (GPInstructLessonTestText, gpinstruct_lesson_test_text, GPINSTRUCT_TYPE_LESSON_TEST);

static void
gpinstruct_lesson_test_text_init (GPInstructLessonTestText *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_TEXT_GET_PRIVATE (object);
	GPInstructLessonTestTextPrivate *priv = object->priv;

	priv->questions = NULL;
}

static void
gpinstruct_lesson_test_text_finalize (GObject *object)
{
	GPInstructLessonTestText *test = GPINSTRUCT_LESSON_TEST_TEXT (object);
	GPInstructLessonTestTextPrivate *priv = test->priv;

	if (priv->questions)
		g_list_free_full (priv->questions, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_lesson_test_text_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_text_class_init (GPInstructLessonTestTextClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GPInstructLessonTestClass *parent_class = GPINSTRUCT_LESSON_TEST_CLASS (klass);

	parent_class->answer_is_string = TRUE;
	parent_class->get_item = get_item;
	parent_class->get_items_length = get_items_length;
	parent_class->get_item_correct_string = get_item_correct_string;

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestTextPrivate));

	object_class->finalize = gpinstruct_lesson_test_text_finalize;
}


GPInstructLessonTestText *
gpinstruct_lesson_test_text_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_TEXT, NULL);
}

void
gpinstruct_lesson_test_text_add_question (GPInstructLessonTestText *test,
                                          GPInstructLessonTestTextQuestion *question)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_TEXT (test));

	GPInstructLessonTestTextPrivate *priv = test->priv;

	priv->questions = g_list_append (priv->questions, question);
}

void
gpinstruct_lesson_test_text_remove_question (GPInstructLessonTestText *test,
                                             guint question)
{
	GPInstructLessonTestTextPrivate *priv = test->priv;

	GList *nth_link = g_list_nth (priv->questions, question);
	g_object_unref (nth_link->data);
	priv->questions = g_list_delete_link (priv->questions, nth_link);
}

GPInstructLessonTestTextQuestion *
gpinstruct_lesson_test_text_get_question (GPInstructLessonTestText *test,
                                          guint question)
{
	GPInstructLessonTestTextPrivate *priv = test->priv;

	return g_list_nth_data (priv->questions, question);
}

GList *
gpinstruct_lesson_test_text_get_questions (GPInstructLessonTestText *test)
{
	GPInstructLessonTestTextPrivate *priv = test->priv;

	return g_list_copy (priv->questions);
}

guint
gpinstruct_lesson_test_text_get_questions_length (GPInstructLessonTestText *test)
{
	GPInstructLessonTestTextPrivate *priv = test->priv;

	return g_list_length (priv->questions);
}

gboolean
gpinstruct_lesson_test_text_is_correct (GPInstructLessonTestText *test,
                                        guint item,
                                        gchar *answer)
{
	GPInstructLessonTestTextQuestion *question = gpinstruct_lesson_test_text_get_question (test, item);

	if (question == NULL)
		return FALSE;

	if (g_strcmp0 (answer, gpinstruct_lesson_test_text_question_get_answer (question)) == 0)
		return TRUE;
	else
		return FALSE;
}
