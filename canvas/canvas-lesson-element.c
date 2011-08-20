/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * canvas
 * Copyright (C) Arnel A. Borja 2011 <galeon@ymail.com>
 * 
 * canvas is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * canvas is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "canvas/canvas.h"

struct _CanvasLessonElementPrivate
{
	gchar* title;
};

#define CANVAS_LESSON_ELEMENT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_ELEMENT, CanvasLessonElementPrivate))



G_DEFINE_TYPE (CanvasLessonElement, canvas_lesson_element, CANVAS_TYPE_OBJECT);

static void
canvas_lesson_element_init (CanvasLessonElement *object)
{
	object->priv = CANVAS_LESSON_ELEMENT_PRIVATE (object);

	object->priv->title = g_strdup ("");
}

static void
canvas_lesson_element_finalize (GObject *object)
{
	CanvasLessonElement* element = CANVAS_LESSON_ELEMENT (object);

	if (element->priv->title)
		g_free (element->priv->title);

	G_OBJECT_CLASS (canvas_lesson_element_parent_class)->finalize (object);
}

static void
canvas_lesson_element_class_init (CanvasLessonElementClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonElementPrivate));

	object_class->finalize = canvas_lesson_element_finalize;
}


CanvasLessonElement*
canvas_lesson_element_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_ELEMENT, NULL);
}

const gchar*
canvas_lesson_element_get_title (CanvasLessonElement* element)
{
	return element->priv->title;
}

void
canvas_lesson_element_set_title (CanvasLessonElement* element, const gchar *title)
{
	if (element->priv->title)
		g_free (element->priv->title);
	element->priv->title = g_strdup (title);
}
