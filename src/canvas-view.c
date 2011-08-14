/*
 * canvas-view.c
 *
 * Copyright (C) 2011 - Arnel A. Borja
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

#include <glib.h>

#include "canvas-view-private.h"

guint*
random_array (guint length)
{
	guint* array = g_new (guint, length);
	guint array_temp[length];
	guint index, curr_length;
	int i;

	for (i=0; i<length; i++)
		array_temp[i] = i;

	g_random_set_seed (time (NULL));
	for (i=0; i<length; i++)
	{
		curr_length = length-i;
		/* Get random index */
		index = g_random_int_range (0, curr_length);
		/* Set data */
		array[i] = array_temp[index];
		/* Replace old data with last data */
		array_temp[index] = array_temp[curr_length-1];
	}

	return array;
}
