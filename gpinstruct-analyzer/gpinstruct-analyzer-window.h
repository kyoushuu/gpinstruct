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

#ifndef _GPINSTRUCT_ANALYZER_WINDOW_H_
#define _GPINSTRUCT_ANALYZER_WINDOW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_ANALYZER_WINDOW             (gpinstruct_analyzer_window_get_type ())
#define GPINSTRUCT_ANALYZER_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_ANALYZER_WINDOW, GPInstructAnalyzerWindow))
#define GPINSTRUCT_ANALYZER_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_ANALYZER_WINDOW, GPInstructAnalyzerWindowClass))
#define GPINSTRUCT_IS_ANALYZER_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_ANALYZER_WINDOW))
#define GPINSTRUCT_IS_ANALYZER_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_ANALYZER_WINDOW))
#define GPINSTRUCT_ANALYZER_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_ANALYZER_WINDOW, GPInstructAnalyzerWindowClass))

typedef struct _GPInstructAnalyzerWindowPrivate GPInstructAnalyzerWindowPrivate;
typedef struct _GPInstructAnalyzerWindowClass GPInstructAnalyzerWindowClass;
typedef struct _GPInstructAnalyzerWindow GPInstructAnalyzerWindow;

struct _GPInstructAnalyzerWindowClass
{
	GtkWindowClass parent_class;
};

struct _GPInstructAnalyzerWindow
{
	GtkWindow parent_instance;

	GPInstructAnalyzerWindowPrivate *priv;
};

GType gpinstruct_analyzer_window_get_type (void) G_GNUC_CONST;
GtkWidget *gpinstruct_analyzer_window_new (void);
void gpinstruct_analyzer_window_new_session (GPInstructAnalyzerWindow *window, const gchar *file);
gboolean gpinstruct_analyzer_window_close_session (GPInstructAnalyzerWindow *window);
gboolean gpinstruct_analyzer_window_add_log_file (GPInstructAnalyzerWindow *window, const gchar *file);
gboolean gpinstruct_analyzer_window_quit (GPInstructAnalyzerWindow *window);

G_END_DECLS

#endif /* _GPINSTRUCT_ANALYZER_WINDOW_H_ */
