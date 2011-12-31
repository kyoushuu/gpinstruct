/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
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

#include <gtk/gtk.h>

#include <libsoup/soup.h>
#include <libsoup-soap/soup-soap.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-server/gpinstruct-server.h"


struct _GPInstructServerSessionPrivate
{
	gchar *project_file;
	GPInstructProject *project;

	gchar *log_folder;

	SoupServer *soap_server;
};

#define GPINSTRUCT_SERVER_SESSION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_SERVER_SESSION, GPInstructServerSessionPrivate))

enum
{
	PROP_0,

	PROP_PROJECT_FILE,
	PROP_LOG_FOLDER
};

enum
{
	CLIENT_SESSION_CREATED,
	CLIENT_SESSION_CLOSED,

	LAST_SIGNAL
};


static guint create_session (GPInstructServerSession *session, SoupMessage *msg, const gchar *lastname, const gchar *firstname, const gchar *password);
static guint get_project (GPInstructServerSession *session, SoupMessage *msg, const gchar *sessionID);
static guint set_log (GPInstructServerSession *session, SoupMessage *msg, const gchar *sessionID, const guchar *log, gsize log_len);
static guint close_session (GPInstructServerSession *session, SoupMessage *msg, const gchar *sessionID);

static gchar *
gen_session_id (void)
{
	static const gint length = 32;
	static const gchar lookup_table[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const gint lookup_table_elements = G_N_ELEMENTS (lookup_table);

	gchar *id = g_new (gchar, length+1);
	id[length] = 0;

	guint i;
	for (i=0; i<length; i++)
		id[i] = lookup_table[g_random_int_range (0, lookup_table_elements)];

	return id;
}

static guint
set_response_fault (SoupMessage *msg,
                    const gchar *format,
                    ...)
{
	va_list var_args;

	va_start (var_args, format);
	gchar *faultstring = g_strdup_vprintf (format, var_args);
	va_end (var_args);

	SoupSoapMessage *response = soup_soap_message_new_response (msg);

	soup_soap_message_set_operation_name (response, "Fault");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (response),
		                                soup_soap_param_new_value ("faultcode", "Server"),
		                                soup_soap_param_new_value ("faultstring", faultstring),
		                                NULL);

	soup_soap_message_persist (response);

	g_object_unref (response);

	return SOUP_STATUS_INTERNAL_SERVER_ERROR;
}

static guint
set_response_missing_param (SoupMessage *msg)
{
	return set_response_fault (msg, "Some parameters missing.");
}

