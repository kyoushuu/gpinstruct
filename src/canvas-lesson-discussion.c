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

typedef struct _CanvasLessonDiscussionPrivate CanvasLessonDiscussionPrivate;
struct _CanvasLessonDiscussionPrivate
{
	gchar* text;
};

#define CANVAS_LESSON_DISCUSSION_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_DISCUSSION, CanvasLessonDiscussionPrivate))



G_DEFINE_TYPE (CanvasLessonDiscussion, canvas_lesson_discussion, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_discussion_init (CanvasLessonDiscussion *object)
{
	CanvasLessonDiscussionPrivate* private_data = CANVAS_LESSON_DISCUSSION_PRIVATE(object);
	private_data->text = g_strdup ("");
}

static void
canvas_lesson_discussion_finalize (GObject *object)
{
	CanvasLessonDiscussionPrivate* private_data = CANVAS_LESSON_DISCUSSION_PRIVATE(object);

	if (private_data->text)
		g_free (private_data->text);

	G_OBJECT_CLASS (canvas_lesson_discussion_parent_class)->finalize (object);
}

static void
canvas_lesson_discussion_class_init (CanvasLessonDiscussionClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (CanvasLessonDiscussionPrivate));

	object_class->finalize = canvas_lesson_discussion_finalize;
}


CanvasLessonDiscussion*
canvas_lesson_discussion_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_DISCUSSION, NULL);
}

const gchar*
canvas_lesson_discussion_get_text (CanvasLessonDiscussion* discussion)
{
	return CANVAS_LESSON_DISCUSSION_PRIVATE(discussion)->text;
}

void
canvas_lesson_discussion_set_text (CanvasLessonDiscussion* discussion, const gchar* text)
{
	CanvasLessonDiscussionPrivate* private_data = CANVAS_LESSON_DISCUSSION_PRIVATE(discussion);

	if (private_data->text)
		g_free (private_data->text);
	private_data->text = g_strdup (text);
}

