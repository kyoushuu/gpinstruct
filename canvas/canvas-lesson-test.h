/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * canvas
 * Copyright (C) Arnel A. Borja 2011 <galeon@ymail.com>
 * 
 * canvas is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * canvas is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CANVAS_LESSON_TEST_H_
#define _CANVAS_LESSON_TEST_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_TEST             (canvas_lesson_test_get_type ())
#define CANVAS_LESSON_TEST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_TEST, CanvasLessonTest))
#define CANVAS_LESSON_TEST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_TEST, CanvasLessonTestClass))
#define CANVAS_IS_LESSON_TEST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_TEST))
#define CANVAS_IS_LESSON_TEST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_TEST))
#define CANVAS_LESSON_TEST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_TEST, CanvasLessonTestClass))

typedef struct _CanvasLessonTestPrivate CanvasLessonTestPrivate;
typedef struct _CanvasLessonTestClass CanvasLessonTestClass;
typedef struct _CanvasLessonTest CanvasLessonTest;

struct _CanvasLessonTestClass
{
	CanvasLessonElementClass parent_class;

	gchar*(* get_item) (CanvasLessonTest* test, guint item);
	gchar*(* get_choice) (CanvasLessonTest* test, guint item, guint choice);
	guint(* get_items_length) (CanvasLessonTest* test);
	guint(* get_choices_length) (CanvasLessonTest* test, guint item);
	guint(* get_item_correct_choice) (CanvasLessonTest* test, guint item);
};

struct _CanvasLessonTest
{
	CanvasLessonElement parent_instance;

	CanvasLessonTestPrivate* priv;
};

GType canvas_lesson_test_get_type (void) G_GNUC_CONST;
CanvasLessonTest* canvas_lesson_test_new (void);
const gchar* canvas_lesson_test_get_directions (CanvasLessonTest* test);
void canvas_lesson_test_set_directions (CanvasLessonTest* test, const gchar* directions);
gboolean canvas_lesson_test_get_explain (CanvasLessonTest* test);
void canvas_lesson_test_set_explain (CanvasLessonTest* test, gboolean explain);
const gchar* canvas_lesson_test_get_id (CanvasLessonTest* test);
void canvas_lesson_test_set_id (CanvasLessonTest* test, const gchar* id);
gchar* canvas_lesson_test_get_item (CanvasLessonTest* test, guint item);
gchar* canvas_lesson_test_get_choice (CanvasLessonTest* test, guint item, guint choice);
guint canvas_lesson_test_get_items_length (CanvasLessonTest* test);
guint canvas_lesson_test_get_choices_length (CanvasLessonTest* test, guint item);
guint canvas_lesson_test_get_item_correct_choice (CanvasLessonTest* test, guint item);

G_END_DECLS

#endif /* _CANVAS_LESSON_TEST_H_ */
