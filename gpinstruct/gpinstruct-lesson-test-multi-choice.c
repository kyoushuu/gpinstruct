/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gpinstruct/gpinstruct.h"

#include "compat/compat-glib.h"

struct _GPInstructLessonTestMultiChoicePrivate
{
	GList* questions;
};

#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE, GPInstructLessonTestMultiChoicePrivate))



static gchar*
get_item (GPInstructLessonTest* test,
          guint item)
{
	GPInstructLessonTestMultiChoiceQuestion* question =
		gpinstruct_lesson_test_multi_choice_get_question (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (test),
		                                                  item);

	if (question)
		return g_strdup (gpinstruct_lesson_test_multi_choice_question_get_text (question));
	else
		return NULL;
}


static gchar*
get_choice (GPInstructLessonTest* test,
            guint item,
            guint choice)
{
	GPInstructLessonTestMultiChoiceQuestion* question =
		gpinstruct_lesson_test_multi_choice_get_question (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (test),
		                                                  item);

	if (question)
		return g_strdup (gpinstruct_lesson_test_multi_choice_question_get_choice (question, choice));
	else
		return NULL;
}


static guint
get_items_length (GPInstructLessonTest* test)
{
	return gpinstruct_lesson_test_multi_choice_get_questions_length (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (test));
}


static guint
get_choices_length (GPInstructLessonTest* test,
                    guint item)
{
	GPInstructLessonTestMultiChoiceQuestion* question =
		gpinstruct_lesson_test_multi_choice_get_question (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (test),
		                                                  item);

	if (question)
		return gpinstruct_lesson_test_multi_choice_question_get_choices_length (question);
	else
		return 0;
}


static guint
get_item_correct_choice (GPInstructLessonTest* test,
                         guint item)
{
	GPInstructLessonTestMultiChoiceQuestion* question =
		gpinstruct_lesson_test_multi_choice_get_question (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (test), item);
	if (question)
		return gpinstruct_lesson_test_multi_choice_question_get_answer (question);
	else
		return 0;
}



G_DEFINE_TYPE (GPInstructLessonTestMultiChoice, gpinstruct_lesson_test_multi_choice, GPINSTRUCT_TYPE_LESSON_TEST);

static void
gpinstruct_lesson_test_multi_choice_init (GPInstructLessonTestMultiChoice *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_PRIVATE (object);

	object->priv->questions = NULL;
}

static void
gpinstruct_lesson_test_multi_choice_finalize (GObject *object)
{
	GPInstructLessonTestMultiChoice* test = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (object);

	if (test->priv->questions)
		g_list_free_full (test->priv->questions, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_lesson_test_multi_choice_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_multi_choice_class_init (GPInstructLessonTestMultiChoiceClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GPInstructLessonTestClass* parent_class = GPINSTRUCT_LESSON_TEST_CLASS (klass);

	parent_class->get_item = get_item;
	parent_class->get_choice = get_choice;
	parent_class->get_items_length = get_items_length;
	parent_class->get_choices_length = get_choices_length;
	parent_class->get_item_correct_choice = get_item_correct_choice;

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestMultiChoicePrivate));

	object_class->finalize = gpinstruct_lesson_test_multi_choice_finalize;
}


GPInstructLessonTestMultiChoice*
gpinstruct_lesson_test_multi_choice_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE, NULL);
}

void
gpinstruct_lesson_test_multi_choice_add_question (GPInstructLessonTestMultiChoice* test,
                                                  GPInstructLessonTestMultiChoiceQuestion* question)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (test));

	test->priv->questions = g_list_append (test->priv->questions, question);
}

void
gpinstruct_lesson_test_multi_choice_remove_question (GPInstructLessonTestMultiChoice* test,
                                                     guint question)
{
	GList* nth_link = g_list_nth (test->priv->questions, question);
	g_object_unref (nth_link->data);
	test->priv->questions = g_list_delete_link (test->priv->questions, nth_link);
}

GPInstructLessonTestMultiChoiceQuestion*
gpinstruct_lesson_test_multi_choice_get_question (GPInstructLessonTestMultiChoice* test,
                                                  guint question)
{
	return g_list_nth_data (test->priv->questions, question);
}

GList*
gpinstruct_lesson_test_multi_choice_get_questions (GPInstructLessonTestMultiChoice* test)
{
	return g_list_copy (test->priv->questions);
}

guint
gpinstruct_lesson_test_multi_choice_get_questions_length (GPInstructLessonTestMultiChoice* test)
{
	return g_list_length (test->priv->questions);
}

gboolean
gpinstruct_lesson_test_multi_choice_is_correct (GPInstructLessonTestMultiChoice* test,
                                                guint item,
                                                guint answer)
{
	GPInstructLessonTestMultiChoiceQuestion* question = gpinstruct_lesson_test_multi_choice_get_question (test, item);

	if (question == NULL)
		return FALSE;

	if (answer == gpinstruct_lesson_test_multi_choice_question_get_answer (question))
		return TRUE;
	else
		return FALSE;
}
