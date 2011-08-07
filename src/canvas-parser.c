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
#include <stdlib.h>
#include <glib/gi18n.h>
#include <gio/gio.h>

#include "canvas.h"

typedef struct _CanvasParserPrivate CanvasParserPrivate;
struct _CanvasParserPrivate
{
	CanvasProject* project;

	CanvasProject* current_project;
	CanvasCategory* current_category;
	CanvasLesson* current_lesson;

	CanvasLessonElement* current_lesson_element;
	CanvasLessonDiscussion* current_lesson_discussion;
	CanvasLessonReading* current_lesson_reading;

	CanvasLessonTest* current_lesson_test;
	CanvasLessonTestMultiChoice* current_lesson_test_multi_choice;
	CanvasLessonTestMultiChoiceQuestion* current_lesson_test_multi_choice_question;
	CanvasLessonTestWordPool* current_lesson_test_word_pool;
	CanvasLessonTestWordPoolQuestion* current_lesson_test_word_pool_question;
	CanvasLessonTestOrder* current_lesson_test_order;
	CanvasLessonTestOrderItem* current_lesson_test_order_item;

	gboolean text;
	gboolean directions;
	gboolean choice;
	gboolean explanation;
};

#define CANVAS_PARSER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_PARSER, CanvasParserPrivate))



G_DEFINE_TYPE (CanvasParser, canvas_parser, CANVAS_TYPE_OBJECT);

static void
canvas_parser_init (CanvasParser *object)
{
	CanvasParserPrivate* priv = CANVAS_PARSER_PRIVATE (object);

	priv->project = NULL;

	priv->current_project = NULL;
	priv->current_category = NULL;
	priv->current_lesson = NULL;

	priv->current_lesson_element = NULL;
	priv->current_lesson_discussion = NULL;
	priv->current_lesson_reading = NULL;

	priv->current_lesson_test = NULL;
	priv->current_lesson_test_multi_choice = NULL;
	priv->current_lesson_test_multi_choice_question = NULL;
	priv->current_lesson_test_word_pool = NULL;
	priv->current_lesson_test_word_pool_question = NULL;
	priv->current_lesson_test_order = NULL;
	priv->current_lesson_test_order_item = NULL;

	priv->text = FALSE;
	priv->directions = FALSE;
	priv->choice = FALSE;
	priv->explanation = FALSE;
}

static void
canvas_parser_finalize (GObject *object)
{

	G_OBJECT_CLASS (canvas_parser_parent_class)->finalize (object);
}

static void
canvas_parser_class_init (CanvasParserClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasParserPrivate));

	object_class->finalize = canvas_parser_finalize;
}


