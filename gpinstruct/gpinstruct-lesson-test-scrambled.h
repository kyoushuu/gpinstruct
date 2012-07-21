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

#ifndef _GPINSTRUCT_LESSON_TEST_SCRAMBLED_H_
#define _GPINSTRUCT_LESSON_TEST_SCRAMBLED_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED             (gpinstruct_lesson_test_scrambled_get_type ())
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED, GPInstructLessonTestScrambled))
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED, GPInstructLessonTestScrambledClass))
#define GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED))
#define GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED))
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED, GPInstructLessonTestScrambledClass))

typedef struct _GPInstructLessonTestScrambledPrivate GPInstructLessonTestScrambledPrivate;
typedef struct _GPInstructLessonTestScrambledClass GPInstructLessonTestScrambledClass;
typedef struct _GPInstructLessonTestScrambled GPInstructLessonTestScrambled;

struct _GPInstructLessonTestScrambledClass
{
	GPInstructLessonTestClass parent_class;
};

struct _GPInstructLessonTestScrambled
{
	GPInstructLessonTest parent_instance;

	GPInstructLessonTestScrambledPrivate *priv;
};

GType gpinstruct_lesson_test_scrambled_get_type (void) G_GNUC_CONST;
GPInstructLessonTestScrambled *gpinstruct_lesson_test_scrambled_new (void);
void gpinstruct_lesson_test_scrambled_add_question (GPInstructLessonTestScrambled *test, GPInstructLessonTestScrambledQuestion *question);
void gpinstruct_lesson_test_scrambled_remove_question (GPInstructLessonTestScrambled *test, guint question);
GPInstructLessonTestScrambledQuestion *gpinstruct_lesson_test_scrambled_get_question (GPInstructLessonTestScrambled *test, guint question);
GList *gpinstruct_lesson_test_scrambled_get_questions (GPInstructLessonTestScrambled *test);
guint gpinstruct_lesson_test_scrambled_get_questions_length (GPInstructLessonTestScrambled *test);
gboolean gpinstruct_lesson_test_scrambled_is_correct (GPInstructLessonTestScrambled *test, guint item, gchar *answer);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_SCRAMBLED_H_ */
