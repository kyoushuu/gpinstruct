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

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "canvas/canvas.h"
#include "canvas-view/canvas-view.h"
#include "canvas-analyzer/canvas-analyzer.h"

struct _CanvasLogAnalyzerPrivate
{
	CanvasLogAnalyzerProject* aproject;
	CanvasProject* project;
	GList* examinees;
};

#define CANVAS_LOG_ANALYZER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LOG_ANALYZER, CanvasLogAnalyzerPrivate))



static void
free_answer (CanvasLogAnalyzerAnswer* answer)
{
	g_free (answer);
}


static void
free_choice (CanvasLogAnalyzerChoice* choice)
{
	g_list_free_full (choice->answers, (GDestroyNotify)free_answer);
	g_free (choice);
}


static void
free_item (CanvasLogAnalyzerItem* item)
{
	g_list_free_full (item->choices, (GDestroyNotify)free_choice);
	g_free (item);
}


static void
free_test (CanvasLogAnalyzerTest* test)
{
	g_list_free_full (test->items, (GDestroyNotify)free_item);
	g_free (test);
}


static void
free_lesson (CanvasLogAnalyzerLesson* lesson)
{
	g_list_free_full (lesson->tests, (GDestroyNotify)free_test);
	g_free (lesson);
}


static void
free_category (CanvasLogAnalyzerCategory* category)
{
	g_list_free_full (category->lessons, (GDestroyNotify)free_lesson);
	g_free (category);
}


static void
free_project (CanvasLogAnalyzerProject* project)
{
	g_list_free_full (project->categories, (GDestroyNotify)free_category);
	g_datalist_clear (&project->tests_list);
	g_free (project);
}


static void
free_examinee (CanvasLogAnalyzerExaminee* examinee)
{
	free_project (examinee->project);
	g_free (examinee->last_name);
	g_free (examinee->first_name);
	g_free (examinee);
}



G_DEFINE_TYPE (CanvasLogAnalyzer, canvas_log_analyzer, CANVAS_TYPE_OBJECT);

static void
canvas_log_analyzer_init (CanvasLogAnalyzer *object)
{
	object->priv = CANVAS_LOG_ANALYZER_PRIVATE (object);

	object->priv->aproject = NULL;
	object->priv->project = NULL;
	object->priv->examinees = NULL;
}

static void
canvas_log_analyzer_finalize (GObject *object)
{
	CanvasLogAnalyzer* analyzer = CANVAS_LOG_ANALYZER (object);

	if (analyzer->priv->aproject)
		free_project (analyzer->priv->aproject);

	if (analyzer->priv->project)
		g_object_unref (analyzer->priv->project);

	if (analyzer->priv->examinees)
		g_list_free_full (analyzer->priv->examinees, (GDestroyNotify)free_examinee);

	G_OBJECT_CLASS (canvas_log_analyzer_parent_class)->finalize (object);
}

static void
canvas_log_analyzer_class_init (CanvasLogAnalyzerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLogAnalyzerPrivate));

	object_class->finalize = canvas_log_analyzer_finalize;
}


