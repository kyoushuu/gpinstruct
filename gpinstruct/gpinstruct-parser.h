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

#ifndef _GPINSTRUCT_PARSER_H_
#define _GPINSTRUCT_PARSER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GPINSTRUCT_TYPE_PARSER             (gpinstruct_parser_get_type ())
#define GPINSTRUCT_PARSER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPINSTRUCT_TYPE_PARSER, GPInstructParser))
#define GPINSTRUCT_PARSER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GPINSTRUCT_TYPE_PARSER, GPInstructParserClass))
#define GPINSTRUCT_IS_PARSER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPINSTRUCT_TYPE_PARSER))
#define GPINSTRUCT_IS_PARSER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GPINSTRUCT_TYPE_PARSER))
#define GPINSTRUCT_PARSER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GPINSTRUCT_TYPE_PARSER, GPInstructParserClass))

typedef struct _GPInstructParserClass GPInstructParserClass;
typedef struct _GPInstructParser GPInstructParser;

struct _GPInstructParserClass
{
	GObjectClass parent_class;
};

struct _GPInstructParser
{
	GObject parent_instance;
};

GType gpinstruct_parser_get_type (void) G_GNUC_CONST;
GPInstructParser* gpinstruct_parser_new (void);
GPInstructProject* gpinstruct_parser_load_from_file (GPInstructParser* parser, const gchar* file, GError** error);
GPInstructProject* gpinstruct_parser_load_from_string (GPInstructParser* parser, const gchar* contents, GError** error);
void gpinstruct_parser_save_to_file (GPInstructParser* parser, GPInstructProject* project, const gchar* file, GError** error);
gchar* gpinstruct_parser_save_to_string (GPInstructParser* parser, GPInstructProject* project, GError** error);

typedef enum
{
  GPINSTRUCT_PARSER_ERROR_PARSE,
  GPINSTRUCT_PARSER_ERROR_UNKNOWN_ELEMENT,
  GPINSTRUCT_PARSER_ERROR_UNKNOWN_ATTRIBUTE
} GPInstructParserError;

#define GPINSTRUCT_PARSER_ERROR gpinstruct_parser_error_quark ()

GQuark gpinstruct_parser_error_quark (void);

G_END_DECLS

#endif /* _GPINSTRUCT_PARSER_H_ */
