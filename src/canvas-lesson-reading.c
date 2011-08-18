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

#include "canvas.h"

struct _CanvasLessonReadingPrivate
{
	gchar* text;
};

#define CANVAS_LESSON_READING_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_READING, CanvasLessonReadingPrivate))



G_DEFINE_TYPE (CanvasLessonReading, canvas_lesson_reading, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_reading_init (CanvasLessonReading *object)
{
	object->priv = CANVAS_LESSON_READING_PRIVATE (object);

	object->priv->text = g_strdup ("");
}

static void
canvas_lesson_reading_finalize (GObject *object)
{
	CanvasLessonReading* reading = CANVAS_LESSON_READING (object);

	if (reading->priv->text)
		g_free (reading->priv->text);

	G_OBJECT_CLASS (canvas_lesson_reading_parent_class)->finalize (object);
}

static void
canvas_lesson_reading_class_init (CanvasLessonReadingClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonReadingPrivate));

	object_class->finalize = canvas_lesson_reading_finalize;
}


CanvasLessonReading*
canvas_lesson_reading_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_READING, NULL);
}

const gchar*
canvas_lesson_reading_get_text (CanvasLessonReading* reading)
{
	return reading->priv->text;
}

void
canvas_lesson_reading_set_text (CanvasLessonReading* reading, const gchar* text)
{
	if (reading->priv->text)
		g_free (reading->priv->text);
	reading->priv->text = g_strdup (text);
}
