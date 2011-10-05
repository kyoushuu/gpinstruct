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

#ifndef _GPINSTRUCT_MESSAGE_POOL_H_
#define _GPINSTRUCT_MESSAGE_POOL_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_MESSAGE_POOL             (gpinstruct_message_pool_get_type ())
#define GPINSTRUCT_MESSAGE_POOL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_MESSAGE_POOL, GPInstructMessagePool))
#define GPINSTRUCT_MESSAGE_POOL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_MESSAGE_POOL, GPInstructMessagePoolClass))
#define GPINSTRUCT_IS_MESSAGE_POOL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_MESSAGE_POOL))
#define GPINSTRUCT_IS_MESSAGE_POOL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_MESSAGE_POOL))
#define GPINSTRUCT_MESSAGE_POOL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_MESSAGE_POOL, GPInstructMessagePoolClass))

typedef struct _GPInstructMessagePoolPrivate GPInstructMessagePoolPrivate;
typedef struct _GPInstructMessagePoolClass GPInstructMessagePoolClass;
typedef struct _GPInstructMessagePool GPInstructMessagePool;

struct _GPInstructMessagePoolClass
{
	GPInstructObjectClass parent_class;
};

struct _GPInstructMessagePool
{
	GPInstructObject parent_instance;

	GPInstructMessagePoolPrivate* priv;
};

typedef enum
{
	GPINSTRUCT_MESSAGE_TYPE_NONE,
	GPINSTRUCT_MESSAGE_TYPE_CORRECT,
	GPINSTRUCT_MESSAGE_TYPE_CORRECT_ALL,
	GPINSTRUCT_MESSAGE_TYPE_WRONG,
	GPINSTRUCT_MESSAGE_TYPE_WRONG_SOME,
	GPINSTRUCT_MESSAGE_TYPE_WRONG_ALL,
	GPINSTRUCT_MESSAGE_TYPE_PASS,
	GPINSTRUCT_MESSAGE_TYPE_FAIL
} GPInstructMessageType;

GType gpinstruct_message_pool_get_type (void) G_GNUC_CONST;
GPInstructMessagePool* gpinstruct_message_pool_new (void);
void gpinstruct_message_pool_load_from_file (GPInstructMessagePool* pool, const gchar *file);
const gchar* gpinstruct_message_pool_get_random (GPInstructMessagePool* pool, GPInstructMessageType type);
void gpinstruct_message_pool_add (GPInstructMessagePool* pool, GPInstructMessageType type, const gchar* message);
void gpinstruct_message_pool_add_multiple (GPInstructMessagePool* pool, ...);
void gpinstruct_message_pool_remove (GPInstructMessagePool* pool, GPInstructMessageType type, guint message);
const gchar* gpinstruct_message_pool_get (GPInstructMessagePool* pool, GPInstructMessageType type, guint message);
void gpinstruct_message_pool_play_sound_random (GPInstructMessagePool* pool, GPInstructMessageType type);

G_END_DECLS

#endif /* _GPINSTRUCT_MESSAGE_POOL_H_ */
