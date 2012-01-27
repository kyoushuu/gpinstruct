/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2012  Arnel A. Borja <kyoushuu@yahoo.com>
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

#include "gtk-orientable-new.h"

#if (GTK_MAJOR_VERSION < 3)
GtkWidget *
gtk_box_new (GtkOrientation orientation,
             gint spacing)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hbox_new (FALSE, spacing);
	else
		return gtk_vbox_new (FALSE, spacing);
}

GtkWidget *
gtk_paned_new (GtkOrientation orientation)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hpaned_new ();
	else
		return gtk_vpaned_new ();
}

GtkWidget *
gtk_button_box_new (GtkOrientation orientation)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hbutton_box_new ();
	else
		return gtk_vbutton_box_new ();
}
#endif

