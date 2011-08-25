/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"

struct _GPInstructLessonScorePrivate
{
	guint total;
	guint score;
};

#define GPINSTRUCT_LESSON_SCORE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_SCORE, GPInstructLessonScorePrivate))



G_DEFINE_TYPE (GPInstructLessonScore, gpinstruct_lesson_score, GPINSTRUCT_TYPE_LESSON_ELEMENT);

static void
gpinstruct_lesson_score_init (GPInstructLessonScore *object)
{
	object->priv = GPINSTRUCT_LESSON_SCORE_PRIVATE (object);

	object->priv->total = 0;
	object->priv->score = 0;
}

static void
gpinstruct_lesson_score_finalize (GObject *object)
{
	G_OBJECT_CLASS (gpinstruct_lesson_score_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_score_class_init (GPInstructLessonScoreClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonElementClass* parent_class = GPINSTRUCT_LESSON_ELEMENT_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonScorePrivate));

	object_class->finalize = gpinstruct_lesson_score_finalize;
}


GPInstructLessonScore*
gpinstruct_lesson_score_new (void)
{
	return g_object_new (GPINSTRUCT_TYPE_LESSON_SCORE, NULL);
}

void
gpinstruct_lesson_score_increase_total (GPInstructLessonScore* score)
{
	score->priv->total++;
}

guint
gpinstruct_lesson_score_get_total (GPInstructLessonScore* score)
{
	return score->priv->total;
}

void
gpinstruct_lesson_score_increase_score (GPInstructLessonScore* score)
{
	score->priv->score++;
}

guint
gpinstruct_lesson_score_get_score (GPInstructLessonScore* score)
{
	return score->priv->score;
}

void
gpinstruct_lesson_score_clear (GPInstructLessonScore* score)
{
	score->priv->total = 0;
	score->priv->score = 0;
}
