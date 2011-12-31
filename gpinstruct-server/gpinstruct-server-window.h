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

#ifndef _GPINSTRUCT_SERVER_WINDOW_H_
#define _GPINSTRUCT_SERVER_WINDOW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_SERVER_WINDOW             (gpinstruct_server_window_get_type ())
#define GPINSTRUCT_SERVER_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_SERVER_WINDOW, GPInstructServerWindow))
#define GPINSTRUCT_SERVER_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_SERVER_WINDOW, GPInstructServerWindowClass))
#define GPINSTRUCT_IS_SERVER_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_SERVER_WINDOW))
#define GPINSTRUCT_IS_SERVER_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_SERVER_WINDOW))
#define GPINSTRUCT_SERVER_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_SERVER_WINDOW, GPInstructServerWindowClass))

typedef struct _GPInstructServerWindowPrivate GPInstructServerWindowPrivate;
typedef struct _GPInstructServerWindowClass GPInstructServerWindowClass;
typedef struct _GPInstructServerWindow GPInstructServerWindow;

struct _GPInstructServerWindowClass
{
	GtkWindowClass parent_class;
};

struct _GPInstructServerWindow
{
	GtkWindow parent_instance;

	GPInstructServerWindowPrivate *priv;
};

GType gpinstruct_server_window_get_type (void) G_GNUC_CONST;
GtkWidget *gpinstruct_server_window_new (void);
void gpinstruct_server_window_new_session (GPInstructServerWindow *window);
gboolean gpinstruct_server_window_close_session (GPInstructServerWindow *window);
gboolean gpinstruct_server_window_quit (GPInstructServerWindow *window);

G_END_DECLS

#endif /* _GPINSTRUCT_SERVER_WINDOW_H_ */
