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

#include <config.h>
#include <glib/gi18n.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gpinstruct/gpinstruct.h"

#define GCHAR_TO_GBOOLEAN(a) (g_ascii_strncasecmp(a, "true", -1) == 0)?TRUE:FALSE


GQuark
gpinstruct_parser_error_quark (void)
{
	return g_quark_from_static_string ("gpinstruct-parser-error-quark");
}


static GPInstructLessonTestMultiChoice*
parse_multi_choice_test (xmlNode *node)
{
	GPInstructLessonTestMultiChoice* test = gpinstruct_lesson_test_multi_choice_new ();

	xmlNode *current_node, *parent_node;
	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (test), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "id");
	if (temp)
	{
		gpinstruct_lesson_test_set_id (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "explain");
	if (temp)
	{
		gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
		xmlFree (temp);
	}

	for (current_node = node->children;
	     current_node != NULL;
	     current_node = current_node->next)
	{
		if (current_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrEqual (current_node->name, BAD_CAST "directions"))
			{
				temp = xmlNodeGetContent (current_node);
				if (temp)
				{
					gpinstruct_lesson_test_set_directions (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
					xmlFree (temp);
				}
			}
			else if (xmlStrEqual (current_node->name, BAD_CAST "question"))
			{
				GPInstructLessonTestMultiChoiceQuestion* question = gpinstruct_lesson_test_multi_choice_question_new ();
				gpinstruct_lesson_test_multi_choice_add_question (test, question);

				temp = xmlGetProp (current_node, BAD_CAST "answer");
				if (temp)
				{
					gpinstruct_lesson_test_multi_choice_question_set_answer (question, atoi((gchar*)temp));
					xmlFree (temp);
				}

				for (parent_node = current_node,
				     current_node = current_node->children;
				     current_node != NULL;
				     current_node = current_node->next)
				{
					if (current_node->type == XML_ELEMENT_NODE)
					{
						if (xmlStrEqual (current_node->name, BAD_CAST "text"))
						{
							temp = xmlNodeGetContent (current_node);
							if (temp)
							{
								gpinstruct_lesson_test_multi_choice_question_set_text (question, (gchar*)temp);
								xmlFree (temp);
							}
						}
						else if (xmlStrEqual (current_node->name, BAD_CAST "explanation"))
						{
							temp = xmlNodeGetContent (current_node);
							if (temp)
							{
								gpinstruct_lesson_test_multi_choice_question_set_explanation (question, (gchar*)temp);
								xmlFree (temp);
							}
						}
						else if (xmlStrEqual (current_node->name, BAD_CAST "choice"))
						{
							temp = xmlNodeGetContent (current_node);
							if (temp)
							{
								gpinstruct_lesson_test_multi_choice_question_add_choice (question, (gchar*)temp);
								xmlFree (temp);
							}
						}
					}
				}

				current_node = parent_node;
				parent_node = current_node->parent;
			}
		}
	}

	return test;
}

