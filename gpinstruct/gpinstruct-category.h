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

#ifndef _GPINSTRUCT_CATEGORY_H_
#define _GPINSTRUCT_CATEGORY_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_CATEGORY             (gpinstruct_category_get_type ())
#define GPINSTRUCT_CATEGORY(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_CATEGORY, GPInstructCategory))
#define GPINSTRUCT_CATEGORY_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_CATEGORY, GPInstructCategoryClass))
#define GPINSTRUCT_IS_CATEGORY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_CATEGORY))
#define GPINSTRUCT_IS_CATEGORY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_CATEGORY))
#define GPINSTRUCT_CATEGORY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_CATEGORY, GPInstructCategoryClass))

typedef struct _GPInstructCategoryPrivate GPInstructCategoryPrivate;
typedef struct _GPInstructCategoryClass GPInstructCategoryClass;
typedef struct _GPInstructCategory GPInstructCategory;

struct _GPInstructCategoryClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructCategory
{
	GPInstructObject parent_instance;

	GPInstructCategoryPrivate* priv;
};

GType gpinstruct_category_get_type (void) G_GNUC_CONST;
GPInstructCategory* gpinstruct_category_new (void);
const gchar* gpinstruct_category_get_title (GPInstructCategory* category);
void gpinstruct_category_set_title (GPInstructCategory* category, const gchar *title);
guint gpinstruct_category_get_lessons_length (GPInstructCategory* category);
void gpinstruct_category_add_lesson (GPInstructCategory* category, GPInstructLesson* lesson);
void gpinstruct_category_remove_lesson (GPInstructCategory* category, guint lesson);
GList* gpinstruct_category_get_lessons (GPInstructCategory* category);

G_END_DECLS

#endif /* _GPINSTRUCT_CATEGORY_H_ */
