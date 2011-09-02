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

typedef struct _CanvasLessonScorePrivate CanvasLessonScorePrivate;
struct _CanvasLessonScorePrivate
{
	guint total;
	guint score;
};

#define CANVAS_LESSON_SCORE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_LESSON_SCORE, CanvasLessonScorePrivate))



G_DEFINE_TYPE (CanvasLessonScore, canvas_lesson_score, CANVAS_TYPE_LESSON_ELEMENT);

static void
canvas_lesson_score_init (CanvasLessonScore *object)
{
	CanvasLessonScorePrivate* priv = CANVAS_LESSON_SCORE_PRIVATE (object);

	priv->total = 0;
	priv->score = 0;
}

static void
canvas_lesson_score_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (canvas_lesson_score_parent_class)->finalize (object);
}

static void
canvas_lesson_score_class_init (CanvasLessonScoreClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*CanvasLessonElementClass* parent_class = CANVAS_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasLessonScorePrivate));

	object_class->finalize = canvas_lesson_score_finalize;
}


CanvasLessonScore*
canvas_lesson_score_new (void)
{
	return g_object_new(CANVAS_TYPE_LESSON_SCORE, NULL);
}

void
canvas_lesson_score_increase_total (CanvasLessonScore* score)
{
	CANVAS_LESSON_SCORE_PRIVATE (score)->total++;
}

guint
canvas_lesson_score_get_total (CanvasLessonScore* score)
{
	return CANVAS_LESSON_SCORE_PRIVATE (score)->total;
}

void
canvas_lesson_score_increase_score (CanvasLessonScore* score)
{
	CANVAS_LESSON_SCORE_PRIVATE (score)->score++;
}

guint
canvas_lesson_score_get_score (CanvasLessonScore* score)
{
	return CANVAS_LESSON_SCORE_PRIVATE (score)->score;
}

void
canvas_lesson_score_clear (CanvasLessonScore* score)
{
	CANVAS_LESSON_SCORE_PRIVATE (score)->total = 0;
	CANVAS_LESSON_SCORE_PRIVATE (score)->score = 0;
}
