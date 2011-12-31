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
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-client/gpinstruct-client.h"

#include "compat/compat-gtk.h"

struct _GPInstructClientWindowPrivate
{
	SoupSession *session;

	gchar *server_uri;
	gchar *session_id;

	GPInstructLog *log;

	gchar *lastname;
	gchar *firstname;
	gchar *password;;

	GtkWidget *server_address_entry;
	GtkWidget *lastname_entry;
	GtkWidget *firstname_entry;
	GtkWidget *password_entry;

	GtkWidget *info_bar;
	GtkWidget *info_bar_label;
	GtkWidget *info_bar_spinner;

	GtkWidget *create_session_button;
	GtkWidget *get_project_button;
	GtkWidget *set_log_button;
	GtkWidget *close_session_button;
};

#define GPINSTRUCT_CLIENT_WINDOW_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_CLIENT_WINDOW, GPInstructClientWindowPrivate))


static void create_session (GPInstructClientWindow *window);
static void get_project (GPInstructClientWindow *window);
static void set_log (GPInstructClientWindow *window);
static void close_session (GPInstructClientWindow *window);

static void got_create_session_response (SoupSession *session, SoupMessage *msg, gpointer user_data);
static void got_get_project_response (SoupSession *session, SoupMessage *msg, gpointer user_data);
static void got_set_log_response (SoupSession *session, SoupMessage *msg, gpointer user_data);
static void got_close_session_response (SoupSession *session, SoupMessage *msg, gpointer user_data);



#define GPINSTRUCT_CLIENT_ERROR gpinstruct_client_error_quark ()

typedef enum
{
	GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED = 1,
	GPINSTRUCT_CLIENT_ERROR_PARSE_RESPONSE_FAILED,
	GPINSTRUCT_CLIENT_ERROR_SERVER_ERROR
} GPInstructClientError;

GQuark
gpinstruct_client_error_quark (void)
{
	return g_quark_from_static_string ("gpinstruct-client-error-quark");
}


typedef enum
{
	GPINSTRUCT_CLIENT_REQUEST_NONE,
	GPINSTRUCT_CLIENT_REQUEST_CREATE_SESSION,
	GPINSTRUCT_CLIENT_REQUEST_GET_PROJECT,
	GPINSTRUCT_CLIENT_REQUEST_SET_LOG,
	GPINSTRUCT_CLIENT_REQUEST_CLOSE_SESSION
} GPInstructClientRequestType;


static void
set_message (GPInstructClientWindow *window,
             GtkMessageType message_type,
             GPInstructClientRequestType request_type,
             gboolean spinner,
             const gchar *message)
{
	GPInstructClientWindowPrivate *priv = window->priv;

	gtk_widget_show_all (GTK_WIDGET (window));

	gtk_widget_hide (priv->info_bar);

	if (message == NULL || *message == '\0')
		return;

	gtk_label_set_markup (GTK_LABEL (priv->info_bar_label), message);
	gtk_widget_show (priv->info_bar_label);
	gtk_widget_show (priv->info_bar);

	if (spinner)
	{
		gtk_widget_show (priv->info_bar_spinner);
		gtk_spinner_start (GTK_SPINNER (priv->info_bar_spinner));
	}
	else
	{
		gtk_widget_hide (priv->info_bar_spinner);
		gtk_spinner_stop (GTK_SPINNER (priv->info_bar_spinner));
	}

	gtk_info_bar_set_message_type (GTK_INFO_BAR (priv->info_bar), message_type);

	gtk_widget_hide (priv->create_session_button);
	gtk_widget_hide (priv->get_project_button);
	gtk_widget_hide (priv->set_log_button);
	gtk_widget_hide (priv->close_session_button);

	switch (request_type)
	{
		case GPINSTRUCT_CLIENT_REQUEST_CREATE_SESSION:
			gtk_widget_show (priv->create_session_button);
			break;
		case GPINSTRUCT_CLIENT_REQUEST_GET_PROJECT:
			gtk_widget_show (priv->get_project_button);
			break;
		case GPINSTRUCT_CLIENT_REQUEST_SET_LOG:
			gtk_widget_show (priv->set_log_button);
			break;
		case GPINSTRUCT_CLIENT_REQUEST_CLOSE_SESSION:
			gtk_widget_show (priv->close_session_button);
			break;
		default:
			break;
	}
}


