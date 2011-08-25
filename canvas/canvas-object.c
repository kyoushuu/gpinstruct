/*
 * GPInstruct - Programmed Instruction
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

#include "gpinstruct/gpinstruct.h"



G_DEFINE_TYPE (GPInstructObject, gpinstruct_object, G_TYPE_OBJECT);

static void
gpinstruct_object_init (GPInstructObject *object)
{
}

static void
gpinstruct_object_finalize (GObject *object)
{

	G_OBJECT_CLASS (gpinstruct_object_parent_class)->finalize (object);
}

static void
gpinstruct_object_class_init (GPInstructObjectClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GObjectClass* parent_class = G_OBJECT_CLASS (klass);*/

	object_class->finalize = gpinstruct_object_finalize;
}

