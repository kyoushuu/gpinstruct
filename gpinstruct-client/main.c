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

#include "compat/compat-gtk.h"


typedef struct _GPInstructSoapClientPrivate GPInstructSoapClientPrivate;
struct _GPInstructSoapClientPrivate
{
	SoupSession* session;

	GtkWidget* waiting_dialog;
	GtkWidget* progress_bar;

	gchar* server_uri;
	gchar* sessionID;

	GPInstructLog* log;

	gchar* lastname;
	gchar* firstname;
	gchar* password;
};

static gboolean create_session (GPInstructSoapClientPrivate* priv, GError** error);
static gboolean get_project (GPInstructSoapClientPrivate* priv, GError** error);
static gboolean set_log (GPInstructSoapClientPrivate* priv, GError** error);
static gboolean close_session (GPInstructSoapClientPrivate* priv, GError** error);

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


static void
quit (GPInstructSoapClientPrivate* priv)
{
	if (priv->server_uri) g_free (priv->server_uri);
	if (priv->sessionID) g_free (priv->sessionID);
	if (priv->log) g_object_unref (priv->log);
	if (priv->lastname) g_free (priv->lastname);
	if (priv->firstname) g_free (priv->firstname);
	if (priv->password) g_free (priv->password);

	g_free (priv);

	if (gtk_main_level ())
		gtk_main_quit ();
}


static gboolean
create_session (GPInstructSoapClientPrivate* priv,
                GError** error)
{
	gtk_widget_show_all (priv->waiting_dialog);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (priv->progress_bar), _("Logging in"));

	SoupSoapMessage* msg = soup_soap_message_new ("POST",
	                                              priv->server_uri,
	                                              FALSE, NULL, NULL, NULL);
	if (!msg)
	{
		g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	soup_soap_message_start_envelope (msg);
	soup_soap_message_start_body (msg);

	soup_soap_message_start_element (msg, "createSession", NULL,
	                                 PACKAGE_URL);
	soup_soap_message_add_namespace (msg, NULL, PACKAGE_URL);
	soup_soap_message_start_element (msg, "lastname", NULL, NULL);
	soup_soap_message_write_string (msg, priv->lastname);
	soup_soap_message_end_element (msg);
	soup_soap_message_start_element (msg, "firstname", NULL, NULL);
	soup_soap_message_write_string (msg, priv->firstname);
	soup_soap_message_end_element (msg);
	soup_soap_message_start_element (msg, "password", NULL, NULL);
	soup_soap_message_write_string (msg, priv->password);
	soup_soap_message_end_element (msg);
	soup_soap_message_end_element (msg);

	soup_soap_message_end_body (msg);
	soup_soap_message_end_envelope (msg);
	soup_soap_message_persist (msg);

	soup_session_queue_message (priv->session, SOUP_MESSAGE (msg),
	                            got_create_session_response, priv);

	return TRUE;

	error:
		if (msg) g_object_unref (msg);
		return FALSE;
}


static gboolean
get_project (GPInstructSoapClientPrivate* priv,
             GError** error)
{
	gtk_widget_show_all (priv->waiting_dialog);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (priv->progress_bar), _("Getting project data"));

	SoupSoapMessage* msg = soup_soap_message_new ("POST",
	                                              priv->server_uri,
	                                              FALSE, NULL, NULL, NULL);
	if (!msg)
	{
		g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	soup_soap_message_start_envelope (msg);
	soup_soap_message_start_body (msg);

	soup_soap_message_start_element (msg, "getProject", NULL,
	                                 PACKAGE_URL);
	soup_soap_message_add_namespace (msg, NULL, PACKAGE_URL);
	soup_soap_message_start_element (msg, "sessionID", NULL, NULL);
	soup_soap_message_write_string (msg, priv->sessionID);
	soup_soap_message_end_element (msg);
	soup_soap_message_end_element (msg);

	soup_soap_message_end_body (msg);
	soup_soap_message_end_envelope (msg);
	soup_soap_message_persist (msg);

	soup_session_queue_message (priv->session, SOUP_MESSAGE (msg),
	                            got_get_project_response, priv);

	return TRUE;

	error:
		if (msg) g_object_unref (msg);
		return FALSE;
}