static void
create_session (GPInstructClientWindow *window)
{
	GPInstructClientWindowPrivate *priv = window->priv;
	GError *error = NULL;

	set_message (window,
	             GTK_MESSAGE_INFO,
	             GPINSTRUCT_CLIENT_REQUEST_NONE,
	             TRUE,
	             _("Logging in"));

	SoupMessage *msg = soup_message_new ("POST", priv->server_uri);
	if (!msg)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	SoupSoapMessage *request = soup_soap_message_new_request (msg);

	soup_soap_message_set_operation_name (request, "createSession");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (request),
	                                    soup_soap_param_new_value ("lastname", priv->lastname),
	                                    soup_soap_param_new_value ("firstname", priv->firstname),
	                                    soup_soap_param_new_value ("password", priv->password),
	                                    NULL);

	soup_soap_message_persist (request);

	g_object_unref (request);

	soup_session_queue_message (priv->session, msg,
	                            got_create_session_response, window);

	error:
		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to create session</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_CREATE_SESSION,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
get_project (GPInstructClientWindow *window)
{
	GPInstructClientWindowPrivate *priv = window->priv;
	GError *error = NULL;

	set_message (window,
	             GTK_MESSAGE_INFO,
	             GPINSTRUCT_CLIENT_REQUEST_NONE,
	             TRUE,
	             _("Getting project data"));

	SoupMessage *msg = soup_message_new ("POST", priv->server_uri);
	if (!msg)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	SoupSoapMessage *request = soup_soap_message_new_request (msg);

	soup_soap_message_set_operation_name (request, "getProject");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (request),
	                                    soup_soap_param_new_value ("sessionID", priv->session_id),
	                                    NULL);

	soup_soap_message_persist (request);

	g_object_unref (request);

	soup_session_queue_message (priv->session, msg,
	                            got_get_project_response, window);

	error:
		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to get project</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_GET_PROJECT,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
set_log (GPInstructClientWindow *window)
{
	GPInstructClientWindowPrivate *priv = window->priv;
	GError *error = NULL;

	set_message (window,
	             GTK_MESSAGE_INFO,
	             GPINSTRUCT_CLIENT_REQUEST_NONE,
	             TRUE,
	             _("Sending log to server"));

	gchar *log = gpinstruct_log_save_to_string (priv->log, &error);
	if (error) goto error;

	SoupMessage *msg = soup_message_new ("POST", priv->server_uri);
	if (!msg)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	SoupSoapMessage *request = soup_soap_message_new_request (msg);

	soup_soap_message_set_operation_name (request, "setLog");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (request),
	                                    soup_soap_param_new_value ("sessionID", priv->session_id),
	                                    soup_soap_param_new_base64_string ("log", log),
	                                    NULL);

	soup_soap_message_persist (request);

	g_object_unref (request);

	soup_session_queue_message (priv->session, msg,
	                            got_set_log_response, window);

	g_free (log);

	error:
		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to send log:</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_SET_LOG,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
