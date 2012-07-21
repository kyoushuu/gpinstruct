/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja (kyoushuu@yahoo.com)
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

#ifndef _GPINSTRUCT_CLIENT_WINDOW_H_
#define _GPINSTRUCT_CLIENT_WINDOW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_CLIENT_WINDOW             (gpinstruct_client_window_get_type ())
#define GPINSTRUCT_CLIENT_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_CLIENT_WINDOW, GPInstructClientWindow))
#define GPINSTRUCT_CLIENT_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_CLIENT_WINDOW, GPInstructClientWindowClass))
#define GPINSTRUCT_IS_CLIENT_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_CLIENT_WINDOW))
#define GPINSTRUCT_IS_CLIENT_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_CLIENT_WINDOW))
#define GPINSTRUCT_CLIENT_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_CLIENT_WINDOW, GPInstructClientWindowClass))

typedef struct _GPInstructClientWindowPrivate GPInstructClientWindowPrivate;
typedef struct _GPInstructClientWindowClass GPInstructClientWindowClass;
typedef struct _GPInstructClientWindow GPInstructClientWindow;

struct _GPInstructClientWindowClass
{
	GtkWindowClass parent_class;
};

struct _GPInstructClientWindow
{
	GtkWindow parent_instance;

	GPInstructClientWindowPrivate *priv;
};

GType gpinstruct_client_window_get_type (void) G_GNUC_CONST;
GtkWidget *gpinstruct_client_window_new (void);

G_END_DECLS

#endif /* _GPINSTRUCT_CLIENT_WINDOW_H_ */