static GPInstructLessonTestWordPool*
parse_word_pool_test (xmlNode *node)
{
	GPInstructLessonTestWordPool* test = gpinstruct_lesson_test_word_pool_new ();

	xmlNode *current_node, *parent_node;
	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (test), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "id");
	if (temp)
	{
		gpinstruct_lesson_test_set_id (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "explain");
	if (temp)
	{
		gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
		xmlFree (temp);
	}

	for (current_node = node->children;
	     current_node != NULL;
	     current_node = current_node->next)
	{
		if (current_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrEqual (current_node->name, BAD_CAST "directions"))
			{
				temp = xmlNodeGetContent (current_node);
				if (temp)
				{
					gpinstruct_lesson_test_set_directions (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
					xmlFree (temp);
				}
			}
			else if (xmlStrEqual (current_node->name, BAD_CAST "choice"))
			{
				temp = xmlNodeGetContent (current_node);
				if (temp)
				{
					gpinstruct_lesson_test_word_pool_add_choice (test, (gchar*)temp);
					xmlFree (temp);
				}
			}
			else if (xmlStrEqual (current_node->name, BAD_CAST "question"))
			{
				GPInstructLessonTestWordPoolQuestion* question = gpinstruct_lesson_test_word_pool_question_new ();
				gpinstruct_lesson_test_word_pool_add_question (test, question);

				temp = xmlGetProp (current_node, BAD_CAST "answer");
				if (temp)
				{
					gpinstruct_lesson_test_word_pool_question_set_answer (question, atoi((gchar*)temp));
					xmlFree (temp);
				}

				for (parent_node = current_node,
				     current_node = current_node->children;
				     current_node != NULL;
				     current_node = current_node->next)
				{
					if (current_node->type == XML_ELEMENT_NODE)
					{
						if (xmlStrEqual (current_node->name, BAD_CAST "text"))
						{
							temp = xmlNodeGetContent (current_node);
							if (temp)
							{
								gpinstruct_lesson_test_word_pool_question_set_text (question, (gchar*)temp);
								xmlFree (temp);
							}
						}
						else if (xmlStrEqual (current_node->name, BAD_CAST "explanation"))
						{
							temp = xmlNodeGetContent (current_node);
							if (temp)
							{
								gpinstruct_lesson_test_word_pool_question_set_explanation (question, (gchar*)temp);
								xmlFree (temp);
							}
						}
					}
				}

				current_node = parent_node;
				parent_node = current_node->parent;
			}
		}
	}

	return test;
}

static GPInstructLessonTestOrder*
parse_order_test (xmlNode *node)
{
	GPInstructLessonTestOrder* test = gpinstruct_lesson_test_order_new ();

	xmlNode *current_node;
	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (test), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "id");
	if (temp)
	{
		gpinstruct_lesson_test_set_id (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "explain");
	if (temp)
	{
		gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
		xmlFree (temp);
	}

	for (current_node = node->children;
	     current_node != NULL;
	     current_node = current_node->next)
	{
		if (current_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrEqual (current_node->name, BAD_CAST "directions"))
			{
				temp = xmlNodeGetContent (current_node);
				if (temp)
				{
					gpinstruct_lesson_test_set_directions (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
					xmlFree (temp);
				}
			}
			else if (xmlStrEqual (current_node->name, BAD_CAST "item"))
			{
				GPInstructLessonTestOrderItem* item = gpinstruct_lesson_test_order_item_new ();
				gpinstruct_lesson_test_order_add_item (test, item);

				temp = xmlGetProp (current_node, BAD_CAST "answer");
				if (temp)
				{
					gpinstruct_lesson_test_order_item_set_answer (item, atoi ((gchar*)temp));
					xmlFree (temp);
				}

				temp = xmlNodeGetContent (current_node);
				if (temp)
				{
					gpinstruct_lesson_test_order_item_set_text (item, (gchar*)temp);
					xmlFree (temp);
				}
			}
			else if (xmlStrEqual (current_node->name, BAD_CAST "explanation"))
			{
				temp = xmlNodeGetContent (current_node);
				if (temp)
				{
					gpinstruct_lesson_test_order_set_explanation (test, (gchar*)temp);
					xmlFree (temp);
				}
			}
		}
	}

	return test;
}

static GPInstructLessonDiscussion*
parse_discussion (xmlNode *node)
{
	GPInstructLessonDiscussion* discussion = gpinstruct_lesson_discussion_new ();

	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (discussion), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlNodeGetContent (node);
	if (temp)
	{
		gpinstruct_lesson_discussion_set_text (discussion, (gchar*)temp);
		xmlFree (temp);
	}

	return discussion;
}

