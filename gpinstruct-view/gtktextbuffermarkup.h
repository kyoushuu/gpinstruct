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

void    gtk_text_buffer_insert_markup             (GtkTextBuffer *buffer,
                                                   GtkTextIter   *iter,
                                                   const gchar   *markup);

void    gtk_text_buffer_insert_markup_with_tag    (GtkTextBuffer *buffer,
                                                   GtkTextIter   *iter,
                                                   const gchar   *markup,
                                                   GtkTextTag    *tag);

void    gtk_text_buffer_set_markup_with_tag       (GtkTextBuffer *buffer,
                                                   const gchar   *markup,
                                                   GtkTextTag    *tag);

void    gtk_text_buffer_set_markup                (GtkTextBuffer *buffer,
                                                   const gchar   *markup);