static void
server_callback (SoupServer        *server,
                 SoupMessage       *msg,
                 const char        *path,
                 GHashTable        *query,
                 SoupClientContext *client,
                 gpointer           user_data)
{
	GPInstructServerSession *session = GPINSTRUCT_SERVER_SESSION (user_data);

	SoupSoapMessage *request = soup_soap_message_new_request (msg);
	guint response_status = SOUP_STATUS_OK;

	const gchar *operation_name = soup_soap_message_get_operation_name (request);

	if (g_strcmp0 (operation_name, "createSession") == 0)
	{
		SoupSoapParam *lastname_param = NULL;
		SoupSoapParam *firstname_param = NULL;
		SoupSoapParam *password_param = NULL;

		soup_soap_param_group_get_multiple (soup_soap_message_get_params (request),
			                                "lastname", &lastname_param,
			                                "firstname", &firstname_param,
			                                "password", &password_param,
			                                NULL);

		if (!lastname_param || !firstname_param || !password_param)
		{
			response_status = set_response_missing_param (msg);
		}
		else
		{
			response_status =
				create_session (session, msg,
				                soup_soap_param_get_value (lastname_param),
				                soup_soap_param_get_value (firstname_param),
				                soup_soap_param_get_value (password_param));
		}
	}
	else if (g_strcmp0 (operation_name, "getProject") == 0)
	{
		SoupSoapParam *sessionID = NULL;

		soup_soap_param_group_get_multiple (soup_soap_message_get_params (request),
			                                "sessionID", &sessionID,
			                                NULL);

		if (!sessionID)
		{
			response_status = set_response_missing_param (msg);
		}
		else
		{
			response_status =
				get_project (session, msg,
				             soup_soap_param_get_value (sessionID));
		}
	}
	else if (g_strcmp0 (operation_name, "setLog") == 0)
	{
		SoupSoapParam *sessionID = NULL;
		SoupSoapParam *log = NULL;

		soup_soap_param_group_get_multiple (soup_soap_message_get_params (request),
			                                "sessionID", &sessionID,
		                                    "log", &log,
			                                NULL);

		if (!sessionID || !log)
		{
			response_status = set_response_missing_param (msg);
		}
		else
		{
			guchar *log_contents;
			gsize log_contents_len;
			GError *error = NULL;

			log_contents = soup_soap_param_get_base64_binary (log,
			                                                  &log_contents_len,
			                                                  &error);
			if (error)
			{
				response_status =
					set_response_fault (msg,
					                    "Failed to parse base64 encoded string: %s",
					                    error->message);
				g_error_free (error);
			}
			else
			{
				response_status =
					set_log (session, msg,
					         soup_soap_param_get_value (sessionID),
					         log_contents, log_contents_len);
			}
		}
	}
	else if (g_strcmp0 (operation_name, "closeSession") == 0)
	{
		SoupSoapParam *sessionID = NULL;

		soup_soap_param_group_get_multiple (soup_soap_message_get_params (request),
			                                "sessionID", &sessionID,
			                                NULL);

		if (!sessionID)
		{
			response_status = set_response_missing_param (msg);
		}
		else
		{
			response_status =
				close_session (session, msg,
				               soup_soap_param_get_value (sessionID));
		}
	}
	else
	{
		response_status = set_response_fault (msg, "Unhandled message");
	}

	soup_message_set_status (msg, response_status);

	g_object_unref (request);
}

static guint
create_session (GPInstructServerSession *session,
                SoupMessage *msg,
                const gchar *lastname,
                const gchar *firstname,
                const gchar *password)
{
	gchar *sessionID = gen_session_id ();

	SoupSoapMessage *response = soup_soap_message_new_response (msg);

	soup_soap_message_set_operation_name (response, "createSessionResponse");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (response),
	                                    soup_soap_param_new_value ("sessionID", sessionID),
	                                    NULL);

	soup_soap_message_persist (response);

	g_object_unref (response);

	g_free (sessionID);

	return SOUP_STATUS_OK;
}

static guint
get_project (GPInstructServerSession *session,
             SoupMessage *msg,
             const gchar *sessionID)
{
	GPInstructServerSessionPrivate *priv = session->priv;

	SoupSoapMessage *response = soup_soap_message_new_response (msg);

	soup_soap_message_set_operation_name (response, "getProjectResponse");

	guchar *contents;
	gsize length;
	g_file_get_contents (priv->project_file,
	                     (gchar **) &contents,
	                     &length,
	                     NULL);

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (response),
	                                    soup_soap_param_new_base64_binary ("project", contents, length),
	                                    NULL);

	g_free (contents);

	soup_soap_message_persist (response);

	g_object_unref (response);

	return SOUP_STATUS_OK;
}

static guint
set_log (GPInstructServerSession *session,
         SoupMessage *msg,
         const gchar *sessionID,
         const guchar *log,
         gsize log_len)
{
	GPInstructServerSessionPrivate *priv = session->priv;

	SoupSoapMessage *response = soup_soap_message_new_response (msg);

	soup_soap_message_set_operation_name (response, "setLogResponse");

	gchar *log_filename = g_strdup_printf ("%s.gpinstruct-log", sessionID);
	gchar *log_file = g_build_filename (priv->log_folder, log_filename, NULL);
	g_file_set_contents (log_file,
	                     (gchar *) log,
	                     log_len,
	                     NULL);
	g_free (log_file);
	g_free (log_filename);

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (response),
	                                    soup_soap_param_new_boolean ("result", TRUE),
	                                    NULL);

	soup_soap_message_persist (response);

	g_object_unref (response);

	return SOUP_STATUS_OK;
}

