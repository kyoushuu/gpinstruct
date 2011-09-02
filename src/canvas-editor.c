/*
 * canvas-editor.c
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

#include <config.h>
#include <gtk/gtk.h>


#include <glib/gi18n.h>

#include "canvas.h"


int
main (int argc, char *argv[])
{
 	GtkWidget *window, *main_vbox;

	GtkWidget *main_menu, *file_menu, *edit_menu, *help_menu;

	GtkWidget *file_menu_item;
	GtkWidget *file_new_menu_item, *file_open_menu_item;
	GtkWidget *file_save_menu_item, *file_saveas_menu_item, *file_close_menu_item;
	GtkWidget *file_quit_menu_item;

	GtkWidget *edit_menu_item;
	GtkWidget *edit_add_menu_item, *edit_remove_menu_item, *edit_edititem_menu_item;
	GtkWidget *edit_preferences_menu_item;

	GtkWidget *help_menu_item;
	GtkWidget *help_about_menu_item;

	GtkWidget *top_hbox, *level_label, *top_buttonbox, *back_button, *next_button;

	GtkWidget *editor_hbox, *editor_treeview;
	GtkWidget *editor_buttonbox, *add_button, *remove_button, *edit_button;


#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Canvas");
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	main_vbox = gtk_vbox_new (FALSE, 3);
	gtk_container_add (GTK_CONTAINER (window), main_vbox);

	main_menu = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX (main_vbox), main_menu, FALSE, TRUE, 0);

	file_menu_item = gtk_menu_item_new_with_mnemonic ("_File");
	gtk_menu_shell_append (GTK_MENU_SHELL (main_menu), file_menu_item);

	file_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu_item), file_menu);

	file_new_menu_item = gtk_menu_item_new_with_mnemonic ("_New");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), file_new_menu_item);

	file_open_menu_item = gtk_menu_item_new_with_mnemonic ("_Open");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), file_open_menu_item);

	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), gtk_separator_menu_item_new ());

	file_save_menu_item = gtk_menu_item_new_with_mnemonic ("_Save");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), file_save_menu_item);

	file_saveas_menu_item = gtk_menu_item_new_with_mnemonic ("Save _As");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), file_saveas_menu_item);

	file_close_menu_item = gtk_menu_item_new_with_mnemonic ("_Close");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), file_close_menu_item);

	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), gtk_separator_menu_item_new ());

	file_quit_menu_item = gtk_menu_item_new_with_mnemonic ("_Quit");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), file_quit_menu_item);

	edit_menu_item = gtk_menu_item_new_with_mnemonic ("_Edit");
	gtk_menu_shell_append (GTK_MENU_SHELL (main_menu), edit_menu_item);

	edit_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (edit_menu_item), edit_menu);

	edit_add_menu_item = gtk_menu_item_new_with_mnemonic ("_Add");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), edit_add_menu_item);

	edit_remove_menu_item = gtk_menu_item_new_with_mnemonic ("_Remove");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), edit_remove_menu_item);

	edit_edititem_menu_item = gtk_menu_item_new_with_mnemonic ("_Edit Item");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), edit_edititem_menu_item);

	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), gtk_separator_menu_item_new ());

	edit_preferences_menu_item = gtk_menu_item_new_with_mnemonic ("_Preferences");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), edit_preferences_menu_item);

	help_menu_item = gtk_menu_item_new_with_mnemonic ("_Help");
	gtk_menu_shell_append (GTK_MENU_SHELL (main_menu), help_menu_item);

	help_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu_item), help_menu);

	help_about_menu_item = gtk_menu_item_new_with_mnemonic ("_About");
	gtk_menu_shell_append (GTK_MENU_SHELL (help_menu), help_about_menu_item);

	top_hbox = gtk_hbox_new (FALSE, 3);
	gtk_box_pack_start (GTK_BOX (main_vbox), top_hbox, FALSE, TRUE, 0);

	level_label = gtk_label_new ("Project Editor");
	gtk_box_pack_start (GTK_BOX (top_hbox), level_label, TRUE, TRUE, 0);

	top_buttonbox = gtk_hbutton_box_new ();
	gtk_box_pack_start (GTK_BOX (top_hbox), top_buttonbox, FALSE, TRUE, 0);

	back_button = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
	gtk_box_pack_start (GTK_BOX (top_buttonbox), back_button, FALSE, TRUE, 0);

	next_button = gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD);
	gtk_box_pack_start (GTK_BOX (top_buttonbox), next_button, FALSE, TRUE, 0);

	editor_hbox = gtk_hbox_new (FALSE, 3);
	gtk_box_pack_start (GTK_BOX (main_vbox), editor_hbox, TRUE, TRUE, 0);

	editor_treeview = gtk_tree_view_new ();
	gtk_box_pack_start (GTK_BOX (editor_hbox), editor_treeview, TRUE, TRUE, 0);

	editor_buttonbox = gtk_vbutton_box_new ();
	gtk_box_pack_start (GTK_BOX (editor_hbox), editor_buttonbox, FALSE, FALSE, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (editor_buttonbox), GTK_BUTTONBOX_START);

	add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	gtk_box_pack_start (GTK_BOX (editor_buttonbox), add_button, FALSE, TRUE, 0);

	remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	gtk_box_pack_start (GTK_BOX (editor_buttonbox), remove_button, FALSE, TRUE, 0);

	edit_button = gtk_button_new_from_stock (GTK_STOCK_EDIT);
	gtk_box_pack_start (GTK_BOX (editor_buttonbox), edit_button, FALSE, TRUE, 0);

	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_show_all (window);

	gtk_main ();
	return 0;
}