close_session (GPInstructClientWindow *window)
{
	GPInstructClientWindowPrivate *priv = window->priv;
	GError *error = NULL;

	set_message (window,
	             GTK_MESSAGE_INFO,
	             GPINSTRUCT_CLIENT_REQUEST_NONE,
	             TRUE,
	             _("Closing session"));

	SoupMessage *msg = soup_message_new ("POST", priv->server_uri);
	if (!msg)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	SoupSoapMessage *request = soup_soap_message_new_request (msg);

	soup_soap_message_set_operation_name (request, "closeSession");

	soup_soap_param_group_add_multiple (soup_soap_message_get_params (request),
	                                    soup_soap_param_new_value ("sessionID", priv->session_id),
	                                    NULL);

	soup_soap_message_persist (request);

	g_object_unref (request);

	soup_session_queue_message (priv->session, msg,
	                            got_close_session_response, window);

	error:
		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to close session:</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_CLOSE_SESSION,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static gboolean
parse_response (SoupMessage *msg,
                SoupSoapMessage **response_return,
                GPInstructClientWindow *window,
                GError **error)
{
	SoupSoapMessage *response = NULL;
	SoupSoapParam *param;

	if (SOUP_STATUS_IS_SERVER_ERROR (msg->status_code))
	{
		response = soup_soap_message_new_response (msg);
		const gchar *op_name = soup_soap_message_get_operation_name (response);
		if (op_name == NULL || *op_name == '\0')
		{
			g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
			                     GPINSTRUCT_CLIENT_ERROR_PARSE_RESPONSE_FAILED,
			                     _("Could not parse SOAP response"));
			goto error;
		}

		param = soup_soap_param_group_get (soup_soap_message_get_params (response),
		                                   "faultstring");
		gchar *faultstring = soup_soap_param_get_string (param, NULL);

		g_set_error (error, GPINSTRUCT_CLIENT_ERROR,
		             GPINSTRUCT_CLIENT_ERROR_SERVER_ERROR,
		             _("Server error: %s"),
		             faultstring);
		g_free (faultstring);
		goto error;
	}
	else if (!SOUP_STATUS_IS_SUCCESSFUL (msg->status_code))
	{
		g_set_error (error, GPINSTRUCT_CLIENT_ERROR, 0, "%d %s",
		             msg->status_code, msg->reason_phrase);
		goto error;
	}

	response = soup_soap_message_new_response (msg);
	const gchar *op_name = soup_soap_message_get_operation_name (response);
	if (op_name == NULL || *op_name == '\0')
	{
		g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_PARSE_RESPONSE_FAILED,
		                     _("Could not parse SOAP response"));
		goto error;
	}

	*response_return = response;
	return TRUE;

	error:
		if (response) g_object_unref (response);
		return FALSE;
}


static void
got_close_session_response (SoupSession *session,
                            SoupMessage *msg,
                            gpointer user_data)
{
	SoupSoapMessage *response = NULL;
	SoupSoapParam *param;
	GPInstructClientWindow *window = user_data;

	GError *error = NULL;
	GtkWidget *dialog;

	if (!parse_response (msg, &response, window, &error))
		goto error;

	param = soup_soap_param_group_get (soup_soap_message_get_params (response),
	                                   "result");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	if (!soup_soap_param_get_boolean (param, NULL))
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Server failed to process request."));
		goto error;
	}

	dialog = gtk_message_dialog_new (NULL,
	                                 GTK_DIALOG_DESTROY_WITH_PARENT,
	                                 GTK_MESSAGE_INFO,
	                                 GTK_BUTTONS_OK,
	                                 _("Sent log to server."));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_main_quit ();
	gtk_widget_destroy (dialog);


	error:
		if (response)
			g_object_unref (response);

		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to close session:</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_CLOSE_SESSION,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
got_set_log_response (SoupSession *session,
                      SoupMessage *msg,
                      gpointer user_data)
{
	SoupSoapMessage *response = NULL;
	SoupSoapParam *param;
	GPInstructClientWindow *window = user_data;

	GError *error = NULL;

	if (!parse_response (msg, &response, window, &error))
		goto error;

	param = soup_soap_param_group_get (soup_soap_message_get_params (response),
	                                   "result");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	if (!soup_soap_param_get_boolean (param, NULL))
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Server failed to process request."));
		goto error;
	}

	close_session (window);

	error:
		if (response)
			g_object_unref (response);

		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to send log:</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_SET_LOG,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
project_view_quit (GtkWidget *view,
                   gpointer user_data)
{
	GPInstructClientWindow *window = user_data;

	gtk_widget_hide (view);

	set_log (window);
}


