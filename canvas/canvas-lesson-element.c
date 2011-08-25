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

struct _GPInstructLessonElementPrivate
{
	gchar* title;
};

#define GPINSTRUCT_LESSON_ELEMENT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_ELEMENT, GPInstructLessonElementPrivate))



G_DEFINE_TYPE (GPInstructLessonElement, gpinstruct_lesson_element, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_lesson_element_init (GPInstructLessonElement *object)
{
	object->priv = GPINSTRUCT_LESSON_ELEMENT_PRIVATE (object);

	object->priv->title = g_strdup ("");
}

static void
gpinstruct_lesson_element_finalize (GObject *object)
{
	GPInstructLessonElement* element = GPINSTRUCT_LESSON_ELEMENT (object);

	if (element->priv->title)
		g_free (element->priv->title);

	G_OBJECT_CLASS (gpinstruct_lesson_element_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_element_class_init (GPInstructLessonElementClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass* parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonElementPrivate));

	object_class->finalize = gpinstruct_lesson_element_finalize;
}


GPInstructLessonElement*
gpinstruct_lesson_element_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_ELEMENT, NULL);
}

const gchar*
gpinstruct_lesson_element_get_title (GPInstructLessonElement* element)
{
	return element->priv->title;
}

void
gpinstruct_lesson_element_set_title (GPInstructLessonElement* element,
                                     const gchar *title)
{
	if (element->priv->title)
		g_free (element->priv->title);
	element->priv->title = g_strdup (title);
}
