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

#ifndef _GPINSTRUCT_ANALYZER_EXAMINEE_VIEW_H_
#define _GPINSTRUCT_ANALYZER_EXAMINEE_VIEW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_ANALYZER_EXAMINEE_VIEW             (gpinstruct_analyzer_examinee_view_get_type ())
#define GPINSTRUCT_ANALYZER_EXAMINEE_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_ANALYZER_EXAMINEE_VIEW, GPInstructAnalyzerExamineeView))
#define GPINSTRUCT_ANALYZER_EXAMINEE_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_ANALYZER_EXAMINEE_VIEW, GPInstructAnalyzerExamineeViewClass))
#define GPINSTRUCT_IS_ANALYZER_EXAMINEE_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_ANALYZER_EXAMINEE_VIEW))
#define GPINSTRUCT_IS_ANALYZER_EXAMINEE_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_ANALYZER_EXAMINEE_VIEW))
#define GPINSTRUCT_ANALYZER_EXAMINEE_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_ANALYZER_EXAMINEE_VIEW, GPInstructAnalyzerExamineeViewClass))

typedef struct _GPInstructAnalyzerExamineeViewPrivate GPInstructAnalyzerExamineeViewPrivate;
typedef struct _GPInstructAnalyzerExamineeViewClass GPInstructAnalyzerExamineeViewClass;
typedef struct _GPInstructAnalyzerExamineeView GPInstructAnalyzerExamineeView;

struct _GPInstructAnalyzerExamineeViewClass
{
	GtkHPanedClass parent_class;
};

struct _GPInstructAnalyzerExamineeView
{
	GtkHPaned parent_instance;

	GPInstructAnalyzerExamineeViewPrivate *priv;
};

GType gpinstruct_analyzer_examinee_view_get_type (void) G_GNUC_CONST;
GtkWidget *gpinstruct_analyzer_examinee_view_new (GPInstructLogAnalyzer *analyzer);

G_END_DECLS

#endif /* _GPINSTRUCT_ANALYZER_EXAMINEE_VIEW_H_ */
