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

#ifndef _GPINSTRUCT_LESSON_TEST_TEXT_EDITOR_H_
#define _GPINSTRUCT_LESSON_TEST_TEXT_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_LESSON_TEST_TEXT_EDITOR             (gpinstruct_lesson_test_text_editor_get_type ())
#define GPINSTRUCT_LESSON_TEST_TEXT_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_EDITOR, GPInstructLessonTestTextEditor))
#define GPINSTRUCT_LESSON_TEST_TEXT_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_EDITOR, GPInstructLessonTestTextEditorClass))
#define GPINSTRUCT_IS_LESSON_TEST_TEXT_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_EDITOR))
#define GPINSTRUCT_IS_LESSON_TEST_TEXT_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_EDITOR))
#define GPINSTRUCT_LESSON_TEST_TEXT_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_LESSON_TEST_TEXT_EDITOR, GPInstructLessonTestTextEditorClass))

typedef struct _GPInstructLessonTestTextEditorPrivate GPInstructLessonTestTextEditorPrivate;
typedef struct _GPInstructLessonTestTextEditorClass GPInstructLessonTestTextEditorClass;
typedef struct _GPInstructLessonTestTextEditor GPInstructLessonTestTextEditor;

struct _GPInstructLessonTestTextEditorClass
{
	GPInstructObjectEditorClass parent_class;
};

struct _GPInstructLessonTestTextEditor
{
	GPInstructObjectEditor parent_instance;

	GPInstructLessonTestTextEditorPrivate *priv;
};

GType gpinstruct_lesson_test_text_editor_get_type (void) G_GNUC_CONST;
GPInstructLessonTestTextEditor *gpinstruct_lesson_test_text_editor_new (GPInstructEditorWindow *window, GPInstructLessonTestText *test);

G_END_DECLS

#endif /* _GPINSTRUCT_LESSON_TEST_TEXT_EDITOR_H_ */
