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

struct _GPInstructLessonTestWordPoolPrivate
{
	GList *questions;
	GList *choices;
};

#define GPINSTRUCT_LESSON_TEST_WORD_POOL_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL, GPInstructLessonTestWordPoolPrivate))



static gchar *
get_item (GPInstructLessonTest *test,
          guint item)
{
	GPInstructLessonTestWordPoolQuestion *question =
		gpinstruct_lesson_test_word_pool_get_question (GPINSTRUCT_LESSON_TEST_WORD_POOL (test),
		                                               item);

	if (question)
		return g_strdup (gpinstruct_lesson_test_word_pool_question_get_text (question));
	else
		return NULL;
}


static gchar *
get_choice (GPInstructLessonTest *test,
            guint item,
            guint choice)
{
	return g_strdup (gpinstruct_lesson_test_word_pool_get_choice (GPINSTRUCT_LESSON_TEST_WORD_POOL (test),
	                                                              choice));
}


static guint
get_items_length (GPInstructLessonTest *test)
{
	return gpinstruct_lesson_test_word_pool_get_questions_length (GPINSTRUCT_LESSON_TEST_WORD_POOL (test));
}


static guint
get_choices_length (GPInstructLessonTest *test,
                    guint item)
{
	return gpinstruct_lesson_test_word_pool_get_choices_length (GPINSTRUCT_LESSON_TEST_WORD_POOL (test));
}


static guint
get_item_correct_choice (GPInstructLessonTest *test,
                         guint item)
{
	GPInstructLessonTestWordPoolQuestion *question =
		gpinstruct_lesson_test_word_pool_get_question (GPINSTRUCT_LESSON_TEST_WORD_POOL (test), item);
	if (question)
		return gpinstruct_lesson_test_word_pool_question_get_answer (question);
	else
		return 0;
}



G_DEFINE_TYPE (GPInstructLessonTestWordPool, gpinstruct_lesson_test_word_pool, GPINSTRUCT_TYPE_LESSON_TEST);

static void
gpinstruct_lesson_test_word_pool_init (GPInstructLessonTestWordPool *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_WORD_POOL_GET_PRIVATE (object);

	object->priv->questions = NULL;
	object->priv->choices = NULL;
}

static void
gpinstruct_lesson_test_word_pool_finalize (GObject *object)
{
	GPInstructLessonTestWordPool *test = GPINSTRUCT_LESSON_TEST_WORD_POOL (object);

	if (test->priv->questions)
		g_list_free_full (test->priv->questions, g_object_unref);
	if (test->priv->choices)
		g_list_free_full (test->priv->choices, g_free);

	G_OBJECT_CLASS (gpinstruct_lesson_test_word_pool_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_word_pool_class_init (GPInstructLessonTestWordPoolClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GPInstructLessonTestClass *parent_class = GPINSTRUCT_LESSON_TEST_CLASS (klass);

	parent_class->get_item = get_item;
	parent_class->get_choice = get_choice;
	parent_class->get_items_length = get_items_length;
	parent_class->get_choices_length = get_choices_length;
	parent_class->get_item_correct_choice = get_item_correct_choice;

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestWordPoolPrivate));

	object_class->finalize = gpinstruct_lesson_test_word_pool_finalize;
}


GPInstructLessonTestWordPool *
gpinstruct_lesson_test_word_pool_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL, NULL);
}

void
gpinstruct_lesson_test_word_pool_add_question (GPInstructLessonTestWordPool *test,
                                               GPInstructLessonTestWordPoolQuestion *question)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (test));

	test->priv->questions = g_list_append (test->priv->questions, question);
}

void
gpinstruct_lesson_test_word_pool_remove_question (GPInstructLessonTestWordPool *test,
                                                  guint question)
{
	GList *nth_link = g_list_nth (test->priv->questions, question);
	g_object_unref (nth_link->data);
	test->priv->questions = g_list_delete_link (test->priv->questions, nth_link);
}

GPInstructLessonTestWordPoolQuestion *
gpinstruct_lesson_test_word_pool_get_question (GPInstructLessonTestWordPool *test,
                                               guint question)
{
	return g_list_nth_data (test->priv->questions, question);
}

GList *
gpinstruct_lesson_test_word_pool_get_questions (GPInstructLessonTestWordPool *test)
{
	return g_list_copy (test->priv->questions);
}

guint
gpinstruct_lesson_test_word_pool_get_questions_length (GPInstructLessonTestWordPool *test)
{
	return g_list_length (test->priv->questions);
}

void
gpinstruct_lesson_test_word_pool_add_choice (GPInstructLessonTestWordPool *test,
                                             const gchar *choice)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (test));

	test->priv->choices = g_list_append (test->priv->choices, g_strdup (choice));
}

void
gpinstruct_lesson_test_word_pool_remove_choice (GPInstructLessonTestWordPool *test,
                                                guint choice)
{
	GList *nth_link = g_list_nth (test->priv->choices, choice);
	g_free (nth_link->data);
	test->priv->choices = g_list_delete_link (test->priv->choices, nth_link);
}

const gchar *
gpinstruct_lesson_test_word_pool_get_choice (GPInstructLessonTestWordPool *test,
                                             guint choice)
{
	return g_list_nth_data (test->priv->choices, choice);
}

void
gpinstruct_lesson_test_word_pool_set_choice (GPInstructLessonTestWordPool *test,
                                             guint choice,
                                             const gchar *text)
{
	GList *nth_node = g_list_nth (test->priv->choices, choice);

	g_free (nth_node->data);
	nth_node->data = g_strdup (text);
}

GList *
gpinstruct_lesson_test_word_pool_get_choices (GPInstructLessonTestWordPool *test)
{
	return g_list_copy (test->priv->choices);
}

guint
gpinstruct_lesson_test_word_pool_get_choices_length (GPInstructLessonTestWordPool *test)
{
	return g_list_length (test->priv->choices);
}

gboolean
gpinstruct_lesson_test_word_pool_is_correct (GPInstructLessonTestWordPool *test,
                                             guint item,
                                             guint answer)
{
	GPInstructLessonTestWordPoolQuestion *question = gpinstruct_lesson_test_word_pool_get_question (test, item);

	if (question == NULL)
		return FALSE;

	if (answer == gpinstruct_lesson_test_word_pool_question_get_answer (question))
		return TRUE;
	else
		return FALSE;
}
