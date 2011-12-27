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

#include <gtk/gtk.h>

#if HAVE_GSTREAMER
#include <gst/gst.h>
#endif

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"

#include "compat/compat-glib.h"

#define GPINSTRUCT_MESSAGE_TYPE_SIZE GPINSTRUCT_MESSAGE_TYPE_FAIL+1



struct _GPInstructMessagePoolPrivate
{
	GList* messages[GPINSTRUCT_MESSAGE_TYPE_SIZE];

#if HAVE_GSTREAMER
	GList* sounds[GPINSTRUCT_MESSAGE_TYPE_SIZE];
	GstElement *pipeline;
	GstElement *source;
#endif
};

#define GPINSTRUCT_MESSAGE_POOL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_MESSAGE_POOL, GPInstructMessagePoolPrivate))


#if HAVE_GSTREAMER
static gboolean
bus_call (GstBus     *bus,
          GstMessage *msg,
          gpointer    data)
{
	switch (GST_MESSAGE_TYPE (msg))
	{
		case GST_MESSAGE_ERROR:
		{
			gchar  *debug;
			GError *error;

			gst_message_parse_error (msg, &error, &debug);
			g_free (debug);

			g_warning ("Error: %s\n", error->message);
			g_error_free (error);
			break;
		}
		default:
			break;
	}

	return TRUE;
}

static void
on_pad_added (GstElement *element,
              GstPad     *pad,
              gpointer    data)
{
	GstPad *sinkpad;
	GstElement *decoder = (GstElement *) data;

	sinkpad = gst_element_get_static_pad (decoder, "sink");

	gst_pad_link (pad, sinkpad);

	gst_object_unref (sinkpad);
}
#endif


G_DEFINE_TYPE (GPInstructMessagePool, gpinstruct_message_pool, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_message_pool_init (GPInstructMessagePool *object)
{
#if HAVE_GSTREAMER
	GstElement *demuxer, *decoder, *conv, *sink;
#endif
	int i;

	object->priv = GPINSTRUCT_MESSAGE_POOL_PRIVATE (object);

	for (i = 0; i < GPINSTRUCT_MESSAGE_TYPE_SIZE; i++)
		object->priv->messages[i] = NULL;

#if HAVE_GSTREAMER
	if (!gst_is_initialized ())
		gst_init (NULL, NULL);

	for (i = 0; i < GPINSTRUCT_MESSAGE_TYPE_SIZE; i++)
		object->priv->sounds[i] = NULL;

	object->priv->pipeline = gst_pipeline_new ("audio-player");
	object->priv->source  = gst_element_factory_make ("filesrc", "file-source");
	demuxer = gst_element_factory_make ("oggdemux",      "ogg-demuxer");
	decoder = gst_element_factory_make ("vorbisdec",     "vorbis-decoder");
	conv    = gst_element_factory_make ("audioconvert",  "converter");
	sink    = gst_element_factory_make ("autoaudiosink", "audio-output");

	gst_bin_add_many (GST_BIN (object->priv->pipeline),
	                  object->priv->source, demuxer, decoder, conv, sink, NULL);

	gst_element_link (object->priv->source, demuxer);
	gst_element_link_many (decoder, conv, sink, NULL);
	g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), decoder);

	GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (object->priv->pipeline));
	gst_bus_add_watch (bus, bus_call, NULL);
	gst_object_unref (bus);
#endif
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

#if HAVE_GSTREAMER
	for (i = 0; i < GPINSTRUCT_MESSAGE_TYPE_SIZE; i++)
	{
		if (pool->priv->sounds[i])
			g_list_free_full (pool->priv->sounds[i], g_free);
	}

	gst_element_set_state (pool->priv->pipeline, GST_STATE_NULL);
	gst_object_unref (GST_OBJECT (pool->priv->pipeline));
#endif

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
	g_key_file_set_list_separator (key_file, ';');
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
			{GPINSTRUCT_MESSAGE_TYPE_WRONG_MOST,	"WrongMost"},
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

#if HAVE_GSTREAMER
			keys = g_key_file_get_string_list (key_file, "Sounds", groups[group].name, NULL, NULL);
			if (keys)
			{
				for (key = 0; keys[key] != NULL; key++)
				{
					gchar* path = NULL;

					if (g_path_is_absolute (keys[key]))
						path = g_strdup (keys[key]);
					else
					{
#ifdef G_OS_WIN32
						gchar* prefix = g_win32_get_package_installation_directory_of_module (NULL);
						path = g_build_filename (prefix, keys[key], NULL);
						g_free (prefix);
#else
						path = g_build_filename (PACKAGE_PREFIX, keys[key], NULL);
#endif
						if (!g_file_test (path, G_FILE_TEST_EXISTS))
						{
							gchar* dirname = g_path_get_dirname (file);
							g_free (path);
							path = g_build_filename (dirname, keys[key], NULL);
							g_free (dirname);
						}

						if (!g_file_test (path, G_FILE_TEST_EXISTS))
						{
							g_free (path);
							path = NULL;
						}
					}

					if (path)
						pool->priv->sounds[type] = g_list_append (pool->priv->sounds[type],
						                                          path);
				}

				g_strfreev (keys);
			}
#endif
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

void
gpinstruct_message_pool_play_sound_random (GPInstructMessagePool* pool,
                                           GPInstructMessageType type)
{
#if HAVE_GSTREAMER
	if (type <= GPINSTRUCT_MESSAGE_TYPE_NONE || type >= GPINSTRUCT_MESSAGE_TYPE_SIZE)
		return;

	GList* sounds = pool->priv->sounds[type];
	guint num_sounds = g_list_length (sounds);
	gint sound = 0;

	if (num_sounds == 0)
		return;
	else if (num_sounds == 1)
		sound = 0;
	else
	{
		g_random_set_seed (time (NULL));
		sound = g_random_int_range (0, num_sounds);
	}

	gst_element_set_state (pool->priv->pipeline, GST_STATE_NULL);
	g_object_set(pool->priv->source, "location", (gchar*)g_list_nth_data (sounds, sound), NULL);
	gst_element_set_state (pool->priv->pipeline, GST_STATE_PLAYING);
#endif
}

