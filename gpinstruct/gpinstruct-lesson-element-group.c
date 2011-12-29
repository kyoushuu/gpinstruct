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

#include "compat/compat-glib.h"

struct _GPInstructLessonElementGroupPrivate
{
	gchar* directions;
	gboolean single_score;
	gboolean single_directions;
	GList* lesson_elements;
};

#define GPINSTRUCT_LESSON_ELEMENT_GROUP_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP, GPInstructLessonElementGroupPrivate))



G_DEFINE_TYPE (GPInstructLessonElementGroup, gpinstruct_lesson_element_group, GPINSTRUCT_TYPE_LESSON_ELEMENT);

static void
gpinstruct_lesson_element_group_init (GPInstructLessonElementGroup *object)
{
	object->priv = GPINSTRUCT_LESSON_ELEMENT_GROUP_PRIVATE (object);

	object->priv->directions = g_strdup ("");
	object->priv->single_score = TRUE;
	object->priv->single_directions = TRUE;
	object->priv->lesson_elements = NULL;
}

static void
gpinstruct_lesson_element_group_finalize (GObject *object)
{
	GPInstructLessonElementGroup* group = GPINSTRUCT_LESSON_ELEMENT_GROUP (object);

	g_free (group->priv->directions);

	if (group->priv->lesson_elements)
		g_list_free_full (group->priv->lesson_elements, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_lesson_element_group_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_element_group_class_init (GPInstructLessonElementGroupClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonElementClass* parent_class = GPINSTRUCT_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonElementGroupPrivate));

	object_class->finalize = gpinstruct_lesson_element_group_finalize;
}


GPInstructLessonElementGroup*
gpinstruct_lesson_element_group_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_ELEMENT_GROUP, NULL);
}

const gchar*
gpinstruct_lesson_element_group_get_directions (GPInstructLessonElementGroup* group)
{
	return group->priv->directions;
}

void
gpinstruct_lesson_element_group_set_directions (GPInstructLessonElementGroup* group,
                                                const gchar* directions)
{
	g_free (group->priv->directions);
	group->priv->directions = g_strdup (directions);
}

gboolean gpinstruct_lesson_element_group_get_single_score (GPInstructLessonElementGroup* group)
{
	return group->priv->single_score;
}

void gpinstruct_lesson_element_group_set_single_score (GPInstructLessonElementGroup* group,
                                                       gboolean single_score)
{
	group->priv->single_score = single_score;
}

gboolean gpinstruct_lesson_element_group_get_single_directions (GPInstructLessonElementGroup* group)
{
	return group->priv->single_directions;
}

void gpinstruct_lesson_element_group_set_single_directions (GPInstructLessonElementGroup* group,
                                                            gboolean single_directions)
{
	group->priv->single_directions = single_directions;
}

void
gpinstruct_lesson_element_group_add_lesson_element (GPInstructLessonElementGroup* group,
                                                    GPInstructLessonElement* lesson_element)
{
	group->priv->lesson_elements = g_list_append (group->priv->lesson_elements, lesson_element);
}

void
gpinstruct_lesson_element_group_remove_lesson_element (GPInstructLessonElementGroup* group,
                                                       guint lesson_element)
{
	GList* nth_link = g_list_nth (group->priv->lesson_elements, lesson_element);
	g_object_unref (nth_link->data);
	group->priv->lesson_elements = g_list_delete_link (group->priv->lesson_elements, nth_link);
}

GList*
gpinstruct_lesson_element_group_get_lesson_elements (GPInstructLessonElementGroup* group)
{
	return g_list_copy (group->priv->lesson_elements);
}

guint
gpinstruct_lesson_element_group_get_lesson_elements_length (GPInstructLessonElementGroup* group)
{
	return g_list_length (group->priv->lesson_elements);
}