static void
got_get_project_response (SoupSession *session,
                          SoupMessage *msg,
                          gpointer user_data)
{
	SoupSoapMessage *response = NULL;
	SoupSoapParam *param;
	GPInstructClientWindow *window = user_data;
	GPInstructClientWindowPrivate *priv = window->priv;

	GError *error = NULL;

	if (!parse_response (msg, &response, window, &error))
		goto error;

	param = soup_soap_param_group_get (soup_soap_message_get_params (response),
	                                   "project");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	gsize project_size;
	gchar *project_string;
	guchar *project_bin = soup_soap_param_get_base64_binary (param, &project_size, &error);
	if (error) goto error;

	/* Check if compressed using GZip */
	if (project_size >= 2 && project_bin[0] == 0x1f && project_bin[1] == 0x8b)
	{
		/* Create input stream then decompress */
		GInputStream *istream = g_memory_input_stream_new_from_data (project_bin, project_size, g_free);
		GConverter *decompressor = G_CONVERTER (g_zlib_decompressor_new (G_ZLIB_COMPRESSOR_FORMAT_GZIP));
		GInputStream *cistream = g_converter_input_stream_new (istream, decompressor);

		/* Get output stream to decompressed data */
		GOutputStream *ostream = g_memory_output_stream_new (NULL, 0, g_realloc, g_free);
		g_output_stream_splice (ostream, cistream, 0, NULL, &error);

		if (!error)
		{
			project_size = g_memory_output_stream_get_data_size (G_MEMORY_OUTPUT_STREAM (ostream));
			project_string = g_strndup (g_memory_output_stream_get_data (G_MEMORY_OUTPUT_STREAM (ostream)),
			                            project_size);
		}

		g_object_unref (istream);
		g_converter_reset (decompressor);
		g_object_unref (decompressor);
		g_object_unref (cistream);
		g_object_unref (ostream);

		if (error) goto error;
	}
	else
	{
		project_string = g_strndup ((gchar*) project_bin, project_size);
		g_free (project_bin);
	}

	GPInstructParser *parser = gpinstruct_parser_new ();

	GPInstructProject *project = gpinstruct_parser_load_from_string (parser,
	                                                                 project_string,
	                                                                 &error);
	if (error) goto error;

	g_free (project_string);

	g_object_unref (parser);

	priv->log = gpinstruct_log_new ();
	gpinstruct_log_set_last_name (priv->log, priv->lastname);
	gpinstruct_log_set_first_name (priv->log, priv->firstname);

#ifdef G_OS_WIN32
	gchar *prefix = g_win32_get_package_installation_directory_of_module (NULL);
	gchar *datadir = g_build_filename (prefix, "share", PACKAGE, NULL);
#else
	gchar *datadir = g_strdup (PACKAGE_DATA_DIR);
#endif

	gchar *message_file = g_build_filename (datadir, "messages.ini", NULL);

	GPInstructMessagePool *message_pool = gpinstruct_message_pool_new ();
	if (g_file_test (message_file, G_FILE_TEST_IS_REGULAR))
		gpinstruct_message_pool_load_from_file (message_pool, message_file);
	else
	{
		g_object_unref (message_pool);
		message_pool = NULL;
	}

	g_free (message_file);
#ifdef G_OS_WIN32
	g_free (prefix);
#endif
	g_free (datadir);

	GtkWidget *view = gpinstruct_project_view_new (project,
	                                               message_pool,
	                                               priv->log);

	if (message_pool) g_object_unref (message_pool);

	gtk_window_set_modal (GTK_WINDOW (view), TRUE);

	/* Exit when the window is closed */
	g_signal_connect (view, "destroy", G_CALLBACK (project_view_quit), window);

	gtk_widget_hide (GTK_WIDGET (window));
	gtk_widget_show_all (view);

	error:
		if (response)
			g_object_unref (response);

		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to get project:</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_GET_PROJECT,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
got_create_session_response (SoupSession *session,
                             SoupMessage *msg,
                             gpointer user_data)
{
	SoupSoapMessage *response = NULL;
	SoupSoapParam *param;
	GPInstructClientWindow *window = user_data;
	GPInstructClientWindowPrivate *priv = window->priv;

	GError *error = NULL;

	if (!parse_response (msg, &response, window, &error))
		goto error;

	param = soup_soap_param_group_get (soup_soap_message_get_params (response),
	                                   "sessionID");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	priv->session_id = soup_soap_param_get_string (param, NULL);

	get_project (window);

	error:
		if (response)
			g_object_unref (response);

		if (!error)
			return;

		gchar *error_msg = g_strdup_printf (_("<b>Failed to create session:</b>\n%s"),
		                                    error->message);
		set_message (window, GTK_MESSAGE_ERROR,
		             GPINSTRUCT_CLIENT_REQUEST_CREATE_SESSION,
		             FALSE,
		             error_msg);
		g_error_free (error);
		g_free (error_msg);
}


static void
info_bar_response (GtkInfoBar *info_bar,
                   gint        response_id,
                   gpointer    user_data)
{
	GPInstructClientWindow *window = user_data;

	switch (response_id)
	{
		case GPINSTRUCT_CLIENT_REQUEST_CREATE_SESSION:
			create_session (window);
			break;
		case GPINSTRUCT_CLIENT_REQUEST_GET_PROJECT:
			get_project (window);
			break;
		case GPINSTRUCT_CLIENT_REQUEST_SET_LOG:
			set_log (window);
			break;
		case GPINSTRUCT_CLIENT_REQUEST_CLOSE_SESSION:
			close_session (window);
			break;
	}
}


static void
ok_button_clicked (GtkButton *button,
                   gpointer   user_data)
{
	GPInstructClientWindow *window = user_data;
	GPInstructClientWindowPrivate *priv = window->priv;

	priv->server_uri = g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->server_address_entry)));
	priv->lastname = g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->lastname_entry)));
	priv->firstname = g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->firstname_entry)));
	priv->password = g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->password_entry)));

	set_message (window,
	             GTK_MESSAGE_INFO,
	             GPINSTRUCT_CLIENT_REQUEST_NONE,
	             TRUE,
	             _("Please wait"));

	priv->session = soup_session_async_new_with_options (SOUP_SESSION_USER_AGENT, PACKAGE_TARNAME "-client/" PACKAGE_VERSION " ",
	                                                     NULL);

	create_session (window);
}


