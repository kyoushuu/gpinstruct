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

#ifndef _GPINSTRUCT_LOG_ANALYZER_H_
#define _GPINSTRUCT_LOG_ANALYZER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LOG_ANALYZER             (gpinstruct_log_analyzer_get_type ())
#define GPINSTRUCT_LOG_ANALYZER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LOG_ANALYZER, GPInstructLogAnalyzer))
#define GPINSTRUCT_LOG_ANALYZER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LOG_ANALYZER, GPInstructLogAnalyzerClass))
#define GPINSTRUCT_IS_LOG_ANALYZER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LOG_ANALYZER))
#define GPINSTRUCT_IS_LOG_ANALYZER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LOG_ANALYZER))
#define GPINSTRUCT_LOG_ANALYZER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LOG_ANALYZER, GPInstructLogAnalyzerClass))

typedef struct _GPInstructLogAnalyzerPrivate GPInstructLogAnalyzerPrivate;
typedef struct _GPInstructLogAnalyzerClass GPInstructLogAnalyzerClass;
typedef struct _GPInstructLogAnalyzer GPInstructLogAnalyzer;

struct _GPInstructLogAnalyzerClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructLogAnalyzer
{
	GPInstructObject parent_instance;

	GPInstructLogAnalyzerPrivate* priv;
};

typedef struct _GPInstructLogAnalyzerProject GPInstructLogAnalyzerProject;
typedef struct _GPInstructLogAnalyzerCategory GPInstructLogAnalyzerCategory;
typedef struct _GPInstructLogAnalyzerLesson GPInstructLogAnalyzerLesson;
typedef struct _GPInstructLogAnalyzerTest GPInstructLogAnalyzerTest;
typedef struct _GPInstructLogAnalyzerItem GPInstructLogAnalyzerItem;
typedef struct _GPInstructLogAnalyzerChoice GPInstructLogAnalyzerChoice;
typedef struct _GPInstructLogAnalyzerAnswer GPInstructLogAnalyzerAnswer;

typedef struct _GPInstructLogAnalyzerExaminee GPInstructLogAnalyzerExaminee;

struct _GPInstructLogAnalyzerProject
{
	GList* categories; /* GPInstructLogAnalyzerCategory */
	GPInstructProject* object;
	GData* tests_list;
	GPInstructCategory* last_category;
	GPInstructLesson* last_lesson;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _GPInstructLogAnalyzerCategory
{
	GPInstructLogAnalyzerProject* project;
	GList* lessons; /* GPInstructLogAnalyzerLesson */
	GPInstructCategory* object;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _GPInstructLogAnalyzerLesson
{
	GPInstructLogAnalyzerCategory* category;
	GList* tests; /* GPInstructLogAnalyzerTest */
	GPInstructLesson* object;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _GPInstructLogAnalyzerTest
{
	GPInstructLogAnalyzerLesson* lesson;
	GList* items; /* GPInstructLogAnalyzerItem */
	GQuark id;
	GPInstructLessonTest* object;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _GPInstructLogAnalyzerItem
{
	GPInstructLogAnalyzerTest* test;
	GList* choices;
	guint id;
	guint answer;

	guint times_answered;
	gdouble time_spent;
	guint choices_length;
	guint times_correctly_answered;
};

struct _GPInstructLogAnalyzerChoice
{
	GPInstructLogAnalyzerItem* item;
	GList* answers;
	guint id;

	guint times_chosen;
	gdouble time_spent;
	guint answers_length;
};

struct _GPInstructLogAnalyzerAnswer
{
	GPInstructLogAnalyzerChoice* choice;
	GPInstructLogAnalyzerExaminee* examinee;

	gdouble time_spent;
};

struct _GPInstructLogAnalyzerExaminee
{
	gchar* first_name;
	gchar* last_name;

	GPInstructLogAnalyzerProject* project;
};

GType gpinstruct_log_analyzer_get_type (void) G_GNUC_CONST;
GPInstructLogAnalyzer* gpinstruct_log_analyzer_new (GPInstructProject* project);
void gpinstruct_log_analyzer_add_log (GPInstructLogAnalyzer* analyzer, GPInstructLog* log);
GPInstructLogAnalyzerProject* gpinstruct_log_analyzer_get_project (GPInstructLogAnalyzer* analyzer);
GList* gpinstruct_log_analyzer_get_examinees (GPInstructLogAnalyzer* analyzer);

G_END_DECLS

#endif /* _GPINSTRUCT_LOG_ANALYZER_H_ */
