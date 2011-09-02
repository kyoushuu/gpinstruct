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

#ifndef _GPINSTRUCT_LESSON_DISCUSSION_EDITOR_H_
#define _GPINSTRUCT_LESSON_DISCUSSION_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR             (gpinstruct_lesson_discussion_editor_get_type ())
#define GPINSTRUCT_LESSON_DISCUSSION_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR, GPInstructLessonDiscussionEditor))
#define GPINSTRUCT_LESSON_DISCUSSION_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR, GPInstructLessonDiscussionEditorClass))
#define GPINSTRUCT_IS_LESSON_DISCUSSION_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR))
#define GPINSTRUCT_IS_LESSON_DISCUSSION_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR))
#define GPINSTRUCT_LESSON_DISCUSSION_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_DISCUSSION_EDITOR, GPInstructLessonDiscussionEditorClass))

typedef struct _GPInstructLessonDiscussionEditorPrivate GPInstructLessonDiscussionEditorPrivate;
typedef struct _GPInstructLessonDiscussionEditorClass GPInstructLessonDiscussionEditorClass;
typedef struct _GPInstructLessonDiscussionEditor GPInstructLessonDiscussionEditor;

struct _GPInstructLessonDiscussionEditorClass
{
	GPInstructObjectEditorClass parent_class;
};

struct _GPInstructLessonDiscussionEditor
{
	GPInstructObjectEditor parent_instance;

	GPInstructLessonDiscussionEditorPrivate* priv;
};

GType gpinstruct_lesson_discussion_editor_get_type (void) G_GNUC_CONST;
GPInstructLessonDiscussionEditor* gpinstruct_lesson_discussion_editor_new (GPInstructEditorWindow* window, GPInstructLessonDiscussion *discussion);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_DISCUSSION_EDITOR_H_ */
