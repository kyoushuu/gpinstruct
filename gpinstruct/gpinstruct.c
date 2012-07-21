/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja <kyoushuu@yahoo.com>
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

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-private.h"

gboolean
_gpinstruct_boolean_handled_accumulator (GSignalInvocationHint *ihint,
                                         GValue                *return_accu,
                                         const GValue          *handler_return,
                                         gpointer               dummy)
{
	gboolean signal_handled;

	signal_handled = g_value_get_boolean (handler_return);
	g_value_set_boolean (return_accu, signal_handled);

	/* Continue emission if signal is not handled */
	return !signal_handled;
}
