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

#include <config.h>
#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "canvas/canvas.h"
#include "canvas-view/canvas-view.h"
#include "canvas-analyzer/canvas-analyzer.h"

struct _CanvasAnalyzerExamineeViewPrivate
{
	GtkListStore* examinee_store;
	GtkWidget* examinee_treeview;

	GtkTreeStore* project_store;
	GtkWidget* project_treeview;

	GtkTreeStore* test_store;
	GtkWidget* test_treeview;
};

#define CANVAS_ANALYZER_EXAMINEE_VIEW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CANVAS_TYPE_ANALYZER_EXAMINEE_VIEW, CanvasAnalyzerExamineeViewPrivate))



enum
{
	EXAMINEE_NAME_COLUMN,
	EXAMINEE_STRUCT_POINTER_COLUMN,
	EXAMINEE_N_COLUMNS
};

enum
{
	PROJECT_TITLE_COLUMN,
	PROJECT_ITEMS_COLUMN,
	PROJECT_FREQUENCY_COLUMN,
	PROJECT_SCORE_COLUMN,
	PROJECT_AVE_SCORE_COLUMN,
	PROJECT_PERCENTAGE_COLUMN,
	PROJECT_TIME_COLUMN,
	PROJECT_AVE_TIME_COLUMN,
	PROJECT_STRUCT_TYPE_COLUMN,
	PROJECT_STRUCT_POINTER_COLUMN,
	PROJECT_N_COLUMNS
};

enum
{
	TEST_TITLE_COLUMN,
	TEST_ITEMS_COLUMN,
	TEST_FREQUENCY_COLUMN,
	TEST_SCORE_COLUMN,
	TEST_PERCENTAGE_COLUMN,
	TEST_TIME_COLUMN,
	TEST_AVE_TIME_COLUMN,
	TEST_N_COLUMNS
};

enum
{
	STRUCT_TYPE_PROJECT,
	STRUCT_TYPE_CATEGORY,
	STRUCT_TYPE_LESSON,
	STRUCT_TYPE_TEST
};


