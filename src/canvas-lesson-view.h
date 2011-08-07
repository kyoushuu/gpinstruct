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

#ifndef _CANVAS_LESSON_VIEW_H_
#define _CANVAS_LESSON_VIEW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_VIEW             (canvas_lesson_view_get_type ())
#define CANVAS_LESSON_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_VIEW, CanvasLessonView))
#define CANVAS_LESSON_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_VIEW, CanvasLessonViewClass))
#define CANVAS_IS_LESSON_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_VIEW))
#define CANVAS_IS_LESSON_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_VIEW))
#define CANVAS_LESSON_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_VIEW, CanvasLessonViewClass))

typedef struct _CanvasLessonViewClass CanvasLessonViewClass;
typedef struct _CanvasLessonView CanvasLessonView;

struct _CanvasLessonViewClass
{
	GtkDialogClass parent_class;

	/* Signals */
	gboolean(* next) (CanvasLessonView* view, gpointer user_data);
	gboolean(* back) (CanvasLessonView* view, gpointer user_data);
};

struct _CanvasLessonView
{
	GtkDialog parent_instance;
};

GType canvas_lesson_view_get_type (void) G_GNUC_CONST;
CanvasLessonView* canvas_lesson_view_new (CanvasLesson* lesson, CanvasMessagePool* pool);
void canvas_lesson_view_append_page (CanvasLessonView* view, CanvasLessonViewPage* page);
guint canvas_lesson_view_get_current_page (CanvasLessonView* view);
void canvas_lesson_view_set_current_page (CanvasLessonView* view, guint page);
CanvasLessonViewPage* canvas_lesson_view_get_current_page_object (CanvasLessonView* view);
void canvas_lesson_view_set_explanation (CanvasLessonView* view, const gchar* explanation);
const gchar* canvas_lesson_view_get_explanation (CanvasLessonView* view);

G_END_DECLS

#endif /* _CANVAS_LESSON_VIEW_H_ */
