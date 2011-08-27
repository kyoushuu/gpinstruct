/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GPINSTRUCT_LOG_H_
#define _GPINSTRUCT_LOG_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LOG             (gpinstruct_log_get_type ())
#define GPINSTRUCT_LOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LOG, GPInstructLog))
#define GPINSTRUCT_LOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LOG, GPInstructLogClass))
#define GPINSTRUCT_IS_LOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LOG))
#define GPINSTRUCT_IS_LOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LOG))
#define GPINSTRUCT_LOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LOG, GPInstructLogClass))

typedef struct _GPInstructLogPrivate GPInstructLogPrivate;
typedef struct _GPInstructLogClass GPInstructLogClass;
typedef struct _GPInstructLog GPInstructLog;

struct _GPInstructLogClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLog
{
	GPInstructObject parent_instance;

	GPInstructLogPrivate* priv;
};

typedef struct {
	GQuark id;
	GList* answers;
} GPInstructLogTest;

typedef struct {
	guint item_id;
	guint answer_id;
	gdouble time_spent;
} GPInstructLogAnswer;

typedef void            (*GPInstructLogForeachFunc) (const gchar* test_id,
                                                 GList* answers,
                                                 gpointer user_data);

GType gpinstruct_log_get_type (void) G_GNUC_CONST;
GPInstructLog* gpinstruct_log_new (void);
void gpinstruct_log_set_last_name (GPInstructLog* log, const gchar* last_name);
const gchar* gpinstruct_log_get_last_name (GPInstructLog* log);
void gpinstruct_log_set_first_name (GPInstructLog* log, const gchar* first_name);
const gchar* gpinstruct_log_get_first_name (GPInstructLog* log);
void gpinstruct_log_set_group (GPInstructLog* log, guint elements);
void gpinstruct_log_add (GPInstructLog* log, GPInstructLessonTest* test, guint item_id, guint answer_id);
void gpinstruct_log_close_test (GPInstructLog* log);
void gpinstruct_log_timer_start (GPInstructLog* log);
void gpinstruct_log_timer_stop (GPInstructLog* log);
GList* gpinstruct_log_get_tests (GPInstructLog* log);
gboolean gpinstruct_log_open (GPInstructLog* log, const gchar* file, GError** error);
void gpinstruct_log_save (GPInstructLog* log, const gchar* file, GError** error);

typedef enum
{
  GPINSTRUCT_LOG_ERROR_PARSE,
  GPINSTRUCT_LOG_ERROR_UNKNOWN_ELEMENT,
  GPINSTRUCT_LOG_ERROR_UNKNOWN_ATTRIBUTE
} GPInstructLogError;

#define GPINSTRUCT_LOG_ERROR gpinstruct_log_error_quark ()

GQuark gpinstruct_log_error_quark (void);

G_END_DECLS

#endif /* _GPINSTRUCT_LOG_H_ */
