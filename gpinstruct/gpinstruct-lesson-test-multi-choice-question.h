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

#ifndef _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_H_
#define _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION             (gpinstruct_lesson_test_multi_choice_question_get_type ())
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, GPInstructLessonTestMultiChoiceQuestion))
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, GPInstructLessonTestMultiChoiceQuestionClass))
#define GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_QUESTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION))
#define GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_QUESTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION))
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_QUESTION, GPInstructLessonTestMultiChoiceQuestionClass))

typedef struct _GPInstructLessonTestMultiChoiceQuestionPrivate GPInstructLessonTestMultiChoiceQuestionPrivate;
typedef struct _GPInstructLessonTestMultiChoiceQuestionClass GPInstructLessonTestMultiChoiceQuestionClass;
typedef struct _GPInstructLessonTestMultiChoiceQuestion GPInstructLessonTestMultiChoiceQuestion;

struct _GPInstructLessonTestMultiChoiceQuestionClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLessonTestMultiChoiceQuestion
{
	GPInstructObject parent_instance;

	GPInstructLessonTestMultiChoiceQuestionPrivate* priv;
};

GType gpinstruct_lesson_test_multi_choice_question_get_type (void) G_GNUC_CONST;
GPInstructLessonTestMultiChoiceQuestion* gpinstruct_lesson_test_multi_choice_question_new (void);
const gchar* gpinstruct_lesson_test_multi_choice_question_get_text (GPInstructLessonTestMultiChoiceQuestion* question);
void gpinstruct_lesson_test_multi_choice_question_set_text (GPInstructLessonTestMultiChoiceQuestion* question, const gchar* text);
guint gpinstruct_lesson_test_multi_choice_question_get_answer (GPInstructLessonTestMultiChoiceQuestion* question);
void gpinstruct_lesson_test_multi_choice_question_set_answer (GPInstructLessonTestMultiChoiceQuestion* question, guint answer);
void gpinstruct_lesson_test_multi_choice_question_add_choice (GPInstructLessonTestMultiChoiceQuestion* question, const gchar* choice);
void gpinstruct_lesson_test_multi_choice_question_remove_choice (GPInstructLessonTestMultiChoiceQuestion* question, guint choice);
const gchar* gpinstruct_lesson_test_multi_choice_question_get_choice (GPInstructLessonTestMultiChoiceQuestion* question, guint choice);
void gpinstruct_lesson_test_multi_choice_question_set_choice (GPInstructLessonTestMultiChoiceQuestion* question, guint choice, const gchar* text);
GList* gpinstruct_lesson_test_multi_choice_question_get_choices (GPInstructLessonTestMultiChoiceQuestion* question);
guint gpinstruct_lesson_test_multi_choice_question_get_choices_length (GPInstructLessonTestMultiChoiceQuestion* question);
const gchar* gpinstruct_lesson_test_multi_choice_question_get_explanation (GPInstructLessonTestMultiChoiceQuestion* question);
void gpinstruct_lesson_test_multi_choice_question_set_explanation (GPInstructLessonTestMultiChoiceQuestion* question, const gchar* explanation);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_QUESTION_H_ */
