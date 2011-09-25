/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gpinstruct
 * Copyright (C) Arnel A. Borja 2011 <kyoushuu@yahoo.com>
 * 
gpinstruct is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gpinstruct is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GTK_SWITCH_H_
#define _GTK_SWITCH_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_SWITCH             (gtk_switch_get_type ())
#define GTK_SWITCH(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SWITCH, GtkSwitch))
#define GTK_SWITCH_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SWITCH, GtkSwitchClass))
#define GTK_IS_SWITCH(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SWITCH))
#define GTK_IS_SWITCH_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SWITCH))
#define GTK_SWITCH_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SWITCH, GtkSwitchClass))

typedef struct _GtkSwitchClass GtkSwitchClass;
typedef struct _GtkSwitch GtkSwitch;

struct _GtkSwitchClass
{
	GtkToggleButtonClass parent_class;
};

struct _GtkSwitch
{
	GtkToggleButton parent_instance;
};

GType gtk_switch_get_type (void) G_GNUC_CONST;
GtkWidget* gtk_switch_new (void);
void gtk_switch_set_active (GtkSwitch *sw, gboolean is_active);
gboolean gtk_switch_get_active (GtkSwitch *sw);

G_END_DECLS

#endif /* _GTK_SWITCH_H_ */
