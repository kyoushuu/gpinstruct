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

#ifndef _CANVAS_EDITOR_WINDOW_H_
#define _CANVAS_EDITOR_WINDOW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_EDITOR_WINDOW             (canvas_editor_window_get_type ())
#define CANVAS_EDITOR_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_EDITOR_WINDOW, CanvasEditorWindow))
#define CANVAS_EDITOR_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_EDITOR_WINDOW, CanvasEditorWindowClass))
#define CANVAS_IS_EDITOR_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_EDITOR_WINDOW))
#define CANVAS_IS_EDITOR_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_EDITOR_WINDOW))
#define CANVAS_EDITOR_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_EDITOR_WINDOW, CanvasEditorWindowClass))

typedef struct _CanvasEditorWindowClass CanvasEditorWindowClass;
typedef struct _CanvasEditorWindow CanvasEditorWindow;

struct _CanvasEditorWindowClass
{
	GtkWindowClass parent_class;
};

struct _CanvasEditorWindow
{
	GtkWindow parent_instance;
};

GType canvas_editor_window_get_type (void) G_GNUC_CONST;
GtkWidget* canvas_editor_window_new (void);
const gchar* canvas_editor_window_get_filename (CanvasEditorWindow* window);
gboolean canvas_editor_window_get_modified (CanvasEditorWindow* window);
void canvas_editor_window_set_modified (CanvasEditorWindow* window, gboolean modified);
void canvas_editor_window_new_file (CanvasEditorWindow* window);
void canvas_editor_window_open_file (CanvasEditorWindow* window, const gchar* file);
gboolean canvas_editor_window_close_current_file (CanvasEditorWindow* window);
gboolean canvas_editor_window_save_file (CanvasEditorWindow* window);
gboolean canvas_editor_window_save_file_as (CanvasEditorWindow* window, const gchar* file);
void canvas_editor_window_update_tree_store (CanvasEditorWindow* window, CanvasObject* object);
gboolean canvas_editor_window_quit (CanvasEditorWindow* window);

G_END_DECLS

#endif /* _CANVAS_EDITOR_WINDOW_H_ */
