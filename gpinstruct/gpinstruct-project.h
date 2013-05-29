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

#ifndef _GPINSTRUCT_PROJECT_H_
#define _GPINSTRUCT_PROJECT_H_

#include <glib-object.h>
#include "gpinstruct-category.h"

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_PROJECT             (gpinstruct_project_get_type ())
#define GPINSTRUCT_PROJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_PROJECT, GPInstructProject))
#define GPINSTRUCT_PROJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_PROJECT, GPInstructProjectClass))
#define GPINSTRUCT_IS_PROJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_PROJECT))
#define GPINSTRUCT_IS_PROJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_PROJECT))
#define GPINSTRUCT_PROJECT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_PROJECT, GPInstructProjectClass))

typedef struct _GPInstructProjectPrivate GPInstructProjectPrivate;
typedef struct _GPInstructProjectClass GPInstructProjectClass;
typedef struct _GPInstructProject GPInstructProject;

struct _GPInstructProjectClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructProject
{
	GPInstructObject parent_instance;

	GPInstructProjectPrivate *priv;
};

GType gpinstruct_project_get_type (void) G_GNUC_CONST;
GPInstructProject *gpinstruct_project_new (void);
const gchar *gpinstruct_project_get_title (GPInstructProject *project);
void gpinstruct_project_set_title (GPInstructProject *project, const gchar *title);
const gchar *gpinstruct_project_get_instructions (GPInstructProject *test);
void gpinstruct_project_set_instructions (GPInstructProject *test, const gchar *instructions);
const gchar *gpinstruct_project_get_stylesheet (GPInstructProject *test);
void gpinstruct_project_set_stylesheet (GPInstructProject *test, const gchar *stylesheet);
guint gpinstruct_project_get_categories_length (GPInstructProject *project);
void gpinstruct_project_add_category (GPInstructProject *project, GPInstructCategory *category);
void gpinstruct_project_remove_category (GPInstructProject *project, guint category);
GList *gpinstruct_project_get_categories (GPInstructProject *project);

G_END_DECLS

#endif /* _GPINSTRUCT_PROJECT_H_ */
