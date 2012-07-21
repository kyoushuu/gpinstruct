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

#ifndef _GPINSTRUCT_EDITOR_WINDOW_H_
#define _GPINSTRUCT_EDITOR_WINDOW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_EDITOR_WINDOW             (gpinstruct_editor_window_get_type ())
#define GPINSTRUCT_EDITOR_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_EDITOR_WINDOW, GPInstructEditorWindow))
#define GPINSTRUCT_EDITOR_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_EDITOR_WINDOW, GPInstructEditorWindowClass))
#define GPINSTRUCT_IS_EDITOR_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_EDITOR_WINDOW))
#define GPINSTRUCT_IS_EDITOR_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_EDITOR_WINDOW))
#define GPINSTRUCT_EDITOR_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_EDITOR_WINDOW, GPInstructEditorWindowClass))

typedef struct _GPInstructEditorWindowPrivate GPInstructEditorWindowPrivate;
typedef struct _GPInstructEditorWindowClass GPInstructEditorWindowClass;
typedef struct _GPInstructEditorWindow GPInstructEditorWindow;

struct _GPInstructEditorWindowClass
{
	GtkWindowClass parent_class;
};

struct _GPInstructEditorWindow
{
	GtkWindow parent_instance;

	GPInstructEditorWindowPrivate *priv;
};

GType gpinstruct_editor_window_get_type (void) G_GNUC_CONST;
GtkWidget *gpinstruct_editor_window_new (void);
const gchar *gpinstruct_editor_window_get_filename (GPInstructEditorWindow *window);
gboolean gpinstruct_editor_window_get_modified (GPInstructEditorWindow *window);
void gpinstruct_editor_window_set_modified (GPInstructEditorWindow *window, gboolean modified);
void gpinstruct_editor_window_new_file (GPInstructEditorWindow *window);
void gpinstruct_editor_window_open_file (GPInstructEditorWindow *window, const gchar *file);
gboolean gpinstruct_editor_window_close_current_file (GPInstructEditorWindow *window);
gboolean gpinstruct_editor_window_save_file (GPInstructEditorWindow *window);
gboolean gpinstruct_editor_window_save_file_as (GPInstructEditorWindow *window, const gchar *file);
void gpinstruct_editor_window_update_tree_store (GPInstructEditorWindow *window, GPInstructObject *object);
gboolean gpinstruct_editor_window_quit (GPInstructEditorWindow *window);

G_END_DECLS

#endif /* _GPINSTRUCT_EDITOR_WINDOW_H_ */
