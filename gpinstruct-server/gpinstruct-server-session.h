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

#ifndef _GPINSTRUCT_SERVER_SESSION_H_
#define _GPINSTRUCT_SERVER_SESSION_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_SERVER_SESSION             (gpinstruct_server_session_get_type ())
#define GPINSTRUCT_SERVER_SESSION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_SERVER_SESSION, GPInstructServerSession))
#define GPINSTRUCT_SERVER_SESSION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_SERVER_SESSION, GPInstructServerSessionClass))
#define GPINSTRUCT_IS_SERVER_SESSION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_SERVER_SESSION))
#define GPINSTRUCT_IS_SERVER_SESSION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_SERVER_SESSION))
#define GPINSTRUCT_SERVER_SESSION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_SERVER_SESSION, GPInstructServerSessionClass))

typedef struct _GPInstructServerSessionPrivate GPInstructServerSessionPrivate;
typedef struct _GPInstructServerSessionClass GPInstructServerSessionClass;
typedef struct _GPInstructServerSession GPInstructServerSession;

struct _GPInstructServerSessionClass
{
	GObjectClass parent_class;

	/* Signals */
	void(* client_session_created) (GPInstructServerSession *self, gchar *session_id, gpointer user_data);
	void(* client_session_closed) (GPInstructServerSession *self, gchar *session_id, gpointer user_data);
};

struct _GPInstructServerSession
{
	GObject parent_instance;

	GPInstructServerSessionPrivate *priv;
};

GType gpinstruct_server_session_get_type (void) G_GNUC_CONST;
GPInstructServerSession *gpinstruct_server_session_new (const gchar *project_file, const gchar *log_folder);
const gchar *gpinstruct_server_session_get_project_filename (GPInstructServerSession *session);
const gchar *gpinstruct_server_session_get_log_folder (GPInstructServerSession *session);
void gpinstruct_server_session_set_log_folder (GPInstructServerSession *session, const gchar *log_folder);
GPInstructProject *gpinstruct_server_session_get_project (GPInstructServerSession *session);
guint gpinstruct_server_session_get_port (GPInstructServerSession *session);

G_END_DECLS

#endif /* _GPINSTRUCT_SERVER_SESSION_H_ */
