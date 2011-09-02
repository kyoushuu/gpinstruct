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

#ifndef _CANVAS_LESSON_VIEW_PAGE_H_
#define _CANVAS_LESSON_VIEW_PAGE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_VIEW_PAGE             (canvas_lesson_view_page_get_type ())
#define CANVAS_LESSON_VIEW_PAGE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_VIEW_PAGE, CanvasLessonViewPage))
#define CANVAS_LESSON_VIEW_PAGE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_VIEW_PAGE, CanvasLessonViewPageClass))
#define CANVAS_IS_LESSON_VIEW_PAGE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_VIEW_PAGE))
#define CANVAS_IS_LESSON_VIEW_PAGE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_VIEW_PAGE))
#define CANVAS_LESSON_VIEW_PAGE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_VIEW_PAGE, CanvasLessonViewPageClass))

typedef struct _CanvasLessonViewPageClass CanvasLessonViewPageClass;
typedef struct _CanvasLessonViewPage CanvasLessonViewPage;

struct _CanvasLessonViewPageClass
{
	GtkScrolledWindowClass parent_class;

	/* Signals */
	gboolean(* show_next) (CanvasLessonViewPage* view, gpointer user_data);
	gboolean(* show_previous) (CanvasLessonViewPage* view, gpointer user_data);
	void(* show_current) (CanvasLessonViewPage* view, gpointer user_data);
	void(* reset) (CanvasLessonViewPage* view, gpointer user_data);
};

struct _CanvasLessonViewPage
{
	GtkScrolledWindow parent_instance;
};

GType canvas_lesson_view_page_get_type (void) G_GNUC_CONST;
CanvasLessonViewPage* canvas_lesson_view_page_new (void);
const gchar* canvas_lesson_view_page_get_title (CanvasLessonViewPage* page);
void canvas_lesson_view_page_set_title (CanvasLessonViewPage* page, const gchar* title);
gboolean canvas_lesson_view_page_get_show_next_button (CanvasLessonViewPage* page);
void canvas_lesson_view_page_set_show_next_button (CanvasLessonViewPage* page, gboolean show);
gboolean canvas_lesson_view_page_get_show_back_button (CanvasLessonViewPage* page);
void canvas_lesson_view_page_set_show_back_button (CanvasLessonViewPage* page, gboolean show);
gboolean canvas_lesson_view_page_show_next (CanvasLessonViewPage* page);
gboolean canvas_lesson_view_page_show_previous (CanvasLessonViewPage* page);
void canvas_lesson_view_page_show_current (CanvasLessonViewPage* page);
void canvas_lesson_view_page_reset (CanvasLessonViewPage* page);

G_END_DECLS

#endif /* _CANVAS_LESSON_VIEW_PAGE_H_ */
