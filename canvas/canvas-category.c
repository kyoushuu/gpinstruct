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

struct _CanvasCategoryPrivate
{
	gchar* title;
	GList* lessons;
};

#define CANVAS_CATEGORY_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_CATEGORY, CanvasCategoryPrivate))



G_DEFINE_TYPE (CanvasCategory, canvas_category, CANVAS_TYPE_OBJECT);

static void
canvas_category_init (CanvasCategory *object)
{
	object->priv = CANVAS_CATEGORY_PRIVATE (object);

	object->priv->title = g_strdup ("");
	object->priv->lessons = NULL;
}

static void
canvas_category_finalize (GObject *object)
{
	CanvasCategory* category = CANVAS_CATEGORY (object);

	if (category->priv->title)
		g_free (category->priv->title);
	if (category->priv->lessons)
		g_list_free_full (category->priv->lessons, g_object_unref);

	G_OBJECT_CLASS (canvas_category_parent_class)->finalize (object);
}

static void
canvas_category_class_init (CanvasCategoryClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasObjectClass* parent_class = CANVAS_OBJECT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasCategoryPrivate));

	object_class->finalize = canvas_category_finalize;
}


CanvasCategory*
canvas_category_new (void)
{
	return g_object_new(CANVAS_TYPE_CATEGORY, NULL);
}

const gchar*
canvas_category_get_title (CanvasCategory* category)
{
	return category->priv->title;
}

void
canvas_category_set_title (CanvasCategory* category,
                           const gchar *title)
{
	if (category->priv->title)
		g_free (category->priv->title);
	category->priv->title = g_strdup (title);
}

guint
canvas_category_get_lessons_length (CanvasCategory* category)
{
	return g_list_length (category->priv->lessons);
}

void
canvas_category_add_lesson (CanvasCategory* category,
                            CanvasLesson* lesson)
{
	g_return_if_fail (CANVAS_IS_LESSON (lesson));

	category->priv->lessons = g_list_append (category->priv->lessons, lesson);
}

void
canvas_category_remove_lesson (CanvasCategory* category,
                               guint lesson)
{
	GList* nth_link = g_list_nth (category->priv->lessons, lesson);
	g_object_unref (nth_link->data);
	category->priv->lessons = g_list_delete_link (category->priv->lessons, nth_link);
}

GList*
canvas_category_get_lessons (CanvasCategory* category)
{
	return g_list_copy (category->priv->lessons);
}
