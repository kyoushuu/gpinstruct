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

struct _GPInstructCategoryPrivate
{
	gchar *title;
	GList *lessons;
};

#define GPINSTRUCT_CATEGORY_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_CATEGORY, GPInstructCategoryPrivate))



G_DEFINE_TYPE (GPInstructCategory, gpinstruct_category, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_category_init (GPInstructCategory *object)
{
	object->priv = GPINSTRUCT_CATEGORY_GET_PRIVATE (object);
	GPInstructCategoryPrivate *priv = object->priv;

	priv->title = g_strdup ("");
	priv->lessons = NULL;
}

static void
gpinstruct_category_finalize (GObject *object)
{
	GPInstructCategory *category = GPINSTRUCT_CATEGORY (object);
	GPInstructCategoryPrivate *priv = category->priv;

	g_free (priv->title);
	g_list_free_full (priv->lessons, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_category_parent_class)->finalize (object);
}

static void
gpinstruct_category_class_init (GPInstructCategoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass *parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructCategoryPrivate));

	object_class->finalize = gpinstruct_category_finalize;
}


GPInstructCategory *
gpinstruct_category_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_CATEGORY, NULL);
}

const gchar *
gpinstruct_category_get_title (GPInstructCategory *category)
{
	GPInstructCategoryPrivate *priv = category->priv;

	return priv->title;
}

void
gpinstruct_category_set_title (GPInstructCategory *category,
                               const gchar *title)
{
	GPInstructCategoryPrivate *priv = category->priv;

	g_free (priv->title);
	priv->title = g_strdup (title);
}

guint
gpinstruct_category_get_lessons_length (GPInstructCategory *category)
{
	GPInstructCategoryPrivate *priv = category->priv;

	return g_list_length (priv->lessons);
}

void
gpinstruct_category_add_lesson (GPInstructCategory *category,
                                GPInstructLesson *lesson)
{
	g_return_if_fail (GPINSTRUCT_IS_LESSON (lesson));

	GPInstructCategoryPrivate *priv = category->priv;

	priv->lessons = g_list_append (priv->lessons, lesson);
}

void
gpinstruct_category_remove_lesson (GPInstructCategory *category,
                                   guint lesson)
{
	GPInstructCategoryPrivate *priv = category->priv;

	GList *nth_link = g_list_nth (priv->lessons, lesson);
	g_object_unref (nth_link->data);
	priv->lessons = g_list_delete_link (priv->lessons, nth_link);
}

GList *
gpinstruct_category_get_lessons (GPInstructCategory *category)
{
	GPInstructCategoryPrivate *priv = category->priv;

	return g_list_copy (priv->lessons);
}
