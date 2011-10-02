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
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-analyzer/gpinstruct-analyzer.h"

#include "compat/compat-glib.h"

struct _GPInstructLogAnalyzerPrivate
{
	GPInstructLogAnalyzerProject* aproject;
	GPInstructProject* project;
	GList* examinees;
};

#define GPINSTRUCT_LOG_ANALYZER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LOG_ANALYZER, GPInstructLogAnalyzerPrivate))



static void
free_answer (GPInstructLogAnalyzerAnswer* answer)
{
	g_free (answer);
}


static void
free_choice (GPInstructLogAnalyzerChoice* choice)
{
	g_list_free_full (choice->answers, (GDestroyNotify)free_answer);
	g_free (choice);
}


static void
free_item (GPInstructLogAnalyzerItem* item)
{
	g_list_free_full (item->choices, (GDestroyNotify)free_choice);
	g_free (item);
}


static void
free_test (GPInstructLogAnalyzerTest* test)
{
	g_list_free_full (test->items, (GDestroyNotify)free_item);
	g_free (test);
}


static void
free_group (GPInstructLogAnalyzerGroup* group)
{
	g_list_free_full (group->tests, (GDestroyNotify)free_test);
	g_free (group);
}


static void
free_lesson_element (GPInstructLogAnalyzerLessonElement* element)
{
	if (element->is_test)
		free_test (element->test);
	else
		free_group (element->group);

	g_free (element);
}


static void
free_lesson (GPInstructLogAnalyzerLesson* lesson)
{
	g_list_free_full (lesson->elements, (GDestroyNotify)free_lesson_element);
	g_free (lesson);
}


static void
free_category (GPInstructLogAnalyzerCategory* category)
{
	g_list_free_full (category->lessons, (GDestroyNotify)free_lesson);
	g_free (category);
}


static void
free_project (GPInstructLogAnalyzerProject* project)
{
	g_list_free_full (project->categories, (GDestroyNotify)free_category);
	g_datalist_clear (&project->tests_list);
	g_free (project);
}


static void
free_examinee (GPInstructLogAnalyzerExaminee* examinee)
{
	free_project (examinee->project);
	g_free (examinee->last_name);
	g_free (examinee->first_name);
	g_free (examinee);
}



G_DEFINE_TYPE (GPInstructLogAnalyzer, gpinstruct_log_analyzer, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_log_analyzer_init (GPInstructLogAnalyzer *object)
{
	object->priv = GPINSTRUCT_LOG_ANALYZER_PRIVATE (object);

	object->priv->aproject = NULL;
	object->priv->project = NULL;
	object->priv->examinees = NULL;
}

static void
gpinstruct_log_analyzer_finalize (GObject *object)
{
	GPInstructLogAnalyzer* analyzer = GPINSTRUCT_LOG_ANALYZER (object);

	if (analyzer->priv->aproject)
		free_project (analyzer->priv->aproject);

	if (analyzer->priv->project)
		g_object_unref (analyzer->priv->project);

	if (analyzer->priv->examinees)
		g_list_free_full (analyzer->priv->examinees, (GDestroyNotify)free_examinee);

	G_OBJECT_CLASS (gpinstruct_log_analyzer_parent_class)->finalize (object);
}

static void
gpinstruct_log_analyzer_class_init (GPInstructLogAnalyzerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass* parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLogAnalyzerPrivate));

	object_class->finalize = gpinstruct_log_analyzer_finalize;
}


