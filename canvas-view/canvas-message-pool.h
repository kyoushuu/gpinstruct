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

#ifndef _CANVAS_MESSAGE_POOL_H_
#define _CANVAS_MESSAGE_POOL_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_MESSAGE_POOL             (canvas_message_pool_get_type ())
#define CANVAS_MESSAGE_POOL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_MESSAGE_POOL, CanvasMessagePool))
#define CANVAS_MESSAGE_POOL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_MESSAGE_POOL, CanvasMessagePoolClass))
#define CANVAS_IS_MESSAGE_POOL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_MESSAGE_POOL))
#define CANVAS_IS_MESSAGE_POOL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_MESSAGE_POOL))
#define CANVAS_MESSAGE_POOL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_MESSAGE_POOL, CanvasMessagePoolClass))

typedef struct _CanvasMessagePoolPrivate CanvasMessagePoolPrivate;
typedef struct _CanvasMessagePoolClass CanvasMessagePoolClass;
typedef struct _CanvasMessagePool CanvasMessagePool;

struct _CanvasMessagePoolClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasMessagePool
{
	CanvasObject parent_instance;

	CanvasMessagePoolPrivate* priv;
};

typedef enum
{
	CANVAS_MESSAGE_TYPE_NONE,
	CANVAS_MESSAGE_TYPE_CORRECT,
	CANVAS_MESSAGE_TYPE_CORRECT_ALL,
	CANVAS_MESSAGE_TYPE_WRONG,
	CANVAS_MESSAGE_TYPE_WRONG_SOME,
	CANVAS_MESSAGE_TYPE_WRONG_ALL,
	CANVAS_MESSAGE_TYPE_PASS,
	CANVAS_MESSAGE_TYPE_FAIL
} CanvasMessageType;

GType canvas_message_pool_get_type (void) G_GNUC_CONST;
CanvasMessagePool* canvas_message_pool_new (void);
void canvas_message_pool_load_from_file (CanvasMessagePool* pool, const gchar *file);
const gchar* canvas_message_pool_get_random (CanvasMessagePool* pool, CanvasMessageType type);
void canvas_message_pool_add (CanvasMessagePool* pool, CanvasMessageType type, const gchar* message);
void canvas_message_pool_add_multiple (CanvasMessagePool* pool, ...);
void canvas_message_pool_remove (CanvasMessagePool* pool, CanvasMessageType type, guint message);
const gchar* canvas_message_pool_get (CanvasMessagePool* pool, CanvasMessageType type, guint message);

G_END_DECLS

#endif /* _CANVAS_MESSAGE_POOL_H_ */
