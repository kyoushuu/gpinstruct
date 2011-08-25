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

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"

#define GPINSTRUCT_MESSAGE_TYPE_SIZE GPINSTRUCT_MESSAGE_TYPE_FAIL+1



struct _GPInstructMessagePoolPrivate
{
	GList* messages[GPINSTRUCT_MESSAGE_TYPE_SIZE];
};

#define GPINSTRUCT_MESSAGE_POOL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_MESSAGE_POOL, GPInstructMessagePoolPrivate))



G_DEFINE_TYPE (GPInstructMessagePool, gpinstruct_message_pool, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_message_pool_init (GPInstructMessagePool *object)
{
	object->priv = GPINSTRUCT_MESSAGE_POOL_PRIVATE (object);

	int i;
	for (i = 0; i < GPINSTRUCT_MESSAGE_TYPE_SIZE; i++)
		object->priv->messages[i] = NULL;
}

static void
gpinstruct_message_pool_finalize (GObject *object)
{
	GPInstructMessagePool* pool = GPINSTRUCT_MESSAGE_POOL (object);

	int i;
	for (i = 0; i < GPINSTRUCT_MESSAGE_TYPE_SIZE; i++)
	{
		if (pool->priv->messages[i])
			g_list_free_full (pool->priv->messages[i], g_free);
	}

	G_OBJECT_CLASS (gpinstruct_message_pool_parent_class)->finalize (object);
}

static void
gpinstruct_message_pool_class_init (GPInstructMessagePoolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass* parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructMessagePoolPrivate));

	object_class->finalize = gpinstruct_message_pool_finalize;
}


GPInstructMessagePool*
gpinstruct_message_pool_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_MESSAGE_POOL, NULL);
}

void
gpinstruct_message_pool_load_from_file (GPInstructMessagePool* pool,
                                        const gchar *file)
{
	GKeyFile* key_file = g_key_file_new ();
	g_key_file_set_list_separator (key_file, ',');
	GError* error = NULL;

	if (g_key_file_load_from_file (key_file, file, G_KEY_FILE_NONE, &error))
	{
		struct {
			GPInstructMessageType type;
			gchar* name;
		} groups[] = {
			{GPINSTRUCT_MESSAGE_TYPE_CORRECT,		"Correct"},
			{GPINSTRUCT_MESSAGE_TYPE_CORRECT_ALL,	"CorrectAll"},
			{GPINSTRUCT_MESSAGE_TYPE_WRONG,			"Wrong"},
			{GPINSTRUCT_MESSAGE_TYPE_WRONG_SOME,	"WrongSome"},
			{GPINSTRUCT_MESSAGE_TYPE_WRONG_ALL,		"WrongAll"},
			{GPINSTRUCT_MESSAGE_TYPE_PASS,			"Pass"},
			{GPINSTRUCT_MESSAGE_TYPE_FAIL,			"Fail"}
		};
		guint groups_num = G_N_ELEMENTS (groups);
		guint group;

		gchar** keys;
		guint key;

		GPInstructMessageType type;

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
gpinstruct_message_pool_get_random (GPInstructMessagePool* pool,
                                    GPInstructMessageType type)
{
	if (type <= GPINSTRUCT_MESSAGE_TYPE_NONE || type >= GPINSTRUCT_MESSAGE_TYPE_SIZE)
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
gpinstruct_message_pool_add (GPInstructMessagePool* pool,
                             GPInstructMessageType type,
                             const gchar* message)
{
	g_return_if_fail (type > GPINSTRUCT_MESSAGE_TYPE_NONE && type < GPINSTRUCT_MESSAGE_TYPE_SIZE);

	pool->priv->messages[type] = g_list_append (pool->priv->messages[type],
	                                            g_strdup (message));
}

void
gpinstruct_message_pool_add_multiple (GPInstructMessagePool* pool,
                                      ...)
{
	GPInstructMessageType type;

	va_list args;
	va_start (args, pool);

	while (TRUE)
	{
		type = va_arg (args, GPInstructMessageType);
		if (type <= GPINSTRUCT_MESSAGE_TYPE_NONE || type > GPINSTRUCT_MESSAGE_TYPE_SIZE)
			return;

		pool->priv->messages[type] = g_list_append (pool->priv->messages[type],
		                                            g_strdup (va_arg (args, gchar*)));
	}
	va_end (args);
}

void
gpinstruct_message_pool_remove (GPInstructMessagePool* pool,
                                GPInstructMessageType type,
                                guint message)
{
	g_return_if_fail (type > GPINSTRUCT_MESSAGE_TYPE_NONE && type < GPINSTRUCT_MESSAGE_TYPE_SIZE);

	GList* messages = pool->priv->messages[type];
	GList* selected = g_list_nth (messages, message);

	g_free (selected->data);
	pool->priv->messages[type] = g_list_delete_link (messages, selected);
}

const gchar*
gpinstruct_message_pool_get (GPInstructMessagePool* pool,
                             GPInstructMessageType type,
                             guint message)
{
	if (type <= GPINSTRUCT_MESSAGE_TYPE_NONE || type >= GPINSTRUCT_MESSAGE_TYPE_SIZE)
		return NULL;

	return g_list_nth_data (pool->priv->messages[type], message);
}
