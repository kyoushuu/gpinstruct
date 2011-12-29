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

#include <config.h>

#include "gpinstruct/gpinstruct.h"

struct _GPInstructLessonReadingPrivate
{
	gchar* text;
};

#define GPINSTRUCT_LESSON_READING_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_READING, GPInstructLessonReadingPrivate))



G_DEFINE_TYPE (GPInstructLessonReading, gpinstruct_lesson_reading, GPINSTRUCT_TYPE_LESSON_ELEMENT);

static void
gpinstruct_lesson_reading_init (GPInstructLessonReading *object)
{
	object->priv = GPINSTRUCT_LESSON_READING_PRIVATE (object);

	object->priv->text = g_strdup ("");
}

static void
gpinstruct_lesson_reading_finalize (GObject *object)
{
	GPInstructLessonReading* reading = GPINSTRUCT_LESSON_READING (object);

	g_free (reading->priv->text);

	G_OBJECT_CLASS (gpinstruct_lesson_reading_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_reading_class_init (GPInstructLessonReadingClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonElementClass* parent_class = GPINSTRUCT_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonReadingPrivate));

	object_class->finalize = gpinstruct_lesson_reading_finalize;
}


GPInstructLessonReading*
gpinstruct_lesson_reading_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_READING, NULL);
}

const gchar*
gpinstruct_lesson_reading_get_text (GPInstructLessonReading* reading)
{
	return reading->priv->text;
}

void
gpinstruct_lesson_reading_set_text (GPInstructLessonReading* reading,
                                    const gchar* text)
{
	g_free (reading->priv->text);
	reading->priv->text = g_strdup (text);
}
