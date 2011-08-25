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

#ifndef _CANVAS_LOG_H_
#define _CANVAS_LOG_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LOG             (canvas_log_get_type ())
#define CANVAS_LOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LOG, CanvasLog))
#define CANVAS_LOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LOG, CanvasLogClass))
#define CANVAS_IS_LOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LOG))
#define CANVAS_IS_LOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LOG))
#define CANVAS_LOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LOG, CanvasLogClass))

typedef struct _CanvasLogPrivate CanvasLogPrivate;
typedef struct _CanvasLogClass CanvasLogClass;
typedef struct _CanvasLog CanvasLog;

struct _CanvasLogClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasLog
{
	CanvasObject parent_instance;

	CanvasLogPrivate* priv;
};

typedef struct {
	GQuark id;
	GList* answers;
} CanvasLogTest;

typedef struct {
	guint item_id;
	guint answer_id;
	gdouble time_spent;
} CanvasLogAnswer;

typedef void            (*CanvasLogForeachFunc) (const gchar* test_id,
                                                 GList* answers,
                                                 gpointer user_data);

GType canvas_log_get_type (void) G_GNUC_CONST;
CanvasLog* canvas_log_new (void);
void canvas_log_set_last_name (CanvasLog* log, const gchar* last_name);
const gchar* canvas_log_get_last_name (CanvasLog* log);
void canvas_log_set_first_name (CanvasLog* log, const gchar* first_name);
const gchar* canvas_log_get_first_name (CanvasLog* log);
void canvas_log_set_group (CanvasLog* log, guint elements);
void canvas_log_add (CanvasLog* log, CanvasLessonTest* test, guint item_id, guint answer_id);
void canvas_log_timer_start (CanvasLog* log);
void canvas_log_timer_stop (CanvasLog* log);
GList* canvas_log_get_tests (CanvasLog* log);
gboolean canvas_log_open (CanvasLog* log, const gchar* file, GError** error);
void canvas_log_save (CanvasLog* log, const gchar* file, GError** error);

typedef enum
{
  CANVAS_LOG_ERROR_PARSE,
  CANVAS_LOG_ERROR_UNKNOWN_ELEMENT,
  CANVAS_LOG_ERROR_UNKNOWN_ATTRIBUTE
} CanvasLogError;

#define CANVAS_LOG_ERROR canvas_log_error_quark ()

GQuark canvas_log_error_quark (void);

G_END_DECLS

#endif /* _CANVAS_LOG_H_ */
