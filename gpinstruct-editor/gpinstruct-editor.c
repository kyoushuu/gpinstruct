/*
 * gpinstruct-editor.c
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

#include <gtk/gtk.h>

#if (GTK_MAJOR_VERSION < 3)
static gboolean
gtk_tree_model_iter_previous_default (GtkTreeModel *tree_model,
                                      GtkTreeIter  *iter)
{
  gboolean retval;
  GtkTreePath *path;

  path = gtk_tree_model_get_path (tree_model, iter);
  if (path == NULL)
    return FALSE;

  retval = gtk_tree_path_prev (path) &&
           gtk_tree_model_get_iter (tree_model, iter, path);
  if (retval == FALSE)
    iter->stamp = 0;

  gtk_tree_path_free (path);

  return retval;
}

/**
 * gtk_tree_model_iter_previous:
 * @tree_model: a #GtkTreeModel
 * @iter: (in): the #GtkTreeIter
 *
 * Sets @iter to point to the previous node at the current level.
 *
 * If there is no previous @iter, %FALSE is returned and @iter is
 * set to be invalid.
 *
 * Return value: %TRUE if @iter has been changed to the previous node
 *
 * Since: 3.0
 */
gboolean
gtk_tree_model_iter_previous (GtkTreeModel *tree_model,
                              GtkTreeIter  *iter)
{
  g_return_val_if_fail (GTK_IS_TREE_MODEL (tree_model), FALSE);
  g_return_val_if_fail (iter != NULL, FALSE);

  return gtk_tree_model_iter_previous_default (tree_model, iter);
}
#endif