static void
examinee_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer user_data)
{
	CanvasAnalyzerExamineeView* view = CANVAS_ANALYZER_EXAMINEE_VIEW (user_data);

	CanvasLogAnalyzerExaminee* examinee;

	GtkTreeIter iter, iterProject, iterCategory, iterLesson;
	GtkTreeModel *model;

	guint min;
	gdouble sec;

	guint items, frequency, score;
	gdouble ave_score, percentage, time_spent, ave_time_spent;

	gchar *items_text, *frequency_text, *score_text, *ave_score_text, *percentage_text, *time_text, *ave_time_text;

	gtk_tree_store_clear (view->priv->project_store);

	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter,
		                    EXAMINEE_STRUCT_POINTER_COLUMN, &examinee,
		                    -1);

		if (examinee)
		{
			CanvasLogAnalyzerProject* project = examinee->project;

			items = project->items_length;
			frequency = project->times_taken;
			score = project->items_correctly_answered;
			ave_score =  (frequency? score/frequency:0);
			percentage = (items?(ave_score/items):0) * 100;
			time_spent = project->time_spent;
			ave_time_spent = frequency? time_spent/frequency:0;

			items_text = g_strdup_printf ("%d", items);
			frequency_text = g_strdup_printf ("%d", frequency);
			score_text = g_strdup_printf ("%d", score);
			ave_score_text = g_strdup_printf ("%.2f", ave_score);
			percentage_text = g_strdup_printf ("%.2f%%", percentage);
			min = time_spent / 60;
			sec = time_spent - (min * 60);
			time_text = g_strdup_printf ("%dm %.2fs", min, sec);
			min = ave_time_spent / 60;
			sec = ave_time_spent - (min * 60);
			ave_time_text = g_strdup_printf ("%dm %.2fs", min, sec);

			gtk_tree_store_insert_with_values (view->priv->project_store, &iterProject, NULL, -1,
					                           PROJECT_TITLE_COLUMN, canvas_project_get_title (project->object),
					                           PROJECT_ITEMS_COLUMN, items_text,
					                           PROJECT_FREQUENCY_COLUMN, frequency_text,
					                           PROJECT_SCORE_COLUMN, score_text,
								               PROJECT_AVE_SCORE_COLUMN, ave_score_text,
					                           PROJECT_PERCENTAGE_COLUMN, percentage_text,
					                           PROJECT_TIME_COLUMN, time_text,
					                           PROJECT_AVE_TIME_COLUMN, ave_time_text,
					                           PROJECT_STRUCT_TYPE_COLUMN, STRUCT_TYPE_PROJECT,
					                           PROJECT_STRUCT_POINTER_COLUMN, project,
					                           -1);

			g_free (items_text);
			g_free (frequency_text);
			g_free (score_text);
			g_free (ave_score_text);
			g_free (percentage_text);
			g_free (time_text);
			g_free (ave_time_text);

			GList* categories = project->categories;

			while (categories)
			{
				CanvasLogAnalyzerCategory* category = (CanvasLogAnalyzerCategory*)categories->data;

				items = category->items_length;
				frequency = category->times_taken;
				score = category->items_correctly_answered;
				ave_score =  (frequency? score/frequency:0);
				percentage = (items?(ave_score/items):0) * 100;
				time_spent = category->time_spent;
				ave_time_spent = frequency? time_spent/frequency:0;

				items_text = g_strdup_printf ("%d", items);
				frequency_text = g_strdup_printf ("%d", frequency);
				score_text = g_strdup_printf ("%d", score);
				ave_score_text = g_strdup_printf ("%.2f", ave_score);
				percentage_text = g_strdup_printf ("%.2f%%", percentage);
				min = time_spent / 60;
				sec = time_spent - (min * 60);
				time_text = g_strdup_printf ("%dm %.2fs", min, sec);
				min = ave_time_spent / 60;
				sec = ave_time_spent - (min * 60);
				ave_time_text = g_strdup_printf ("%dm %.2fs", min, sec);

				gtk_tree_store_insert_with_values (view->priv->project_store, &iterCategory, &iterProject, -1,
							                       PROJECT_TITLE_COLUMN, canvas_category_get_title (category->object),
							                       PROJECT_ITEMS_COLUMN, items_text,
							                       PROJECT_FREQUENCY_COLUMN, frequency_text,
							                       PROJECT_SCORE_COLUMN, score_text,
								                   PROJECT_AVE_SCORE_COLUMN, ave_score_text,
							                       PROJECT_PERCENTAGE_COLUMN, percentage_text,
							                       PROJECT_TIME_COLUMN, time_text,
							                       PROJECT_AVE_TIME_COLUMN, ave_time_text,
							                       PROJECT_STRUCT_TYPE_COLUMN, STRUCT_TYPE_CATEGORY,
							                       PROJECT_STRUCT_POINTER_COLUMN, category,
							                       -1);

				g_free (items_text);
				g_free (frequency_text);
				g_free (score_text);
				g_free (ave_score_text);
				g_free (percentage_text);
				g_free (time_text);
				g_free (ave_time_text);

				GList* lessons = category->lessons;

				while (lessons)
				{
					CanvasLogAnalyzerLesson* lesson = (CanvasLogAnalyzerLesson*)lessons->data;

					items = lesson->items_length;
					frequency = lesson->times_taken;
					score = lesson->items_correctly_answered;
					ave_score =  (frequency? score/frequency:0);
					percentage = (items?(ave_score/items):0) * 100;
					time_spent = lesson->time_spent;
					ave_time_spent = frequency? time_spent/frequency:0;

					items_text = g_strdup_printf ("%d", items);
					frequency_text = g_strdup_printf ("%d", frequency);
					score_text = g_strdup_printf ("%d", score);
					ave_score_text = g_strdup_printf ("%.2f", ave_score);
					percentage_text = g_strdup_printf ("%.2f%%", percentage);
					min = time_spent / 60;
					sec = time_spent - (min * 60);
					time_text = g_strdup_printf ("%dm %.2fs", min, sec);
					min = ave_time_spent / 60;
					sec = ave_time_spent - (min * 60);
					ave_time_text = g_strdup_printf ("%dm %.2fs", min, sec);

					gtk_tree_store_insert_with_values (view->priv->project_store, &iterLesson, &iterCategory, -1,
									                   PROJECT_TITLE_COLUMN, canvas_lesson_get_title (lesson->object),
									                   PROJECT_ITEMS_COLUMN, items_text,
									                   PROJECT_FREQUENCY_COLUMN, frequency_text,
									                   PROJECT_SCORE_COLUMN, score_text,
								                       PROJECT_AVE_SCORE_COLUMN, ave_score_text,
									                   PROJECT_PERCENTAGE_COLUMN, percentage_text,
									                   PROJECT_TIME_COLUMN, time_text,
									                   PROJECT_AVE_TIME_COLUMN, ave_time_text,
									                   PROJECT_STRUCT_TYPE_COLUMN, STRUCT_TYPE_LESSON,
									                   PROJECT_STRUCT_POINTER_COLUMN, lesson,
									                   -1);

					g_free (items_text);
					g_free (frequency_text);
					g_free (score_text);
					g_free (ave_score_text);
					g_free (percentage_text);
					g_free (time_text);
					g_free (ave_time_text);

					GList* tests = lesson->tests;

					while (tests)
					{
						CanvasLogAnalyzerTest* test = (CanvasLogAnalyzerTest*)tests->data;

						items = test->items_length;
						frequency = test->times_taken;
						score = test->items_correctly_answered;
						ave_score =  (frequency? score/frequency:0);
						percentage = (items?(ave_score/items):0) * 100;
						time_spent = test->time_spent;
						ave_time_spent = frequency? time_spent/frequency:0;

						items_text = g_strdup_printf ("%d", items);
						frequency_text = g_strdup_printf ("%d", frequency);
						score_text = g_strdup_printf ("%d", score);
						ave_score_text = g_strdup_printf ("%.2f", ave_score);
						percentage_text = g_strdup_printf ("%.2f%%", percentage);
						min = time_spent / 60;
						sec = time_spent - (min * 60);
						time_text = g_strdup_printf ("%dm %.2fs", min, sec);
						min = ave_time_spent / 60;
						sec = ave_time_spent - (min * 60);
						ave_time_text = g_strdup_printf ("%dm %.2fs", min, sec);

						gtk_tree_store_insert_with_values (view->priv->project_store, NULL, &iterLesson, -1,
											               PROJECT_TITLE_COLUMN, canvas_lesson_element_get_title (CANVAS_LESSON_ELEMENT (test->object)),
											               PROJECT_ITEMS_COLUMN, items_text,
											               PROJECT_FREQUENCY_COLUMN, frequency_text,
											               PROJECT_SCORE_COLUMN, score_text,
								                           PROJECT_AVE_SCORE_COLUMN, ave_score_text,
											               PROJECT_PERCENTAGE_COLUMN, percentage_text,
											               PROJECT_TIME_COLUMN, time_text,
											               PROJECT_AVE_TIME_COLUMN, ave_time_text,
											               PROJECT_STRUCT_TYPE_COLUMN, STRUCT_TYPE_TEST,
											               PROJECT_STRUCT_POINTER_COLUMN, test,
											               -1);

						g_free (items_text);
						g_free (frequency_text);
						g_free (score_text);
						g_free (ave_score_text);
						g_free (percentage_text);
						g_free (time_text);
						g_free (ave_time_text);

						tests = tests->next;
					}

					lessons = lessons->next;
				}

				categories = categories->next;
			}

			gtk_tree_view_expand_all (GTK_TREE_VIEW (view->priv->project_treeview));
		}
	}
}


