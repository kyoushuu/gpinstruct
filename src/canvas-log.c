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
};

#define CANVAS_LOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LOG, CanvasLogPrivate))



typedef struct {
	CanvasLog* log;
	CanvasLogForeachFunc func;
	gpointer user_data;
} CanvasLogForeachFuncData;

static void
add_answer_node (CanvasLogAnswer* answer,
                 xmlNodePtr test_node)
{
	gchar *question_id = g_strdup_printf ("%d", answer->question_id);
	gchar *answer_id = g_strdup_printf ("%d", answer->answer_id);
	gchar *time_spent = g_strdup_printf ("%f", answer->time_spent);

	xmlNodePtr current_node = xmlNewChild (test_node, NULL, BAD_CAST "question", NULL);
	xmlSetProp (current_node, BAD_CAST "id", BAD_CAST question_id);
	xmlSetProp (current_node, BAD_CAST "answer", BAD_CAST answer_id);
	xmlSetProp (current_node, BAD_CAST "time-spent", BAD_CAST time_spent);

	g_free (question_id);
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

static void
canvas_log_foreach_test (CanvasLogTest* test,
                           CanvasLogForeachFuncData* foreach_data)
{
	GList* answers = g_list_copy (test->answers);
	foreach_data->func (g_quark_to_string (test->id), answers, foreach_data->user_data);
	g_list_free (answers);
}



G_DEFINE_TYPE (CanvasLog, canvas_log, CANVAS_TYPE_OBJECT);

static void
canvas_log_init (CanvasLog *object)
{
	object->priv = CANVAS_LOG_PRIVATE (object);

	object->priv->tests_list = NULL;

	object->priv->timer = g_timer_new ();

	object->priv->last_test = 0;
}

static void
canvas_log_finalize (GObject *object)
{
	CanvasLog* log = CANVAS_LOG (object);

	g_list_free_full (log->priv->tests_list, (GDestroyNotify)free_answers_foreach);

	g_timer_destroy (log->priv->timer);

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
canvas_log_add (CanvasLog* log, CanvasLessonTest* test, guint question_id, guint answer_id)
{
	GQuark test_quark = g_quark_from_string (canvas_lesson_test_get_id (test));

	CanvasLogAnswer* answer = g_new (CanvasLogAnswer, 1);
	answer->question_id = question_id;
	answer->answer_id = answer_id;
	answer->time_spent = g_timer_elapsed (log->priv->timer, NULL);

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

void
canvas_log_foreach (CanvasLog* log, CanvasLogForeachFunc func, gpointer user_data)
{
	CanvasLogForeachFuncData foreach_data = {log, func, user_data};
	g_list_foreach (log->priv->tests_list, (GFunc)canvas_log_foreach_test, &foreach_data);
}

void
canvas_log_save (CanvasLog* log,
                 const gchar* file,
                 GError** error)
{
	xmlNodePtr current_node;

	current_node = xmlNewNode (NULL, BAD_CAST "log");

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
