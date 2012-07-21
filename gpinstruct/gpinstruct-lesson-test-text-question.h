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

#ifndef _GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_H_
#define _GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION             (gpinstruct_lesson_test_text_question_get_type ())
#define GPINSTRUCT_LESSON_TEST_TEXT_QUESTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION, GPInstructLessonTestTextQuestion))
#define GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION, GPInstructLessonTestTextQuestionClass))
#define GPINSTRUCT_IS_LESSON_TEST_TEXT_QUESTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION))
#define GPINSTRUCT_IS_LESSON_TEST_TEXT_QUESTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION))
#define GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_QUESTION, GPInstructLessonTestTextQuestionClass))

typedef struct _GPInstructLessonTestTextQuestionPrivate GPInstructLessonTestTextQuestionPrivate;
typedef struct _GPInstructLessonTestTextQuestionClass GPInstructLessonTestTextQuestionClass;
typedef struct _GPInstructLessonTestTextQuestion GPInstructLessonTestTextQuestion;

struct _GPInstructLessonTestTextQuestionClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLessonTestTextQuestion
{
	GPInstructObject parent_instance;

	GPInstructLessonTestTextQuestionPrivate *priv;
};

GType gpinstruct_lesson_test_text_question_get_type (void) G_GNUC_CONST;
GPInstructLessonTestTextQuestion *gpinstruct_lesson_test_text_question_new (void);
const gchar *gpinstruct_lesson_test_text_question_get_text (GPInstructLessonTestTextQuestion *question);
void gpinstruct_lesson_test_text_question_set_text (GPInstructLessonTestTextQuestion *question, const gchar *text);
const gchar *gpinstruct_lesson_test_text_question_get_answer (GPInstructLessonTestTextQuestion *question);
void gpinstruct_lesson_test_text_question_set_answer (GPInstructLessonTestTextQuestion *question, const gchar *answer);
const gchar *gpinstruct_lesson_test_text_question_get_explanation (GPInstructLessonTestTextQuestion *question);
void gpinstruct_lesson_test_text_question_set_explanation (GPInstructLessonTestTextQuestion *question, const gchar *explanation);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_TEXT_QUESTION_H_ */
