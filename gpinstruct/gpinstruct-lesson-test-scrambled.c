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

struct _GPInstructLessonTestScrambledPrivate
{
	GList *questions;
};

#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED, GPInstructLessonTestScrambledPrivate))



static gchar *
get_item (GPInstructLessonTest *test,
          guint item)
{
	GPInstructLessonTestScrambledQuestion *question =
		gpinstruct_lesson_test_scrambled_get_question (GPINSTRUCT_LESSON_TEST_SCRAMBLED (test),
		                                               item);

	if (question)
		return g_strdup (gpinstruct_lesson_test_scrambled_question_get_text (question));
	else
		return NULL;
}


static guint
get_items_length (GPInstructLessonTest *test)
{
	return gpinstruct_lesson_test_scrambled_get_questions_length (GPINSTRUCT_LESSON_TEST_SCRAMBLED (test));
}


static gchar *
get_item_correct_string (GPInstructLessonTest *test,
                         guint item)
{
	GPInstructLessonTestScrambledQuestion *question =
		gpinstruct_lesson_test_scrambled_get_question (GPINSTRUCT_LESSON_TEST_SCRAMBLED (test), item);
	if (question)
		return g_strdup (gpinstruct_lesson_test_scrambled_question_get_answer (question));
	else
		return NULL;
}



G_DEFINE_TYPE (GPInstructLessonTestScrambled, gpinstruct_lesson_test_scrambled, GPINSTRUCT_TYPE_LESSON_TEST);

static void
gpinstruct_lesson_test_scrambled_init (GPInstructLessonTestScrambled *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_SCRAMBLED_GET_PRIVATE (object);

	object->priv->questions = NULL;
}

static void
gpinstruct_lesson_test_scrambled_finalize (GObject *object)
{
	GPInstructLessonTestScrambled *test = GPINSTRUCT_LESSON_TEST_SCRAMBLED (object);

	if (test->priv->questions)
		g_list_free_full (test->priv->questions, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_lesson_test_scrambled_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_scrambled_class_init (GPInstructLessonTestScrambledClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GPInstructLessonTestClass *parent_class = GPINSTRUCT_LESSON_TEST_CLASS (klass);

	parent_class->answer_is_string = TRUE;
	parent_class->get_item = get_item;
	parent_class->get_items_length = get_items_length;
	parent_class->get_item_correct_string = get_item_correct_string;

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestScrambledPrivate));

	object_class->finalize = gpinstruct_lesson_test_scrambled_finalize;
}


GPInstructLessonTestScrambled *
gpinstruct_lesson_test_scrambled_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED, NULL);
}

void
gpinstruct_lesson_test_scrambled_add_question (GPInstructLessonTestScrambled *test,
                                               GPInstructLessonTestScrambledQuestion *question)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED (test));

	test->priv->questions = g_list_append (test->priv->questions, question);
}

void
gpinstruct_lesson_test_scrambled_remove_question (GPInstructLessonTestScrambled *test,
                                                  guint question)
{
	GList *nth_link = g_list_nth (test->priv->questions, question);
	g_object_unref (nth_link->data);
	test->priv->questions = g_list_delete_link (test->priv->questions, nth_link);
}

GPInstructLessonTestScrambledQuestion *
gpinstruct_lesson_test_scrambled_get_question (GPInstructLessonTestScrambled *test,
                                               guint question)
{
	return g_list_nth_data (test->priv->questions, question);
}

GList *
gpinstruct_lesson_test_scrambled_get_questions (GPInstructLessonTestScrambled *test)
{
	return g_list_copy (test->priv->questions);
}

guint
gpinstruct_lesson_test_scrambled_get_questions_length (GPInstructLessonTestScrambled *test)
{
	return g_list_length (test->priv->questions);
}

gboolean
gpinstruct_lesson_test_scrambled_is_correct (GPInstructLessonTestScrambled *test,
                                             guint item,
                                             gchar *answer)
{
	GPInstructLessonTestScrambledQuestion *question = gpinstruct_lesson_test_scrambled_get_question (test, item);

	if (question == NULL)
		return FALSE;

	if (g_strcmp0 (answer, gpinstruct_lesson_test_scrambled_question_get_answer (question)) == 0)
		return TRUE;
	else
		return FALSE;
}
