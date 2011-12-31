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

#ifndef _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_H_
#define _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR             (gpinstruct_lesson_test_multi_choice_editor_get_type ())
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, GPInstructLessonTestMultiChoiceEditor))
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, GPInstructLessonTestMultiChoiceEditorClass))
#define GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR))
#define GPINSTRUCT_IS_LESSON_TEST_MULTI_CHOICE_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR))
#define GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, GPInstructLessonTestMultiChoiceEditorClass))

typedef struct _GPInstructLessonTestMultiChoiceEditorPrivate GPInstructLessonTestMultiChoiceEditorPrivate;
typedef struct _GPInstructLessonTestMultiChoiceEditorClass GPInstructLessonTestMultiChoiceEditorClass;
typedef struct _GPInstructLessonTestMultiChoiceEditor GPInstructLessonTestMultiChoiceEditor;

struct _GPInstructLessonTestMultiChoiceEditorClass
{
	GPInstructObjectEditorClass parent_class;
};

struct _GPInstructLessonTestMultiChoiceEditor
{
	GPInstructObjectEditor parent_instance;

	GPInstructLessonTestMultiChoiceEditorPrivate *priv;
};

GType gpinstruct_lesson_test_multi_choice_editor_get_type (void) G_GNUC_CONST;
GPInstructLessonTestMultiChoiceEditor *gpinstruct_lesson_test_multi_choice_editor_new (GPInstructEditorWindow *window, GPInstructLessonTestMultiChoice *test);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_MULTI_CHOICE_EDITOR_H_ */
