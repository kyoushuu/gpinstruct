/*
 * GPInstruct - Programmed Instruction
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

#include <gpinstruct/gpinstruct-object.h>

#include <gpinstruct/gpinstruct-lesson-element.h>

#include <gpinstruct/gpinstruct-lesson-discussion.h>
#include <gpinstruct/gpinstruct-lesson-reading.h>


#include <gpinstruct/gpinstruct-lesson-test.h>

#include <gpinstruct/gpinstruct-lesson-test-multi-choice-question.h>
#include <gpinstruct/gpinstruct-lesson-test-multi-choice.h>
#include <gpinstruct/gpinstruct-lesson-test-word-pool-question.h>
#include <gpinstruct/gpinstruct-lesson-test-word-pool.h>
#include <gpinstruct/gpinstruct-lesson-test-order-item.h>
#include <gpinstruct/gpinstruct-lesson-test-order.h>


#include <gpinstruct/gpinstruct-lesson.h>
#include <gpinstruct/gpinstruct-category.h>
#include <gpinstruct/gpinstruct-project.h>

#include <gpinstruct/gpinstruct-parser.h>

#if (GLIB_MAJOR_VERSION < 2) || (GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 28)
void g_list_free_full (GList *list, GDestroyNotify free_func);
#endif
