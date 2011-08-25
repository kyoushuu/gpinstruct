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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "canvas/canvas.h"
#include "canvas-view/canvas-view.h"

struct _CanvasLogPrivate
{
	GList* tests_list;
	GTimer* timer;
	GQuark last_test;

	gchar* last_name;
	gchar* first_name;

	guint group_elements;
	guint curr_group_element;
};

#define CANVAS_LOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LOG, CanvasLogPrivate))


GQuark
canvas_log_error_quark (void)
{
  return g_quark_from_static_string ("canvas-log-error-quark");
}



typedef struct {
	CanvasLog* log;
	CanvasLogForeachFunc func;
	gpointer user_data;
} CanvasLogForeachFuncData;

static void
add_answer_node (CanvasLogAnswer* answer,
                 xmlNodePtr test_node)
{
	gchar *item_id = g_strdup_printf ("%d", answer->item_id);
	gchar *answer_id = g_strdup_printf ("%d", answer->answer_id);
	gchar *time_spent = g_strdup_printf ("%f", answer->time_spent);

	xmlNodePtr current_node = xmlNewChild (test_node, NULL, BAD_CAST "item", NULL);
	xmlSetProp (current_node, BAD_CAST "id", BAD_CAST item_id);
	xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST answer_id);
	xmlSetProp (current_node, BAD_CAST "time-spent", BAD_CAST time_spent);

	g_free (item_id);
	g_free (answer_id);
	g_free (time_spent);
}

static void
add_test_node (CanvasLogTest* test,
                 xmlNodePtr log_node)
{
	xmlNodePtr current_node = xmlNewChild (log_node, NULL, BAD_CAST "test", NULL);
	xmlSetProp (current_node, BAD_CAST "id", BAD_CAST g_quark_to_string (test->id));

	g_list_foreach (test->answers, (GFunc)add_answer_node, current_node);
}

static void
free_answers_foreach (CanvasLogTest* test)
{
	g_list_free_full (test->answers, g_free);
	g_free (test);
}



G_DEFINE_TYPE (CanvasLog, canvas_log, CANVAS_TYPE_OBJECT);

static void
canvas_log_init (CanvasLog *object)
{
	object->priv = CANVAS_LOG_PRIVATE (object);

	object->priv->tests_list = NULL;

	object->priv->timer = g_timer_new ();

	object->priv->last_test = 0;

	object->priv->last_name = NULL;
	object->priv->first_name = NULL;

	object->priv->group_elements = 1;
	object->priv->curr_group_element = 0;
}

static void
canvas_log_finalize (GObject *object)
{
	CanvasLog* log = CANVAS_LOG (object);

	g_list_free_full (log->priv->tests_list, (GDestroyNotify)free_answers_foreach);

	g_timer_destroy (log->priv->timer);

	g_free (log->priv->last_name);
	g_free (log->priv->first_name);

	G_OBJECT_CLASS (canvas_log_parent_class)->finalize (object);
}

static void
canvas_log_class_init (CanvasLogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLogPrivate));

	object_class->finalize = canvas_log_finalize;
}


CanvasLog*
canvas_log_new (void)
{
	return g_object_new (CANVAS_TYPE_LOG, NULL);
}

void
canvas_log_set_last_name (CanvasLog* log, const gchar* last_name)
{
	if (log->priv->last_name)
		g_free (log->priv->last_name);
	log->priv->last_name = g_strdup (last_name);
}

const gchar*
canvas_log_get_last_name (CanvasLog* log)
{
	return log->priv->last_name;
}

void
canvas_log_set_first_name (CanvasLog* log, const gchar* first_name)
{
	if (log->priv->first_name)
		g_free (log->priv->first_name);
	log->priv->first_name = g_strdup (first_name);
}

const gchar*
canvas_log_get_first_name (CanvasLog* log)
{
	return log->priv->first_name;
}

void
canvas_log_set_group (CanvasLog* log, guint elements)
{
	if (elements == 0)
		return;

	log->priv->group_elements = elements;
	log->priv->curr_group_element = elements;
}

void
canvas_log_add (CanvasLog* log, CanvasLessonTest* test, guint item_id, guint answer_id)
{
	GQuark test_quark = g_quark_from_string (canvas_lesson_test_get_id (test));

	CanvasLogAnswer* answer = g_new (CanvasLogAnswer, 1);
	answer->item_id = item_id;
	answer->answer_id = answer_id;
	answer->time_spent = g_timer_elapsed (log->priv->timer, NULL);

	if (log->priv->curr_group_element)
	{
		answer->time_spent /= log->priv->group_elements;
		log->priv->curr_group_element--;
	}

	if (log->priv->last_test == test_quark)
	{
		GList* last_test = g_list_last (log->priv->tests_list);
		last_test->data = g_list_append ((GList*)last_test->data, answer);
	}
	else
	{
		GList* answers_list = g_list_append (NULL, answer);
		CanvasLogTest* test = g_new (CanvasLogTest, 1);
		test->id = test_quark;
		test->answers = answers_list;
		log->priv->tests_list = g_list_append (log->priv->tests_list, test);
		log->priv->last_test = test_quark;
	}
}