static GPInstructLessonReading*
parse_reading (xmlNode *node)
{
	GPInstructLessonReading* reading = gpinstruct_lesson_reading_new ();

	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (reading), (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlNodeGetContent (node);
	if (temp)
	{
		gpinstruct_lesson_reading_set_text (reading, (gchar*)temp);
		xmlFree (temp);
	}

	return reading;
}

static GPInstructLesson*
parse_lesson (xmlNode *node)
{
	GPInstructLesson* lesson = gpinstruct_lesson_new ();

	xmlNode *current_node;
	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_lesson_set_title (lesson, (gchar*)temp);
		xmlFree (temp);
	}

	temp = xmlGetProp (node, BAD_CAST "single-score");
	if (temp)
	{
		gpinstruct_lesson_set_single_score (lesson, GCHAR_TO_GBOOLEAN ((gchar*)temp));
		xmlFree (temp);
	}

	for (current_node = node->children;
	     current_node != NULL;
	     current_node = current_node->next)
	{
		if (current_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrEqual (current_node->name, BAD_CAST "test-multi-choice"))
				gpinstruct_lesson_add_lesson_element (lesson,
				                                      GPINSTRUCT_LESSON_ELEMENT (parse_multi_choice_test (current_node)));
			else if (xmlStrEqual (current_node->name, BAD_CAST "test-word-pool"))
				gpinstruct_lesson_add_lesson_element (lesson,
				                                      GPINSTRUCT_LESSON_ELEMENT (parse_word_pool_test (current_node)));
			else if (xmlStrEqual (current_node->name, BAD_CAST "test-order"))
				gpinstruct_lesson_add_lesson_element (lesson,
				                                      GPINSTRUCT_LESSON_ELEMENT (parse_order_test (current_node)));
			else if (xmlStrEqual (current_node->name, BAD_CAST "discussion"))
				gpinstruct_lesson_add_lesson_element (lesson,
				                                      GPINSTRUCT_LESSON_ELEMENT (parse_discussion (current_node)));
			else if (xmlStrEqual (current_node->name, BAD_CAST "reading"))
				gpinstruct_lesson_add_lesson_element (lesson,
				                                      GPINSTRUCT_LESSON_ELEMENT (parse_reading (current_node)));
		}
	}

	return lesson;
}

static GPInstructCategory*
parse_category (xmlNode *node)
{
	GPInstructCategory* category = gpinstruct_category_new ();

	xmlNode *current_node;
	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_category_set_title (category, (gchar*)temp);
		xmlFree (temp);
	}

	for (current_node = node->children;
	     current_node != NULL;
	     current_node = current_node->next)
	{
		if (current_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrEqual (current_node->name, BAD_CAST "lesson"))
				gpinstruct_category_add_lesson (category,
				                                parse_lesson (current_node));
		}
	}

	return category;
}

static GPInstructProject*
parse_project (xmlNode *node)
{
	GPInstructProject* project = gpinstruct_project_new ();

	xmlNode *current_node;
	xmlChar *temp;

	temp = xmlGetProp (node, BAD_CAST "title");
	if (temp)
	{
		gpinstruct_project_set_title (project, (gchar*)temp);
		xmlFree (temp);
	}

	for (current_node = node->children;
	     current_node != NULL;
	     current_node = current_node->next)
	{
		if (current_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrEqual (current_node->name, BAD_CAST "category"))
				gpinstruct_project_add_category (project,
				                                 parse_category (current_node));
		}
	}

	return project;
}


static xmlNodePtr
add_discussion (GPInstructLessonDiscussion* discussion,
                xmlNodePtr parent_node)
{
	xmlNodePtr node = xmlNewChild (parent_node, NULL, BAD_CAST "discussion",
	                               BAD_CAST gpinstruct_lesson_discussion_get_text (discussion));
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (discussion)));

	return node;
}

static xmlNodePtr
add_reading (GPInstructLessonReading* reading,
             xmlNodePtr parent_node)
{
	xmlNodePtr node = xmlNewChild (parent_node, NULL, BAD_CAST "reading",
	                               BAD_CAST gpinstruct_lesson_reading_get_text (reading));
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (reading)));

	return node;
}