/* Called for open tags <foo bar="baz"> */
void parse_start_element (GMarkupParseContext *context,
                          const gchar         *element_name,
                          const gchar        **attribute_names,
                          const gchar        **attribute_values,
                          gpointer             user_data,
                          GError             **error)
{
	CanvasParserPrivate* priv = user_data;
	if (g_strcmp0 ("canvas-project", element_name) == 0)
	{
		if (priv->current_project == NULL)
		{
			CanvasProject* project = canvas_project_new ();
			priv->current_project = project;
			priv->project = project;
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_project_set_title (project, attribute_values[i]);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Only one project allowed per project file."));
	}
	else if (g_strcmp0 ("category", element_name) == 0)
	{
		if (priv->current_project)
		{
			CanvasCategory* category = canvas_category_new ();
			priv->current_category = category;
			canvas_project_add_category (priv->current_project, category);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_category_set_title (category, attribute_values[i]);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Category element without Project element parent."));
	}
	else if (g_strcmp0 ("lesson", element_name) == 0)
	{
		if (priv->current_category)
		{
			CanvasLesson* lesson = canvas_lesson_new ();
			priv->current_lesson = lesson;
			canvas_category_add_lesson (priv->current_category, lesson);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_set_title (lesson, attribute_values[i]);
				else if (g_strcmp0 ("single-score", attribute_names[i]) == 0)
				{
					if (g_strcmp0 ("true", attribute_values[i]) == 0)
						canvas_lesson_set_single_score (lesson, TRUE);
					else
						canvas_lesson_set_single_score (lesson, FALSE);
				}
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson element without Category element parent."));
	}
/*	else if (g_strcmp0 ("lesson-element", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonElement* lesson_element = canvas_lesson_element_new ();
			priv->current_lesson_element = lesson_element;
			canvas_lesson_add_lesson_element (priv->current_lesson, lesson_element);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (lesson_element, attribute_values[i]);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Element element without Lesson element parent."));
	}*/
	else if (g_strcmp0 ("discussion", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonDiscussion* lesson_discussion = canvas_lesson_discussion_new ();
			priv->current_lesson_discussion = lesson_discussion;
			priv->current_lesson_element = CANVAS_LESSON_ELEMENT (lesson_discussion);
			canvas_lesson_add_lesson_element (priv->current_lesson, CANVAS_LESSON_ELEMENT (lesson_discussion));
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (lesson_discussion), attribute_values[i]);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Discussion element without Lesson element parent."));
	}
	else if (g_strcmp0 ("reading", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonReading* lesson_reading = canvas_lesson_reading_new ();
			priv->current_lesson_reading = lesson_reading;
			priv->current_lesson_element = CANVAS_LESSON_ELEMENT (lesson_reading);
			canvas_lesson_add_lesson_element (priv->current_lesson, CANVAS_LESSON_ELEMENT (lesson_reading));
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (lesson_reading), attribute_values[i]);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Reading element without Lesson element parent."));
	}
/*	else if (g_strcmp0 ("test", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonTest* lesson_test = canvas_lesson_test_new ();
			priv->current_lesson_test = lesson_test;
			priv->current_lesson_element = CANVAS_LESSON_ELEMENT (lesson_test);
			canvas_lesson_add_lesson_element (priv->current_lesson, CANVAS_LESSON_ELEMENT (lesson_test));
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (lesson_test), attribute_values[i]);
				else if (g_strcmp0 ("directions", attribute_names[i]) == 0)
					canvas_lesson_test_set_direction (lesson_test, attribute_values[i]);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Test element without Lesson element parent."));
	}*/
	else if (g_strcmp0 ("test-multi-choice", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonTestMultiChoice* lesson_test_multi_choice = canvas_lesson_test_multi_choice_new ();
			priv->current_lesson_test_multi_choice = lesson_test_multi_choice;
			priv->current_lesson_test = CANVAS_LESSON_TEST (lesson_test_multi_choice);
			priv->current_lesson_element = CANVAS_LESSON_ELEMENT (lesson_test_multi_choice);
			canvas_lesson_add_lesson_element (priv->current_lesson, CANVAS_LESSON_ELEMENT (lesson_test_multi_choice));
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (lesson_test_multi_choice), attribute_values[i]);
				if (g_strcmp0 ("explain", attribute_names[i]) == 0)
					canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (lesson_test_multi_choice), (g_ascii_strncasecmp(attribute_values[i], "true", -1) == 0)?TRUE:FALSE);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Test element without Lesson element parent."));
	}
	else if (g_strcmp0 ("test-word-pool", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonTestWordPool* lesson_test_word_pool = canvas_lesson_test_word_pool_new ();
			priv->current_lesson_test_word_pool = lesson_test_word_pool;
			priv->current_lesson_test = CANVAS_LESSON_TEST (lesson_test_word_pool);
			priv->current_lesson_element = CANVAS_LESSON_ELEMENT (lesson_test_word_pool);
			canvas_lesson_add_lesson_element (priv->current_lesson, CANVAS_LESSON_ELEMENT (lesson_test_word_pool));
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (lesson_test_word_pool), attribute_values[i]);
				if (g_strcmp0 ("explain", attribute_names[i]) == 0)
					canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (lesson_test_word_pool), (g_ascii_strncasecmp(attribute_values[i], "true", -1) == 0)?TRUE:FALSE);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Test element without Lesson element parent."));
	}
	else if (g_strcmp0 ("test-order", element_name) == 0)
	{
		if (priv->current_lesson)
		{
			CanvasLessonTestOrder* lesson_test_order = canvas_lesson_test_order_new ();
			priv->current_lesson_test_order = lesson_test_order;
			priv->current_lesson_test = CANVAS_LESSON_TEST (lesson_test_order);
			priv->current_lesson_element = CANVAS_LESSON_ELEMENT (lesson_test_order);
			canvas_lesson_add_lesson_element (priv->current_lesson, CANVAS_LESSON_ELEMENT (lesson_test_order));
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
					canvas_lesson_element_set_title (CANVAS_LESSON_ELEMENT (lesson_test_order), attribute_values[i]);
				if (g_strcmp0 ("explain", attribute_names[i]) == 0)
					canvas_lesson_test_set_explain (CANVAS_LESSON_TEST (lesson_test_order), (g_ascii_strncasecmp(attribute_values[i], "true", -1) == 0)?TRUE:FALSE);
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Test element without Lesson element parent."));
	}
	else if (g_strcmp0 ("text", element_name) == 0)
		priv->text = TRUE;
	else if (g_strcmp0 ("directions", element_name) == 0)
		priv->directions = TRUE;
	else if (g_strcmp0 ("question", element_name) == 0)
	{
		if (priv->current_lesson_test_multi_choice)
		{
			CanvasLessonTestMultiChoiceQuestion* lesson_test_multi_choice_question = canvas_lesson_test_multi_choice_question_new ();
			priv->current_lesson_test_multi_choice_question = lesson_test_multi_choice_question;
			canvas_lesson_test_multi_choice_add_question (priv->current_lesson_test_multi_choice, lesson_test_multi_choice_question);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("answer", attribute_names[i]) == 0)
					canvas_lesson_test_multi_choice_question_set_answer (lesson_test_multi_choice_question, atoi (attribute_values[i]));
			}
		}
		else if (priv->current_lesson_test_word_pool)
		{
			CanvasLessonTestWordPoolQuestion* lesson_test_word_pool_question = canvas_lesson_test_word_pool_question_new ();
			priv->current_lesson_test_word_pool_question = lesson_test_word_pool_question;
			canvas_lesson_test_word_pool_add_question (priv->current_lesson_test_word_pool, lesson_test_word_pool_question);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("answer", attribute_names[i]) == 0)
					canvas_lesson_test_word_pool_question_set_answer (lesson_test_word_pool_question, atoi (attribute_values[i]));
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Question element without Lesson Test element parent."));
	}
	else if (g_strcmp0 ("choice", element_name) == 0)
		priv->choice = TRUE;
	else if (g_strcmp0 ("item", element_name) == 0)
	{
		if (priv->current_lesson_test_order)
		{
			CanvasLessonTestOrderItem* lesson_test_order_item = canvas_lesson_test_order_item_new ();
			priv->current_lesson_test_order_item = lesson_test_order_item;
			canvas_lesson_test_order_add_item (priv->current_lesson_test_order, lesson_test_order_item);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("answer", attribute_names[i]) == 0)
					canvas_lesson_test_order_item_set_answer (lesson_test_order_item, atoi (attribute_values[i]));
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Item element without Lesson Test element parent."));
	}
	else if (g_strcmp0 ("explanation", element_name) == 0)
		priv->explanation = TRUE;
	else
		g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ELEMENT, _("Unknown element: %s"), element_name);
}

