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

#ifndef _GPINSTRUCT_LESSON_TEST_TEXT_H_
#define _GPINSTRUCT_LESSON_TEST_TEXT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_TEXT             (gpinstruct_lesson_test_text_get_type ())
#define GPINSTRUCT_LESSON_TEST_TEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT, GPInstructLessonTestText))
#define GPINSTRUCT_LESSON_TEST_TEXT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_TEXT, GPInstructLessonTestTextClass))
#define GPINSTRUCT_IS_LESSON_TEST_TEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT))
#define GPINSTRUCT_IS_LESSON_TEST_TEXT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_TEXT))
#define GPINSTRUCT_LESSON_TEST_TEXT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT, GPInstructLessonTestTextClass))

typedef struct _GPInstructLessonTestTextPrivate GPInstructLessonTestTextPrivate;
typedef struct _GPInstructLessonTestTextClass GPInstructLessonTestTextClass;
typedef struct _GPInstructLessonTestText GPInstructLessonTestText;

struct _GPInstructLessonTestTextClass
{
	GPInstructLessonTestClass parent_class;
};

struct _GPInstructLessonTestText
{
	GPInstructLessonTest parent_instance;

	GPInstructLessonTestTextPrivate *priv;
};

GType gpinstruct_lesson_test_text_get_type (void) G_GNUC_CONST;
GPInstructLessonTestText *gpinstruct_lesson_test_text_new (void);
void gpinstruct_lesson_test_text_add_question (GPInstructLessonTestText *test, GPInstructLessonTestTextQuestion *question);
void gpinstruct_lesson_test_text_remove_question (GPInstructLessonTestText *test, guint question);
GPInstructLessonTestTextQuestion *gpinstruct_lesson_test_text_get_question (GPInstructLessonTestText *test, guint question);
GList *gpinstruct_lesson_test_text_get_questions (GPInstructLessonTestText *test);
guint gpinstruct_lesson_test_text_get_questions_length (GPInstructLessonTestText *test);
gboolean gpinstruct_lesson_test_text_is_correct (GPInstructLessonTestText *test, guint item, gchar *answer);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_TEXT_H_ */
