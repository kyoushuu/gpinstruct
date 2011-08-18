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

struct _CanvasLessonDiscussionPrivate
{
	gchar* text;
};

#define CANVAS_LESSON_DISCUSSION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_DISCUSSION, CanvasLessonDiscussionPrivate))



G_DEFINE_TYPE (CanvasLessonDiscussion, canvas_lesson_discussion, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_discussion_init (CanvasLessonDiscussion *object)
{
	object->priv = CANVAS_LESSON_DISCUSSION_PRIVATE (object);

	object->priv->text = g_strdup ("");
}

static void
canvas_lesson_discussion_finalize (GObject *object)
{
	CanvasLessonDiscussion* discussion = CANVAS_LESSON_DISCUSSION (object);

	if (discussion->priv->text)
		g_free (discussion->priv->text);

	G_OBJECT_CLASS (canvas_lesson_discussion_parent_class)->finalize (object);
}

static void
canvas_lesson_discussion_class_init (CanvasLessonDiscussionClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonDiscussionPrivate));

	object_class->finalize = canvas_lesson_discussion_finalize;
}


CanvasLessonDiscussion*
canvas_lesson_discussion_new (void)
{
	return g_object_new (CANVAS_TYPE_LESSON_DISCUSSION, NULL);
}

const gchar*
canvas_lesson_discussion_get_text (CanvasLessonDiscussion* discussion)
{
	return discussion->priv->text;
}

void
canvas_lesson_discussion_set_text (CanvasLessonDiscussion* discussion, const gchar* text)
{
	if (discussion->priv->text)
		g_free (discussion->priv->text);
	discussion->priv->text = g_strdup (text);
}

