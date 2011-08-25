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

#ifndef _CANVAS_LOG_ANALYZER_H_
#define _CANVAS_LOG_ANALYZER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LOG_ANALYZER             (canvas_log_analyzer_get_type ())
#define CANVAS_LOG_ANALYZER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LOG_ANALYZER, CanvasLogAnalyzer))
#define CANVAS_LOG_ANALYZER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LOG_ANALYZER, CanvasLogAnalyzerClass))
#define CANVAS_IS_LOG_ANALYZER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LOG_ANALYZER))
#define CANVAS_IS_LOG_ANALYZER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LOG_ANALYZER))
#define CANVAS_LOG_ANALYZER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LOG_ANALYZER, CanvasLogAnalyzerClass))

typedef struct _CanvasLogAnalyzerPrivate CanvasLogAnalyzerPrivate;
typedef struct _CanvasLogAnalyzerClass CanvasLogAnalyzerClass;
typedef struct _CanvasLogAnalyzer CanvasLogAnalyzer;

struct _CanvasLogAnalyzerClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasLogAnalyzer
{
	CanvasObject parent_instance;

	CanvasLogAnalyzerPrivate* priv;
};

typedef struct _CanvasLogAnalyzerProject CanvasLogAnalyzerProject;
typedef struct _CanvasLogAnalyzerCategory CanvasLogAnalyzerCategory;
typedef struct _CanvasLogAnalyzerLesson CanvasLogAnalyzerLesson;
typedef struct _CanvasLogAnalyzerTest CanvasLogAnalyzerTest;
typedef struct _CanvasLogAnalyzerItem CanvasLogAnalyzerItem;
typedef struct _CanvasLogAnalyzerChoice CanvasLogAnalyzerChoice;
typedef struct _CanvasLogAnalyzerAnswer CanvasLogAnalyzerAnswer;

typedef struct _CanvasLogAnalyzerExaminee CanvasLogAnalyzerExaminee;

struct _CanvasLogAnalyzerProject
{
	GList* categories; /* CanvasLogAnalyzerCategory */
	CanvasProject* object;
	GData* tests_list;
	CanvasCategory* last_category;
	CanvasLesson* last_lesson;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _CanvasLogAnalyzerCategory
{
	CanvasLogAnalyzerProject* project;
	GList* lessons; /* CanvasLogAnalyzerLesson */
	CanvasCategory* object;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _CanvasLogAnalyzerLesson
{
	CanvasLogAnalyzerCategory* category;
	GList* tests; /* CanvasLogAnalyzerTest */
	CanvasLesson* object;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _CanvasLogAnalyzerTest
{
	CanvasLogAnalyzerLesson* lesson;
	GList* items; /* CanvasLogAnalyzerItem */
	GQuark id;
	CanvasLessonTest* object;

	guint times_taken;
	gdouble time_spent;
	guint items_length;
	guint items_correctly_answered;
};

struct _CanvasLogAnalyzerItem
{
	CanvasLogAnalyzerTest* test;
	GList* choices;
	guint id;
	guint answer;

	guint times_answered;
	gdouble time_spent;
	guint choices_length;
	guint times_correctly_answered;
};

struct _CanvasLogAnalyzerChoice
{
	CanvasLogAnalyzerItem* item;
	GList* answers;
	guint id;

	guint times_chosen;
	gdouble time_spent;
	guint answers_length;
};

struct _CanvasLogAnalyzerAnswer
{
	CanvasLogAnalyzerChoice* choice;
	CanvasLogAnalyzerExaminee* examinee;

	gdouble time_spent;
};

struct _CanvasLogAnalyzerExaminee
{
	gchar* first_name;
	gchar* last_name;

	CanvasLogAnalyzerProject* project;
};

GType canvas_log_analyzer_get_type (void) G_GNUC_CONST;
CanvasLogAnalyzer* canvas_log_analyzer_new (CanvasProject* project);
void canvas_log_analyzer_add_log (CanvasLogAnalyzer* analyzer, CanvasLog* log);
CanvasLogAnalyzerProject* canvas_log_analyzer_get_project (CanvasLogAnalyzer* analyzer);
GList* canvas_log_analyzer_get_examinees (CanvasLogAnalyzer* analyzer);

G_END_DECLS

#endif /* _CANVAS_LOG_ANALYZER_H_ */