/* Called for close tags </foo> */
void parse_end_element (GMarkupParseContext *context,
                        const gchar         *element_name,
                        gpointer             user_data,
                        GError             **error)
{
	CanvasParserPrivate* priv = user_data;
	if (g_strcmp0 ("canvas-project", element_name) == 0)
		priv->current_project = NULL;
	else if (g_strcmp0 ("category", element_name) == 0)
		priv->current_category = NULL;
	else if (g_strcmp0 ("lesson", element_name) == 0)
		priv->current_lesson = NULL;
/*	else if (g_strcmp0 ("lesson-element", element_name) == 0)
		priv->current_lesson_element = NULL;*/
	else if (g_strcmp0 ("discussion", element_name) == 0)
	{
		priv->current_lesson_element = NULL;
		priv->current_lesson_discussion = NULL;
	}
	else if (g_strcmp0 ("reading", element_name) == 0)
	{
		priv->current_lesson_element = NULL;
		priv->current_lesson_reading = NULL;
	}
/*	else if (g_strcmp0 ("test", element_name) == 0)
	{
		priv->current_lesson_element = NULL;
		priv->current_lesson_test = NULL;
	}*/
	else if (g_strcmp0 ("test-multi-choice", element_name) == 0)
	{
		priv->current_lesson_element = NULL;
		priv->current_lesson_test = NULL;
		priv->current_lesson_test_multi_choice = NULL;
	}
	else if (g_strcmp0 ("test-word-pool", element_name) == 0)
	{
		priv->current_lesson_element = NULL;
		priv->current_lesson_test = NULL;
		priv->current_lesson_test_word_pool = NULL;
	}
	else if (g_strcmp0 ("test-order", element_name) == 0)
	{
		priv->current_lesson_element = NULL;
		priv->current_lesson_test = NULL;
		priv->current_lesson_test_order = NULL;
	}
	else if (g_strcmp0 ("text", element_name) == 0)
		priv->text = FALSE;
	else if (g_strcmp0 ("directions", element_name) == 0)
		priv->directions = FALSE;
	else if (g_strcmp0 ("question", element_name) == 0)
	{
		if (priv->current_lesson_test_multi_choice_question)
			priv->current_lesson_test_multi_choice_question = NULL;
		else if (priv->current_lesson_test_word_pool_question)
			priv->current_lesson_test_word_pool_question = NULL;
	}
	else if (g_strcmp0 ("choice", element_name) == 0)
		priv->choice = FALSE;
	else if (g_strcmp0 ("item", element_name) == 0)
		priv->current_lesson_test_order_item = NULL;
	else if (g_strcmp0 ("explanation", element_name) == 0)
		priv->explanation = FALSE;
	else
		g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ELEMENT, _("Unknown element: %s"), element_name);
}

