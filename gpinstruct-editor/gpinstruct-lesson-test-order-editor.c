/*
 * GPInstruct - Programmed Instruction
 * Copyright (C) 2011-2012  Arnel A. Borja <kyoushuu@yahoo.com>
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

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-editor/gpinstruct-editor.h"

struct _GPInstructLessonTestOrderEditorPrivate
{
	GPInstructEditorWindow *window;
	GPInstructLessonTestOrder *test;

	GtkWidget *title_entry;
	GtkWidget *directions_view;
	GtkWidget *explain_switch;
	GtkWidget *explanation_view;

	GtkWidget *tree_view;
	GtkListStore *store;
};

#define GPINSTRUCT_LESSON_TEST_ORDER_EDITOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_EDITOR, GPInstructLessonTestOrderEditorPrivate))


enum
{
	TITLE_COLUMN,
	DATA_COLUMN,
	N_COLUMNS
};

static void
update_tree_view (GPInstructLessonTestOrderEditor *editor)
{
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	gtk_list_store_clear (priv->store);

	GtkTreeIter iterItem;
	const gchar *text;

	GList *items = gpinstruct_lesson_test_order_get_items (priv->test);
	GList *curr_items = items;

	while (curr_items)
	{
		GPInstructLessonTestOrderItem *item = GPINSTRUCT_LESSON_TEST_ORDER_ITEM (curr_items->data);

		text = gpinstruct_lesson_test_order_item_get_text (item);
		gtk_list_store_append (priv->store, &iterItem);
		gtk_list_store_set (priv->store, &iterItem,
		                    TITLE_COLUMN, (text != NULL && *text != '\0')? text:_("(Empty Item)"),
		                    DATA_COLUMN, item,
		                    -1);

		curr_items = curr_items->next;
	}

	g_list_free (items);

	gtk_tree_view_expand_all (GTK_TREE_VIEW (priv->tree_view));
}


static void
tree_view_row_activated (GtkTreeView       *tree_view,
                         GtkTreePath       *path,
                         GtkTreeViewColumn *column,
                         gpointer           user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	GPInstructObject *object;
	GtkTreeIter iter;
	GtkWidget *scrolled_window, *text_view, *answer_spin;

	if (gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->store), &iter, path))
	{
		gtk_tree_model_get (GTK_TREE_MODEL (priv->store), &iter,
		                    DATA_COLUMN, &object,
		                    -1);
		if (GPINSTRUCT_IS_LESSON_TEST_ORDER_ITEM (object))
		{
			GPInstructLessonTestOrderItem *item = GPINSTRUCT_LESSON_TEST_ORDER_ITEM (object);
			guint items_num = gpinstruct_lesson_test_order_get_items_length (priv->test);

			GtkWidget *dialog = gtk_dialog_new_with_buttons (_("Item Properties"),
			                                                 GTK_WINDOW (priv->window),
			                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
			                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			                                                 NULL);
			GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
			gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 300);

			scrolled_window = gtk_scrolled_window_new (NULL, NULL);
			gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
			text_view = gtk_text_view_new ();
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD_CHAR);
			gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
			                          gpinstruct_lesson_test_order_item_get_text (item),
			                          -1);
			gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Text:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    scrolled_window,
			                    TRUE, TRUE, 0);

			answer_spin = gtk_spin_button_new_with_range (1, items_num, 1);
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (answer_spin),
			                           gpinstruct_lesson_test_order_item_get_answer (item)+1);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    gtk_label_new (_("Answer:")),
			                    FALSE, TRUE, 0);
			gtk_box_pack_start (GTK_BOX (content_area),
			                    answer_spin,
			                    FALSE, TRUE, 0);

			gtk_widget_show_all (content_area);
			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				GtkTextIter start, end;
				gtk_text_buffer_get_bounds (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)),
				                            &start, &end);
				gchar *text = gtk_text_iter_get_text (&start, &end);
				gpinstruct_lesson_test_order_item_set_text (item, text);
				g_free (text);

				gpinstruct_lesson_test_order_item_set_answer (item,
				                                              gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (answer_spin))-1);

				update_tree_view (GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data));
				gpinstruct_editor_window_set_modified (priv->window, TRUE);

				gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (priv->store),
				                               &iter, NULL, gtk_tree_path_get_indices (path)[0]);
				gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view)), &iter);
			}

			gtk_widget_destroy (dialog);
		}
	}
}


static void
items_add_button_clicked (GtkButton *button,
                          gpointer   user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	GPInstructLessonTestOrderItem *item;
	GtkTreeIter iter;

	item = gpinstruct_lesson_test_order_item_new ();
	gpinstruct_lesson_test_order_item_set_answer (item,
	                                              gtk_tree_model_iter_n_children (GTK_TREE_MODEL (priv->store), NULL));
	gpinstruct_lesson_test_order_add_item (priv->test, item);

	gtk_list_store_append (priv->store, &iter);
	gtk_list_store_set (priv->store, &iter,
	                    TITLE_COLUMN, _("(Empty Item)"),
	                    DATA_COLUMN, item,
	                    -1);

	gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->tree_view)),
	                                &iter);

	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}


static void
items_remove_button_clicked (GtkButton *button,
                             gpointer   user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	GtkTreeSelection *selection;
	GtkTreeIter iter, iterSel;
	GtkTreePath *path;
	gboolean select = FALSE;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->tree_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
	{
		iterSel = iter;
		if (gtk_tree_model_iter_next (GTK_TREE_MODEL (priv->store), &iterSel))
			select = TRUE;
		else
		{
			iterSel = iter;
			if (gtk_tree_model_iter_previous (GTK_TREE_MODEL (priv->store), &iterSel))
				select = TRUE;
		}

		if (select)
			gtk_tree_selection_select_iter (selection, &iterSel);

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (priv->store), &iter);

		gpinstruct_lesson_test_order_remove_item (priv->test, gtk_tree_path_get_indices (path)[0]);

		gtk_list_store_remove (priv->store, &iter);

		gtk_tree_path_free (path);

		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}


G_DEFINE_TYPE (GPInstructLessonTestOrderEditor, gpinstruct_lesson_test_order_editor, GPINSTRUCT_TYPE_OBJECT_EDITOR);

static void
gpinstruct_lesson_test_order_editor_init (GPInstructLessonTestOrderEditor *object)
{
	GtkStyleContext *context;

	object->priv = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR_GET_PRIVATE (object);
	GPInstructLessonTestOrderEditorPrivate *priv = object->priv;

	priv->store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);

	GtkWidget *title_label = gtk_label_new (_("Title"));
	context = gtk_widget_get_style_context (title_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (title_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), title_label);

	priv->title_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (priv->title_entry, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->title_entry,
	                         title_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *directions_label = gtk_label_new (_("Directions"));
	context = gtk_widget_get_style_context (directions_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (directions_label, GTK_ALIGN_END);
	gtk_widget_set_valign (directions_label, GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (object), directions_label);

	GtkWidget *directions_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (directions_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	priv->directions_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (directions_view_scrolled_window), priv->directions_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->directions_view), GTK_WRAP_WORD_CHAR);
	gtk_widget_set_hexpand (priv->directions_view, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), directions_view_scrolled_window,
	                         directions_label, GTK_POS_RIGHT, 1, 3);

	GtkWidget *explain_label = gtk_label_new (_("Explain"));
	context = gtk_widget_get_style_context (explain_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (explain_label, GTK_ALIGN_END);
	gtk_container_add (GTK_CONTAINER (object), explain_label);

	priv->explain_switch = gtk_switch_new ();
	gtk_widget_set_halign (priv->explain_switch, GTK_ALIGN_START);
	gtk_grid_attach_next_to (GTK_GRID (object), priv->explain_switch,
	                         explain_label, GTK_POS_RIGHT, 1, 1);

	GtkWidget *explanation_label = gtk_label_new (_("Explanation"));
	context = gtk_widget_get_style_context (explanation_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (explanation_label, GTK_ALIGN_END);
	gtk_widget_set_valign (explanation_label, GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (object), explanation_label);

	GtkWidget *explanation_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (explanation_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	priv->explanation_view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (explanation_view_scrolled_window), priv->explanation_view);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (priv->explanation_view), GTK_WRAP_WORD_CHAR);
	gtk_widget_set_hexpand (priv->explanation_view, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), explanation_view_scrolled_window,
	                         explanation_label, GTK_POS_RIGHT, 1, 3);

	GtkWidget *items_label = gtk_label_new (_("Items"));
	context = gtk_widget_get_style_context (items_label);
	gtk_style_context_add_class (context, "dim-label");
	gtk_widget_set_halign (items_label, GTK_ALIGN_END);
	gtk_widget_set_valign (items_label, GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (object), items_label);

	GtkWidget *items_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_widget_set_vexpand (items_hbox, TRUE);
	gtk_grid_attach_next_to (GTK_GRID (object), items_hbox,
	                         items_label, GTK_POS_RIGHT, 1, 3);

	GtkWidget *tree_view_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (tree_view_scrolled_window),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (items_hbox), tree_view_scrolled_window, TRUE, TRUE, 0);

	priv->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (priv->tree_view), FALSE);
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (priv->tree_view), TRUE);
	gtk_widget_set_events (priv->tree_view, GDK_BUTTON_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect (priv->tree_view, "row-activated", G_CALLBACK (tree_view_row_activated), object);
	gtk_container_add (GTK_CONTAINER (tree_view_scrolled_window), priv->tree_view);

	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes (_("Items:"), gtk_cell_renderer_text_new (),
	                                                                      "text", 0,
	                                                                      NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->tree_view), column);

	GtkWidget *items_buttonbox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (items_buttonbox), GTK_BUTTONBOX_START);
	gtk_box_pack_start (GTK_BOX (items_hbox), items_buttonbox, FALSE, TRUE, 0);

	GtkWidget *items_add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	g_signal_connect (items_add_button, "clicked", G_CALLBACK (items_add_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (items_buttonbox), items_add_button, FALSE, TRUE, 0);

	GtkWidget *items_remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	g_signal_connect (items_remove_button, "clicked", G_CALLBACK (items_remove_button_clicked), object);
	gtk_box_pack_start (GTK_BOX (items_buttonbox), items_remove_button, FALSE, TRUE, 0);
}

static void
gpinstruct_lesson_test_order_editor_finalize (GObject *object)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (object);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	if (priv->store)
		g_object_unref (priv->store);

	G_OBJECT_CLASS (gpinstruct_lesson_test_order_editor_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_order_editor_class_init (GPInstructLessonTestOrderEditorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructObjectEditorClass *parent_class = GPINSTRUCT_OBJECT_EDITOR_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestOrderEditorPrivate));

	object_class->finalize = gpinstruct_lesson_test_order_editor_finalize;
}


static void
title_entry_activate (GtkEntry *entry,
                      gpointer  user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	gpinstruct_lesson_element_set_title (GPINSTRUCT_LESSON_ELEMENT (priv->test),
	                                     gtk_entry_get_text (GTK_ENTRY (priv->title_entry)));
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
	gpinstruct_editor_window_update_tree_store (priv->window, (gpointer)priv->test);
}

static void
directions_buffer_changed (GtkTextBuffer *textbuffer,
                           gpointer       user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_test_set_directions (GPINSTRUCT_LESSON_TEST (priv->test),
	                                       text);
	g_free (text);
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}

static void
explain_activate (GObject    *gobject,
                  GParamSpec *pspec,
                  gpointer    user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	gboolean active = gtk_switch_get_active (GTK_SWITCH (priv->explain_switch));

	if (active != gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (priv->test)))
	{
		gpinstruct_lesson_test_set_explain (GPINSTRUCT_LESSON_TEST (priv->test),
		                                    active);
		gpinstruct_editor_window_set_modified (priv->window, TRUE);
	}
}

static void
explanation_buffer_changed (GtkTextBuffer *textbuffer,
                            gpointer       user_data)
{
	GPInstructLessonTestOrderEditor *editor = GPINSTRUCT_LESSON_TEST_ORDER_EDITOR (user_data);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	GtkTextIter start, end;
	gchar *text;
	gtk_text_buffer_get_bounds (textbuffer, &start, &end);
	text = gtk_text_iter_get_text (&start, &end);
	gpinstruct_lesson_test_order_set_explanation (priv->test,
	                                              text);
	g_free (text);
	gpinstruct_editor_window_set_modified (priv->window, TRUE);
}


GPInstructLessonTestOrderEditor *
gpinstruct_lesson_test_order_editor_new (GPInstructEditorWindow *window,
                                         GPInstructLessonTestOrder *test)
{
	GPInstructLessonTestOrderEditor *editor = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_ORDER_EDITOR, NULL);
	GPInstructLessonTestOrderEditorPrivate *priv = editor->priv;

	GtkTextBuffer *buffer;

	priv->window = window;
	priv->test = test;

	gtk_entry_set_text (GTK_ENTRY (priv->title_entry),
	                    gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	g_signal_connect (priv->title_entry, "activate",
	                  G_CALLBACK (title_entry_activate), editor);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->directions_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_test_get_directions (GPINSTRUCT_LESSON_TEST (test)), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (directions_buffer_changed), editor);

	gtk_switch_set_active (GTK_SWITCH (priv->explain_switch),
	                       gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (test)));
	g_signal_connect (priv->explain_switch, "notify::active",
	                  G_CALLBACK (explain_activate), editor);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->explanation_view));
	gtk_text_buffer_set_text (buffer,
	                          gpinstruct_lesson_test_order_get_explanation (test), -1);
	g_signal_connect (buffer, "changed",
	                  G_CALLBACK (explanation_buffer_changed), editor);

	update_tree_view (editor);

	return editor;
}
