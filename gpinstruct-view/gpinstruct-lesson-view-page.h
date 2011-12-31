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

#ifndef _GPINSTRUCT_LESSON_VIEW_PAGE_H_
#define _GPINSTRUCT_LESSON_VIEW_PAGE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_VIEW_PAGE             (gpinstruct_lesson_view_page_get_type ())
#define GPINSTRUCT_LESSON_VIEW_PAGE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_VIEW_PAGE, GPInstructLessonViewPage))
#define GPINSTRUCT_LESSON_VIEW_PAGE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_VIEW_PAGE, GPInstructLessonViewPageClass))
#define GPINSTRUCT_IS_LESSON_VIEW_PAGE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_VIEW_PAGE))
#define GPINSTRUCT_IS_LESSON_VIEW_PAGE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_VIEW_PAGE))
#define GPINSTRUCT_LESSON_VIEW_PAGE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_VIEW_PAGE, GPInstructLessonViewPageClass))

typedef struct _GPInstructLessonViewPagePrivate GPInstructLessonViewPagePrivate;
typedef struct _GPInstructLessonViewPageClass GPInstructLessonViewPageClass;
typedef struct _GPInstructLessonViewPage GPInstructLessonViewPage;

struct _GPInstructLessonViewPageClass
{
	GtkScrolledWindowClass parent_class;

	/* Signals */
	gboolean(* show_next) (GPInstructLessonViewPage *view, gpointer user_data);
	gboolean(* show_previous) (GPInstructLessonViewPage *view, gpointer user_data);
	void(* show_current) (GPInstructLessonViewPage *view, gpointer user_data);
	void(* reset) (GPInstructLessonViewPage *view, gpointer user_data);
};

struct _GPInstructLessonViewPage
{
	GtkScrolledWindow parent_instance;

	GPInstructLessonViewPagePrivate *priv;
};

GType gpinstruct_lesson_view_page_get_type (void) G_GNUC_CONST;
GPInstructLessonViewPage *gpinstruct_lesson_view_page_new (void);
const gchar *gpinstruct_lesson_view_page_get_title (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_set_title (GPInstructLessonViewPage *page, const gchar *title);
GPInstructMessageType gpinstruct_lesson_view_page_get_message (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_set_message (GPInstructLessonViewPage *page, GPInstructMessageType message);
const gchar *gpinstruct_lesson_view_page_get_explanation (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_set_explanation (GPInstructLessonViewPage *page, const gchar *explanation);
gboolean gpinstruct_lesson_view_page_get_show_next_button (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_set_show_next_button (GPInstructLessonViewPage *page, gboolean show);
gboolean gpinstruct_lesson_view_page_get_show_back_button (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_set_show_back_button (GPInstructLessonViewPage *page, gboolean show);
gboolean gpinstruct_lesson_view_page_show_next (GPInstructLessonViewPage *page);
gboolean gpinstruct_lesson_view_page_show_previous (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_show_current (GPInstructLessonViewPage *page);
void gpinstruct_lesson_view_page_reset (GPInstructLessonViewPage *page);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_VIEW_PAGE_H_ */
