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

/* GtkSwitch widget implementation for GTK+ 2.x */

#include <config.h>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION < 3
#include "gtk-switch.h"

G_DEFINE_TYPE (GtkSwitch, gtk_switch, GTK_TYPE_TOGGLE_BUTTON);


static void
gtk_switch_init (GtkSwitch *object)
{
}

static void
gtk_switch_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtk_switch_parent_class)->finalize (object);
}

static void
gtk_switch_class_init (GtkSwitchClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkToggleButtonClass* parent_class = GTK_TOGGLE_BUTTON_CLASS (klass);*/

	object_class->finalize = gtk_switch_finalize;
}


static void
gtk_switch_activate (GtkToggleButton *togglebutton,
                     gpointer         user_data)
{
	gboolean active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
	gtk_button_set_label (GTK_BUTTON (togglebutton), active? GTK_STOCK_YES:GTK_STOCK_NO);
}

GtkWidget*
gtk_switch_new (void)
{
	GtkSwitch* sw = g_object_new (GTK_TYPE_SWITCH, NULL);

	g_signal_connect (GTK_TOGGLE_BUTTON (sw), "toggled",
	                  G_CALLBACK (gtk_switch_activate), NULL);
	g_object_set (sw,
	              "use-stock", TRUE,
	              "label", GTK_STOCK_NO,
	              "active", FALSE,
	              NULL);

	return GTK_WIDGET (sw);
}

void
gtk_switch_set_active (GtkSwitch *sw, gboolean is_active)
{
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sw), is_active);
}

gboolean
gtk_switch_get_active (GtkSwitch *sw)
{
	return gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (sw));
}

#endif
