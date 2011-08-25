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

#ifndef _GPINSTRUCT_LESSON_TEST_WORD_POOL_H_
#define _GPINSTRUCT_LESSON_TEST_WORD_POOL_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL             (gpinstruct_lesson_test_word_pool_get_type ())
#define GPINSTRUCT_LESSON_TEST_WORD_POOL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL, GPInstructLessonTestWordPool))
#define GPINSTRUCT_LESSON_TEST_WORD_POOL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL, GPInstructLessonTestWordPoolClass))
#define GPINSTRUCT_IS_LESSON_TEST_WORD_POOL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL))
#define GPINSTRUCT_IS_LESSON_TEST_WORD_POOL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL))
#define GPINSTRUCT_LESSON_TEST_WORD_POOL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL, GPInstructLessonTestWordPoolClass))

typedef struct _GPInstructLessonTestWordPoolPrivate GPInstructLessonTestWordPoolPrivate;
typedef struct _GPInstructLessonTestWordPoolClass GPInstructLessonTestWordPoolClass;
typedef struct _GPInstructLessonTestWordPool GPInstructLessonTestWordPool;

struct _GPInstructLessonTestWordPoolClass
{
	GPInstructLessonTestClass parent_class;
};

struct _GPInstructLessonTestWordPool
{
	GPInstructLessonTest parent_instance;

	GPInstructLessonTestWordPoolPrivate* priv;
};

GType gpinstruct_lesson_test_word_pool_get_type (void) G_GNUC_CONST;
GPInstructLessonTestWordPool* gpinstruct_lesson_test_word_pool_new (void);
void gpinstruct_lesson_test_word_pool_add_question (GPInstructLessonTestWordPool* test, GPInstructLessonTestWordPoolQuestion* question);
void gpinstruct_lesson_test_word_pool_remove_question (GPInstructLessonTestWordPool* test, guint question);
GPInstructLessonTestWordPoolQuestion* gpinstruct_lesson_test_word_pool_get_question (GPInstructLessonTestWordPool* test, guint question);
GList* gpinstruct_lesson_test_word_pool_get_questions (GPInstructLessonTestWordPool* test);
guint gpinstruct_lesson_test_word_pool_get_questions_length (GPInstructLessonTestWordPool* test);
void gpinstruct_lesson_test_word_pool_add_choice (GPInstructLessonTestWordPool* test, const gchar* choice);
void gpinstruct_lesson_test_word_pool_remove_choice (GPInstructLessonTestWordPool* test, guint choice);
const gchar* gpinstruct_lesson_test_word_pool_get_choice (GPInstructLessonTestWordPool* test, guint choice);
void gpinstruct_lesson_test_word_pool_set_choice (GPInstructLessonTestWordPool* test, guint choice, const gchar* text);
GList* gpinstruct_lesson_test_word_pool_get_choices (GPInstructLessonTestWordPool* test);
guint gpinstruct_lesson_test_word_pool_get_choices_length (GPInstructLessonTestWordPool* test);
gboolean gpinstruct_lesson_test_word_pool_is_correct (GPInstructLessonTestWordPool* test, guint item, guint answer);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_WORD_POOL_H_ */
