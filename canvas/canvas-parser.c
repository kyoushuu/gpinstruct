/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja
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
			GPInstructProject* project = gpinstruct_project_new ();

			temp = xmlGetProp (current_node, BAD_CAST "title");
			if (temp)
			{
				gpinstruct_project_set_title (project, (gchar*)temp);
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
						GPInstructCategory* category = gpinstruct_category_new ();
						gpinstruct_project_add_category (project, category);

						temp = xmlGetProp (current_node, BAD_CAST "title");
						if (temp)
						{
							gpinstruct_category_set_title (category, (gchar*)temp);
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
									GPInstructLesson* lesson = gpinstruct_lesson_new ();
									gpinstruct_category_add_lesson (category, lesson);

									temp = xmlGetProp (current_node, BAD_CAST "title");
									if (temp)
									{
										gpinstruct_lesson_set_title (lesson, (gchar*)temp);
										xmlFree (temp);
									}

									temp = xmlGetProp (current_node, BAD_CAST "single-score");
									if (temp)
									{
										gpinstruct_lesson_set_single_score (lesson, GCHAR_TO_GBOOLEAN ((gchar*)temp));
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
												GPInstructLessonTestMultiChoice* test = gpinstruct_lesson_test_multi_choice_new ();
												gpinstruct_lesson_add_lesson_element (lesson, GPINSTRUCT_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "id");
												if (temp)
												{
													gpinstruct_lesson_test_set_id (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "explain");
												if (temp)
												{
													gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
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

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, BAD_CAST "test-word-pool"))
											{
												GPInstructLessonTestWordPool* test = gpinstruct_lesson_test_word_pool_new ();
												gpinstruct_lesson_add_lesson_element (lesson, GPINSTRUCT_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "id");
												if (temp)
												{
													gpinstruct_lesson_test_set_id (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "explain");
												if (temp)
												{
													gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
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

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, BAD_CAST "test-order"))
											{
												GPInstructLessonTestOrder* test = gpinstruct_lesson_test_order_new ();
												gpinstruct_lesson_add_lesson_element (lesson, GPINSTRUCT_LESSON_ELEMENT (test));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "id");
												if (temp)
												{
													gpinstruct_lesson_test_set_id (GPINSTRUCT_LESSON_TEST (test), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlGetProp (current_node, BAD_CAST "explain");
												if (temp)
												{
													gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (test), GCHAR_TO_GBOOLEAN ((gchar*)temp));
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

												current_node = parent_node;
												parent_node = current_node->parent;
											}
											else if (xmlStrEqual (current_node->name, BAD_CAST "discussion"))
											{
												GPInstructLessonDiscussion* discussion = gpinstruct_lesson_discussion_new ();
												gpinstruct_lesson_add_lesson_element (lesson, GPINSTRUCT_LESSON_ELEMENT (discussion));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (discussion), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlNodeGetContent (current_node);
												if (temp)
												{
													gpinstruct_lesson_discussion_set_text (discussion, (gchar*)temp);
													xmlFree (temp);
												}
											}
											else if (xmlStrEqual (current_node->name, BAD_CAST "reading"))
											{
												GPInstructLessonReading* reading = gpinstruct_lesson_reading_new ();
												gpinstruct_lesson_add_lesson_element (lesson, GPINSTRUCT_LESSON_ELEMENT (reading));

												temp = xmlGetProp (current_node, BAD_CAST "title");
												if (temp)
												{
													gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (reading), (gchar*)temp);
													xmlFree (temp);
												}

												temp = xmlNodeGetContent (current_node);
												if (temp)
												{
													gpinstruct_lesson_reading_set_text (reading, (gchar*)temp);
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
	GPInstructCategory*							curr_category;
	GPInstructLesson*							curr_lesson;
	GPInstructLessonElement*					curr_lesson_element;
	GPInstructLessonDiscussion*					curr_lesson_discussion;
	GPInstructLessonReading*					curr_lesson_reading;
	GPInstructLessonTestMultiChoice*			curr_lesson_test_multi_choice;
	GPInstructLessonTestWordPool*				curr_lesson_test_word_pool;
	GPInstructLessonTestOrder*					curr_lesson_test_order;
	GPInstructLessonTestMultiChoiceQuestion*	curr_lesson_test_multi_choice_question;
	GPInstructLessonTestWordPoolQuestion*		curr_lesson_test_word_pool_question;
	GPInstructLessonTestOrderItem*				curr_lesson_test_order_item;
	gchar*									curr_choice;

	GList *categories, *lessons, *lesson_elements, *questions, *choices, *items;
	GList *curr_categories, *curr_lessons, *curr_lesson_elements, *curr_questions, *curr_choices, *curr_items;
	gchar *temp;

	xmlNodePtr current_node;

	current_node = xmlNewNode (NULL, BAD_CAST "project");
	xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_project_get_title (project));

	xmlSetNs (current_node, xmlNewNs (current_node,
	                                  BAD_CAST "http://gpinstruct.sourceforge.net",
	                                  BAD_CAST "gpinstruct"));

	xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
	xmlDocSetRootElement (doc, current_node);

	categories = gpinstruct_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = GPINSTRUCT_CATEGORY (curr_categories->data);

		current_node = xmlNewChild (current_node, NULL, BAD_CAST "category", NULL);
		xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_category_get_title (curr_category));

		lessons = gpinstruct_category_get_lessons (curr_category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			curr_lesson = GPINSTRUCT_LESSON (curr_lessons->data);

			current_node = xmlNewChild (current_node, NULL, BAD_CAST "lesson", NULL);
			xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_lesson_get_title (curr_lesson));
			xmlSetProp (current_node, BAD_CAST "single-score", gpinstruct_lesson_get_single_score (curr_lesson)?BAD_CAST "true":BAD_CAST "false");

			lesson_elements = gpinstruct_lesson_get_lesson_elements (curr_lesson);
			curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				curr_lesson_element = GPINSTRUCT_LESSON_ELEMENT (curr_lesson_elements->data);

				if (GPINSTRUCT_IS_LESSON_DISCUSSION (curr_lesson_element))
				{
					curr_lesson_discussion = GPINSTRUCT_LESSON_DISCUSSION (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "discussion",
					                            BAD_CAST gpinstruct_lesson_discussion_get_text (curr_lesson_discussion));
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_lesson_element_get_title (curr_lesson_element));

					/* Currently in a "discussion" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}
				else if (GPINSTRUCT_IS_LESSON_READING (curr_lesson_element))
				{
					curr_lesson_reading = GPINSTRUCT_LESSON_READING (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "reading",
					                            BAD_CAST gpinstruct_lesson_reading_get_text (curr_lesson_reading));
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_lesson_element_get_title (curr_lesson_element));

					/* Currently in a "reading" type element, go back to "lesson" element */
					current_node = current_node->parent;
				}
				else if (GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element))
				{
					curr_lesson_test_multi_choice = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "test-multi-choice", NULL);
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, BAD_CAST "id", BAD_CAST gpinstruct_lesson_test_get_id (GPINSTRUCT_LESSON_TEST (curr_lesson_test_multi_choice)));
					xmlSetProp (current_node, BAD_CAST "explain", gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (curr_lesson_test_multi_choice))?BAD_CAST "true":BAD_CAST "false");

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "directions",
					                            BAD_CAST gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_lesson_test_multi_choice)));

					/* Currently in a "directions" element, go back to "test-multi-choice" element */
					current_node = current_node->parent;

					questions = gpinstruct_lesson_test_multi_choice_get_questions (curr_lesson_test_multi_choice);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_multi_choice_question = GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION (curr_questions->data);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "question", NULL);
						temp = g_strdup_printf ("%d", gpinstruct_lesson_test_multi_choice_question_get_answer (curr_lesson_test_multi_choice_question));
						xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST temp);
						g_free (temp);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "text",
						                            BAD_CAST gpinstruct_lesson_test_multi_choice_question_get_text (curr_lesson_test_multi_choice_question));

						/* Currently in a "text" element, go back to "question" element */
						current_node = current_node->parent;

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "explanation",
						                            BAD_CAST gpinstruct_lesson_test_multi_choice_question_get_explanation (curr_lesson_test_multi_choice_question));

						/* Currently in a "explanation" element, go back to "question" element */
						current_node = current_node->parent;

						choices = gpinstruct_lesson_test_multi_choice_question_get_choices (curr_lesson_test_multi_choice_question);
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
				else if (GPINSTRUCT_IS_LESSON_TEST_WORD_POOL (curr_lesson_element))
				{
					curr_lesson_test_word_pool = GPINSTRUCT_LESSON_TEST_WORD_POOL (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "test-word-pool", NULL);
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, BAD_CAST "id", BAD_CAST gpinstruct_lesson_test_get_id (GPINSTRUCT_LESSON_TEST (curr_lesson_test_word_pool)));
					xmlSetProp (current_node, BAD_CAST "explain", gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (curr_lesson_test_word_pool))?BAD_CAST "true":BAD_CAST "false");

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "directions",
					                            BAD_CAST gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_lesson_test_word_pool)));

					/* Currently in a "directions" element, go back to "test-word-pool" element */
					current_node = current_node->parent;

					choices = gpinstruct_lesson_test_word_pool_get_choices (curr_lesson_test_word_pool);
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

					questions = gpinstruct_lesson_test_word_pool_get_questions (curr_lesson_test_word_pool);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_word_pool_question = GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION (curr_questions->data);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "question", NULL);
						temp = g_strdup_printf ("%d", gpinstruct_lesson_test_word_pool_question_get_answer (curr_lesson_test_word_pool_question));
						xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST temp);
						g_free (temp);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "text",
						                            BAD_CAST gpinstruct_lesson_test_word_pool_question_get_text (curr_lesson_test_word_pool_question));

						/* Currently in a "text" element, go back to "question" element */
						current_node = current_node->parent;

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "explanation",
						                            BAD_CAST gpinstruct_lesson_test_word_pool_question_get_explanation (curr_lesson_test_word_pool_question));

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
				else if (GPINSTRUCT_IS_LESSON_TEST_ORDER (curr_lesson_element))
				{
					curr_lesson_test_order = GPINSTRUCT_LESSON_TEST_ORDER (curr_lesson_element);

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "test-order", NULL);
					xmlSetProp (current_node, BAD_CAST "title", BAD_CAST gpinstruct_lesson_element_get_title (curr_lesson_element));
					xmlSetProp (current_node, BAD_CAST "id", BAD_CAST gpinstruct_lesson_test_get_id (GPINSTRUCT_LESSON_TEST (curr_lesson_test_order)));
					xmlSetProp (current_node, BAD_CAST "explain", gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (curr_lesson_test_order))?BAD_CAST "true":BAD_CAST "false");

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "directions",
					                            BAD_CAST gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (curr_lesson_test_order)));

					/* Currently in a "directions" element, go back to "test-order" element */
					current_node = current_node->parent;

					current_node = xmlNewChild (current_node, NULL, BAD_CAST "explanation",
					                            BAD_CAST gpinstruct_lesson_test_order_get_explanation (curr_lesson_test_order));

					/* Currently in a "explanation" element, go back to "test-order" element */
					current_node = current_node->parent;

					items = gpinstruct_lesson_test_order_get_items (curr_lesson_test_order);
					curr_items = items;

					while (curr_items)
					{
						curr_lesson_test_order_item = GPINSTRUCT_LESSON_TEST_ORDER_ITEM (curr_items->data);

						current_node = xmlNewChild (current_node, NULL, BAD_CAST "item", BAD_CAST gpinstruct_lesson_test_order_item_get_text (curr_lesson_test_order_item));
						temp = g_strdup_printf ("%d", gpinstruct_lesson_test_order_item_get_answer (curr_lesson_test_order_item));
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
