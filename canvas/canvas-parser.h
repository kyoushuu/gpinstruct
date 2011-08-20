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

#ifndef _CANVAS_PARSER_H_
#define _CANVAS_PARSER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CANVAS_TYPE_PARSER             (canvas_parser_get_type ())
#define CANVAS_PARSER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANVAS_TYPE_PARSER, CanvasParser))
#define CANVAS_PARSER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CANVAS_TYPE_PARSER, CanvasParserClass))
#define CANVAS_IS_PARSER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANVAS_TYPE_PARSER))
#define CANVAS_IS_PARSER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CANVAS_TYPE_PARSER))
#define CANVAS_PARSER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CANVAS_TYPE_PARSER, CanvasParserClass))

typedef struct _CanvasParserClass CanvasParserClass;
typedef struct _CanvasParser CanvasParser;

struct _CanvasParserClass
{
	CanvasObjectClass parent_class;
};

struct _CanvasParser
{
	CanvasObject parent_instance;
};

GType canvas_parser_get_type (void) G_GNUC_CONST;
CanvasParser* canvas_parser_new (void);
CanvasProject* canvas_parser_open (CanvasParser* parser, const gchar* file, GError** error);
void canvas_parser_save (CanvasParser* parser, CanvasProject* project, const gchar* file, GError** error);

typedef enum
{
  CANVAS_PARSER_ERROR_PARSE,
  CANVAS_PARSER_ERROR_UNKNOWN_ELEMENT,
  CANVAS_PARSER_ERROR_UNKNOWN_ATTRIBUTE
} CanvasParserError;

#define CANVAS_PARSER_ERROR canvas_parser_error_quark ()

GQuark canvas_parser_error_quark (void);

G_END_DECLS

#endif /* _CANVAS_PARSER_H_ */