static void
cancel_button_clicked (GtkButton *button,
                       gpointer   user_data)
{
	gtk_main_quit ();
}


G_DEFINE_TYPE (GPInstructClientWindow, gpinstruct_client_window, GTK_TYPE_WINDOW);

static void
gpinstruct_client_window_init (GPInstructClientWindow *object)
{
	object->priv = GPINSTRUCT_CLIENT_WINDOW_GET_PRIVATE (object);
	GPInstructClientWindowPrivate *priv = object->priv;

	priv->server_uri = NULL;
	priv->session_id = NULL;
	priv->log = NULL;
	priv->lastname = NULL;
	priv->firstname = NULL;
	priv->password = NULL;

	g_object_set (object, "icon-name", "gpinstruct-view", NULL);

	GtkWidget *main_vbox = gtk_vbox_new (FALSE, 3);
	gtk_container_add (GTK_CONTAINER (object), main_vbox);

	GtkWidget *content_area = gtk_vbox_new (FALSE, 3);
	gtk_box_pack_start (GTK_BOX (main_vbox),
	                    content_area,
	                    FALSE, FALSE, 3);

	priv->info_bar = gtk_info_bar_new ();
	priv->create_session_button
		= gtk_info_bar_add_button (GTK_INFO_BAR (priv->info_bar),
		                           _("Try Again"),
		                           GPINSTRUCT_CLIENT_REQUEST_CREATE_SESSION);
	priv->get_project_button
		= gtk_info_bar_add_button (GTK_INFO_BAR (priv->info_bar),
		                           _("Try Again"),
		                           GPINSTRUCT_CLIENT_REQUEST_GET_PROJECT);
	priv->set_log_button
		= gtk_info_bar_add_button (GTK_INFO_BAR (priv->info_bar),
		                           _("Try Again"),
		                           GPINSTRUCT_CLIENT_REQUEST_SET_LOG);
	priv->close_session_button
		= gtk_info_bar_add_button (GTK_INFO_BAR (priv->info_bar),
		                           _("Try Again"),
		                           GPINSTRUCT_CLIENT_REQUEST_CLOSE_SESSION);
	gtk_widget_set_no_show_all (priv->info_bar, TRUE);
	g_signal_connect (priv->info_bar, "response",
	                  G_CALLBACK (info_bar_response), object);
	gtk_box_pack_start (GTK_BOX (content_area),
	                    priv->info_bar,
	                    FALSE, FALSE, 3);

	GtkWidget *info_bar_content_area = gtk_info_bar_get_content_area (GTK_INFO_BAR (priv->info_bar));

	priv->info_bar_spinner = gtk_spinner_new ();
	gtk_box_pack_start (GTK_BOX (info_bar_content_area),
	                    priv->info_bar_spinner,
	                    FALSE, FALSE, 0);

	priv->info_bar_label = gtk_label_new (NULL);
	gtk_box_pack_start (GTK_BOX (info_bar_content_area),
	                    priv->info_bar_label,
	                    FALSE, FALSE, 3);

	GtkWidget *main_table = gtk_table_new (3, 2, FALSE);
	gtk_box_pack_start (GTK_BOX (content_area),
	                    main_table,
	                    FALSE, FALSE, 3);

	GtkWidget *server_address_label = gtk_label_new (_("Server Address:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  server_address_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	priv->server_address_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (main_table),
	                  priv->server_address_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget *lastname_label = gtk_label_new (_("Last name:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  lastname_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	priv->lastname_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (main_table),
	                  priv->lastname_entry,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget *firstname_label = gtk_label_new (_("First name:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  firstname_label,
	                  0, 1, 2, 3,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	priv->firstname_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (main_table),
	                  priv->firstname_entry,
	                  1, 2, 2, 3,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget *password_label = gtk_label_new (_("Password:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  password_label,
	                  0, 1, 3, 4,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	priv->password_entry = gtk_entry_new ();
	gtk_entry_set_visibility (GTK_ENTRY (priv->password_entry), FALSE);
	gtk_table_attach (GTK_TABLE (main_table),
	                  priv->password_entry,
	                  1, 2, 3, 4,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	if (priv->server_uri)
		gtk_entry_set_text (GTK_ENTRY (priv->server_address_entry),
			                priv->server_uri);
	if (priv->lastname)
		gtk_entry_set_text (GTK_ENTRY (priv->lastname_entry),
			                priv->lastname);
	if (priv->firstname)
		gtk_entry_set_text (GTK_ENTRY (priv->firstname_entry),
			                priv->firstname);
	if (priv->password)
		gtk_entry_set_text (GTK_ENTRY (priv->password_entry),
			                priv->password);

	GtkWidget *action_area = gtk_hbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (action_area), GTK_BUTTONBOX_END);
	gtk_box_pack_start (GTK_BOX (main_vbox),
	                    action_area,
	                    FALSE, FALSE, 3);

	GtkWidget *ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
	g_signal_connect (ok_button, "clicked",
	                  G_CALLBACK (ok_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (action_area),
	                    ok_button,
	                    FALSE, FALSE, 3);

	GtkWidget *cancel_button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
	g_signal_connect (cancel_button, "clicked",
	                  G_CALLBACK (cancel_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (action_area),
	                    cancel_button,
	                    FALSE, FALSE, 3);
}

static void
gpinstruct_client_window_finalize (GObject *object)
{
	GPInstructClientWindow *window = GPINSTRUCT_CLIENT_WINDOW (object);
	GPInstructClientWindowPrivate *priv = window->priv;

	g_free (priv->server_uri);
	g_free (priv->session_id);
	g_free (priv->lastname);
	g_free (priv->firstname);
	g_free (priv->password);

	if (priv->log) g_object_unref (priv->log);

	G_OBJECT_CLASS (gpinstruct_client_window_parent_class)->finalize (object);
}

static void
gpinstruct_client_window_class_init (GPInstructClientWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GtkWindowClass *parent_class = GTK_WINDOW_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructClientWindowPrivate));

	object_class->finalize = gpinstruct_client_window_finalize;
}


GtkWidget *
gpinstruct_client_window_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_CLIENT_WINDOW, NULL);
}
