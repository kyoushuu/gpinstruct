/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja <kyoushuu@yahoo.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_H_
#define _GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION             (gpinstruct_lesson_test_word_pool_question_get_type ())
#define GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION, GPInstructLessonTestWordPoolQuestion))
#define GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION, GPInstructLessonTestWordPoolQuestionClass))
#define GPINSTRUCT_IS_LESSON_TEST_WORD_POOL_QUESTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION))
#define GPINSTRUCT_IS_LESSON_TEST_WORD_POOL_QUESTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION))
#define GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_WORD_POOL_QUESTION, GPInstructLessonTestWordPoolQuestionClass))

typedef struct _GPInstructLessonTestWordPoolQuestionPrivate GPInstructLessonTestWordPoolQuestionPrivate;
typedef struct _GPInstructLessonTestWordPoolQuestionClass GPInstructLessonTestWordPoolQuestionClass;
typedef struct _GPInstructLessonTestWordPoolQuestion GPInstructLessonTestWordPoolQuestion;

struct _GPInstructLessonTestWordPoolQuestionClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLessonTestWordPoolQuestion
{
	GPInstructObject parent_instance;

	GPInstructLessonTestWordPoolQuestionPrivate *priv;
};

GType gpinstruct_lesson_test_word_pool_question_get_type (void) G_GNUC_CONST;
GPInstructLessonTestWordPoolQuestion *gpinstruct_lesson_test_word_pool_question_new (void);
const gchar *gpinstruct_lesson_test_word_pool_question_get_text (GPInstructLessonTestWordPoolQuestion *question);
void gpinstruct_lesson_test_word_pool_question_set_text (GPInstructLessonTestWordPoolQuestion *question, const gchar *text);
guint gpinstruct_lesson_test_word_pool_question_get_answer (GPInstructLessonTestWordPoolQuestion *question);
void gpinstruct_lesson_test_word_pool_question_set_answer (GPInstructLessonTestWordPoolQuestion *question, guint answer);
const gchar *gpinstruct_lesson_test_word_pool_question_get_explanation (GPInstructLessonTestWordPoolQuestion *question);
void gpinstruct_lesson_test_word_pool_question_set_explanation (GPInstructLessonTestWordPoolQuestion *question, const gchar *explanation);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_WORD_POOL_QUESTION_H_ */
