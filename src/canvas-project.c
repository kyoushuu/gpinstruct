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

typedef struct _CanvasProjectPrivate CanvasProjectPrivate;
struct _CanvasProjectPrivate
{
	gchar* title;
	GList* categories;
};

#define CANVAS_PROJECT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_PROJECT, CanvasProjectPrivate))



G_DEFINE_TYPE (CanvasProject, canvas_project, CANVAS_TYPE_OBJECT);

static void
canvas_project_init (CanvasProject *object)
{
	CanvasProjectPrivate* private_data = CANVAS_PROJECT_PRIVATE(object);
	private_data->title = g_strdup ("");
	private_data->categories = NULL;
}

static void
canvas_project_finalize (GObject *object)
{
	CanvasProjectPrivate* private_data = CANVAS_PROJECT_PRIVATE(object);

	if (private_data->title)
		g_free (private_data->title);
	if (private_data->categories)
		g_list_free_full (private_data->categories, g_object_unref);

	G_OBJECT_CLASS (canvas_project_parent_class)->finalize (object);
}

static void
canvas_project_class_init (CanvasProjectClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasProjectPrivate));

	object_class->finalize = canvas_project_finalize;
}


CanvasProject*
canvas_project_new (void)
{
	return g_object_new(CANVAS_TYPE_PROJECT, NULL);
}

const gchar*
canvas_project_get_title (CanvasProject* project)
{
	return CANVAS_PROJECT_PRIVATE(project)->title;
}

void
canvas_project_set_title (CanvasProject* project,
                          const gchar *title)
{
	CanvasProjectPrivate* private_data = CANVAS_PROJECT_PRIVATE(project);

	if (private_data->title)
		g_free (private_data->title);
	private_data->title = g_strdup (title);
}

guint
canvas_project_get_categories_length (CanvasProject* project)
{
	return g_list_length (CANVAS_PROJECT_PRIVATE(project)->categories);
}

void
canvas_project_add_category (CanvasProject* project,
                             CanvasCategory* category)
{
	g_return_if_fail (CANVAS_IS_CATEGORY (category));

	CanvasProjectPrivate* private_data = CANVAS_PROJECT_PRIVATE(project);
	private_data->categories = g_list_append (private_data->categories, category);
}

void
canvas_project_remove_category (CanvasProject* project,
                                CanvasCategory* category)
{
	g_return_if_fail (CANVAS_IS_CATEGORY (category));

	CanvasProjectPrivate* private_data = CANVAS_PROJECT_PRIVATE(project);
	private_data->categories = g_list_remove (private_data->categories, category);
}

GList*
canvas_project_get_categories (CanvasProject* project)
{
	return g_list_copy (CANVAS_PROJECT_PRIVATE(project)->categories);
}
