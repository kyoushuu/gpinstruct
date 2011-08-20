/***************************************************************************
 *            canvas.h
 *
 *  Sat July 23 22:50:29 2011
 *  Copyright  2011  Arnel A. Borja
 *  <galeon@ymail.com>
 ****************************************************************************/
/*
 * canvas.h
 *
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

#include <canvas/canvas-object.h>

#include <canvas/canvas-lesson-element.h>

#include <canvas/canvas-lesson-discussion.h>
#include <canvas/canvas-lesson-reading.h>


#include <canvas/canvas-lesson-test.h>

#include <canvas/canvas-lesson-test-multi-choice-question.h>
#include <canvas/canvas-lesson-test-multi-choice.h>
#include <canvas/canvas-lesson-test-word-pool-question.h>
#include <canvas/canvas-lesson-test-word-pool.h>
#include <canvas/canvas-lesson-test-order-item.h>
#include <canvas/canvas-lesson-test-order.h>


#include <canvas/canvas-lesson.h>
#include <canvas/canvas-category.h>
#include <canvas/canvas-project.h>

#include <canvas/canvas-parser.h>

#if (GLIB_MAJOR_VERSION < 2) || (GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 28)
void g_list_free_full (GList *list, GDestroyNotify free_func);
#endif