static void
project_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer user_data)
{
	CanvasAnalyzerExamineeView* view = CANVAS_ANALYZER_EXAMINEE_VIEW (user_data);

	GtkTreeIter iter, iterItem, iterChoice;
	GtkTreeModel *model;
	guint struct_type;
	CanvasLogAnalyzerTest* test;

	guint min;
	gdouble sec;

	guint children, frequency, score;
	gdouble percentage, time_spent, ave_time_spent;

	gchar *title_text, *children_text, *frequency_text, *score_text,
	*percentage_text, *time_text, *ave_time_text;

	gtk_tree_store_clear (view->priv->test_store);

	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter,
		                    PROJECT_STRUCT_TYPE_COLUMN, &struct_type,
		                    PROJECT_STRUCT_POINTER_COLUMN, &test,
		                    -1);

		if (struct_type == STRUCT_TYPE_TEST)
		{
			GList* items = test->items;
			guint item_num = 0;

			while (items)
			{
				CanvasLogAnalyzerItem* item = items->data;

				children = item->choices_length;
				frequency = item->times_answered;
				score = item->times_correctly_answered;
				percentage = (children?(score/children):0) * 100;
				time_spent = item->time_spent;
				ave_time_spent = frequency? time_spent/frequency:0;

				title_text = canvas_lesson_test_get_item (test->object,
				                                          item_num);
				children_text = g_strdup_printf ("%d", children);
				frequency_text = g_strdup_printf ("%d", frequency);
				score_text = g_strdup_printf ("%d", score);
				percentage_text = g_strdup_printf ("%.2f%%", percentage);
				min = time_spent / 60;
				sec = time_spent - (min * 60);
				time_text = g_strdup_printf ("%dm %.2fs", min, sec);
				min = ave_time_spent / 60;
				sec = ave_time_spent - (min * 60);
				ave_time_text = g_strdup_printf ("%dm %.2fs", min, sec);

				gtk_tree_store_insert_with_values (view->priv->test_store,
				                                   &iterItem, NULL,
				                                   -1,
							                       TEST_TITLE_COLUMN, title_text,
							                       TEST_ITEMS_COLUMN, children_text,
							                       TEST_FREQUENCY_COLUMN, frequency_text,
							                       TEST_SCORE_COLUMN, score_text,
							                       TEST_PERCENTAGE_COLUMN, percentage_text,
							                       TEST_TIME_COLUMN, time_text,
							                       TEST_AVE_TIME_COLUMN, ave_time_text,
				                                   -1);

				g_free (title_text);
				g_free (children_text);
				g_free (frequency_text);
				g_free (score_text);
				g_free (percentage_text);
				g_free (time_text);
				g_free (ave_time_text);

				GList* choices = item->choices;
				guint choice_num = 0;

				while (choices)
				{
					CanvasLogAnalyzerChoice* choice = choices->data;

					title_text = canvas_lesson_test_get_choice (test->object,
					                                            item_num,
					                                            choice_num);
					frequency = choice->times_chosen;
					percentage = (item->times_answered?(frequency/item->times_answered):0) * 100;
					time_spent = choice->time_spent;
					ave_time_spent = frequency? time_spent/frequency:0;

					frequency_text = g_strdup_printf ("%d", frequency);
					percentage_text = g_strdup_printf ("%.2f%%", percentage);
					min = time_spent / 60;
					sec = time_spent - (min * 60);
					time_text = g_strdup_printf ("%dm %.2fs", min, sec);
					min = ave_time_spent / 60;
					sec = ave_time_spent - (min * 60);
					ave_time_text = g_strdup_printf ("%dm %.2fs", min, sec);

					gtk_tree_store_insert_with_values (view->priv->test_store,
							                           &iterChoice, &iterItem,
							                           -1,
										               TEST_TITLE_COLUMN, title_text,
										               TEST_FREQUENCY_COLUMN, frequency_text,
					                                   TEST_PERCENTAGE_COLUMN, percentage_text,
										               TEST_TIME_COLUMN, time_text,
										               TEST_AVE_TIME_COLUMN, ave_time_text,
							                           -1);

					g_free (title_text);
					g_free (frequency_text);
					g_free (percentage_text);
					g_free (time_text);
					g_free (ave_time_text);

					choice_num++;
					choices = choices->next;
				}

				item_num++;
				items = items->next;
			}
		}
	}
}



