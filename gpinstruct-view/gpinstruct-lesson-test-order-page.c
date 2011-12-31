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

#include <config.h>

#include <gtk/gtk.h>

#include "gpinstruct/gpinstruct.h"
#include "gpinstruct-view/gpinstruct-view.h"
#include "gpinstruct-view-private.h"

struct _GPInstructLessonTestOrderPagePrivate
{
	GPInstructLessonTestOrder *test;
	GPInstructLessonScore *score;
	GPInstructLog *log;

	guint *items;

	GtkListStore *store;
};

#define GPINSTRUCT_LESSON_TEST_ORDER_PAGE_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GPINSTRUCT_TYPE_LESSON_TEST_ORDER_PAGE, GPInstructLessonTestOrderPagePrivate))



static void
page_reset (GPInstructLessonTestOrderPage *page,
            gpointer user_data)
{
	GPInstructLessonTestOrderPagePrivate *priv = page->priv;

	if (priv->log)
		gpinstruct_log_close_test (priv->log);

	GList *items = gpinstruct_lesson_test_order_get_items (priv->test);
	guint length = g_list_length (items);

	g_free (priv->items);

	priv->items = random_array (length);
	gtk_list_store_clear (priv->store);

	int i;
	for (i=0; i<length; i++)
		gtk_list_store_insert_with_values (priv->store, NULL, -1,
		                                   0, gpinstruct_lesson_test_order_item_get_text (GPINSTRUCT_LESSON_TEST_ORDER_ITEM (g_list_nth_data (items, priv->items[i]))),
		                                   1, gpinstruct_lesson_test_order_item_get_answer (GPINSTRUCT_LESSON_TEST_ORDER_ITEM (g_list_nth_data (items, priv->items[i]))),
		                                   -1);

	g_list_free (items);
}

G_DEFINE_TYPE (GPInstructLessonTestOrderPage, gpinstruct_lesson_test_order_page, GPINSTRUCT_TYPE_LESSON_VIEW_PAGE);

static void
gpinstruct_lesson_test_order_page_init (GPInstructLessonTestOrderPage *object)
{
	object->priv = GPINSTRUCT_LESSON_TEST_ORDER_PAGE_GET_PRIVATE (object);
	GPInstructLessonTestOrderPagePrivate *priv = object->priv;

	priv->test = NULL;
	priv->score = NULL;
	priv->log = NULL;
	priv->items = NULL;
	priv->store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_UINT);
}

static void
gpinstruct_lesson_test_order_page_finalize (GObject *object)
{
	GPInstructLessonTestOrderPage *page = GPINSTRUCT_LESSON_TEST_ORDER_PAGE (object);
	GPInstructLessonTestOrderPagePrivate *priv = page->priv;

	g_free (priv->items);

	if (priv->store)
		g_object_unref (priv->store);

	G_OBJECT_CLASS (gpinstruct_lesson_test_order_page_parent_class)->finalize (object);
}

static void
gpinstruct_lesson_test_order_page_class_init (GPInstructLessonTestOrderPageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	/*GPInstructLessonViewPageClass *parent_class = GPINSTRUCT_LESSON_VIEW_PAGE_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (GPInstructLessonTestOrderPagePrivate));

	object_class->finalize = gpinstruct_lesson_test_order_page_finalize;
}



static gboolean
page_show_next (GPInstructLessonTestOrderPage *page,
                gpointer user_data)
{
	GtkTreeIter iter;
	guint questions_num = 0, wrong = 0, position;

	GPInstructLessonTestOrderPagePrivate *priv = page->priv;

	if (priv->log)
		gpinstruct_log_set_group (priv->log,
		                          gtk_tree_model_iter_n_children (GTK_TREE_MODEL (priv->store), NULL));

	if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (priv->store), &iter))
	{
		do
		{
			gpinstruct_lesson_score_increase_total (priv->score);

			gtk_tree_model_get (GTK_TREE_MODEL (priv->store), &iter, 1, &position, -1);

			if (priv->log)
				gpinstruct_log_add_choice (priv->log,
				                           GPINSTRUCT_LESSON_TEST (priv->test),
				                           position, questions_num);

			if (questions_num == position)
				gpinstruct_lesson_score_increase_score (priv->score);
			else
				wrong++;

			questions_num++;
		} while (gtk_tree_model_iter_next (GTK_TREE_MODEL (priv->store), &iter));
	}

	if (gpinstruct_lesson_test_get_explain (GPINSTRUCT_LESSON_TEST (priv->test)))
	{
		if (wrong)
		{
			if (wrong == questions_num)
			{
				gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
					                                     GPINSTRUCT_MESSAGE_TYPE_WRONG_ALL);
			}
			else if ((gfloat)wrong / questions_num > 0.5)
			{
				gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
					                                     GPINSTRUCT_MESSAGE_TYPE_WRONG_MOST);
			}
			else
			{
				gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
					                                     GPINSTRUCT_MESSAGE_TYPE_WRONG_SOME);
			}

			const gchar *explanation = gpinstruct_lesson_test_order_get_explanation (priv->test);

			if (explanation && *explanation != '\0')
				gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
				                                             explanation);
			else
			{
				GString *explanation_answer = g_string_new (NULL);

				GList *items = gpinstruct_lesson_test_order_get_items (priv->test);
				guint length = g_list_length (items);
				const gchar *answers[length];
				int i;
				for (i = 0; i<length; i++)
				{
					GPInstructLessonTestOrderItem *item = g_list_nth_data (items, i);
					answers[gpinstruct_lesson_test_order_item_get_answer (item)] =
						gpinstruct_lesson_test_order_item_get_text (item);
				}
				for (i = 0; i<length; i++)
				{
					g_string_append_printf (explanation_answer,
								            "%d. %s\n",
					                        i+1,
								            answers[i]);
				}
				g_list_free (items);

				gpinstruct_lesson_view_page_set_explanation (GPINSTRUCT_LESSON_VIEW_PAGE (page),
						                                     explanation_answer->str);

				g_string_free (explanation_answer, TRUE);
			}
		}
		else
			gpinstruct_lesson_view_page_set_message (GPINSTRUCT_LESSON_VIEW_PAGE (page),
			                                         GPINSTRUCT_MESSAGE_TYPE_CORRECT_ALL);
	}

	return FALSE;
}


GPInstructLessonTestOrderPage *
gpinstruct_lesson_test_order_page_new (GPInstructLessonTestOrder *test,
                                       GPInstructLessonScore *score,
                                       GPInstructLog *log)
{
	GPInstructLessonTestOrderPage *page = g_object_new (GPINSTRUCT_TYPE_LESSON_TEST_ORDER_PAGE, NULL);
	GPInstructLessonTestOrderPagePrivate *priv = page->priv;

	gpinstruct_lesson_view_page_set_title (GPINSTRUCT_LESSON_VIEW_PAGE (page),
	                                       gpinstruct_lesson_element_get_title (GPINSTRUCT_LESSON_ELEMENT (test)));
	gpinstruct_lesson_view_page_set_show_back_button (GPINSTRUCT_LESSON_VIEW_PAGE (page), FALSE);

	g_signal_connect (page, "show-next", G_CALLBACK (page_show_next), NULL);
	g_signal_connect (page, "reset", G_CALLBACK (page_reset), NULL);

	priv->test = test;
	priv->score = score;
	priv->log = log;

	GtkWidget *treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (priv->store));
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (treeview), TRUE);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), FALSE);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (page), treeview);

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ("Items", renderer, "text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

	return page;
}