static guint
close_session (GPInstructServerSession *session,
               SoupMessage *msg,
               const gchar *sessionID)
{
	SoupSoapMessage *response = soup_soap_message_new_response (msg);

	soup_soap_message_set_operation_name (response, "closeSessionResponse");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (response),
	                                    soup_soap_param_new_boolean ("result", TRUE),
	                                    NULL);

	soup_soap_message_persist (response);

	g_object_unref (response);

	return SOUP_STATUS_OK;
}


static guint server_session_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GPInstructServerSession, gpinstruct_server_session, G_TYPE_OBJECT);

static void
gpinstruct_server_session_init (GPInstructServerSession *object)
{
	object->priv = GPINSTRUCT_SERVER_SESSION_PRIVATE (object);
	GPInstructServerSessionPrivate *priv = object->priv;

	priv->project_file = NULL;
	priv->project = NULL;
	priv->log_folder = NULL;

	priv->soap_server = soup_server_new (SOUP_SERVER_SERVER_HEADER,
	                                     PACKAGE_TARNAME "-server ",
	                                     NULL);
}

static void
gpinstruct_server_session_finalize (GObject *object)
{
	GPInstructServerSession *session = GPINSTRUCT_SERVER_SESSION (object);
	GPInstructServerSessionPrivate *priv = session->priv;

	if (priv->soap_server)
	{
		soup_server_quit (priv->soap_server);
		g_object_unref (priv->soap_server);
		priv->soap_server = NULL;
    }

	g_free (priv->project_file);
	g_free (priv->log_folder);

	G_OBJECT_CLASS (gpinstruct_server_session_parent_class)->finalize (object);
}

static void
gpinstruct_server_session_constructed (GObject *object)
{
	GPInstructServerSession *session = GPINSTRUCT_SERVER_SESSION (object);
	GPInstructServerSessionPrivate *priv = session->priv;

	GError *error = NULL;

	GPInstructParser *parser = gpinstruct_parser_new ();

	priv->project = gpinstruct_parser_load_from_file (parser,
	                                                  priv->project_file,
	                                                  &error);
	if (error)
	{
		g_critical(_("Error: %s\n"), error->message);
		g_error_free (error);
		goto error;
	}

	if (priv->project == NULL)
		goto error;

	soup_server_add_handler (priv->soap_server,
	                         "/", server_callback,
	                         session, NULL); 
	soup_server_run_async (priv->soap_server);

	error:
		g_object_unref (parser);

	G_OBJECT_CLASS (gpinstruct_server_session_parent_class)->constructed (object);
}

