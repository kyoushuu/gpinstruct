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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "canvas.h"

#define GCHAR_TO_GBOOLEAN(a) (g_ascii_strncasecmp(a, "true", -1) == 0)?TRUE:FALSE


GQuark
canvas_parser_error_quark (void)
{
  return g_quark_from_static_string ("canvas-parser-error-quark");
}


G_DEFINE_TYPE (CanvasParser, canvas_parser, CANVAS_TYPE_OBJECT);

static void
canvas_parser_init (CanvasParser *object)
{
}

static void
canvas_parser_finalize (GObject *object)
{

	G_OBJECT_CLASS (canvas_parser_parent_class)->finalize (object);

	xmlCleanupParser ();
}

static void
canvas_parser_class_init (CanvasParserClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	object_class->finalize = canvas_parser_finalize;

	xmlInitParser ();
}


CanvasParser*
canvas_parser_new (void)
{
	CanvasParser* parser = g_object_new(CANVAS_TYPE_PARSER, NULL);
	return parser;
}

CanvasProject*
canvas_parser_open (CanvasParser* parser,
                    const gchar* file,
                    GError** error)
{
	CanvasProject* curr_project = NULL;

	xmlNode *current_node, *parent_node;
	xmlChar *temp;

	xmlDocPtr doc = xmlParseFile (file);

	if (doc)
	{
		current_node = xmlDocGetRootElement (doc);

		if (current_node &&
		    current_node->name &&
		    xmlStrEqual (current_node->name, "canvas-project"))
		{
			CanvasProject* project = canvas_project_new ();

			temp = xmlGetProp (current_node, "title");
			if (temp)
			{
				canvas_project_set_title (project, temp);
				xmlFree (temp);
			}

			for (parent_node = current_node,
			     current_node = current_node->children;
			     current_node != NULL;
			     current_node = current_node->next)
			{
				if (current_node->type == XML_ELEMENT_NODE)
				{
					if (xmlStrEqual (current_node->name, "category"))
					{
						CanvasCategory* category = canvas_category_new ();
						canvas_project_add_category (project, category);

						temp = xmlGetProp (current_node, "title");
						if (temp)
						{
							canvas_category_set_title (category, temp);
							xmlFree (temp);
						}

						for (parent_node = current_node,
						     current_node = current_node->children;
							 current_node != NULL;
							 current_node = current_node->next)
						{
							if (current_node->type == XML_ELEMENT_NODE)
							{
								if (xmlStrEqual (current_node->name, "lesson"))
								{
									CanvasLesson* lesson = canvas_lesson_new ();
									canvas_category_add_lesson (category, lesson);

									temp = xmlGetProp (current_node, "title");
									if (temp)
									{
										canvas_lesson_set_title (lesson, temp);
										xmlFree (temp);
									}

									temp = xmlGetProp (current_node, "single-score");
									if (temp)
									{
										canvas_lesson_set_single_score (lesson, GCHAR_TO_GBOOLEAN (temp));
										xmlFree (temp);
									}

									for (parent_node = current_node,
										 current_node = current_node->children;
										 current_node != NULL;
										 current_node = current_node->next)
									{
										if (current_node->type == XML_ELEMENT_NODE)
										{
											if (xmlStrEqual (current_node->name, "test-multi-choice"))
											{
												CanvasLessonTestMultiChoice* test = canvas_lesson_test_multi_choice_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (test), temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, "explain");
												if (temp)
												{
													canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (test), GCHAR_TO_GBOOLEAN (temp));
													xmlFree (temp);
												}

												for (parent_node = current_node,
													 current_node = current_node->children;
													 current_node != NULL;
													 current_node = current_node->next)
												{
													if (current_node->type == XML_ELEMENT_NODE)
													{
														if (xmlStrEqual (current_node->name, "directions"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (test), temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, "question"))
														{
															CanvasLessonTestMultiChoiceQuestion* question = canvas_lesson_test_multi_choice_question_new ();
															canvas_lesson_test_multi_choice_add_question (test, question);

															temp = xmlGetProp (current_node, "answer");
															if (temp)
															{
																canvas_lesson_test_multi_choice_question_set_answer (question, atoi(temp));
																xmlFree (temp);
															}

															for (parent_node = current_node,
																 current_node = current_node->children;
																 current_node != NULL;
																 current_node = current_node->next)
															{
																if (current_node->type == XML_ELEMENT_NODE)
																{
																	if (xmlStrEqual (current_node->name, "text"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_multi_choice_question_set_text (question, temp);
																			xmlFree (temp);
																		}
																	}
																	else if (xmlStrEqual (current_node->name, "explanation"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_multi_choice_question_set_explanation (question, temp);
																			xmlFree (temp);
																		}
																	}
																	else if (xmlStrEqual (current_node->name, "choice"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_multi_choice_question_add_choice (question, temp);
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

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, "test-word-pool"))
											{
												CanvasLessonTestWordPool* test = canvas_lesson_test_word_pool_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (test), temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, "explain");
												if (temp)
												{
													canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (test), GCHAR_TO_GBOOLEAN (temp));
													xmlFree (temp);
												}

												for (parent_node = current_node,
													 current_node = current_node->children;
													 current_node != NULL;
													 current_node = current_node->next)
												{
													if (current_node->type == XML_ELEMENT_NODE)
													{
														if (xmlStrEqual (current_node->name, "directions"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (test), temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, "choice"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_word_pool_add_choice (test, temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, "question"))
														{
															CanvasLessonTestWordPoolQuestion* question = canvas_lesson_test_word_pool_question_new ();
															canvas_lesson_test_word_pool_add_question (test, question);

															temp = xmlGetProp (current_node, "answer");
															if (temp)
															{
																canvas_lesson_test_word_pool_question_set_answer (question, atoi(temp));
																xmlFree (temp);
															}

															for (parent_node = current_node,
																 current_node = current_node->children;
																 current_node != NULL;
																 current_node = current_node->next)
															{
																if (current_node->type == XML_ELEMENT_NODE)
																{
																	if (xmlStrEqual (current_node->name, "text"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_word_pool_question_set_text (question, temp);
																			xmlFree (temp);
																		}
																	}
																	else if (xmlStrEqual (current_node->name, "explanation"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_word_pool_question_set_explanation (question, temp);
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

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, "test-order"))
											{
												CanvasLessonTestOrder* test = canvas_lesson_test_order_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (test), temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, "explain");
												if (temp)
												{
													canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (test), GCHAR_TO_GBOOLEAN (temp));
													xmlFree (temp);
												}

												for (parent_node = current_node,
													 current_node = current_node->children;
													 current_node != NULL;
													 current_node = current_node->next)
												{
													if (current_node->type == XML_ELEMENT_NODE)
													{
														if (xmlStrEqual (current_node->name, "directions"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (test), temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, "item"))
														{
															CanvasLessonTestOrderItem* item = canvas_lesson_test_order_item_new ();
															canvas_lesson_test_order_add_item (test, item);

															temp = xmlGetProp (current_node, "answer");
															if (temp)
															{
																canvas_lesson_test_order_item_set_answer (item, atoi (temp));
																xmlFree (temp);
															}

															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_order_item_set_text (item, temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, "explanation"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_order_set_explanation (test, temp);
																xmlFree (temp);
															}
														}
													}
												}

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, "discussion"))
											{
												CanvasLessonDiscussion* discussion = canvas_lesson_discussion_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (discussion));

												temp = xmlGetProp (current_node, "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (discussion), temp);
													xmlFree (temp);
												}

												temp = xmlNodeGetContent (current_node);
												if (temp)
												{
													canvas_lesson_discussion_set_text (discussion, temp);
													xmlFree (temp);
												}
											}
											else if (xmlStrEqual (current_node->name, "reading"))
											{
												CanvasLessonReading* reading = canvas_lesson_reading_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (reading));

												temp = xmlGetProp (current_node, "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (reading), temp);
													xmlFree (temp);
												}

												temp = xmlNodeGetContent (current_node);
												if (temp)
												{
													canvas_lesson_reading_set_text (reading, temp);
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

						current_node = parent_node;
						parent_node = current_node->parent;
					}
				}
			}

			current_node = parent_node;
			parent_node = current_node->parent;

			curr_project = project;
		}
		else
		{
			g_set_error (error, CANVAS_PARSER_ERROR, CANVAS_PARSER_ERROR_PARSE,
				         _("Failed to parse file."));
			return NULL;
		}

		xmlFreeDoc (doc);
	}
	else
	{
		g_set_error (error, CANVAS_PARSER_ERROR, CANVAS_PARSER_ERROR_PARSE,
		             _("Failed to parse file."));
		return NULL;
	}

	return curr_project;
}

void
canvas_parser_save (CanvasParser* parser,
                    CanvasProject* project,
                    const gchar* file,
                    GError** error)
{
	CanvasCategory*							curr_category;
	CanvasLesson*							curr_lesson;
	CanvasLessonElement*					curr_lesson_element;
	CanvasLessonDiscussion*					curr_lesson_discussion;
	CanvasLessonReading*					curr_lesson_reading;
	CanvasLessonTestMultiChoice*			curr_lesson_test_multi_choice;
	CanvasLessonTestWordPool*				curr_lesson_test_word_pool;
	CanvasLessonTestOrder*					curr_lesson_test_order;
	CanvasLessonTestMultiChoiceQuestion*	curr_lesson_test_multi_choice_question;
	CanvasLessonTestWordPoolQuestion*		curr_lesson_test_word_pool_question;
	CanvasLessonTestOrderItem*				curr_lesson_test_order_item;
	gchar*									curr_choice;

	GList *categories, *lessons, *lesson_elements, *questions, *choices, *items;
	GList *curr_categories, *curr_lessons, *curr_lesson_elements, *curr_questions, *curr_choices, *curr_items;
	gchar *temp;

	GError* error_local = NULL;

	xmlNodePtr current_node;

	current_node = xmlNewNode (NULL, "canvas-project");
	xmlSetProp (current_node, "title", canvas_project_get_title (project));

	xmlDocPtr doc = xmlNewDoc ("1.0");
	xmlDocSetRootElement (doc, current_node);

	categories = canvas_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = CANVAS_CATEGORY (curr_categories->data);

		current_node = xmlNewChild (current_node, NULL, "category", NULL);
		xmlSetProp (current_node, "title", canvas_category_get_title (curr_category));

		lessons = canvas_category_get_lessons (curr_category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			curr_lesson = CANVAS_LESSON (curr_lessons->data);

			current_node = xmlNewChild (current_node, NULL, "lesson", NULL);
			xmlSetProp (current_node, "title", canvas_lesson_get_title (curr_lesson));
			xmlSetProp (current_node, "single-score", canvas_lesson_get_single_score (curr_lesson)?"true":"false");

			lesson_elements = canvas_lesson_get_lesson_elements (curr_lesson);
			curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				curr_lesson_element = CANVAS_LESSON_ELEMENT (curr_lesson_elements->data);

				if (CANVAS_IS_LESSON_DISCUSSION (curr_lesson_element))
				{
					curr_lesson_discussion = CANVAS_LESSON_DISCUSSION (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, "discussion",
					                            canvas_lesson_discussion_get_text (curr_lesson_discussion));
					xmlSetProp (current_node, "title", canvas_lesson_element_get_title (curr_lesson_element));

					/* Currently in a "discussion" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}
				else if (CANVAS_IS_LESSON_READING (curr_lesson_element))
				{
					curr_lesson_reading = CANVAS_LESSON_READING (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, "reading",
					                            canvas_lesson_reading_get_text (curr_lesson_reading));
					xmlSetProp (current_node, "title", canvas_lesson_element_get_title (curr_lesson_element));

					/* Currently in a "reading" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}
				else if (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element))
				{
					curr_lesson_test_multi_choice = CANVAS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, "test-multi-choice", NULL);
					xmlSetProp (current_node, "title", canvas_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, "explain", (canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)))?"true":"false");

					current_node = xmlNewChild (current_node, NULL, "directions",
					                            canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));

					/* Currently in a "directions" element, go back to "test-multi-choice" element */
					current_node = current_node->parent;

					questions = canvas_lesson_test_multi_choice_get_questions (curr_lesson_test_multi_choice);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_multi_choice_question = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION (curr_questions->data);

						current_node = xmlNewChild (current_node, NULL, "question", NULL);
						temp = g_strdup_printf ("%d", canvas_lesson_test_multi_choice_question_get_answer (curr_lesson_test_multi_choice_question));
						xmlSetProp (current_node, "answer", temp);
						g_free (temp);

						current_node = xmlNewChild (current_node, NULL, "text",
						                            canvas_lesson_test_multi_choice_question_get_text (curr_lesson_test_multi_choice_question));

						/* Currently in a "text" element, go back to "question" element */
						current_node = current_node->parent;

						current_node = xmlNewChild (current_node, NULL, "explanation",
						                            canvas_lesson_test_multi_choice_question_get_explanation (curr_lesson_test_multi_choice_question));

						/* Currently in a "explanation" element, go back to "question" element */
						current_node = current_node->parent;

						choices = canvas_lesson_test_multi_choice_question_get_choices (curr_lesson_test_multi_choice_question);
						curr_choices = choices;

						while (curr_choices)
						{
							curr_choice = curr_choices->data;

							current_node = xmlNewChild (current_node, NULL, "choice", curr_choice);

							/* Currently in a "choice" element, go back to "question" element */
							current_node = current_node->parent;

							curr_choices = curr_choices->next;
						}

						g_list_free (curr_choices);

						/* Currently in a "question" element, go back to "test-multi-choice" element */
						current_node = current_node->parent;

						curr_questions = curr_questions->next;
					}

					/* Currently in a "test-multi-choice" type element, go back to "lesson" element */
					current_node = current_node->parent;

					g_list_free (questions);
				}
				else if (CANVAS_IS_LESSON_TEST_WORD_POOL (curr_lesson_element))
				{
					curr_lesson_test_word_pool = CANVAS_LESSON_TEST_WORD_POOL (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, "test-word-pool", NULL);
					xmlSetProp (current_node, "title", canvas_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, "explain", (canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (curr_lesson_test_word_pool)))?"true":"false");

					current_node = xmlNewChild (current_node, NULL, "directions",
					                            canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));

					/* Currently in a "directions" element, go back to "test-word-pool" element */
					current_node = current_node->parent;

					choices = canvas_lesson_test_word_pool_get_choices (curr_lesson_test_word_pool);
					curr_choices = choices;

					while (curr_choices)
					{
						curr_choice = curr_choices->data;

						current_node = xmlNewChild (current_node, NULL, "choice", curr_choice);

						/* Currently in a "choice" element, go back to "test-word-pool" element */
						current_node = current_node->parent;

						curr_choices = curr_choices->next;
					}

					g_list_free (curr_choices);

					questions = canvas_lesson_test_word_pool_get_questions (curr_lesson_test_word_pool);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_word_pool_question = CANVAS_LESSON_TEST_WORD_POOL_QUESTION (curr_questions->data);

						current_node = xmlNewChild (current_node, NULL, "question", NULL);
						temp = g_strdup_printf ("%d", canvas_lesson_test_word_pool_question_get_answer (curr_lesson_test_word_pool_question));
						xmlSetProp (current_node, "answer", temp);
						g_free (temp);

						current_node = xmlNewChild (current_node, NULL, "text",
						                            canvas_lesson_test_word_pool_question_get_text (curr_lesson_test_word_pool_question));

						/* Currently in a "text" element, go back to "question" element */
						current_node = current_node->parent;

						current_node = xmlNewChild (current_node, NULL, "explanation",
						                            canvas_lesson_test_word_pool_question_get_explanation (curr_lesson_test_word_pool_question));

						/* Currently in a "explanation" element, go back to "question" element */
						current_node = current_node->parent;

						/* Currently in a "question" element, go back to "test-word-pool" element */
						current_node = current_node->parent;

						curr_questions = curr_questions->next;
					}

					/* Currently in a "test-word-pool" type element, go back to "lesson" element */
					current_node = current_node->parent;

					g_list_free (curr_questions);
				}
				else if (CANVAS_IS_LESSON_TEST_ORDER (curr_lesson_element))
				{
					curr_lesson_test_order = CANVAS_LESSON_TEST_ORDER (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, "test-order", NULL);
					xmlSetProp (current_node, "title", canvas_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, "explain", (canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (curr_lesson_test_order)))?"true":"false");

					current_node = xmlNewChild (current_node, NULL, "directions",
					                            canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));

					/* Currently in a "directions" element, go back to "test-order" element */
					current_node = current_node->parent;

					current_node = xmlNewChild (current_node, NULL, "explanation",
					                            canvas_lesson_test_order_get_explanation (curr_lesson_test_order));

					/* Currently in a "explanation" element, go back to "test-order" element */
					current_node = current_node->parent;

					items = canvas_lesson_test_order_get_items (curr_lesson_test_order);
					curr_items = items;

					while (curr_items)
					{
						curr_lesson_test_order_item = CANVAS_LESSON_TEST_ORDER_ITEM (curr_items->data);

						current_node = xmlNewChild (current_node, NULL, "item", canvas_lesson_test_order_item_get_text (curr_lesson_test_order_item));
						temp = g_strdup_printf ("%d", canvas_lesson_test_order_item_get_answer (curr_lesson_test_order_item));
						xmlSetProp (current_node, "answer", temp);
						g_free (temp);

						/* Currently in a "item" element, go back to "test-order" element */
						current_node = current_node->parent;

						curr_items = curr_items->next;
					}

					g_list_free (curr_items);

					/* Currently in a "test-order" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}

				curr_lesson_elements = curr_lesson_elements->next;
			}

			g_list_free (lesson_elements);

			/* Currently in a "lesson" element, go back to "category" element */
			current_node = current_node->parent;

			curr_lessons = curr_lessons->next;
		}

		g_list_free (lessons);

		/* Currently in a "category" element, go back to "project" element */
		current_node = current_node->parent;

		curr_categories = curr_categories->next;
	}
	
	g_list_free (categories);

	xmlIndentTreeOutput = 1;
	xmlSaveFormatFileEnc(file, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
}