static xmlNodePtr
add_multi_choice_test (GPInstructLessonTestMultiChoice* test,
                       xmlNodePtr parent_node)
{
	GPInstructLessonTestMultiChoiceQuestion* curr_question;

	GList *questions, *curr_questions;
	GList *choices, *curr_choices;

	gchar *curr_choice, *temp;

	xmlNodePtr question_node;

	xmlNodePtr node = xmlNewChild (parent_node, NULL,
	                               BAD_CAST "test-multi-choice", NULL);
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	xmlSetProp (node, BAD_CAST "id",
	            BAD_CAST gpinstruct_lesson_test_get_id (GPINSTRUCT_LESSON_TEST (test)));
	xmlSetProp (node, BAD_CAST "explain",
	            gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test))?BAD_CAST "true":BAD_CAST "false");

	xmlNewChild (node, NULL, BAD_CAST "directions",
	             BAD_CAST gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (test)));

	questions = gpinstruct_lesson_test_multi_choice_get_questions (test);
	curr_questions = questions;

	while (curr_questions)
	{
		curr_question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (curr_questions->data);

		question_node = xmlNewChild (node, NULL, BAD_CAST "question", NULL);
		temp = g_strdup_printf ("%d", gpinstruct_lesson_test_multi_choice_question_get_answer (curr_question));
		xmlSetProp (question_node, BAD_CAST "answer", BAD_CAST temp);
		g_free (temp);

		xmlNewChild (question_node, NULL, BAD_CAST "text",
		             BAD_CAST gpinstruct_lesson_test_multi_choice_question_get_text (curr_question));

		xmlNewChild (question_node, NULL, BAD_CAST "explanation",
		             BAD_CAST gpinstruct_lesson_test_multi_choice_question_get_explanation (curr_question));

		choices = gpinstruct_lesson_test_multi_choice_question_get_choices (curr_question);
		curr_choices = choices;

		while (curr_choices)
		{
			curr_choice = curr_choices->data;

			xmlNewChild (question_node, NULL, BAD_CAST "choice",
			             BAD_CAST curr_choice);

			curr_choices = curr_choices->next;
		}

		g_list_free (choices);

		curr_questions = curr_questions->next;
	}

	g_list_free (questions);

	return node;
}

static xmlNodePtr
add_word_pool_test (GPInstructLessonTestWordPool* test,
                    xmlNodePtr parent_node)
{
	GPInstructLessonTestWordPoolQuestion* curr_question;

	GList *questions, *curr_questions;
	GList *choices, *curr_choices;

	gchar *curr_choice, *temp;

	xmlNodePtr question_node;

	xmlNodePtr node = xmlNewChild (parent_node, NULL,
	                               BAD_CAST "test-word-pool", NULL);
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	xmlSetProp (node, BAD_CAST "id",
	            BAD_CAST gpinstruct_lesson_test_get_id (GPINSTRUCT_LESSON_TEST (test)));
	xmlSetProp (node, BAD_CAST "explain",
	            gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test))?BAD_CAST "true":BAD_CAST "false");

	xmlNewChild (node, NULL, BAD_CAST "directions",
	             BAD_CAST gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (test)));

	choices = gpinstruct_lesson_test_word_pool_get_choices (test);
	curr_choices = choices;

	while (curr_choices)
	{
		curr_choice = curr_choices->data;

		xmlNewChild (node, NULL, BAD_CAST "choice", BAD_CAST curr_choice);

		curr_choices = curr_choices->next;
	}

	g_list_free (choices);

	questions = gpinstruct_lesson_test_word_pool_get_questions (test);
	curr_questions = questions;

	while (curr_questions)
	{
		curr_question = GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION (curr_questions->data);

		question_node = xmlNewChild (node, NULL, BAD_CAST "question", NULL);
		temp = g_strdup_printf ("%d", gpinstruct_lesson_test_word_pool_question_get_answer (curr_question));
		xmlSetProp (question_node, BAD_CAST "answer", BAD_CAST temp);
		g_free (temp);

		xmlNewChild (question_node, NULL, BAD_CAST "text",
		             BAD_CAST gpinstruct_lesson_test_word_pool_question_get_text (curr_question));

		xmlNewChild (question_node, NULL, BAD_CAST "explanation",
		             BAD_CAST gpinstruct_lesson_test_word_pool_question_get_explanation (curr_question));

		curr_questions = curr_questions->next;
	}

	g_list_free (questions);

	return node;
}