static GPInstructLogAnalyzerProject*
create_project_tree (GPInstructLogAnalyzer* analyzer,
                     GPInstructProject* project)
{
	GPInstructCategory*				curr_category;
	GPInstructLesson*				curr_lesson;
	GPInstructLessonElement*		curr_lesson_element;
	GPInstructLessonElementGroup*	curr_lesson_element_group;
	GPInstructLessonElement*		curr_lesson_group_element;
	GPInstructLessonTest*			curr_lesson_test;

	GList *categories, *curr_categories;
	GList *lessons, *curr_lessons;
	GList *lesson_elements, *curr_lesson_elements;
	GList *lesson_elements_group, *curr_lesson_elements_group;

	int i, j;

	GPInstructLogAnalyzerProject* aproject = g_new0 (GPInstructLogAnalyzerProject, 1);
	aproject->object = project;
	g_datalist_init (&aproject->tests_list);

	categories = gpinstruct_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = GPINSTRUCT_CATEGORY (curr_categories->data);

		GPInstructLogAnalyzerCategory* acategory = g_new0 (GPInstructLogAnalyzerCategory, 1);
		acategory->object = curr_category;
		acategory->project = aproject;

		aproject->categories = g_list_append (aproject->categories, acategory);

		lessons = gpinstruct_category_get_lessons (curr_category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			curr_lesson = GPINSTRUCT_LESSON (curr_lessons->data);

			GPInstructLogAnalyzerLesson* alesson = g_new0 (GPInstructLogAnalyzerLesson, 1);
			alesson->object = curr_lesson;
			alesson->category = acategory;

			acategory->lessons = g_list_append (acategory->lessons, alesson);

			lesson_elements = gpinstruct_lesson_get_lesson_elements (curr_lesson);
			curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				curr_lesson_element = GPINSTRUCT_LESSON_ELEMENT (curr_lesson_elements->data);

				if (GPINSTRUCT_IS_LESSON_TEST (curr_lesson_element))
				{
					curr_lesson_test = GPINSTRUCT_LESSON_TEST (curr_lesson_element);

					GPInstructLogAnalyzerLessonElement* aelement = g_new0 (GPInstructLogAnalyzerLessonElement, 1);
					aelement->is_test = TRUE;

					alesson->elements = g_list_append (alesson->elements, aelement);

					GPInstructLogAnalyzerTest* atest = g_new0 (GPInstructLogAnalyzerTest, 1);
					atest->object = curr_lesson_test;
					atest->lesson = alesson;
					atest->id = g_quark_from_string (gpinstruct_lesson_test_get_id (curr_lesson_test));
					aelement->test = atest;

					g_datalist_set_data (&aproject->tests_list,
					                     gpinstruct_lesson_test_get_id (curr_lesson_test),
					                     atest);

					guint items_num = gpinstruct_lesson_test_get_items_length (curr_lesson_test);
					atest->items_length = items_num;

					for (i=0; i<items_num; i++)
					{
						GPInstructLogAnalyzerItem* aitem = g_new0 (GPInstructLogAnalyzerItem, 1);
						aitem->id = i;
						aitem->test = atest;
						aitem->answer = gpinstruct_lesson_test_get_item_correct_choice (curr_lesson_test, i);

						atest->items = g_list_append (atest->items, aitem);

						guint choices_num = gpinstruct_lesson_test_get_choices_length (curr_lesson_test, i);
						aitem->choices_length = choices_num;

						for (j=0; j<choices_num; j++)
						{
							GPInstructLogAnalyzerChoice* achoice = g_new0 (GPInstructLogAnalyzerChoice, 1);
							achoice->id = j;
							achoice->item = aitem;

							aitem->choices = g_list_append (aitem->choices, achoice);
						}
					}

					alesson->items_length += atest->items_length;
				}
				else if (GPINSTRUCT_IS_LESSON_ELEMENT_GROUP (curr_lesson_element))
				{
					curr_lesson_element_group = GPINSTRUCT_LESSON_ELEMENT_GROUP (curr_lesson_element);

					GPInstructLogAnalyzerLessonElement* aelement = g_new0 (GPInstructLogAnalyzerLessonElement, 1);
					aelement->is_test = FALSE;

					alesson->elements = g_list_append (alesson->elements, aelement);

					GPInstructLogAnalyzerGroup* agroup = g_new0 (GPInstructLogAnalyzerGroup, 1);
					agroup->object = curr_lesson_element_group;
					aelement->group = agroup;

					lesson_elements_group = gpinstruct_lesson_element_group_get_lesson_elements (curr_lesson_element_group);
					curr_lesson_elements_group = lesson_elements_group;

					while (curr_lesson_elements_group)
					{
						curr_lesson_group_element = GPINSTRUCT_LESSON_ELEMENT (curr_lesson_elements_group->data);

						if (GPINSTRUCT_IS_LESSON_TEST (curr_lesson_group_element))
						{
							curr_lesson_test = GPINSTRUCT_LESSON_TEST (curr_lesson_group_element);

							GPInstructLogAnalyzerTest* atest = g_new0 (GPInstructLogAnalyzerTest, 1);
							atest->object = curr_lesson_test;
							atest->lesson = alesson;
							atest->group = agroup;
							atest->id = g_quark_from_string (gpinstruct_lesson_test_get_id (curr_lesson_test));

							agroup->tests = g_list_append (agroup->tests, atest);

							g_datalist_set_data (&aproject->tests_list,
							                     gpinstruct_lesson_test_get_id (curr_lesson_test),
							                     atest);

							guint items_num = gpinstruct_lesson_test_get_items_length (curr_lesson_test);
							atest->items_length = items_num;

							for (i=0; i<items_num; i++)
							{
								GPInstructLogAnalyzerItem* aitem = g_new0 (GPInstructLogAnalyzerItem, 1);
								aitem->id = i;
								aitem->test = atest;
								aitem->answer = gpinstruct_lesson_test_get_item_correct_choice (curr_lesson_test, i);

								atest->items = g_list_append (atest->items, aitem);

								guint choices_num = gpinstruct_lesson_test_get_choices_length (curr_lesson_test, i);
								aitem->choices_length = choices_num;

								for (j=0; j<choices_num; j++)
								{
									GPInstructLogAnalyzerChoice* achoice = g_new0 (GPInstructLogAnalyzerChoice, 1);
									achoice->id = j;
									achoice->item = aitem;

									aitem->choices = g_list_append (aitem->choices, achoice);
								}
							}

							agroup->items_length += atest->items_length;
						}

						curr_lesson_elements_group = curr_lesson_elements_group->next;
					}

					alesson->items_length += agroup->items_length;
				}

				curr_lesson_elements = curr_lesson_elements->next;
			}

			g_list_free (lesson_elements);

			acategory->items_length += alesson->items_length;

			curr_lessons = curr_lessons->next;
		}

		g_list_free (lessons);

		aproject->items_length += acategory->items_length;

		curr_categories = curr_categories->next;
	}

	g_list_free (categories);

	return aproject;
}

