/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011 - Arnel A. Borja (kyoushuu@yahoo.com)
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

#ifndef _GPINSTRUCT_PROJECT_VIEW_H_
#define _GPINSTRUCT_PROJECT_VIEW_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_PROJECT_VIEW             (gpinstruct_project_view_get_type ())
#define GPINSTRUCT_PROJECT_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_PROJECT_VIEW, GPInstructProjectView))
#define GPINSTRUCT_PROJECT_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_PROJECT_VIEW, GPInstructProjectViewClass))
#define GPINSTRUCT_IS_PROJECT_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_PROJECT_VIEW))
#define GPINSTRUCT_IS_PROJECT_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_PROJECT_VIEW))
#define GPINSTRUCT_PROJECT_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_PROJECT_VIEW, GPInstructProjectViewClass))

typedef struct _GPInstructProjectViewPrivate GPInstructProjectViewPrivate;
typedef struct _GPInstructProjectViewClass GPInstructProjectViewClass;
typedef struct _GPInstructProjectView GPInstructProjectView;

struct _GPInstructProjectViewClass
{
	GtkWindowClass parent_class;
};

struct _GPInstructProjectView
{
	GtkWindow parent_instance;

	GPInstructProjectViewPrivate* priv;
};

GType gpinstruct_project_view_get_type (void) G_GNUC_CONST;
GtkWidget* gpinstruct_project_view_new (GPInstructProject* project, GPInstructMessagePool* pool, GPInstructLog* log);

G_END_DECLS

#endif /* _GPINSTRUCT_PROJECT_VIEW_H_ */