static gboolean
set_log (GPInstructSoapClientPrivate* priv,
         GError** error)
{
	gtk_widget_show_all (priv->waiting_dialog);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (priv->progress_bar), _("Sending log to server"));

	SoupSoapMessage* msg = soup_soap_message_new ("POST",
	                                              priv->server_uri,
	                                              FALSE, NULL, NULL, NULL);
	if (!msg)
	{
		g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}	

	gchar* log = gpinstruct_log_save_to_string (priv->log, error);

	if (*error) goto error;

	soup_soap_message_start_envelope (msg);
	soup_soap_message_start_body (msg);

	soup_soap_message_start_element (msg, "setLog", NULL,
	                                 PACKAGE_URL);
	soup_soap_message_add_namespace (msg, NULL, PACKAGE_URL);
	soup_soap_message_start_element (msg, "sessionID", NULL, NULL);
	soup_soap_message_write_string (msg, priv->sessionID);
	soup_soap_message_end_element (msg);
	soup_soap_message_start_element (msg, "log", NULL, NULL);
	soup_soap_message_write_base64 (msg, log, strlen (log));
	soup_soap_message_end_element (msg);
	soup_soap_message_end_element (msg);

	soup_soap_message_end_body (msg);
	soup_soap_message_end_envelope (msg);
	soup_soap_message_persist (msg);

	soup_session_queue_message (priv->session, SOUP_MESSAGE (msg),
	                            got_set_log_response, priv);

	g_free (log);

	return TRUE;

	error:
		if (msg) g_object_unref (msg);
		return FALSE;
}


static gboolean
close_session (GPInstructSoapClientPrivate* priv,
               GError** error)
{
	gtk_widget_show_all (priv->waiting_dialog);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (priv->progress_bar), _("Closing session"));

	SoupSoapMessage* msg = soup_soap_message_new ("POST",
	                                              priv->server_uri,
	                                              FALSE, NULL, NULL, NULL);
	if (!msg)
	{
		g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
		                     GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED,
		                     _("Could not create web service request"));
		goto error;
	}

	soup_soap_message_start_envelope (msg);
	soup_soap_message_start_body (msg);

	soup_soap_message_start_element (msg, "closeSession", NULL,
	                                 PACKAGE_URL);
	soup_soap_message_add_namespace (msg, NULL, PACKAGE_URL);
	soup_soap_message_start_element (msg, "sessionID", NULL, NULL);
	soup_soap_message_write_string (msg, priv->sessionID);
	soup_soap_message_end_element (msg);
	soup_soap_message_end_element (msg);

	soup_soap_message_end_body (msg);
	soup_soap_message_end_envelope (msg);
	soup_soap_message_persist (msg);

	soup_session_queue_message (priv->session, SOUP_MESSAGE (msg),
	                            got_close_session_response, priv);

	return TRUE;

	error:
		if (msg) g_object_unref (msg);
		return FALSE;
}


static gboolean
parse_response (SoupMessage *msg,
                SoupSoapResponse **response_return,
                GPInstructSoapClientPrivate* priv,
                GError** error)
{
	SoupSoapResponse *response = NULL;
	SoupSoapParameter *param;

	if (SOUP_STATUS_IS_SERVER_ERROR (msg->status_code))
	{
		response = soup_soap_message_parse_response (SOUP_SOAP_MESSAGE (msg));
		if (!response)
		{
			g_set_error_literal (error, GPINSTRUCT_CLIENT_ERROR,
			                     GPINSTRUCT_CLIENT_ERROR_PARSE_RESPONSE_FAILED,
			                     _("Could not parse SOAP response"));
			goto error;
		}

		param = soup_soap_response_get_first_parameter_by_name (response, "faultstring");
		gchar* faultstring = soup_soap_parameter_get_string_value (param);

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

	response = soup_soap_message_parse_response (SOUP_SOAP_MESSAGE (msg));
	if (!response)
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
	SoupSoapResponse *response = NULL;
	SoupSoapParameter *param;
	GPInstructSoapClientPrivate *priv = user_data;
	GError* error = NULL;
	GtkWidget* dialog;

	if (!parse_response (msg, &response, priv, &error))
		goto error;

	param = soup_soap_response_get_first_parameter_by_name (response, "result");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	if (!soup_soap_parameter_get_boolean_value (param))
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Server failed to process request."));
		goto error;
	}

	gtk_progress_bar_pulse (GTK_PROGRESS_BAR (priv->progress_bar));

	dialog = gtk_message_dialog_new (NULL,
	                                 GTK_DIALOG_DESTROY_WITH_PARENT,
	                                 GTK_MESSAGE_INFO,
	                                 GTK_BUTTONS_OK,
	                                 _("Sent log to server."));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);


	error:
		if (response)
	{
		g_object_unref (response);
		response = NULL;
	}

		if (!error)
	{
		quit (priv);
		return;
	}

		dialog = gtk_message_dialog_new (NULL,
		                                 GTK_DIALOG_DESTROY_WITH_PARENT,
		                                 GTK_MESSAGE_ERROR,
		                                 GTK_BUTTONS_YES_NO,
		                                 _("Failed to close session: %s\nTry again?"),
		                                 error->message);
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;

		if (response)
	{
		g_object_unref (response);
		response = NULL;
	}

		if (result == GTK_RESPONSE_YES)
	{
		if (!close_session (priv, &error))
			goto error;
	}
		else
			quit (priv);
}


