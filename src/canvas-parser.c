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

#include "canvas.h"

typedef struct _CanvasParserPrivate CanvasParserPrivate;
struct _CanvasParserPrivate
{
	CanvasProject* project;
	CanvasProject* current_project;
	CanvasCategory* current_category;
	CanvasLesson* current_lesson;
	CanvasLessonElement* current_lesson_element;
};

#define CANVAS_PARSER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_PARSER, CanvasParserPrivate))



G_DEFINE_TYPE (CanvasParser, canvas_parser, G_TYPE_OBJECT);

static void
canvas_parser_init (CanvasParser *object)
{
	CanvasParserPrivate* private_data = CANVAS_PARSER_PRIVATE (object);
	private_data->project = NULL;
	private_data->current_project = NULL;
	private_data->current_category = NULL;
	private_data->current_lesson = NULL;
	private_data->current_lesson_element = NULL;
}

static void
canvas_parser_finalize (GObject *object)
{
	CanvasParserPrivate* private_data = CANVAS_PARSER_PRIVATE (object);
	if (private_data->current_project)
		g_free (private_data->current_project);

	G_OBJECT_CLASS (canvas_parser_parent_class)->finalize (object);
}

static void
canvas_parser_class_init (CanvasParserClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

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
	CanvasParserPrivate* private_data = user_data;
	if (g_strcmp0 ("canvas-project", element_name) == 0)
	{
		if (private_data->current_project == NULL)
		{
			g_print("Start of Project\n");
			CanvasProject* project = canvas_project_new ();
			private_data->current_project = project;
			private_data->project = project;
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
				{
					g_print("Title of Project: %s\n", attribute_names[i]);
					canvas_project_set_title (project, attribute_names[i]);
				}
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Only one project allowed per project file."));
	}
	else if (g_strcmp0 ("category", element_name) == 0)
	{
		if (private_data->current_project)
		{
			g_print("Start of Category\n");
			CanvasCategory* category = canvas_category_new ();
			private_data->current_category = category;
			canvas_project_add_category (private_data->current_project, category);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
				{
					g_print("Title of Category: %s\n", attribute_names[i]);
					canvas_category_set_title (category, attribute_names[i]);
				}
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Category element without Project element parent."));
	}
	else if (g_strcmp0 ("lesson", element_name) == 0)
	{
		if (private_data->current_category)
		{
			g_print("Start of Lesson\n");
			CanvasLesson* lesson = canvas_lesson_new ();
			private_data->current_lesson = lesson;
			canvas_category_add_lesson (private_data->current_category, lesson);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
				{
					g_print("Title of Lesson: %s\n", attribute_names[i]);
					canvas_lesson_set_title (lesson, attribute_names[i]);
				}
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson element without Category element parent."));
	}
	else if (g_strcmp0 ("lesson-element_name", element_name) == 0)
	{
		if (private_data->current_lesson)
		{
			g_print("Start of Lesson Element\n");
			CanvasLessonElement* lesson_element = canvas_lesson_element_new ();
			private_data->current_lesson_element = lesson_element;
			canvas_lesson_add_lesson_element (private_data->current_lesson, lesson_element);
			int i;
			for (i=0; attribute_names[i] != NULL; i++)
			{
				if (g_strcmp0 ("title", attribute_names[i]) == 0)
				{
					g_print("Title of Lesson Element: %s\n", attribute_names[i]);
					canvas_lesson_element_set_title (lesson_element, attribute_names[i]);
				}
			}
		}
		else
			g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, _("Started a Lesson Element element without Lesson element parent."));
	}
	else
		g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ELEMENT, _("Unknown element: %s"), element_name);
}

/* Called for close tags </foo> */
void parse_end_element (GMarkupParseContext *context,
                        const gchar         *element_name,
                        gpointer             user_data,
                        GError             **error)
{
	CanvasParserPrivate* private_data = user_data;
	if (g_strcmp0 ("canvas-project", element_name) == 0)
		private_data->current_project = NULL;
	else if (g_strcmp0 ("category", element_name) == 0)
		private_data->current_category = NULL;
	else if (g_strcmp0 ("lesson", element_name) == 0)
		private_data->current_lesson = NULL;
	else if (g_strcmp0 ("lesson-element_name", element_name) == 0)
		private_data->current_lesson_element = NULL;
	else
		g_set_error (error, G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ELEMENT, _("Unknown element: %s"), element_name);
}


CanvasParser*
canvas_parser_new (void)
{
	CanvasParser* parser = g_object_new(CANVAS_TYPE_PARSER, NULL);
	return parser;
}

CanvasProject*
canvas_parser_parse (CanvasParser* parser, const gchar* text, GError** error)
{
	GMarkupParser markup_parser = {
		parse_start_element,
		parse_end_element,
		NULL,
		NULL,
		NULL
	};

	CanvasParserPrivate* private_data = CANVAS_PARSER_PRIVATE (parser);
	GMarkupParseContext* markup_context = 
		g_markup_parse_context_new (&markup_parser, 
		                            G_MARKUP_TREAT_CDATA_AS_TEXT | G_MARKUP_PREFIX_ERROR_POSITION, 
		                            private_data, NULL);
	g_markup_parse_context_parse (markup_context, text, -1, error);
	return private_data->project;
}