static CanvasLogAnalyzerProject*
create_project_tree (CanvasLogAnalyzer* analyzer, CanvasProject* project)
{
	CanvasCategory*			curr_category;
	CanvasLesson*			curr_lesson;
	CanvasLessonElement*	curr_lesson_element;
	CanvasLessonTest*		curr_lesson_test;

	GList *categories, *lessons, *lesson_elements;
	GList *curr_categories, *curr_lessons, *curr_lesson_elements;

	int i, j;

	CanvasLogAnalyzerProject* aproject = g_new0 (CanvasLogAnalyzerProject, 1);
	aproject->object = project;
	g_datalist_init (&aproject->tests_list);

	categories = canvas_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = CANVAS_CATEGORY (curr_categories->data);

		CanvasLogAnalyzerCategory* acategory = g_new0 (CanvasLogAnalyzerCategory, 1);
		acategory->object = curr_category;
		acategory->project = aproject;

		aproject->categories = g_list_append (aproject->categories, acategory);

		lessons = canvas_category_get_lessons (curr_category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			curr_lesson = CANVAS_LESSON (curr_lessons->data);

			CanvasLogAnalyzerLesson* alesson = g_new0 (CanvasLogAnalyzerLesson, 1);
			alesson->object = curr_lesson;
			alesson->category = acategory;

			acategory->lessons = g_list_append (acategory->lessons, alesson);

			lesson_elements = canvas_lesson_get_lesson_elements (curr_lesson);
			curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				curr_lesson_element = CANVAS_LESSON_ELEMENT (curr_lesson_elements->data);

				if (CANVAS_IS_LESSON_TEST (curr_lesson_element))
				{
					curr_lesson_test = CANVAS_LESSON_TEST (curr_lesson_element);

					CanvasLogAnalyzerTest* atest = g_new0 (CanvasLogAnalyzerTest, 1);
					atest->object = curr_lesson_test;
					atest->lesson = alesson;
					atest->id = g_quark_from_string (canvas_lesson_test_get_id (curr_lesson_test));

					alesson->tests = g_list_append (alesson->tests, atest);

					g_datalist_set_data (&aproject->tests_list,
					                     canvas_lesson_test_get_id (curr_lesson_test),
					                     atest);

					guint items_num = canvas_lesson_test_get_items_length (curr_lesson_test);
					atest->items_length = items_num;

					for (i=0; i<items_num; i++)
					{
						CanvasLogAnalyzerItem* aitem = g_new0 (CanvasLogAnalyzerItem, 1);
						aitem->id = i;
						aitem->test = atest;
						aitem->answer = canvas_lesson_test_get_item_correct_choice (curr_lesson_test, i);

						atest->items = g_list_append (atest->items, aitem);

						guint choices_num = canvas_lesson_test_get_choices_length (curr_lesson_test, i);
						aitem->choices_length = choices_num;

						for (j=0; j<choices_num; j++)
						{
							CanvasLogAnalyzerChoice* achoice = g_new0 (CanvasLogAnalyzerChoice, 1);
							achoice->id = j;
							achoice->item = aitem;

							aitem->choices = g_list_append (aitem->choices, achoice);
						}
					}

					alesson->items_length += atest->items_length;
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
add_test (CanvasLogAnalyzerProject* aproject, CanvasLogTest* log_test, CanvasLogAnalyzerExaminee* examinee)
{
	CanvasLogAnalyzerTest* test = g_datalist_id_get_data (&aproject->tests_list, log_test->id);

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

	GList* answers = log_test->answers;

	while (answers)
	{
		CanvasLogAnswer* answer = answers->data;
		CanvasLogAnalyzerItem* item = g_list_nth_data (test->items, answer->item_id);

		if (item)
		{
			item->times_answered += 1;

			item->time_spent += answer->time_spent;
			test->time_spent += answer->time_spent;
			test->lesson->time_spent += answer->time_spent;
			test->lesson->category->time_spent += answer->time_spent;
			test->lesson->category->project->time_spent += answer->time_spent;

			if (answer->answer_id == item->answer)
			{
				item->times_correctly_answered += 1;
				test->items_correctly_answered += 1;
				test->lesson->items_correctly_answered += 1;
				test->lesson->category->items_correctly_answered += 1;
				test->lesson->category->project->items_correctly_answered += 1;
			}

			CanvasLogAnalyzerChoice* choice = g_list_nth_data (item->choices, answer->answer_id);

			if (choice)
			{
				choice->times_chosen += 1;
				choice->time_spent += answer->time_spent;

				CanvasLogAnalyzerAnswer* aanswer = g_new0 (CanvasLogAnalyzerAnswer, 1);
				aanswer->choice = choice;
				aanswer->examinee = examinee;
				aanswer->time_spent = answer->time_spent;

				choice->answers = g_list_append (choice->answers, aanswer);
			}
		}

		answers = answers->next;
	}
}


CanvasLogAnalyzer*
canvas_log_analyzer_new (CanvasProject* project)
{
	CanvasLogAnalyzer* analyzer = g_object_new (CANVAS_TYPE_LOG_ANALYZER, NULL);

	analyzer->priv->aproject = create_project_tree (analyzer, project);

	analyzer->priv->project = g_object_ref (project);

	return analyzer;
}

void
canvas_log_analyzer_add_log (CanvasLogAnalyzer* analyzer, CanvasLog* log)
{
	analyzer->priv->aproject->times_taken += 1;

	CanvasLogAnalyzerExaminee* examinee = g_new0 (CanvasLogAnalyzerExaminee, 1);
	examinee->last_name = g_strdup (canvas_log_get_last_name (log));
	examinee->first_name = g_strdup (canvas_log_get_first_name (log));
	examinee->project = create_project_tree (analyzer, analyzer->priv->project);
	analyzer->priv->examinees = g_list_append (analyzer->priv->examinees, examinee);

	GList* tests = canvas_log_get_tests (log);
	GList* curr_tests = tests;

	while (curr_tests)
	{
		CanvasLogTest* log_test = curr_tests->data;
		add_test (analyzer->priv->aproject, log_test, examinee);
		add_test (examinee->project, log_test, examinee);
		curr_tests = curr_tests->next;
	}

	g_list_free (tests);
}

CanvasLogAnalyzerProject*
canvas_log_analyzer_get_project (CanvasLogAnalyzer* analyzer)
{
	return analyzer->priv->aproject;
}

GList*
canvas_log_analyzer_get_examinees (CanvasLogAnalyzer* analyzer)
{
	return g_list_copy (analyzer->priv->examinees);
}
