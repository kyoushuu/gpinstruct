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

#ifndef _CANVAS_LESSON_TEST_WORD_POOL_QUESTION_H_
#define _CANVAS_LESSON_TEST_WORD_POOL_QUESTION_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION             (canvas_lesson_test_word_pool_question_get_type ())
#define CANVAS_LESSON_TEST_WORD_POOL_QUESTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION, CanvasLessonTestWordPoolQuestion))
#define CANVAS_LESSON_TEST_WORD_POOL_QUESTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION, CanvasLessonTestWordPoolQuestionClass))
#define CANVAS_IS_LESSON_TEST_WORD_POOL_QUESTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION))
#define CANVAS_IS_LESSON_TEST_WORD_POOL_QUESTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION))
#define CANVAS_LESSON_TEST_WORD_POOL_QUESTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_TEST_WORD_POOL_QUESTION, CanvasLessonTestWordPoolQuestionClass))

typedef struct _CanvasLessonTestWordPoolQuestionClass CanvasLessonTestWordPoolQuestionClass;
typedef struct _CanvasLessonTestWordPoolQuestion CanvasLessonTestWordPoolQuestion;

struct _CanvasLessonTestWordPoolQuestionClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasLessonTestWordPoolQuestion
{
	CanvasObject parent_instance;
};

GType canvas_lesson_test_word_pool_question_get_type (void) G_GNUC_CONST;
CanvasLessonTestWordPoolQuestion* canvas_lesson_test_word_pool_question_new (void);
const gchar* canvas_lesson_test_word_pool_question_get_text (CanvasLessonTestWordPoolQuestion* question);
void canvas_lesson_test_word_pool_question_set_text (CanvasLessonTestWordPoolQuestion* question, const gchar* text);
guint canvas_lesson_test_word_pool_question_get_answer (CanvasLessonTestWordPoolQuestion* question);
void canvas_lesson_test_word_pool_question_set_answer (CanvasLessonTestWordPoolQuestion* question, guint answer);

G_END_DECLS

#endif /* _CANVAS_LESSON_TEST_WORD_POOL_QUESTION_H_ */
