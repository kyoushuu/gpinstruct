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

#ifndef _GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_H_
#define _GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE             (gpinstruct_lesson_test_scrambled_page_get_type ())
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE, GPInstructLessonTestScrambledPage))
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE, GPInstructLessonTestScrambledPageClass))
#define GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED_PAGE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE))
#define GPINSTRUCT_IS_LESSON_TEST_SCRAMBLED_PAGE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE))
#define GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_SCRAMBLED_PAGE, GPInstructLessonTestScrambledPageClass))

typedef struct _GPInstructLessonTestScrambledPagePrivate GPInstructLessonTestScrambledPagePrivate;
typedef struct _GPInstructLessonTestScrambledPageClass GPInstructLessonTestScrambledPageClass;
typedef struct _GPInstructLessonTestScrambledPage GPInstructLessonTestScrambledPage;

struct _GPInstructLessonTestScrambledPageClass
{
	GPInstructLessonViewPageClass parent_class;
};

struct _GPInstructLessonTestScrambledPage
{
	GPInstructLessonViewPage parent_instance;

	GPInstructLessonTestScrambledPagePrivate* priv;
};

GType gpinstruct_lesson_test_scrambled_page_get_type (void) G_GNUC_CONST;
GPInstructLessonTestScrambledPage* gpinstruct_lesson_test_scrambled_page_new (GPInstructLessonTestScrambled* test, GPInstructLessonScore* score, GPInstructLog* log);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_SCRAMBLED_PAGE_H_ */
