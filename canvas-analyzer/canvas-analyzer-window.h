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

#ifndef _CANVAS_ANALYZER_WINDOW_H_
#define _CANVAS_ANALYZER_WINDOW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_ANALYZER_WINDOW             (canvas_analyzer_window_get_type ())
#define CANVAS_ANALYZER_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_ANALYZER_WINDOW, CanvasAnalyzerWindow))
#define CANVAS_ANALYZER_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_ANALYZER_WINDOW, CanvasAnalyzerWindowClass))
#define CANVAS_IS_ANALYZER_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_ANALYZER_WINDOW))
#define CANVAS_IS_ANALYZER_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_ANALYZER_WINDOW))
#define CANVAS_ANALYZER_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_ANALYZER_WINDOW, CanvasAnalyzerWindowClass))

typedef struct _CanvasAnalyzerWindowPrivate CanvasAnalyzerWindowPrivate;
typedef struct _CanvasAnalyzerWindowClass CanvasAnalyzerWindowClass;
typedef struct _CanvasAnalyzerWindow CanvasAnalyzerWindow;

struct _CanvasAnalyzerWindowClass
{
	GtkWindowClass parent_class;
};

struct _CanvasAnalyzerWindow
{
	GtkWindow parent_instance;

	CanvasAnalyzerWindowPrivate* priv;
};

GType canvas_analyzer_window_get_type (void) G_GNUC_CONST;
GtkWidget* canvas_analyzer_window_new (void);
void canvas_analyzer_window_new_session (CanvasAnalyzerWindow* window);
gboolean canvas_analyzer_window_close_session (CanvasAnalyzerWindow* window);
gboolean canvas_analyzer_window_add_log_file (CanvasAnalyzerWindow* window, const gchar* file);
gboolean canvas_analyzer_window_quit (CanvasAnalyzerWindow* window);

G_END_DECLS

#endif /* _CANVAS_ANALYZER_WINDOW_H_ */