void
add_test (GPInstructLogAnalyzerProject* aproject,
          GPInstructLogTest* log_test,
          GPInstructLogAnalyzerExaminee* examinee)
{
	GPInstructLogAnalyzerTest* test = g_datalist_id_get_data (&aproject->tests_list, log_test->id);

	if (test == NULL)
		return;

	test->times_taken += 1;

	if (test->lesson->object != aproject->last_lesson)
	{
		test->lesson->times_taken += 1;
		aproject->last_lesson = test->lesson->object;

		if (test->lesson->category->object != aproject->last_category)
		{
			test->lesson->category->times_taken += 1;
			aproject->last_category = test->lesson->category->object;
		}
	}

	if (test->group != NULL && test->group->object != aproject->last_group)
	{
		test->group->times_taken += 1;
		aproject->last_group = test->group->object;
	}

	GList* answers = log_test->answers;

	while (answers)
	{
		GPInstructLogAnswer* answer = answers->data;
		GPInstructLogAnalyzerItem* item = g_list_nth_data (test->items, answer->item_id);

		if (item)
		{
			item->times_answered += 1;

			item->time_spent += answer->time_spent;
			test->time_spent += answer->time_spent;
			if (test->group) test->group->time_spent += answer->time_spent;
			test->lesson->time_spent += answer->time_spent;
			test->lesson->category->time_spent += answer->time_spent;
			test->lesson->category->project->time_spent += answer->time_spent;

			if (answer->answer_id == item->answer)
			{
				item->times_correctly_answered += 1;
				test->items_correctly_answered += 1;
				if (test->group) test->group->items_correctly_answered += 1;
				test->lesson->items_correctly_answered += 1;
				test->lesson->category->items_correctly_answered += 1;
				test->lesson->category->project->items_correctly_answered += 1;
			}

			GPInstructLogAnalyzerChoice* choice = g_list_nth_data (item->choices, answer->answer_id);

			if (choice)
			{
				choice->times_chosen += 1;
				choice->time_spent += answer->time_spent;

				GPInstructLogAnalyzerAnswer* aanswer = g_new0 (GPInstructLogAnalyzerAnswer, 1);
				aanswer->choice = choice;
				aanswer->examinee = examinee;
				aanswer->time_spent = answer->time_spent;

				choice->answers = g_list_append (choice->answers, aanswer);
			}
		}

		answers = answers->next;
	}
}


GPInstructLogAnalyzer*
gpinstruct_log_analyzer_new (GPInstructProject* project)
{
	GPInstructLogAnalyzer* analyzer = g_object_new (GPINSTRUCT_TYPE_LOG_ANALYZER, NULL);

	analyzer->priv->aproject = create_project_tree (analyzer, project);

	analyzer->priv->project = g_object_ref (project);

	return analyzer;
}

void
gpinstruct_log_analyzer_add_log (GPInstructLogAnalyzer* analyzer,
                                 GPInstructLog* log)
{
	analyzer->priv->aproject->times_taken += 1;

	GPInstructLogAnalyzerExaminee* examinee = g_new0 (GPInstructLogAnalyzerExaminee, 1);
	examinee->last_name = g_strdup (gpinstruct_log_get_last_name (log));
	examinee->first_name = g_strdup (gpinstruct_log_get_first_name (log));
	examinee->project = create_project_tree (analyzer, analyzer->priv->project);
	analyzer->priv->examinees = g_list_append (analyzer->priv->examinees, examinee);

	GList* tests = gpinstruct_log_get_tests (log);
	GList* curr_tests = tests;

	while (curr_tests)
	{
		GPInstructLogTest* log_test = curr_tests->data;
		add_test (analyzer->priv->aproject, log_test, examinee);
		add_test (examinee->project, log_test, examinee);
		curr_tests = curr_tests->next;
	}

	g_list_free (tests);
}

GPInstructLogAnalyzerProject*
gpinstruct_log_analyzer_get_project (GPInstructLogAnalyzer* analyzer)
{
	return analyzer->priv->aproject;
}

GList*
gpinstruct_log_analyzer_get_examinees (GPInstructLogAnalyzer* analyzer)
{
	return g_list_copy (analyzer->priv->examinees);
}
