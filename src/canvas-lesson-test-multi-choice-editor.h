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

#ifndef _CANVAS_LESSON_TEST_MULTI_CHOICE_EDITOR_H_
#define _CANVAS_LESSON_TEST_MULTI_CHOICE_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR             (canvas_lesson_test_multi_choice_editor_get_type ())
#define CANVAS_LESSON_TEST_MULTI_CHOICE_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, CanvasLessonTestMultiChoiceEditor))
#define CANVAS_LESSON_TEST_MULTI_CHOICE_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, CanvasLessonTestMultiChoiceEditorClass))
#define CANVAS_IS_LESSON_TEST_MULTI_CHOICE_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR))
#define CANVAS_IS_LESSON_TEST_MULTI_CHOICE_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR))
#define CANVAS_LESSON_TEST_MULTI_CHOICE_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_LESSON_TEST_MULTI_CHOICE_EDITOR, CanvasLessonTestMultiChoiceEditorClass))

typedef struct _CanvasLessonTestMultiChoiceEditorClass CanvasLessonTestMultiChoiceEditorClass;
typedef struct _CanvasLessonTestMultiChoiceEditor CanvasLessonTestMultiChoiceEditor;

struct _CanvasLessonTestMultiChoiceEditorClass
{
	CanvasObjectEditorClass parent_class;
};

struct _CanvasLessonTestMultiChoiceEditor
{
	CanvasObjectEditor parent_instance;
};

GType canvas_lesson_test_multi_choice_editor_get_type (void) G_GNUC_CONST;
CanvasLessonTestMultiChoiceEditor* canvas_lesson_test_multi_choice_editor_new (CanvasEditorWindow* window, CanvasLessonTestMultiChoice *test);

G_END_DECLS

#endif /* _CANVAS_LESSON_TEST_MULTI_CHOICE_EDITOR_H_ */
