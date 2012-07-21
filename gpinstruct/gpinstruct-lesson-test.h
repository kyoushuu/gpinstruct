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

#ifndef _GPINSTRUCT_LESSON_TEST_H_
#define _GPINSTRUCT_LESSON_TEST_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST             (gpinstruct_lesson_test_get_type ())
#define GPINSTRUCT_LESSON_TEST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST, GPInstructLessonTest))
#define GPINSTRUCT_LESSON_TEST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST, GPInstructLessonTestClass))
#define GPINSTRUCT_IS_LESSON_TEST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST))
#define GPINSTRUCT_IS_LESSON_TEST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST))
#define GPINSTRUCT_LESSON_TEST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST, GPInstructLessonTestClass))

typedef struct _GPInstructLessonTestPrivate GPInstructLessonTestPrivate;
typedef struct _GPInstructLessonTestClass GPInstructLessonTestClass;
typedef struct _GPInstructLessonTest GPInstructLessonTest;

struct _GPInstructLessonTestClass
{
	GPInstructLessonElementClass parent_class;

	gboolean answer_is_string;
	gchar*(* get_item) (GPInstructLessonTest *test, guint item);
	gchar*(* get_choice) (GPInstructLessonTest *test, guint item, guint choice);
	guint(* get_items_length) (GPInstructLessonTest *test);
	guint(* get_choices_length) (GPInstructLessonTest *test, guint item);
	guint(* get_item_correct_choice) (GPInstructLessonTest *test, guint item);
	gchar*(* get_item_correct_string) (GPInstructLessonTest *test, guint item);
};

struct _GPInstructLessonTest
{
	GPInstructLessonElement parent_instance;

	GPInstructLessonTestPrivate *priv;
};

GType gpinstruct_lesson_test_get_type (void) G_GNUC_CONST;
GPInstructLessonTest *gpinstruct_lesson_test_new (void);
const gchar *gpinstruct_lesson_test_get_directions (GPInstructLessonTest *test);
void gpinstruct_lesson_test_set_directions (GPInstructLessonTest *test, const gchar *directions);
gboolean gpinstruct_lesson_test_get_explain (GPInstructLessonTest *test);
void gpinstruct_lesson_test_set_explain (GPInstructLessonTest *test, gboolean explain);
const gchar *gpinstruct_lesson_test_get_id (GPInstructLessonTest *test);
void gpinstruct_lesson_test_set_id (GPInstructLessonTest *test, const gchar *id);
gchar *gpinstruct_lesson_test_get_item (GPInstructLessonTest *test, guint item);
gchar *gpinstruct_lesson_test_get_choice (GPInstructLessonTest *test, guint item, guint choice);
guint gpinstruct_lesson_test_get_items_length (GPInstructLessonTest *test);
gboolean gpinstruct_lesson_test_get_answer_is_string (GPInstructLessonTest *test);
guint gpinstruct_lesson_test_get_choices_length (GPInstructLessonTest *test, guint item);
guint gpinstruct_lesson_test_get_item_correct_choice (GPInstructLessonTest *test, guint item);
gchar *gpinstruct_lesson_test_get_item_correct_string (GPInstructLessonTest *test, guint item);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_H_ */
