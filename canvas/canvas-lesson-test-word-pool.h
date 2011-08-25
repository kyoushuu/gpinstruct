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

#ifndef _CANVAS_LESSON_TEST_WORD_POOL_H_
#define _CANVAS_LESSON_TEST_WORD_POOL_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_TEST_WORD_POOL             (canvas_lesson_test_word_pool_get_type ())
#define CANVAS_LESSON_TEST_WORD_POOL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_TEST_WORD_POOL, CanvasLessonTestWordPool))
#define CANVAS_LESSON_TEST_WORD_POOL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_TEST_WORD_POOL, CanvasLessonTestWordPoolClass))
#define CANVAS_IS_LESSON_TEST_WORD_POOL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_TEST_WORD_POOL))
#define CANVAS_IS_LESSON_TEST_WORD_POOL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_TEST_WORD_POOL))
#define CANVAS_LESSON_TEST_WORD_POOL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_TEST_WORD_POOL, CanvasLessonTestWordPoolClass))

typedef struct _CanvasLessonTestWordPoolPrivate CanvasLessonTestWordPoolPrivate;
typedef struct _CanvasLessonTestWordPoolClass CanvasLessonTestWordPoolClass;
typedef struct _CanvasLessonTestWordPool CanvasLessonTestWordPool;

struct _CanvasLessonTestWordPoolClass
{
	CanvasLessonTestClass parent_class;
};

struct _CanvasLessonTestWordPool
{
	CanvasLessonTest parent_instance;

	CanvasLessonTestWordPoolPrivate* priv;
};

GType canvas_lesson_test_word_pool_get_type (void) G_GNUC_CONST;
CanvasLessonTestWordPool* canvas_lesson_test_word_pool_new (void);
void canvas_lesson_test_word_pool_add_question (CanvasLessonTestWordPool* test, CanvasLessonTestWordPoolQuestion* question);
void canvas_lesson_test_word_pool_remove_question (CanvasLessonTestWordPool* test, guint question);
CanvasLessonTestWordPoolQuestion* canvas_lesson_test_word_pool_get_question (CanvasLessonTestWordPool* test, guint question);
GList* canvas_lesson_test_word_pool_get_questions (CanvasLessonTestWordPool* test);
guint canvas_lesson_test_word_pool_get_questions_length (CanvasLessonTestWordPool* test);
void canvas_lesson_test_word_pool_add_choice (CanvasLessonTestWordPool* test, const gchar* choice);
void canvas_lesson_test_word_pool_remove_choice (CanvasLessonTestWordPool* test, guint choice);
const gchar* canvas_lesson_test_word_pool_get_choice (CanvasLessonTestWordPool* test, guint choice);
void canvas_lesson_test_word_pool_set_choice (CanvasLessonTestWordPool* test, guint choice, const gchar* text);
GList* canvas_lesson_test_word_pool_get_choices (CanvasLessonTestWordPool* test);
guint canvas_lesson_test_word_pool_get_choices_length (CanvasLessonTestWordPool* test);
gboolean canvas_lesson_test_word_pool_is_correct (CanvasLessonTestWordPool* test, guint item, guint answer);

G_END_DECLS

#endif /* _CANVAS_LESSON_TEST_WORD_POOL_H_ */