static void
gpinstruct_server_session_set_property (GObject *object,
                                        guint prop_id,
                                        const GValue *value,
                                        GParamSpec *pspec)
{
	g_return_if_fail (GPINSTRUCT_IS_SERVER_SESSION (object));

	GPInstructServerSession *session = GPINSTRUCT_SERVER_SESSION (object);
	GPInstructServerSessionPrivate *priv = session->priv;

	switch (prop_id)
	{
		case PROP_PROJECT_FILE:
			g_free (priv->project_file);
			priv->project_file = g_value_dup_string (value);
			break;
		case PROP_LOG_FOLDER:
			gpinstruct_server_session_set_log_folder (session,
			                                          g_value_get_string (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gpinstruct_server_session_get_property (GObject *object,
                                        guint prop_id,
                                        GValue *value,
                                        GParamSpec *pspec)
{
	g_return_if_fail (GPINSTRUCT_IS_SERVER_SESSION (object));

	GPInstructServerSession *session = GPINSTRUCT_SERVER_SESSION (object);
	GPInstructServerSessionPrivate *priv = session->priv;

	switch (prop_id)
	{
		case PROP_PROJECT_FILE:
			g_value_set_string (value, priv->project_file);
			break;
		case PROP_LOG_FOLDER:
			g_value_set_string (value,
			                    gpinstruct_server_session_get_log_folder (session));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gpinstruct_server_session_client_session_created (GPInstructServerSession *self,
                                                  gchar *session_id,
                                                  gpointer user_data)
{
	/* TODO: Add default signal handler implementation here */
}

static void
gpinstruct_server_session_client_session_closed (GPInstructServerSession *self,
                                                 gchar *session_id,
                                                 gpointer user_data)
{
	/* TODO: Add default signal handler implementation here */
}

static void
gpinstruct_server_session_class_init (GPInstructServerSessionClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GObjectClass *parent_class = G_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructServerSessionPrivate));

	object_class->constructed = gpinstruct_server_session_constructed;
	object_class->finalize = gpinstruct_server_session_finalize;
	object_class->set_property = gpinstruct_server_session_set_property;
	object_class->get_property = gpinstruct_server_session_get_property;

	klass->client_session_created = gpinstruct_server_session_client_session_created;
	klass->client_session_closed = gpinstruct_server_session_client_session_closed;

	g_object_class_install_property (object_class,
	                                 PROP_PROJECT_FILE,
	                                 g_param_spec_string ("project-file",
	                                                      "Project file",
	                                                      "The filename of the project",
	                                                      NULL,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (object_class,
	                                 PROP_LOG_FOLDER,
	                                 g_param_spec_string ("log-folder",
	                                                      "Log folder",
	                                                      "Path to folder where log files will be saved",
	                                                      NULL,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

	server_session_signals[CLIENT_SESSION_CREATED] =
		g_signal_new ("client-session-created",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructServerSessionClass, client_session_created),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__STRING,
		              G_TYPE_NONE, 1,
		              G_TYPE_STRING);

	server_session_signals[CLIENT_SESSION_CLOSED] =
		g_signal_new ("client-session-closed",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GPInstructServerSessionClass, client_session_closed),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__STRING,
		              G_TYPE_NONE, 1,
		              G_TYPE_STRING);
}


GPInstructServerSession *
gpinstruct_server_session_new (const gchar *project_file,
                               const gchar *log_folder)
{
	return g_object_new (GPINSTRUCT_TYPE_SERVER_SESSION,
	                     "project-file", project_file,
	                     "log-folder", log_folder,
	                     NULL);
}

const gchar *
gpinstruct_server_session_get_project_filename (GPInstructServerSession *session)
{
	g_return_val_if_fail (GPINSTRUCT_IS_SERVER_SESSION (session), NULL);

	return session->priv->project_file;
}

const gchar *
gpinstruct_server_session_get_log_folder (GPInstructServerSession *session)
{
	g_return_val_if_fail (GPINSTRUCT_IS_SERVER_SESSION (session), NULL);

	return session->priv->log_folder;
}

void
gpinstruct_server_session_set_log_folder (GPInstructServerSession *session,
                                          const gchar *log_folder)
{
	g_return_if_fail (GPINSTRUCT_IS_SERVER_SESSION (session));
	g_return_if_fail (g_file_test (log_folder, G_FILE_TEST_IS_DIR));

	GPInstructServerSessionPrivate *priv = session->priv;

	g_free (priv->log_folder);
	priv->log_folder = g_strdup (log_folder);
}

GPInstructProject *
gpinstruct_server_session_get_project (GPInstructServerSession *session)
{
	g_return_val_if_fail (GPINSTRUCT_IS_SERVER_SESSION (session), 0);

	return session->priv->project;
}

guint
gpinstruct_server_session_get_port (GPInstructServerSession *session)
{
	g_return_val_if_fail (GPINSTRUCT_IS_SERVER_SESSION (session), 0);

	return soup_server_get_port (session->priv->soap_server);
}
