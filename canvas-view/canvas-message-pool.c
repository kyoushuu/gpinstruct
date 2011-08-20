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

#include "canvas/canvas.h"
#include "canvas-view/canvas-view.h"

#define CANVAS_MESSAGE_TYPE_SIZE CANVAS_MESSAGE_TYPE_FAIL+1



struct _CanvasMessagePoolPrivate
{
	GList* messages[CANVAS_MESSAGE_TYPE_SIZE];
};

#define CANVAS_MESSAGE_POOL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_MESSAGE_POOL, CanvasMessagePoolPrivate))



G_DEFINE_TYPE (CanvasMessagePool, canvas_message_pool, CANVAS_TYPE_OBJECT);

static void
canvas_message_pool_init (CanvasMessagePool *object)
{
	object->priv = CANVAS_MESSAGE_POOL_PRIVATE (object);

	int i;
	for (i = 0; i < CANVAS_MESSAGE_TYPE_SIZE; i++)
		object->priv->messages[i] = NULL;
}

static void
canvas_message_pool_finalize (GObject *object)
{
	CanvasMessagePool* pool = CANVAS_MESSAGE_POOL (object);

	int i;
	for (i = 0; i < CANVAS_MESSAGE_TYPE_SIZE; i++)
	{
		if (pool->priv->messages[i])
			g_list_free_full (pool->priv->messages[i], g_free);
	}

	G_OBJECT_CLASS (canvas_message_pool_parent_class)->finalize (object);
}

static void
canvas_message_pool_class_init (CanvasMessagePoolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasMessagePoolPrivate));

	object_class->finalize = canvas_message_pool_finalize;
}


CanvasMessagePool*
canvas_message_pool_new (void)
{
	return g_object_new (CANVAS_TYPE_MESSAGE_POOL, NULL);
}

void
canvas_message_pool_load_from_file (CanvasMessagePool* pool, const gchar *file)
{
	GKeyFile* key_file = g_key_file_new ();
	g_key_file_set_list_separator (key_file, ',');
	GError* error = NULL;

	if (g_key_file_load_from_file (key_file, file, G_KEY_FILE_NONE, &error))
	{
		struct {
			CanvasMessageType type;
			gchar* name;
		} groups[] = {
			{CANVAS_MESSAGE_TYPE_CORRECT,		"Correct"},
			{CANVAS_MESSAGE_TYPE_CORRECT_ALL,	"CorrectAll"},
			{CANVAS_MESSAGE_TYPE_WRONG,			"Wrong"},
			{CANVAS_MESSAGE_TYPE_WRONG_SOME,	"WrongSome"},
			{CANVAS_MESSAGE_TYPE_WRONG_ALL,		"WrongAll"},
			{CANVAS_MESSAGE_TYPE_PASS,			"Pass"},
			{CANVAS_MESSAGE_TYPE_FAIL,			"Fail"}
		};
		guint groups_num = G_N_ELEMENTS (groups);
		guint group;

		gchar** keys;
		guint key;

		CanvasMessageType type;

		for (group = 0; group < groups_num; group++)
		{
			type = groups[group].type;
			keys = g_key_file_get_string_list (key_file, "Messages", groups[group].name, NULL, NULL);

			if (keys)
			{
				for (key = 0; keys[key] != NULL; key++)
				{
					pool->priv->messages[type] = g_list_append (pool->priv->messages[type],
					                                            g_strdup (keys[key]));
				}

				g_strfreev (keys);
			}
		}
	}
	else
	{
		g_warning ("Error: %s", error->message);
		g_error_free (error);
	}

	g_key_file_free (key_file);
}

const gchar*
canvas_message_pool_get_random (CanvasMessagePool* pool, CanvasMessageType type)
{
	if (type <= CANVAS_MESSAGE_TYPE_NONE || type >= CANVAS_MESSAGE_TYPE_SIZE)
		return NULL;

	GList* messages = pool->priv->messages[type];
	guint num_messages = g_list_length (messages);

	if (num_messages == 0)
		return NULL;
	else if (num_messages == 1)
		return messages->data;

	g_random_set_seed (time (NULL));

	return g_list_nth_data (messages, g_random_int_range (0, num_messages));
}

void
canvas_message_pool_add (CanvasMessagePool* pool, CanvasMessageType type, const gchar* message)
{
	g_return_if_fail (type > CANVAS_MESSAGE_TYPE_NONE && type < CANVAS_MESSAGE_TYPE_SIZE);

	pool->priv->messages[type] = g_list_append (pool->priv->messages[type],
	                                            g_strdup (message));
}

void
canvas_message_pool_add_multiple (CanvasMessagePool* pool, ...)
{
	CanvasMessageType type;

	va_list args;
	va_start (args, pool);

	while (TRUE)
	{
		type = va_arg (args, CanvasMessageType);
		if (type <= CANVAS_MESSAGE_TYPE_NONE || type > CANVAS_MESSAGE_TYPE_SIZE)
			return;

		pool->priv->messages[type] = g_list_append (pool->priv->messages[type],
		                                            g_strdup (va_arg (args, gchar*)));
	}
	va_end (args);
}

void
canvas_message_pool_remove (CanvasMessagePool* pool, CanvasMessageType type, guint message)
{
	g_return_if_fail (type > CANVAS_MESSAGE_TYPE_NONE && type < CANVAS_MESSAGE_TYPE_SIZE);

	GList* messages = pool->priv->messages[type];
	GList* selected = g_list_nth (messages, message);

	g_free (selected->data);
	pool->priv->messages[type] = g_list_delete_link (messages, selected);
}

const gchar*
canvas_message_pool_get (CanvasMessagePool* pool, CanvasMessageType type, guint message)
{
	if (type <= CANVAS_MESSAGE_TYPE_NONE || type >= CANVAS_MESSAGE_TYPE_SIZE)
		return NULL;

	return g_list_nth_data (pool->priv->messages[type], message);
}