/* Called for character data */
/* text is not nul-terminated */
void parse_text (GMarkupParseContext *context,
                 const gchar         *text,
                 gsize                text_len,
                 gpointer             user_data,
                 GError             **error)
{
	CanvasParserPrivate* priv = user_data;
	gchar* text_nul = g_strndup (text, text_len);

	if (priv->text)
	{
		if (priv->current_lesson_test_multi_choice_question)
			canvas_lesson_test_multi_choice_question_set_text (priv->current_lesson_test_multi_choice_question,
			                                                   text_nul);
		else if (priv->current_lesson_test_word_pool_question)
			canvas_lesson_test_word_pool_question_set_text (priv->current_lesson_test_word_pool_question,
			                                                text_nul);
		else if (priv->current_lesson_discussion)
			canvas_lesson_discussion_set_text (priv->current_lesson_discussion,
			                                   text_nul);
		else if (priv->current_lesson_reading)
			canvas_lesson_reading_set_text (priv->current_lesson_reading,
			                                text_nul);
	}
	else if (priv->directions)
	{
		if (priv->current_lesson_test)
			canvas_lesson_test_set_directions (priv->current_lesson_test,
			                                   text_nul);
	}
	else if (priv->choice)
	{
		if (priv->current_lesson_test_multi_choice_question)
			canvas_lesson_test_multi_choice_question_add_choice (priv->current_lesson_test_multi_choice_question,
			                                                     text_nul);
		else if (priv->current_lesson_test_word_pool)
			canvas_lesson_test_word_pool_add_choice (priv->current_lesson_test_word_pool,
			                                         text_nul);
	}
	else if (priv->current_lesson_test_order_item)
	{
		canvas_lesson_test_order_item_set_text (priv->current_lesson_test_order_item,
		                                        text_nul);
	}
	else if (priv->explanation)
	{
		if (priv->current_lesson_test_multi_choice_question)
			canvas_lesson_test_multi_choice_question_set_explanation (priv->current_lesson_test_multi_choice_question,
			                                                          text_nul);
		else if (priv->current_lesson_test_word_pool_question)
			canvas_lesson_test_word_pool_question_set_explanation (priv->current_lesson_test_word_pool_question,
			                                                       text_nul);
		else if (priv->current_lesson_test_order)
			canvas_lesson_test_order_set_explanation (priv->current_lesson_test_order,
			                                          text_nul);
	}

	g_free (text_nul);
}


CanvasParser*
canvas_parser_new (void)
{
	CanvasParser* parser = g_object_new(CANVAS_TYPE_PARSER, NULL);
	return parser;
}

CanvasProject*
canvas_parser_open (CanvasParser* parser, const gchar* text, GError** error)
{
	GMarkupParser markup_parser = {
		parse_start_element,
		parse_end_element,
		parse_text,
		NULL,
		NULL
	};

	CanvasParserPrivate* priv = CANVAS_PARSER_PRIVATE (parser);
	GMarkupParseContext* markup_context = 
		g_markup_parse_context_new (&markup_parser, 
		                            G_MARKUP_TREAT_CDATA_AS_TEXT | G_MARKUP_PREFIX_ERROR_POSITION, 
		                            priv, NULL);
	g_markup_parse_context_parse (markup_context, text, -1, error);

	g_free (markup_context);

	return priv->project;
}