void
canvas_log_timer_start (CanvasLog* log)
{
	g_timer_start (log->priv->timer);
}

void
canvas_log_timer_stop (CanvasLog* log)
{
	g_timer_stop (log->priv->timer);
}

GList*
canvas_log_get_tests (CanvasLog* log)
{
	return g_list_copy (log->priv->tests_list);
}

gboolean
canvas_log_open (CanvasLog* log,
                 const gchar* file,
                 GError** error)
{
	xmlNode *current_node, *parent_node;
	xmlChar *temp;

	xmlDocPtr doc = xmlParseFile (file);

	if (doc)
	{
		current_node = xmlDocGetRootElement (doc);

		if (current_node &&
		    current_node->name &&
		    xmlStrEqual (current_node->name, BAD_CAST "log"))
		{
			temp = xmlGetProp (current_node, BAD_CAST "last-name");
			if (temp)
			{
				canvas_log_set_last_name (log, (gchar*)temp);
				xmlFree (temp);
			}

			temp = xmlGetProp (current_node, BAD_CAST "first-name");
			if (temp)
			{
				canvas_log_set_first_name (log, (gchar*)temp);
				xmlFree (temp);
			}

			for (parent_node = current_node,
			     current_node = current_node->children;
			     current_node != NULL;
			     current_node = current_node->next)
			{
				if (current_node->type == XML_ELEMENT_NODE && 
				    xmlStrEqual (current_node->name, BAD_CAST "test"))
				{
					CanvasLogTest* test = g_new0 (CanvasLogTest, 1);
					log->priv->tests_list = g_list_append (log->priv->tests_list, test);

					temp = xmlGetProp (current_node, BAD_CAST "id");
					if (temp)
					{
						test->id = g_quark_from_string ((gchar*)temp);
						xmlFree (temp);
					}

					for (parent_node = current_node,
						 current_node = current_node->children;
						 current_node != NULL;
						 current_node = current_node->next)
					{
						if (current_node->type == XML_ELEMENT_NODE && 
							xmlStrEqual (current_node->name, BAD_CAST "item"))
						{
							CanvasLogAnswer* answer = g_new0 (CanvasLogAnswer, 1);
							test->answers = g_list_append (test->answers, answer);

							temp = xmlGetProp (current_node, BAD_CAST "id");
							if (temp)
							{
								answer->item_id = atoi ((gchar*)temp);
								xmlFree (temp);
							}

							temp = xmlGetProp (current_node, BAD_CAST "answer");
							if (temp)
							{
								answer->answer_id = atoi ((gchar*)temp);
								xmlFree (temp);
							}

							temp = xmlGetProp (current_node, BAD_CAST "time-spent");
							if (temp)
							{
								answer->time_spent = g_ascii_strtod ((gchar*)temp, NULL);
								xmlFree (temp);
							}
						}
					}

						current_node = parent_node;
						parent_node = current_node->parent;
				}
			}

			current_node = parent_node;
			parent_node = current_node->parent;
		}
		else
		{
			g_set_error (error, CANVAS_LOG_ERROR, CANVAS_LOG_ERROR_PARSE,
				         _("Failed to parse file."));
			return FALSE;
		}

		xmlFreeDoc (doc);
	}
	else
	{
		g_set_error (error, CANVAS_LOG_ERROR, CANVAS_LOG_ERROR_PARSE,
		             _("Failed to parse file."));
		return FALSE;
	}

	return TRUE;
}

void
canvas_log_save (CanvasLog* log,
                 const gchar* file,
                 GError** error)
{
	xmlNodePtr current_node;

	current_node = xmlNewNode (NULL, BAD_CAST "log");
	xmlSetProp (current_node, BAD_CAST "last-name", BAD_CAST log->priv->last_name);
	xmlSetProp (current_node, BAD_CAST "first-name", BAD_CAST log->priv->first_name);

	xmlSetNs (current_node, xmlNewNs (current_node,
	                                  BAD_CAST "http://kyoushuu.users.sourceforge.net/project_canvas",
	                                  BAD_CAST "canvas"));

	xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
	xmlDocSetRootElement (doc, current_node);

	g_list_foreach (log->priv->tests_list, (GFunc)add_test_node, current_node);

	xmlIndentTreeOutput = 1;
	xmlSaveFormatFileEnc(file, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
}