static xmlNodePtr
add_order_test (GPInstructLessonTestOrder* test,
                xmlNodePtr parent_node)
{
	GPInstructLessonTestOrderItem* curr_item;

	GList *items, *curr_items;

	gchar *temp;

	xmlNodePtr item_node;

	xmlNodePtr node = xmlNewChild (parent_node, NULL,
	                               BAD_CAST "test-order", NULL);
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	xmlSetProp (node, BAD_CAST "id",
	            BAD_CAST gpinstruct_lesson_test_get_id (GPINSTRUCT_LESSON_TEST (test)));
	xmlSetProp (node, BAD_CAST "explain",
	            gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test))?BAD_CAST "true":BAD_CAST "false");

	xmlNewChild (node, NULL, BAD_CAST "directions",
	             BAD_CAST gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (test)));

	xmlNewChild (node, NULL, BAD_CAST "explanation",
	             BAD_CAST gpinstruct_lesson_test_order_get_explanation (test));

	items = gpinstruct_lesson_test_order_get_items (test);
	curr_items = items;

	while (curr_items)
	{
		curr_item = GPINSTRUCT_LESSON_TEST_ORDER_ITEM (curr_items->data);

		item_node = xmlNewChild (node, NULL, BAD_CAST "item",
		                         BAD_CAST gpinstruct_lesson_test_order_item_get_text (curr_item));
		temp = g_strdup_printf ("%d", gpinstruct_lesson_test_order_item_get_answer (curr_item));
		xmlSetProp (item_node, BAD_CAST "answer", BAD_CAST temp);
		g_free (temp);

		curr_items = curr_items->next;
	}

	g_list_free (items);

	return node;
}

static xmlNodePtr
add_lesson (GPInstructLesson* lesson,
            xmlNodePtr parent_node)
{
	GPInstructLessonElement* curr_lesson_element;

	GList *lesson_elements;
	GList *curr_lesson_elements;

	xmlNodePtr current_node = xmlNewChild (parent_node, NULL,
	                                       BAD_CAST "lesson", NULL);
	xmlSetProp (current_node, BAD_CAST "title",
	            BAD_CAST gpinstruct_lesson_get_title (lesson));
	xmlSetProp (current_node, BAD_CAST "single-score",
	            gpinstruct_lesson_get_single_score (lesson)?BAD_CAST "true":BAD_CAST "false");

	lesson_elements = gpinstruct_lesson_get_lesson_elements (lesson);
	curr_lesson_elements = lesson_elements;

	while (curr_lesson_elements)
	{
		curr_lesson_element = GPINSTRUCT_LESSON_ELEMENT (curr_lesson_elements->data);

		if (GPINSTRUCT_IS_LESSON_DISCUSSION (curr_lesson_element))
			add_discussion (GPINSTRUCT_LESSON_DISCUSSION (curr_lesson_element),
			                current_node);
		else if (GPINSTRUCT_IS_LESSON_READING (curr_lesson_element))
			add_reading (GPINSTRUCT_LESSON_READING (curr_lesson_element),
			             current_node);
		else if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element))
			add_multi_choice_test (GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (curr_lesson_element),
			                       current_node);
		else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (curr_lesson_element))
			add_word_pool_test (GPINSTRUCT_LESSON_TEST_WORD_POOL (curr_lesson_element),
			                    current_node);
		else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (curr_lesson_element))
			add_order_test (GPINSTRUCT_LESSON_TEST_ORDER (curr_lesson_element),
			                current_node);

		curr_lesson_elements = curr_lesson_elements->next;
	}

	g_list_free (lesson_elements);

	return current_node;
}

