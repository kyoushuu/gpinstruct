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

struct _GPInstructProjectPrivate
{
	gchar *title;
	GList *categories;
};

#define GPINSTRUCT_PROJECT_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_PROJECT, GPInstructProjectPrivate))



G_DEFINE_TYPE (GPInstructProject, gpinstruct_project, GPINSTRUCT_TYPE_OBJECT);

static void
gpinstruct_project_init (GPInstructProject *object)
{
	object->priv = GPINSTRUCT_PROJECT_GET_PRIVATE (object);

	object->priv->title = g_strdup ("");
	object->priv->categories = NULL;
}

static void
gpinstruct_project_finalize (GObject *object)
{
	GPInstructProject *project = GPINSTRUCT_PROJECT (object);

	g_free (project->priv->title);

	if (project->priv->categories)
		g_list_free_full (project->priv->categories, g_object_unref);

	G_OBJECT_CLASS (gpinstruct_project_parent_class)->finalize (object);
}

static void
gpinstruct_project_class_init (GPInstructProjectClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectClass *parent_class = GPINSTRUCT_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructProjectPrivate));

	object_class->finalize = gpinstruct_project_finalize;
}


GPInstructProject *
gpinstruct_project_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_PROJECT, NULL);
}

const gchar *
gpinstruct_project_get_title (GPInstructProject *project)
{
	return project->priv->title;
}

void
gpinstruct_project_set_title (GPInstructProject *project,
                              const gchar *title)
{
	g_free (project->priv->title);
	project->priv->title = g_strdup (title);
}

guint
gpinstruct_project_get_categories_length (GPInstructProject *project)
{
	return g_list_length (project->priv->categories);
}

void
gpinstruct_project_add_category (GPInstructProject *project,
                                 GPInstructCategory *category)
{
	g_return_if_fail (GPINSTRUCT_IS_CATEGORY (category));

	project->priv->categories = g_list_append (project->priv->categories, category);
}

void
gpinstruct_project_remove_category (GPInstructProject *project,
                                    guint category)
{
	GList *nth_link = g_list_nth (project->priv->categories, category);
	g_object_unref (nth_link->data);
	project->priv->categories = g_list_delete_link (project->priv->categories, nth_link);
}

GList *
gpinstruct_project_get_categories (GPInstructProject *project)
{
	return g_list_copy (project->priv->categories);
}