static void
got_set_log_response (SoupSession *session,
                      SoupMessage *msg,
                      gpointer user_data)
{
	SoupSoapResponse *response = NULL;
	SoupSoapParameter *param;
	GPInstructSoapClientPrivate *priv = user_data;
	GError* error = NULL;
	GtkWidget* dialog;

	if (!parse_response (msg, &response, priv, &error))
		goto error;

	param = soup_soap_response_get_first_parameter_by_name (response, "result");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	if (!soup_soap_parameter_get_boolean_value (param))
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Server failed to process request."));
		goto error;
	}

	gtk_progress_bar_pulse (GTK_PROGRESS_BAR (priv->progress_bar));

	retry:
		if (!close_session (priv, &error))
			goto error;

	error:
		if (response)
	{
		g_object_unref (response);
		response = NULL;
	}

		if (!error) return;

		dialog = gtk_message_dialog_new (NULL,
		                                 GTK_DIALOG_DESTROY_WITH_PARENT,
		                                 GTK_MESSAGE_ERROR,
		                                 GTK_BUTTONS_YES_NO,
		                                 (error->domain == GPINSTRUCT_CLIENT_ERROR &&
		                                  error->code == GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED)?
		                                 _("Failed to close session: %s\nTry again?"):_("Failed to save log to server: %s\nTry again?"),
		                                 error->message);
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;

		if (result == GTK_RESPONSE_YES)
	{
		if (error->domain == GPINSTRUCT_CLIENT_ERROR &&
		    error->code == GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED)
			goto retry;
		else
		{
			if (!set_log (priv, &error))
				goto error;
		}
	}
		else
			quit (priv);
}


static void
project_view_quit (GtkWidget *window,
                   gpointer user_data)
{
	GPInstructSoapClientPrivate *priv = user_data;
	GError* error = NULL;

	gtk_widget_hide (window);

	retry:
		if (!set_log (priv, &error))
	{
		GtkWidget* dialog = gtk_message_dialog_new (NULL,
		                                            GTK_DIALOG_DESTROY_WITH_PARENT,
		                                            GTK_MESSAGE_ERROR,
		                                            GTK_BUTTONS_CLOSE,
		                                            _("Failed to save log to server: %s\nTry again?"),
		                                            error->message);
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;

		if (result == GTK_RESPONSE_YES)
			goto retry;
		else
			quit (priv);
	}
}


static void
got_get_project_response (SoupSession *session,
                          SoupMessage *msg,
                          gpointer user_data)
{
	SoupSoapResponse *response = NULL;
	SoupSoapParameter *param;
	GPInstructSoapClientPrivate *priv = user_data;
	GError* error = NULL;
	GtkWidget* dialog;

	if (!parse_response (msg, &response, priv, &error))
		goto error;

	param = soup_soap_response_get_first_parameter_by_name (response, "project");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	gchar* project_base64 = soup_soap_parameter_get_string_value (param);

	gsize project_size;
	guchar* project_bin = g_base64_decode_inplace (project_base64, &project_size);
	gchar* project_string;

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
		project_string = g_strndup ((gchar*)project_bin, project_size);
		g_free (project_bin);
	}

	GPInstructParser* parser = gpinstruct_parser_new ();

	GPInstructProject* project = gpinstruct_parser_load_from_string (parser,
	                                                                 project_string,
	                                                                 &error);
	if (error) goto error;

	gtk_progress_bar_pulse (GTK_PROGRESS_BAR (priv->progress_bar));

	g_free (project_string);

	g_object_unref (parser);

	priv->log = gpinstruct_log_new ();
	gpinstruct_log_set_last_name (priv->log, priv->lastname);
	gpinstruct_log_set_first_name (priv->log, priv->firstname);

