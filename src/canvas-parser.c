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
		    xmlStrEqual (current_node->name, BAD_CAST "project"))
		{
			CanvasProject* project = canvas_project_new ();

			temp = xmlGetProp (current_node, BAD_CAST "title");
			if (temp)
			{
				canvas_project_set_title (project, (gchar*)temp);
				xmlFree (temp);
			}

			for (parent_node = current_node,
			     current_node = current_node->children;
			     current_node != NULL;
			     current_node = current_node->next)
			{
				if (current_node->type == XML_ELEMENT_NODE)
				{
					if (xmlStrEqual (current_node->name, BAD_CAST "category"))
					{
						CanvasCategory* category = canvas_category_new ();
						canvas_project_add_category (project, category);

						temp = xmlGetProp (current_node, BAD_CAST "title");
						if (temp)
						{
							canvas_category_set_title (category, (gchar*)temp);
							xmlFree (temp);
						}

						for (parent_node = current_node,
						     current_node = current_node->children;
							 current_node != NULL;
							 current_node = current_node->next)
						{
							if (current_node->type == XML_ELEMENT_NODE)
							{
								if (xmlStrEqual (current_node->name, BAD_CAST "lesson"))
								{
									CanvasLesson* lesson = canvas_lesson_new ();
									canvas_category_add_lesson (category, lesson);

									temp = xmlGetProp (current_node, BAD_CAST "title");
									if (temp)
									{
										canvas_lesson_set_title (lesson, (gchar*)temp);
										xmlFree (temp);
									}

									temp = xmlGetProp (current_node, BAD_CAST "single-score");
									if (temp)
									{
										canvas_lesson_set_single_score (lesson, GCHAR_TO_GBOOLEAN ((gchar*)temp));
										xmlFree (temp);
									}

									for (parent_node = current_node,
										 current_node = current_node->children;
										 current_node != NULL;
										 current_node = current_node->next)
									{
										if (current_node->type == XML_ELEMENT_NODE)
										{
											if (xmlStrEqual (current_node->name, BAD_CAST "test-multi-choice"))
											{
												CanvasLessonTestMultiChoice* test = canvas_lesson_test_multi_choice_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "id");
												if (temp)
												{
													canvas_lesson_test_set_id (CANVAS_LESSON_TEST (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "explain");
												if (temp)
												{
													canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
													xmlFree (temp);
												}

												for (parent_node = current_node,
													 current_node = current_node->children;
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
																canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (test), (gchar*)temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, BAD_CAST "question"))
														{
															CanvasLessonTestMultiChoiceQuestion* question = canvas_lesson_test_multi_choice_question_new ();
															canvas_lesson_test_multi_choice_add_question (test, question);

															temp = xmlGetProp (current_node, BAD_CAST "answer");
															if (temp)
															{
																canvas_lesson_test_multi_choice_question_set_answer (question, atoi((gchar*)temp));
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
																			canvas_lesson_test_multi_choice_question_set_text (question, (gchar*)temp);
																			xmlFree (temp);
																		}
																	}
																	else if (xmlStrEqual (current_node->name, BAD_CAST "explanation"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_multi_choice_question_set_explanation (question, (gchar*)temp);
																			xmlFree (temp);
																		}
																	}
																	else if (xmlStrEqual (current_node->name, BAD_CAST "choice"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_multi_choice_question_add_choice (question, (gchar*)temp);
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
											else if (xmlStrEqual (current_node->name, BAD_CAST "test-word-pool"))
											{
												CanvasLessonTestWordPool* test = canvas_lesson_test_word_pool_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "id");
												if (temp)
												{
													canvas_lesson_test_set_id (CANVAS_LESSON_TEST (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "explain");
												if (temp)
												{
													canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
													xmlFree (temp);
												}

												for (parent_node = current_node,
													 current_node = current_node->children;
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
																canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (test), (gchar*)temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, BAD_CAST "choice"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_word_pool_add_choice (test, (gchar*)temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, BAD_CAST "question"))
														{
															CanvasLessonTestWordPoolQuestion* question = canvas_lesson_test_word_pool_question_new ();
															canvas_lesson_test_word_pool_add_question (test, question);

															temp = xmlGetProp (current_node, BAD_CAST "answer");
															if (temp)
															{
																canvas_lesson_test_word_pool_question_set_answer (question, atoi((gchar*)temp));
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
																			canvas_lesson_test_word_pool_question_set_text (question, (gchar*)temp);
																			xmlFree (temp);
																		}
																	}
																	else if (xmlStrEqual (current_node->name, BAD_CAST "explanation"))
																	{
																		temp = xmlNodeGetContent (current_node);
																		if (temp)
																		{
																			canvas_lesson_test_word_pool_question_set_explanation (question, (gchar*)temp);
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
											else if (xmlStrEqual (current_node->name, BAD_CAST "test-order"))
											{
												CanvasLessonTestOrder* test = canvas_lesson_test_order_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "id");
												if (temp)
												{
													canvas_lesson_test_set_id (CANVAS_LESSON_TEST (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "explain");
												if (temp)
												{
													canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
													xmlFree (temp);
												}

												for (parent_node = current_node,
													 current_node = current_node->children;
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
																canvas_lesson_test_set_directions (CANVAS_LESSON_TEST (test), (gchar*)temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, BAD_CAST "item"))
														{
															CanvasLessonTestOrderItem* item = canvas_lesson_test_order_item_new ();
															canvas_lesson_test_order_add_item (test, item);

															temp = xmlGetProp (current_node, BAD_CAST "answer");
															if (temp)
															{
																canvas_lesson_test_order_item_set_answer (item, atoi ((gchar*)temp));
																xmlFree (temp);
															}

															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_order_item_set_text (item, (gchar*)temp);
																xmlFree (temp);
															}
														}
														else if (xmlStrEqual (current_node->name, BAD_CAST "explanation"))
														{
															temp = xmlNodeGetContent (current_node);
															if (temp)
															{
																canvas_lesson_test_order_set_explanation (test, (gchar*)temp);
																xmlFree (temp);
															}
														}
													}
												}

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, BAD_CAST "discussion"))
											{
												CanvasLessonDiscussion* discussion = canvas_lesson_discussion_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (discussion));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (discussion), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlNodeGetContent (current_node);
												if (temp)
												{
													canvas_lesson_discussion_set_text (discussion, (gchar*)temp);
													xmlFree (temp);
												}
											}
											else if (xmlStrEqual (current_node->name, BAD_CAST "reading"))
											{
												CanvasLessonReading* reading = canvas_lesson_reading_new ();
												canvas_lesson_add_lesson_element (lesson, CANVAS_LESSON_ELEMENT (reading));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (reading), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlNodeGetContent (current_node);
												if (temp)
												{
													canvas_lesson_reading_set_text (reading, (gchar*)temp);
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

	xmlNodePtr current_node;

	current_node = xmlNewNode (NULL, BAD_CAST "project");
	xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_project_get_title (project));

	xmlSetNs (current_node, xmlNewNs (current_node,
	                                  BAD_CAST "http://kyoushuu.users.sourceforge.net/project_canvas",
	                                  BAD_CAST "canvas"));

	xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
	xmlDocSetRootElement (doc, current_node);

	categories = canvas_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = CANVAS_CATEGORY (curr_categories->data);

		current_node = xmlNewChild (current_node, NULL, BAD_CAST "category", NULL);
		xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_category_get_title (curr_category));

		lessons = canvas_category_get_lessons (curr_category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			curr_lesson = CANVAS_LESSON (curr_lessons->data);

			current_node = xmlNewChild (current_node, NULL, BAD_CAST "lesson", NULL);
			xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_lesson_get_title (curr_lesson));
			xmlSetProp (current_node, BAD_CAST "single-score", canvas_lesson_get_single_score (curr_lesson)?BAD_CAST "true":BAD_CAST "false");

			lesson_elements = canvas_lesson_get_lesson_elements (curr_lesson);
			curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				curr_lesson_element = CANVAS_LESSON_ELEMENT (curr_lesson_elements->data);

				if (CANVAS_IS_LESSON_DISCUSSION (curr_lesson_element))
				{
					curr_lesson_discussion = CANVAS_LESSON_DISCUSSION (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "discussion",
					                            BAD_CAST canvas_lesson_discussion_get_text (curr_lesson_discussion));
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_lesson_element_get_title (curr_lesson_element));

					/* Currently in a "discussion" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}
				else if (CANVAS_IS_LESSON_READING (curr_lesson_element))
				{
					curr_lesson_reading = CANVAS_LESSON_READING (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "reading",
					                            BAD_CAST canvas_lesson_reading_get_text (curr_lesson_reading));
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_lesson_element_get_title (curr_lesson_element));

					/* Currently in a "reading" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}
				else if (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element))
				{
					curr_lesson_test_multi_choice = CANVAS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "test-multi-choice", NULL);
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, BAD_CAST "id", BAD_CAST canvas_lesson_test_get_id (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));
					xmlSetProp (current_node, BAD_CAST "explain", canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice))?BAD_CAST "true":BAD_CAST "false");

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "directions",
					                            BAD_CAST canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));

					/* Currently in a "directions" element, go back to "test-multi-choice" element */
					current_node = current_node->parent;

					questions = canvas_lesson_test_multi_choice_get_questions (curr_lesson_test_multi_choice);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_multi_choice_question = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION (curr_questions->data);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "question", NULL);
						temp = g_strdup_printf ("%d", canvas_lesson_test_multi_choice_question_get_answer (curr_lesson_test_multi_choice_question));
						xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST temp);
						g_free (temp);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "text",
						                            BAD_CAST canvas_lesson_test_multi_choice_question_get_text (curr_lesson_test_multi_choice_question));

						/* Currently in a "text" element, go back to "question" element */
						current_node = current_node->parent;

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "explanation",
						                            BAD_CAST canvas_lesson_test_multi_choice_question_get_explanation (curr_lesson_test_multi_choice_question));

						/* Currently in a "explanation" element, go back to "question" element */
						current_node = current_node->parent;

						choices = canvas_lesson_test_multi_choice_question_get_choices (curr_lesson_test_multi_choice_question);
						curr_choices = choices;

						while (curr_choices)
						{
							curr_choice = curr_choices->data;

							current_node = xmlNewChild (current_node, NULL, BAD_CAST "choice", BAD_CAST curr_choice);

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

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "test-word-pool", NULL);
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, BAD_CAST "id", BAD_CAST canvas_lesson_test_get_id (CANVAS_LESSON_TEST (curr_lesson_test_word_pool)));
					xmlSetProp (current_node, BAD_CAST "explain", canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (curr_lesson_test_word_pool))?BAD_CAST "true":BAD_CAST "false");

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "directions",
					                            BAD_CAST canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_word_pool)));

					/* Currently in a "directions" element, go back to "test-word-pool" element */
					current_node = current_node->parent;

					choices = canvas_lesson_test_word_pool_get_choices (curr_lesson_test_word_pool);
					curr_choices = choices;

					while (curr_choices)
					{
						curr_choice = curr_choices->data;

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "choice", BAD_CAST curr_choice);

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

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "question", NULL);
						temp = g_strdup_printf ("%d", canvas_lesson_test_word_pool_question_get_answer (curr_lesson_test_word_pool_question));
						xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST temp);
						g_free (temp);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "text",
						                            BAD_CAST canvas_lesson_test_word_pool_question_get_text (curr_lesson_test_word_pool_question));

						/* Currently in a "text" element, go back to "question" element */
						current_node = current_node->parent;

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "explanation",
						                            BAD_CAST canvas_lesson_test_word_pool_question_get_explanation (curr_lesson_test_word_pool_question));

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

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "test-order", NULL);
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST canvas_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, BAD_CAST "id", BAD_CAST canvas_lesson_test_get_id (CANVAS_LESSON_TEST (curr_lesson_test_order)));
					xmlSetProp (current_node, BAD_CAST "explain", canvas_lesson_test_get_explain (CANVAS_LESSON_TEST (curr_lesson_test_order))?BAD_CAST "true":BAD_CAST "false");

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "directions",
					                            BAD_CAST canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_order)));

					/* Currently in a "directions" element, go back to "test-order" element */
					current_node = current_node->parent;

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "explanation",
					                            BAD_CAST canvas_lesson_test_order_get_explanation (curr_lesson_test_order));

					/* Currently in a "explanation" element, go back to "test-order" element */
					current_node = current_node->parent;

					items = canvas_lesson_test_order_get_items (curr_lesson_test_order);
					curr_items = items;

					while (curr_items)
					{
						curr_lesson_test_order_item = CANVAS_LESSON_TEST_ORDER_ITEM (curr_items->data);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "item", BAD_CAST canvas_lesson_test_order_item_get_text (curr_lesson_test_order_item));
						temp = g_strdup_printf ("%d", canvas_lesson_test_order_item_get_answer (curr_lesson_test_order_item));
						xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST temp);
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
