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

#ifndef _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_H_
#define _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE             (gpinstruct_lesson_test_multi_choice_get_type ())
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE, GPInstructLessonTestMultiChoice))
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE, GPInstructLessonTestMultiChoiceClass))
#define GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE))
#define GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE))
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE, GPInstructLessonTestMultiChoiceClass))

typedef struct _GPInstructLessonTestMultiChoicePrivate GPInstructLessonTestMultiChoicePrivate;
typedef struct _GPInstructLessonTestMultiChoiceClass GPInstructLessonTestMultiChoiceClass;
typedef struct _GPInstructLessonTestMultiChoice GPInstructLessonTestMultiChoice;

struct _GPInstructLessonTestMultiChoiceClass
{
	GPInstructLessonTestClass parent_class;
};

struct _GPInstructLessonTestMultiChoice
{
	GPInstructLessonTest parent_instance;

	GPInstructLessonTestMultiChoicePrivate* priv;
};

GType gpinstruct_lesson_test_multi_choice_get_type (void) G_GNUC_CONST;
GPInstructLessonTestMultiChoice* gpinstruct_lesson_test_multi_choice_new (void);
void gpinstruct_lesson_test_multi_choice_add_question (GPInstructLessonTestMultiChoice* test, GPInstructLessonTestMultiChoiceQuestion* question);
void gpinstruct_lesson_test_multi_choice_remove_question (GPInstructLessonTestMultiChoice* test, guint question);
GPInstructLessonTestMultiChoiceQuestion* gpinstruct_lesson_test_multi_choice_get_question (GPInstructLessonTestMultiChoice* test, guint question);
GList* gpinstruct_lesson_test_multi_choice_get_questions (GPInstructLessonTestMultiChoice* test);
guint gpinstruct_lesson_test_multi_choice_get_questions_length (GPInstructLessonTestMultiChoice* test);
gboolean gpinstruct_lesson_test_multi_choice_is_correct (GPInstructLessonTestMultiChoice* test, guint item, guint answer);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_H_ */