#ifdef G_OS_WIN32
	gchar* prefix = g_win32_get_package_installation_directory_of_module (NULL);
	gchar* datadir = g_build_filename (prefix, "share", PACKAGE, NULL);
#else
	gchar* datadir = g_strdup (PACKAGE_DATA_DIR);
#endif

	gchar* message_file = g_build_filename (datadir, "messages.ini", NULL);

	GPInstructMessagePool* message_pool = gpinstruct_message_pool_new ();
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

	GtkWidget* window = gpinstruct_project_view_new (project,
	                                                 message_pool,
	                                                 priv->log);

	if (message_pool) g_object_unref (message_pool);

	gtk_window_set_modal (GTK_WINDOW (window), TRUE);

	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (project_view_quit), priv);

	gtk_widget_hide (priv->waiting_dialog);
	gtk_widget_show_all (window);

	error:
		if (response)
	{
		g_object_unref (response);
		response = NULL;
	}

		if (!error) return;

		dialog = gtk_message_dialog_new (NULL,
		                                 GTK_DIALOG_DESTROY_WITH_PARENT,
		                                 GTK_MESSAGE_ERROR,
		                                 GTK_BUTTONS_YES_NO,
		                                 _("Failed to get project: %s\nTry again?"),
		                                 error->message);
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;

		if (result == GTK_RESPONSE_YES)
	{
		if (!get_project (priv, &error))
			goto error;
	}
		else
			quit (priv);
}


static void
got_create_session_response (SoupSession *session,
                             SoupMessage *msg,
                             gpointer user_data)
{
	SoupSoapResponse *response = NULL;
	SoupSoapParameter *param;
	GPInstructSoapClientPrivate *priv = user_data;
	GError* error = NULL;
	GtkWidget* dialog;

	if (!parse_response (msg, &response, priv, &error))
		goto error;

	param = soup_soap_response_get_first_parameter_by_name (response, "sessionID");
	if (!param)
	{
		g_set_error_literal (&error, GPINSTRUCT_CLIENT_ERROR,
		                     0, _("Invalid response from server."));
		goto error;
	}

	priv->sessionID = soup_soap_parameter_get_string_value (param);

	gtk_progress_bar_pulse (GTK_PROGRESS_BAR (priv->progress_bar));

	retry:
		if (!get_project (priv, &error))
			goto error;

		return;

	error:
		if (!error) return;

		if (response)
	{
		g_object_unref (response);
		response = NULL;
	}

		dialog = gtk_message_dialog_new (NULL,
		                                 GTK_DIALOG_DESTROY_WITH_PARENT,
		                                 GTK_MESSAGE_ERROR,
		                                 GTK_BUTTONS_YES_NO,
		                                 (error->domain == GPINSTRUCT_CLIENT_ERROR &&
		                                  error->code == GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED)?
		                                 _("Failed to get project: %s\nTry again?"):_("Failed to log in: %s\nTry again?"),
		                                 error->message);
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;

		if (result == GTK_RESPONSE_YES)
	{
		if (error->domain == GPINSTRUCT_CLIENT_ERROR &&
		    error->code == GPINSTRUCT_CLIENT_ERROR_CREATE_WS_REQUEST_FAILED)
			goto retry;
		else
		{
			if (!create_session (priv, &error))
				goto error;
		}
	}
		else
			quit (priv);
}


static void
waiting_dialog_response (GtkDialog *dialog,
                         gint       response_id,
                         gpointer   user_data)
{
	GPInstructSoapClientPrivate *priv = user_data;

	if (response_id == GTK_RESPONSE_REJECT)
		quit (priv);
}


