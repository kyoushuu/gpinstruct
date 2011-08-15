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

#ifndef _CANVAS_PROJECT_H_
#define _CANVAS_PROJECT_H_

#include <glib-object.h>
#include "canvas-category.h"

G_BEGIN_DECLS

#define CANVAS_TYPE_PROJECT             (canvas_project_get_type ())
#define CANVAS_PROJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_PROJECT, CanvasProject))
#define CANVAS_PROJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_PROJECT, CanvasProjectClass))
#define CANVAS_IS_PROJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_PROJECT))
#define CANVAS_IS_PROJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_PROJECT))
#define CANVAS_PROJECT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_PROJECT, CanvasProjectClass))

typedef struct _CanvasProjectClass CanvasProjectClass;
typedef struct _CanvasProject CanvasProject;

struct _CanvasProjectClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasProject
{
	CanvasObject parent_instance;
};

GType canvas_project_get_type (void) G_GNUC_CONST;
CanvasProject* canvas_project_new (void);
const gchar* canvas_project_get_title (CanvasProject* project);
void canvas_project_set_title (CanvasProject* project, const gchar *title);
guint canvas_project_get_categories_length (CanvasProject* project);
void canvas_project_add_category (CanvasProject* project, CanvasCategory* category);
void canvas_project_remove_category (CanvasProject* project, guint category);
GList* canvas_project_get_categories (CanvasProject* project);

G_END_DECLS

#endif /* _CANVAS_PROJECT_H_ */
