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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"

#include "compat/compat-glib.h"

struct _GPInstructLogPrivate
{
	GList *tests_list;
	GTimer *timer;
	GQuark last_test;

	gchar *last_name;
	gchar *first_name;

	guint group_elements;
	guint curr_group_element;
};

#define GPINSTRUCT_LOG_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LOG, GPInstructLogPrivate))


GQuark
gpinstruct_log_error_quark (void)
{
	return g_quark_from_static_string ("gpinstruct-log-error-quark");
}



typedef struct {
	GPInstructLog *log;
	GPInstructLogForeachFunc func;
	gpointer user_data;
} GPInstructLogForeachFuncData;

static void
add_answer_node (GPInstructLogAnswer *answer,
                 xmlNodePtr test_node)
{
	gchar *item_id = g_strdup_printf ("%d", answer->item_id);
	gchar *answer_id = g_strdup_printf ("%d", answer->answer_id);
	gchar *time_spent = g_strdup_printf ("%f", answer->time_spent);

	xmlNodePtr current_node = xmlNewChild (test_node, NULL, BAD_CAST "item", NULL);
	xmlSetProp (current_node, BAD_CAST "id", BAD_CAST item_id);
	xmlSetProp (current_node, BAD_CAST "answer-id", BAD_CAST answer_id);
	if (answer->answer_string)
		xmlSetProp (current_node, BAD_CAST "answer-string", BAD_CAST answer->answer_string);
	xmlSetProp (current_node, BAD_CAST "time-spent", BAD_CAST time_spent);

	g_free (item_id);
	g_free (answer_id);
	g_free (time_spent);
}

static void
add_test_node (GPInstructLogTest *test,
               xmlNodePtr log_node)
{
	xmlNodePtr current_node = xmlNewChild (log_node, NULL, BAD_CAST "test", NULL);
	xmlSetProp (current_node, BAD_CAST "id", BAD_CAST g_quark_to_string (test->id));

	g_list_foreach (test->answers, (GFunc)add_answer_node, current_node);
}

static void
free_answer (GPInstructLogAnswer *answer)
{
	g_free (answer->answer_string);
	g_free (answer);
}

static void
free_answers_foreach (GPInstructLogTest *test)
{
	g_list_foreach (test->answers, (GFunc) free_answer, NULL);
	g_list_free (test->answers);
	g_free (test);
}



G_DEFINE_TYPE (GPInstructLog, gpinstruct_log, G_TYPE_OBJECT);

static void
gpinstruct_log_init (GPInstructLog *object)
{
	object->priv = GPINSTRUCT_LOG_GET_PRIVATE (object);
	GPInstructLogPrivate *priv = object->priv;

	priv->tests_list = NULL;

	priv->timer = g_timer_new ();

	priv->last_test = 0;

	priv->last_name = NULL;
	priv->first_name = NULL;

	priv->group_elements = 1;
	priv->curr_group_element = 0;
}

static void
gpinstruct_log_finalize (GObject *object)
{
	GPInstructLog *log = GPINSTRUCT_LOG (object);
	GPInstructLogPrivate *priv = log->priv;

	g_list_free_full (priv->tests_list, (GDestroyNotify)free_answers_foreach);

	g_timer_destroy (priv->timer);

	g_free (priv->last_name);
	g_free (priv->first_name);

	G_OBJECT_CLASS (gpinstruct_log_parent_class)->finalize (object);
}

static void
gpinstruct_log_class_init (GPInstructLogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GObjectClass *parent_class = G_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLogPrivate));

	object_class->finalize = gpinstruct_log_finalize;
}


GPInstructLog *
gpinstruct_log_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LOG, NULL);
}

void
gpinstruct_log_set_last_name (GPInstructLog *log,
                              const gchar *last_name)
{
	GPInstructLogPrivate *priv = log->priv;

	g_free (priv->last_name);
	priv->last_name = g_strdup (last_name);
}

const gchar *
gpinstruct_log_get_last_name (GPInstructLog *log)
{
	GPInstructLogPrivate *priv = log->priv;

	return priv->last_name;
}

void
gpinstruct_log_set_first_name (GPInstructLog *log,
                               const gchar *first_name)
{
	GPInstructLogPrivate *priv = log->priv;

	g_free (priv->first_name);
	priv->first_name = g_strdup (first_name);
}

const gchar *
gpinstruct_log_get_first_name (GPInstructLog *log)
{
	GPInstructLogPrivate *priv = log->priv;

	return priv->first_name;
}

