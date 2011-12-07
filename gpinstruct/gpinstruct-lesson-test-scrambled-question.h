/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011  Arnel A. Borja <kyoushuu@yahoo.com>
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

#ifndef _GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_H_
#define _GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION             (gpinstruct_lesson_test_scrambled_question_get_type ())
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION, GPInstructLessonTestScrambledQuestion))
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION, GPInstructLessonTestScrambledQuestionClass))
#define GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED_QUESTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION))
#define GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED_QUESTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION))
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_QUESTION, GPInstructLessonTestScrambledQuestionClass))

typedef struct _GPInstructLessonTestScrambledQuestionPrivate GPInstructLessonTestScrambledQuestionPrivate;
typedef struct _GPInstructLessonTestScrambledQuestionClass GPInstructLessonTestScrambledQuestionClass;
typedef struct _GPInstructLessonTestScrambledQuestion GPInstructLessonTestScrambledQuestion;

struct _GPInstructLessonTestScrambledQuestionClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLessonTestScrambledQuestion
{
	GPInstructObject parent_instance;

	GPInstructLessonTestScrambledQuestionPrivate* priv;
};

GType gpinstruct_lesson_test_scrambled_question_get_type (void) G_GNUC_CONST;
GPInstructLessonTestScrambledQuestion* gpinstruct_lesson_test_scrambled_question_new (void);
const gchar* gpinstruct_lesson_test_scrambled_question_get_text (GPInstructLessonTestScrambledQuestion* question);
void gpinstruct_lesson_test_scrambled_question_set_text (GPInstructLessonTestScrambledQuestion* question, const gchar* text);
const gchar* gpinstruct_lesson_test_scrambled_question_get_answer (GPInstructLessonTestScrambledQuestion* question);
void gpinstruct_lesson_test_scrambled_question_set_answer (GPInstructLessonTestScrambledQuestion* question, const gchar* answer);
const gchar* gpinstruct_lesson_test_scrambled_question_get_explanation (GPInstructLessonTestScrambledQuestion* question);
void gpinstruct_lesson_test_scrambled_question_set_explanation (GPInstructLessonTestScrambledQuestion* question, const gchar* explanation);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_SCRAMBLED_QUESTION_H_ */