static xmlNodePtr
add_category (GPInstructCategory* category,
              xmlNodePtr parent_node)
{
	GPInstructLesson* curr_lesson;
	GList *lessons, *curr_lessons;

	xmlNodePtr node = xmlNewChild (parent_node, NULL,
	                               BAD_CAST "category", NULL);
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_category_get_title (category));

	lessons = gpinstruct_category_get_lessons (category);
	curr_lessons = lessons;

	while (curr_lessons)
	{
		curr_lesson = GPINSTRUCT_LESSON (curr_lessons->data);

		add_lesson (curr_lesson, node);

		curr_lessons = curr_lessons->next;
	}

	g_list_free (lessons);

	return node;
}

static xmlNodePtr
add_project (GPInstructProject* project)
{
	GPInstructCategory* curr_category;
	GList *categories, *curr_categories;

	xmlNodePtr node = xmlNewNode (NULL, BAD_CAST "project");
	xmlSetProp (node, BAD_CAST "title",
	            BAD_CAST gpinstruct_project_get_title (project));

	xmlSetNs (node, xmlNewNs (node,
	                          BAD_CAST PACKAGE_URL,
	                          BAD_CAST PACKAGE_TARNAME));

	categories = gpinstruct_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = GPINSTRUCT_CATEGORY (curr_categories->data);

		add_category (curr_category, node);

		curr_categories = curr_categories->next;
	}

	g_list_free (categories);

	return node;
}


G_DEFINE_TYPE (GPInstructParser, gpinstruct_parser, GPINSTRUCT_TYPE_OBJECT);


static void
gpinstruct_parser_init (GPInstructParser *object)
{
}

static void
gpinstruct_parser_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_parser_parent_class)->finalize (object);

	xmlCleanupParser ();
}

static void
gpinstruct_parser_class_init (GPInstructParserClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass* parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	object_class->finalize = gpinstruct_parser_finalize;

	xmlInitParser ();
}


GPInstructParser*
gpinstruct_parser_new (void)
{
	GPInstructParser* parser = g_object_new(GPINSTRUCT_TYPE_PARSER, NULL);
	return parser;
}

GPInstructProject*
gpinstruct_parser_open (GPInstructParser* parser,
                        const gchar* file,
                        GError** error)
{
	GPInstructProject* curr_project = NULL;

	xmlDocPtr doc = xmlParseFile (file);
	xmlNode *current_node;

	if (doc)
	{
		current_node = xmlDocGetRootElement (doc);

		if (current_node &&
		    current_node->name &&
		    xmlStrEqual (current_node->name, BAD_CAST "project"))
		{
			curr_project = parse_project (current_node);
		}
		else
		{
			g_set_error (error, GPINSTRUCT_PARSER_ERROR, GPINSTRUCT_PARSER_ERROR_PARSE,
			             _("Failed to parse file."));
			return NULL;
		}

		xmlFreeDoc (doc);
	}
	else
	{
		g_set_error (error, GPINSTRUCT_PARSER_ERROR, GPINSTRUCT_PARSER_ERROR_PARSE,
		             _("Failed to parse file."));
		return NULL;
	}

	return curr_project;
}

void
gpinstruct_parser_save (GPInstructParser* parser,
                        GPInstructProject* project,
                        const gchar* file,
                        GError** error)
{
	xmlNodePtr current_node = add_project (project);

	xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
	xmlSetDocCompressMode (doc, 9);
	xmlDocSetRootElement (doc, current_node);

	xmlIndentTreeOutput = 1;
	xmlSaveFormatFileEnc(file, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
}