void
gpinstruct_log_set_group (GPInstructLog *log,
                          guint elements)
{
	GPInstructLogPrivate *priv = log->priv;

	if (elements == 0)
		return;

	priv->group_elements = elements;
	priv->curr_group_element = elements;
}

void
gpinstruct_log_add_choice (GPInstructLog *log,
                           GPInstructLessonTest *test,
                           guint item_id, guint answer_id)
{
	GPInstructLogPrivate *priv = log->priv;

	GQuark test_quark = g_quark_from_string (gpinstruct_lesson_test_get_id (test));

	GPInstructLogAnswer *answer = g_new0 (GPInstructLogAnswer, 1);
	answer->item_id = item_id;
	answer->answer_id = answer_id;
	answer->time_spent = g_timer_elapsed (priv->timer, NULL);

	if (priv->curr_group_element)
	{
		answer->time_spent /= priv->group_elements;
		priv->curr_group_element--;
	}

	if (priv->last_test == test_quark)
	{
		GList *last_test = g_list_last (priv->tests_list);
		last_test->data = g_list_append ((GList*) last_test->data, answer);
	}
	else
	{
		GList *answers_list = g_list_append (NULL, answer);
		GPInstructLogTest *test = g_new (GPInstructLogTest, 1);
		test->id = test_quark;
		test->answers = answers_list;
		priv->tests_list = g_list_append (priv->tests_list, test);
		priv->last_test = test_quark;
	}
}

void
gpinstruct_log_add_string (GPInstructLog *log,
                           GPInstructLessonTest *test,
                           guint item_id, const gchar *answer_string)
{
	GPInstructLogPrivate *priv = log->priv;

	GQuark test_quark = g_quark_from_string (gpinstruct_lesson_test_get_id (test));

	GPInstructLogAnswer *answer = g_new0 (GPInstructLogAnswer, 1);
	answer->item_id = item_id;
	answer->answer_string = g_strdup (answer_string);
	answer->time_spent = g_timer_elapsed (priv->timer, NULL);

	if (priv->curr_group_element)
	{
		answer->time_spent /= priv->group_elements;
		priv->curr_group_element--;
	}

	if (priv->last_test == test_quark)
	{
		GList *last_test = g_list_last (priv->tests_list);
		last_test->data = g_list_append ((GList*) last_test->data, answer);
	}
	else
	{
		GList *answers_list = g_list_append (NULL, answer);
		GPInstructLogTest *test = g_new (GPInstructLogTest, 1);
		test->id = test_quark;
		test->answers = answers_list;
		priv->tests_list = g_list_append (priv->tests_list, test);
		priv->last_test = test_quark;
	}
}

void
gpinstruct_log_close_test (GPInstructLog *log)
{
	GPInstructLogPrivate *priv = log->priv;

	priv->last_test = 0;
}

void
gpinstruct_log_timer_start (GPInstructLog *log)
{
	GPInstructLogPrivate *priv = log->priv;

	g_timer_start (priv->timer);
}

void
gpinstruct_log_timer_stop (GPInstructLog *log)
{
	GPInstructLogPrivate *priv = log->priv;

	g_timer_stop (priv->timer);
}

GList *
gpinstruct_log_get_tests (GPInstructLog *log)
{
	GPInstructLogPrivate *priv = log->priv;

	return g_list_copy (priv->tests_list);
}

gboolean
load_log_from_xml_document (GPInstructLog *log,
                            xmlDocPtr doc)
{
	xmlNode *current_node, *parent_node;
	xmlChar *temp;

	GPInstructLogPrivate *priv = log->priv;

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
				gpinstruct_log_set_last_name (log, (gchar*) temp);
				xmlFree (temp);
			}

			temp = xmlGetProp (current_node, BAD_CAST "first-name");
			if (temp)
			{
				gpinstruct_log_set_first_name (log, (gchar*) temp);
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
					GPInstructLogTest *test = g_new0 (GPInstructLogTest, 1);
					priv->tests_list = g_list_append (priv->tests_list, test);

					temp = xmlGetProp (current_node, BAD_CAST "id");
					if (temp)
					{
						test->id = g_quark_from_string ((gchar*) temp);
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
							GPInstructLogAnswer *answer = g_new0 (GPInstructLogAnswer, 1);
							test->answers = g_list_append (test->answers, answer);

							temp = xmlGetProp (current_node, BAD_CAST "id");
							if (temp)
							{
								answer->item_id = atoi ((gchar*) temp);
								xmlFree (temp);
							}

							temp = xmlGetProp (current_node, BAD_CAST "answer-id");
							if (temp)
							{
								answer->answer_id = atoi ((gchar*) temp);
								xmlFree (temp);
							}

							temp = xmlGetProp (current_node, BAD_CAST "answer-string");
							if (temp)
							{
								answer->answer_string = g_strdup ((gchar*) temp);
								xmlFree (temp);
							}

							temp = xmlGetProp (current_node, BAD_CAST "time-spent");
							if (temp)
							{
								answer->time_spent = g_ascii_strtod ((gchar*) temp, NULL);
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

			return TRUE;
		}
	}

	return FALSE;
}

