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

#ifndef _GPINSTRUCT_LESSON_SCORE_H_
#define _GPINSTRUCT_LESSON_SCORE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_SCORE             (gpinstruct_lesson_score_get_type ())
#define GPINSTRUCT_LESSON_SCORE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_SCORE, GPInstructLessonScore))
#define GPINSTRUCT_LESSON_SCORE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_SCORE, GPInstructLessonScoreClass))
#define GPINSTRUCT_IS_LESSON_SCORE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_SCORE))
#define GPINSTRUCT_IS_LESSON_SCORE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_SCORE))
#define GPINSTRUCT_LESSON_SCORE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_SCORE, GPInstructLessonScoreClass))

typedef struct _GPInstructLessonScorePrivate GPInstructLessonScorePrivate;
typedef struct _GPInstructLessonScoreClass GPInstructLessonScoreClass;
typedef struct _GPInstructLessonScore GPInstructLessonScore;

struct _GPInstructLessonScoreClass
{
	GPInstructLessonElementClass parent_class;
};

struct _GPInstructLessonScore
{
	GPInstructLessonElement parent_instance;

	GPInstructLessonScorePrivate* priv;
};

GType gpinstruct_lesson_score_get_type (void) G_GNUC_CONST;
GPInstructLessonScore* gpinstruct_lesson_score_new (void);
void gpinstruct_lesson_score_increase_total (GPInstructLessonScore* score);
guint gpinstruct_lesson_score_get_total (GPInstructLessonScore* score);
void gpinstruct_lesson_score_increase_score (GPInstructLessonScore* score);
guint gpinstruct_lesson_score_get_score (GPInstructLessonScore* score);
void gpinstruct_lesson_score_clear (GPInstructLessonScore* score);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_SCORE_H_ */