void
canvas_parser_save (CanvasParser* parser, CanvasProject* project, const gchar* filename, GError** error)
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
	gchar *output;

	GError* error_local = NULL;
	GFile* file = g_file_new_for_path (filename);

	GFileOutputStream* fileout = g_file_replace (file, NULL, TRUE, G_FILE_CREATE_NONE, NULL, &error_local);
	if (error_local)
	{
		*error = error_local;
		return;
	}

	GDataOutputStream* dataout = g_data_output_stream_new (G_OUTPUT_STREAM (fileout));

	output = g_markup_printf_escaped ("<canvas-project title=\"%s\">\n",
	                                  canvas_project_get_title (project));
	g_data_output_stream_put_string (dataout, output, NULL, NULL);
	g_free (output);

	categories = canvas_project_get_categories (project);
	curr_categories = categories;

	while (curr_categories)
	{
		curr_category = CANVAS_CATEGORY (curr_categories->data);

		output = g_markup_printf_escaped ("<category title=\"%s\">\n",
		                                  canvas_category_get_title (curr_category));
		g_data_output_stream_put_string (dataout, output, NULL, NULL);
		g_free (output);

		lessons = canvas_category_get_lessons (curr_category);
		curr_lessons = lessons;

		while (curr_lessons)
		{
			curr_lesson = CANVAS_LESSON (curr_lessons->data);

			output = g_markup_printf_escaped ("<lesson title=\"%s\" single-score=\"%s\">\n",
			                                  canvas_lesson_get_title (curr_lesson),
			                                  canvas_lesson_get_single_score (curr_lesson)?"true":"false");
			g_data_output_stream_put_string (dataout, output, NULL, NULL);
			g_free (output);

			lesson_elements = canvas_lesson_get_lesson_elements (curr_lesson);
			curr_lesson_elements = lesson_elements;

			while (curr_lesson_elements)
			{
				curr_lesson_element = CANVAS_LESSON_ELEMENT (curr_lesson_elements->data);

				if (CANVAS_IS_LESSON_DISCUSSION (curr_lesson_element))
				{
					curr_lesson_discussion = CANVAS_LESSON_DISCUSSION (curr_lesson_element);

					output = g_markup_printf_escaped ("<discussion title=\"%s\">\n",
					                                  canvas_lesson_element_get_title (curr_lesson_element));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					output = g_markup_printf_escaped ("<text>%s</text>\n",
					                                  canvas_lesson_discussion_get_text (curr_lesson_discussion));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					g_data_output_stream_put_string (dataout, "</discussion>\n", NULL, NULL);
				}
				else if (CANVAS_IS_LESSON_READING (curr_lesson_element))
				{
					curr_lesson_reading = CANVAS_LESSON_READING (curr_lesson_element);

					output = g_markup_printf_escaped ("<reading title=\"%s\">\n",
					                                  canvas_lesson_element_get_title (curr_lesson_element));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					output = g_markup_printf_escaped ("<text>%s</text>\n",
					                                  canvas_lesson_reading_get_text (curr_lesson_reading));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					g_data_output_stream_put_string (dataout, "</reading>\n", NULL, NULL);
				}
				else if (CANVAS_IS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element))
				{
					curr_lesson_test_multi_choice = CANVAS_LESSON_TEST_MULTI_CHOICE (curr_lesson_element);

					output = g_markup_printf_escaped ("<test-multi-choice title=\"%s\">\n",
					                                  canvas_lesson_element_get_title (curr_lesson_element));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					output = g_markup_printf_escaped ("<directions>%s</directions>\n",
					                                  canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					questions = canvas_lesson_test_multi_choice_get_questions (curr_lesson_test_multi_choice);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_multi_choice_question = CANVAS_LESSON_TEST_MULTI_CHOICE_QUESTION (curr_questions->data);

						output = g_markup_printf_escaped ("<question answer=\"%u\">\n",
						                                  canvas_lesson_test_multi_choice_question_get_answer (curr_lesson_test_multi_choice_question));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						output = g_markup_printf_escaped ("<text>%s</text>\n",
						                                  canvas_lesson_test_multi_choice_question_get_text (curr_lesson_test_multi_choice_question));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						output = g_markup_printf_escaped ("<explanation>%s</explanation>\n",
						                                  canvas_lesson_test_multi_choice_question_get_explanation (curr_lesson_test_multi_choice_question));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						choices = canvas_lesson_test_multi_choice_question_get_choices (curr_lesson_test_multi_choice_question);
						curr_choices = choices;

						while (curr_choices)
						{
							curr_choice = curr_choices->data;

							output = g_markup_printf_escaped ("<choice>%s</choice>\n", curr_choice);
							g_data_output_stream_put_string (dataout, output, NULL, NULL);
							g_free (output);

							curr_choices = curr_choices->next;
						}

						g_list_free (curr_choices);

						g_data_output_stream_put_string (dataout, "</question>\n", NULL, NULL);

						curr_questions = curr_questions->next;
					}

					g_list_free (questions);

					g_data_output_stream_put_string (dataout, "</test-multi-choice>\n", NULL, NULL);
				}
				else if (CANVAS_IS_LESSON_TEST_WORD_POOL (curr_lesson_element))
				{
					curr_lesson_test_word_pool = CANVAS_LESSON_TEST_WORD_POOL (curr_lesson_element);

					output = g_markup_printf_escaped ("<test-word-pool title=\"%s\">\n",
					                                  canvas_lesson_element_get_title (curr_lesson_element));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					output = g_markup_printf_escaped ("<directions>%s</directions>\n",
					                                  canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					choices = canvas_lesson_test_word_pool_get_choices (curr_lesson_test_word_pool);
					curr_choices = choices;

					while (curr_choices)
					{
						curr_choice = curr_choices->data;

						output = g_markup_printf_escaped ("<choice>%s</choice>\n", curr_choice);
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						curr_choices = curr_choices->next;
					}

					g_list_free (curr_choices);

					questions = canvas_lesson_test_word_pool_get_questions (curr_lesson_test_word_pool);
					curr_questions = questions;

					while (curr_questions)
					{
						curr_lesson_test_word_pool_question = CANVAS_LESSON_TEST_WORD_POOL_QUESTION (curr_questions->data);

						output = g_markup_printf_escaped ("<question answer=\"%u\">\n",
						                                  canvas_lesson_test_word_pool_question_get_answer (curr_lesson_test_word_pool_question));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						output = g_markup_printf_escaped ("<text>%s</text>\n",
						                                  canvas_lesson_test_word_pool_question_get_text (curr_lesson_test_word_pool_question));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						output = g_markup_printf_escaped ("<explanation>%s</explanation>\n",
						                                  canvas_lesson_test_word_pool_question_get_explanation (curr_lesson_test_word_pool_question));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						g_data_output_stream_put_string (dataout, "</question>\n", NULL, NULL);

						curr_questions = curr_questions->next;
					}

					g_list_free (curr_questions);

					g_data_output_stream_put_string (dataout, "</test-word-pool>\n", NULL, NULL);
				}
				else if (CANVAS_IS_LESSON_TEST_ORDER (curr_lesson_element))
				{
					curr_lesson_test_order = CANVAS_LESSON_TEST_ORDER (curr_lesson_element);

					output = g_markup_printf_escaped ("<test-order title=\"%s\">\n",
					                                  canvas_lesson_element_get_title (curr_lesson_element));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					output = g_markup_printf_escaped ("<directions>%s</directions>\n",
					                                  canvas_lesson_test_get_directions (CANVAS_LESSON_TEST (curr_lesson_test_multi_choice)));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					output = g_markup_printf_escaped ("<explanation>%s</explanation>\n",
					                                  canvas_lesson_test_order_get_explanation (curr_lesson_test_order));
					g_data_output_stream_put_string (dataout, output, NULL, NULL);
					g_free (output);

					items = canvas_lesson_test_order_get_items (curr_lesson_test_order);
					curr_items = items;

					while (curr_items)
					{
						curr_lesson_test_order_item = CANVAS_LESSON_TEST_ORDER_ITEM (curr_items->data);

						output = g_markup_printf_escaped ("<item answer=\"%u\"/>\n",
						                                  canvas_lesson_test_order_item_get_answer (curr_lesson_test_order_item));
						g_data_output_stream_put_string (dataout, output, NULL, NULL);
						g_free (output);

						curr_items = curr_items->next;
					}

					g_list_free (curr_items);

					g_data_output_stream_put_string (dataout, "</test-order>\n", NULL, NULL);
				}

				curr_lesson_elements = curr_lesson_elements->next;
			}

			g_list_free (lesson_elements);

			g_data_output_stream_put_string (dataout, "</lesson>\n", NULL, NULL);

			curr_lessons = curr_lessons->next;
		}

		g_list_free (lessons);

		g_data_output_stream_put_string (dataout, "</category>\n", NULL, NULL);

		curr_categories = curr_categories->next;
	}
	
	g_list_free (categories);

	g_data_output_stream_put_string (dataout, "</canvas-project>\n", NULL, NULL);

	g_object_unref (dataout);

	g_object_unref (fileout);

	g_object_unref (file);
}