int
main (int argc,
      char *argv[])
{
#ifdef G_OS_WIN32
	gchar* prefix = g_win32_get_package_installation_directory_of_module (NULL);
#endif

#ifdef ENABLE_NLS
#ifdef G_OS_WIN32
	gchar* localedir = g_build_filename (prefix, "share", "locale", NULL);
#else
	gchar* localedir = g_strdup (PACKAGE_LOCALE_DIR);
#endif
	bindtextdomain (GETTEXT_PACKAGE, localedir);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	g_free (localedir);
#endif

	GtkWidget *window, *dialog;
	GError* error = NULL;


	gtk_init (&argc, &argv);


	GPInstructSoapClientPrivate* priv = g_new0 (GPInstructSoapClientPrivate, 1);


	window = gtk_dialog_new_with_buttons (_("GPInstruct Client"),
	                                      NULL,
	                                      GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_OK,
	                                      GTK_RESPONSE_ACCEPT,
	                                      GTK_STOCK_CANCEL,
	                                      GTK_RESPONSE_REJECT,
	                                      NULL);
	GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (window));

	GtkWidget* main_table = gtk_table_new (3, 2, FALSE);
	gtk_box_pack_start (GTK_BOX (content_area),
	                    main_table,
	                    FALSE, FALSE, 3);

	GtkWidget* server_address_label = gtk_label_new (_("Server Address:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  server_address_label,
	                  0, 1, 0, 1,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* server_address_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (main_table),
	                  server_address_entry,
	                  1, 2, 0, 1,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* lastname_label = gtk_label_new (_("Last name:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  lastname_label,
	                  0, 1, 1, 2,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* lastname_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (main_table),
	                  lastname_entry,
	                  1, 2, 1, 2,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* firstname_label = gtk_label_new (_("First name:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  firstname_label,
	                  0, 1, 2, 3,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* firstname_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (main_table),
	                  firstname_entry,
	                  1, 2, 2, 3,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* password_label = gtk_label_new (_("Password:"));
	gtk_table_attach (GTK_TABLE (main_table),
	                  password_label,
	                  0, 1, 3, 4,
	                  GTK_SHRINK | GTK_FILL, GTK_FILL,
	                  0, 0);

	GtkWidget* password_entry = gtk_entry_new ();
	gtk_entry_set_visibility (GTK_ENTRY (password_entry), FALSE);
	gtk_table_attach (GTK_TABLE (main_table),
	                  password_entry,
	                  1, 2, 3, 4,
	                  GTK_EXPAND | GTK_FILL, GTK_FILL,
	                  0, 0);

	/* Testing */
	gtk_entry_set_text (GTK_ENTRY (server_address_entry), "http:/" "/localhost/~kyoushuu/gpinstruct/server/");
	gtk_entry_set_text (GTK_ENTRY (lastname_entry), "Borja");
	gtk_entry_set_text (GTK_ENTRY (firstname_entry), "Arnel");
	gtk_entry_set_text (GTK_ENTRY (password_entry), "test");

	gtk_widget_show_all (main_table);

	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_ACCEPT)
	{
		priv->server_uri = g_strdup (gtk_entry_get_text (GTK_ENTRY (server_address_entry)));
		priv->lastname = g_strdup (gtk_entry_get_text (GTK_ENTRY (lastname_entry)));
		priv->firstname = g_strdup (gtk_entry_get_text (GTK_ENTRY (firstname_entry)));
		priv->password = g_strdup (gtk_entry_get_text (GTK_ENTRY (password_entry)));

		priv->waiting_dialog = gtk_dialog_new_with_buttons (_("Please wait"),
		                                                    NULL,
		                                                    GTK_DIALOG_DESTROY_WITH_PARENT,
		                                                    GTK_STOCK_CANCEL,
		                                                    GTK_RESPONSE_REJECT,
		                                                    NULL);
		g_signal_connect (priv->waiting_dialog,
		                  "response",
		                  G_CALLBACK (waiting_dialog_response),
		                  priv);

		gtk_widget_hide (window);

		priv->progress_bar = gtk_progress_bar_new ();
		gtk_progress_bar_set_pulse_step (GTK_PROGRESS_BAR (priv->progress_bar), 1.0 / 2);
		gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (priv->progress_bar), TRUE);
		gtk_progress_bar_set_text (GTK_PROGRESS_BAR (priv->progress_bar), _("Logging in..."));
		gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (priv->waiting_dialog))),
		                    priv->progress_bar,
		                    FALSE, FALSE, 3);

		gtk_widget_show_all (priv->waiting_dialog);

		gchar *user_agent = g_strdup_printf (_("GPInstruct Client %s"), PACKAGE_VERSION);
		priv->session = soup_session_async_new_with_options (SOUP_SESSION_USER_AGENT, user_agent,
		                                                     NULL);
		g_free (user_agent);

		if (!create_session (priv, &error))
			goto error;

		gtk_main ();

		priv = NULL;
	}

	error:
		if (error)
	{
		dialog = gtk_message_dialog_new (NULL,
		                                 GTK_DIALOG_DESTROY_WITH_PARENT,
		                                 GTK_MESSAGE_ERROR,
		                                 GTK_BUTTONS_CLOSE,
		                                 _("Failed to create session: %s"),
		                                 error->message);
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;
	}

		gtk_widget_destroy (window);

		if (priv) quit (priv);

#ifdef G_OS_WIN32
		g_free (prefix);
#endif

		return 0;
}