xmlDocPtr
create_xml_document_from_log (GPInstructLog *log)
{
	xmlNodePtr current_node;

	GPInstructLogPrivate *priv = log->priv;

	current_node = xmlNewNode (NULL, BAD_CAST "log");
	xmlSetProp (current_node, BAD_CAST "last-name", BAD_CAST priv->last_name);
	xmlSetProp (current_node, BAD_CAST "first-name", BAD_CAST priv->first_name);

	xmlSetNs (current_node, xmlNewNs (current_node,
	                                  BAD_CAST PACKAGE_URL,
	                                  BAD_CAST PACKAGE_TARNAME));

	xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
	xmlDocSetRootElement (doc, current_node);

	g_list_foreach (priv->tests_list, (GFunc)add_test_node, current_node);

	return doc;
}

gboolean
gpinstruct_log_load_from_file (GPInstructLog *log,
                               const gchar *file,
                               GError **error)
{
	gboolean load;

	xmlDocPtr doc = xmlParseFile (file);

	if (!doc)
	{
		g_set_error (error, GPINSTRUCT_LOG_ERROR, GPINSTRUCT_LOG_ERROR_PARSE,
		             _("Failed to parse file."));
		return FALSE;
	}

	load = load_log_from_xml_document (log, doc);

	xmlFreeDoc (doc);

	if (!load)
	{
		g_set_error (error, GPINSTRUCT_LOG_ERROR, GPINSTRUCT_LOG_ERROR_INVALID,
		             _("File is not a valid log."));
		return FALSE;
	}

	return TRUE;
}

gboolean
gpinstruct_log_load_from_string (GPInstructLog *log,
                                 const gchar *contents,
                                 GError **error)
{
	gboolean load;

	xmlDocPtr doc = xmlParseDoc (BAD_CAST contents);

	if (!doc)
	{
		g_set_error (error, GPINSTRUCT_LOG_ERROR, GPINSTRUCT_LOG_ERROR_PARSE,
		             _("Failed to parse file."));
		return FALSE;
	}

	load = load_log_from_xml_document (log, doc);

	xmlFreeDoc (doc);

	if (!load)
	{
		g_set_error (error, GPINSTRUCT_LOG_ERROR, GPINSTRUCT_LOG_ERROR_INVALID,
		             _("File is not a valid log."));
		return FALSE;
	}

	return TRUE;
}

void
gpinstruct_log_save_to_file (GPInstructLog *log,
                             const gchar *file,
                             GError **error)
{
	xmlDocPtr doc = create_xml_document_from_log (log);

	if (!doc)
	{
		g_set_error (error, GPINSTRUCT_PARSER_ERROR, GPINSTRUCT_PARSER_ERROR_PARSE,
		             _("Failed to create an XML document from log."));
		return;
	}

	xmlIndentTreeOutput = 1;
	xmlTreeIndentString = "\t";
	xmlSetDocCompressMode (doc, 9);
	xmlSaveFormatFileEnc (file, doc, "UTF-8", 1);
	xmlFreeDoc (doc);
}

gchar *
gpinstruct_log_save_to_string (GPInstructLog *log,
                               GError **error)
{
	xmlChar *buffer;
	gchar *contents;

	xmlDocPtr doc = create_xml_document_from_log (log);

	if (!doc)
	{
		g_set_error (error, GPINSTRUCT_PARSER_ERROR, GPINSTRUCT_PARSER_ERROR_PARSE,
		             _("Failed to create an XML document from log."));
		return NULL;
	}

	xmlIndentTreeOutput = 1;
	xmlTreeIndentString = "\t";
	xmlDocDumpFormatMemoryEnc (doc, &buffer, NULL, "UTF-8", 1);
	contents = g_strdup ((char*) buffer);
	xmlFree (buffer);

	xmlFreeDoc (doc);

	return contents;
}
