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

#ifndef _CANVAS_CATEGORY_H_
#define _CANVAS_CATEGORY_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_CATEGORY             (canvas_category_get_type ())
#define CANVAS_CATEGORY(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_CATEGORY, CanvasCategory))
#define CANVAS_CATEGORY_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_CATEGORY, CanvasCategoryClass))
#define CANVAS_IS_CATEGORY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_CATEGORY))
#define CANVAS_IS_CATEGORY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_CATEGORY))
#define CANVAS_CATEGORY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_CATEGORY, CanvasCategoryClass))

typedef struct _CanvasCategoryClass CanvasCategoryClass;
typedef struct _CanvasCategory CanvasCategory;

struct _CanvasCategoryClass
{
	GObjectClass parent_class;
};

struct _CanvasCategory
{
	GObject parent_instance;
};

GType canvas_category_get_type (void) G_GNUC_CONST;
CanvasCategory* canvas_category_new (void);
const gchar* canvas_category_get_title (CanvasCategory* category);
void canvas_category_set_title (CanvasCategory* category, const gchar *title);
guint canvas_category_get_lessons_length (CanvasCategory* category);
void canvas_category_add_lesson (CanvasCategory* category, CanvasLesson* lesson);
void canvas_category_remove_lesson (CanvasCategory* category, CanvasLesson* lesson);

G_END_DECLS

#endif /* _CANVAS_CATEGORY_H_ */