G_DEFINE_TYPE (CanvasAnalyzerExamineeView, canvas_analyzer_examinee_view, GTK_TYPE_HPANED);

static void
canvas_analyzer_examinee_view_init (CanvasAnalyzerExamineeView *object)
{
	object->priv = CANVAS_ANALYZER_EXAMINEE_VIEW_PRIVATE (object);

	GtkTreeSelection *selection;
	GtkCellRenderer* renderer;
	GtkTreeViewColumn *column;

	gtk_paned_set_position (GTK_PANED (object), 200);

	GtkWidget* examinee_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (examinee_scrolledwindow),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack1 (GTK_PANED (object), examinee_scrolledwindow, FALSE, TRUE);

	object->priv->examinee_store = gtk_list_store_new (EXAMINEE_N_COLUMNS,
	                                                   G_TYPE_STRING,
	                                                   G_TYPE_POINTER);

	GtkWidget* examinee_treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->examinee_store));
	gtk_container_add (GTK_CONTAINER (examinee_scrolledwindow), examinee_treeview);
	object->priv->examinee_treeview = examinee_treeview;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (examinee_treeview));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
	g_signal_connect (selection, "changed",
		              G_CALLBACK (examinee_tree_selection_changed_cb),
		              object);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Examinees"),
	                                                   renderer,
	                                                   "text", EXAMINEE_NAME_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_expand (column, TRUE);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (examinee_treeview), column);

	GtkWidget* vpaned = gtk_vpaned_new ();
	gtk_paned_pack2 (GTK_PANED (object), vpaned, TRUE, TRUE);

	GtkWidget* project_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (project_scrolledwindow),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack1 (GTK_PANED (vpaned), project_scrolledwindow, TRUE, TRUE);

	object->priv->project_store = gtk_tree_store_new (PROJECT_N_COLUMNS,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_STRING,
	                                                  G_TYPE_UINT,
	                                                  G_TYPE_POINTER);

	GtkWidget* project_treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->project_store));
	gtk_container_add (GTK_CONTAINER (project_scrolledwindow), project_treeview);
	object->priv->project_treeview = project_treeview;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (project_treeview));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
	g_signal_connect (selection, "changed",
		              G_CALLBACK (project_tree_selection_changed_cb),
		              object);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Project View"),
	                                                   renderer,
	                                                   "text", PROJECT_TITLE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_expand (column, TRUE);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Total Items"),
	                                                   renderer,
	                                                   "text", PROJECT_ITEMS_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Frequency"),
	                                                   renderer,
	                                                   "text", PROJECT_FREQUENCY_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Total Score"),
	                                                   renderer,
	                                                   "text", PROJECT_SCORE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Average Score"),
	                                                   renderer,
	                                                   "text", PROJECT_AVE_SCORE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Percentage"),
	                                                   renderer,
	                                                   "text", PROJECT_PERCENTAGE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Time Spent"),
	                                                   renderer,
	                                                   "text", PROJECT_TIME_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Average Time Spent"),
	                                                   renderer,
	                                                   "text", PROJECT_AVE_TIME_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (project_treeview), column);

	GtkWidget* test_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (test_scrolledwindow),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack2 (GTK_PANED (vpaned), test_scrolledwindow, TRUE, TRUE);

	object->priv->test_store = gtk_tree_store_new (TEST_N_COLUMNS,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_STRING,
	                                               G_TYPE_UINT,
	                                               G_TYPE_POINTER);

	GtkWidget* test_treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (object->priv->test_store));
	gtk_container_add (GTK_CONTAINER (test_scrolledwindow), test_treeview);
	object->priv->test_treeview = test_treeview;

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Test View"),
	                                                   renderer,
	                                                   "text", TEST_TITLE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_expand (column, TRUE);
	gtk_tree_view_column_set_resizable (column, TRUE);
	g_object_set (renderer,
	              "ellipsize", PANGO_ELLIPSIZE_END,
	              "ellipsize-set", TRUE,
	              NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Number of Choices"),
	                                                   renderer,
	                                                   "text", TEST_ITEMS_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Frequency"),
	                                                   renderer,
	                                                   "text", TEST_FREQUENCY_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Total Score"),
	                                                   renderer,
	                                                   "text", TEST_SCORE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Percentage"),
	                                                   renderer,
	                                                   "text", TEST_PERCENTAGE_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Time Spent"),
	                                                   renderer,
	                                                   "text", TEST_TIME_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_renderer_set_alignment (renderer, 1.0, 0.5);
	column = gtk_tree_view_column_new_with_attributes (_("Average Time Spent"),
	                                                   renderer,
	                                                   "text", TEST_AVE_TIME_COLUMN,
	                                                   NULL);
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (test_treeview), column);
}

