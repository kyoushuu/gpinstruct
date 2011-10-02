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

#ifndef _GPINSTRUCT_LESSON_VIEW_H_
#define _GPINSTRUCT_LESSON_VIEW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_VIEW             (gpinstruct_lesson_view_get_type ())
#define GPINSTRUCT_LESSON_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_VIEW, GPInstructLessonView))
#define GPINSTRUCT_LESSON_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_VIEW, GPInstructLessonViewClass))
#define GPINSTRUCT_IS_LESSON_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_VIEW))
#define GPINSTRUCT_IS_LESSON_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_VIEW))
#define GPINSTRUCT_LESSON_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_VIEW, GPInstructLessonViewClass))

typedef struct _GPInstructLessonViewPrivate GPInstructLessonViewPrivate;
typedef struct _GPInstructLessonViewClass GPInstructLessonViewClass;
typedef struct _GPInstructLessonView GPInstructLessonView;

struct _GPInstructLessonViewClass
{
	GtkDialogClass parent_class;

	/* Signals */
	gboolean(* next) (GPInstructLessonView* view, gpointer user_data);
	gboolean(* back) (GPInstructLessonView* view, gpointer user_data);
};

struct _GPInstructLessonView
{
	GtkDialog parent_instance;

	GPInstructLessonViewPrivate* priv;
};

GType gpinstruct_lesson_view_get_type (void) G_GNUC_CONST;
GPInstructLessonView* gpinstruct_lesson_view_new (GPInstructLesson* lesson, GPInstructMessagePool* pool, GPInstructLog* log);
void gpinstruct_lesson_view_append_page (GPInstructLessonView* view, GPInstructLessonViewPage* page);
guint gpinstruct_lesson_view_get_current_page (GPInstructLessonView* view);
void gpinstruct_lesson_view_set_current_page (GPInstructLessonView* view, guint page);
GPInstructLessonViewPage* gpinstruct_lesson_view_get_current_page_object (GPInstructLessonView* view);
void gpinstruct_lesson_view_set_explanation (GPInstructLessonView* view, const gchar* explanation);
const gchar* gpinstruct_lesson_view_get_explanation (GPInstructLessonView* view);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_VIEW_H_ */