static void
canvas_analyzer_examinee_view_finalize (GObject *object)
{
	CanvasAnalyzerExamineeView* view = CANVAS_ANALYZER_EXAMINEE_VIEW (object);

	if (view->priv->examinee_store)
		g_object_unref (view->priv->examinee_store);

	if (view->priv->project_store)
		g_object_unref (view->priv->project_store);

	if (view->priv->test_store)
		g_object_unref (view->priv->test_store);

	G_OBJECT_CLASS (canvas_analyzer_examinee_view_parent_class)->finalize (object);
}

static void
canvas_analyzer_examinee_view_class_init (CanvasAnalyzerExamineeViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	/*GtkHPanedClass* parent_class = GTK_HPANED_CLASS (klass);*/

	g_type_class_add_private (klass, sizeof (CanvasAnalyzerExamineeViewPrivate));

	object_class->finalize = canvas_analyzer_examinee_view_finalize;
}


GtkWidget*
canvas_analyzer_examinee_view_new (CanvasLogAnalyzer* analyzer)
{
	CanvasAnalyzerExamineeView* view = g_object_new (CANVAS_TYPE_ANALYZER_EXAMINEE_VIEW, NULL);

	gchar* name;

	GList* examinees = canvas_log_analyzer_get_examinees (analyzer);
	GList* current_examinees = examinees;

	while (current_examinees)
	{
		CanvasLogAnalyzerExaminee* examinee = current_examinees->data;

		name = g_strdup_printf ("%s, %s",
		                        examinee->last_name,
		                        examinee->first_name);

		gtk_list_store_insert_with_values (view->priv->examinee_store, NULL, -1,
			                               EXAMINEE_NAME_COLUMN, name,
			                               EXAMINEE_STRUCT_POINTER_COLUMN, examinee,
			                               -1);

		g_free (name);

		current_examinees = current_examinees->next;
	}

	g_list_free (examinees);

	return GTK_WIDGET (view);
}
