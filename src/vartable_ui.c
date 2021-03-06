/* vartable_ui.c */ 
/*
 * ggobi
 * Copyright (C) AT&T, Duncan Temple Lang, Dianne Cook 1999-2005
 *
 * ggobi is free software; you may use, redistribute, and/or modify it
 * under the terms of the Common Public License, which is distributed
 * with the source code and displayed on the ggobi web site, 
 * www.ggobi.org.  For more information, contact the authors:
 *
 *   Deborah F. Swayne   dfs@research.att.com
 *   Di Cook             dicook@iastate.edu
 *   Duncan Temple Lang  duncan@wald.ucdavis.edu
 *   Andreas Buja        andreas.buja@wharton.upenn.edu
*/

/* interface code for the variable statistics table: dialogs and buttons */

#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include "vars.h"
#include "externs.h"
#include "utils_ui.h"

#include "ggobi-variable.h"

static void close_btn_cb (GtkWidget *w, GGobiSession *gg)
{
  gtk_widget_hide (gg->vartable_ui.window);
}

static void
clone_vars_cb (GtkWidget *w, GGobiSession *gg)
{
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint *cols;
  gint ncols = selected_cols_get (&cols, d, gg);

  if (ggobi_stage_has_vars(d))
    clone_vars (cols, ncols, d);

  g_free (cols);
}


/* not implemented
static void
delete_vars_cb (GtkWidget *w, GGobiSession *gg)
{
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint *cols = (gint *) g_malloc (d->n_cols * sizeof (gint));
  gint ncols = selected_cols_get (cols, d, gg);

  if (ggobi_stage_has_vars(d))
    delete_vars (cols, ncols, d);

  g_free (cols);
}
*/

/*-------------------------------------------------------------------------*/
/*--------------- Setting and clearing variable ranges --------------------*/
/*-------------------------------------------------------------------------*/

/* disabled while we move to new limits system */
#if 0

static void
limits_type_cb (GtkToggleButton *w, GGobiSession *gg) 
{
  gg->lims_use_visible = w->active;
}

static void
dialog_range_set (GtkWidget *w, GGobiSession *gg) 
{
  GtkWidget *dialog = w;
  GtkWidget *umin_entry, *umax_entry;
  GtkTreeModel *model;
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint *cols;
  gint ncols = selected_cols_get (&cols, d, gg);
  gint j, k;
  gchar *val_str;
  gdouble min_val = 0, max_val = 0; // compiler pacification
  gboolean min_p = false, max_p = false;
  GGobiVariable *var;

  umin_entry = widget_find_by_name (GTK_DIALOG(dialog)->vbox, "umin_entry");
  if (umin_entry == NULL || !GTK_IS_ENTRY(umin_entry)) {
    g_printerr ("found the wrong widget; bail out\n");
    return;
  }
  umax_entry = widget_find_by_name (GTK_DIALOG(dialog)->vbox, "umax_entry");
  if (umax_entry == NULL || !GTK_IS_ENTRY(umax_entry)) {
    g_printerr ("found the wrong widget; bail out\n");
    return;
  }

  /*-- minimum --*/
  val_str = gtk_editable_get_chars (GTK_EDITABLE (umin_entry),
    0, -1);
  if (val_str != NULL && strlen (val_str) > 0) {
    min_val = (gdouble) atof (val_str);
    g_free (val_str);
    min_p = true;
  }

  /*-- maximum --*/
  val_str = gtk_editable_get_chars (GTK_EDITABLE (umax_entry),
    0, -1);
  if (val_str != NULL && strlen (val_str) > 0) {
    max_val = (gdouble) atof (val_str);
    g_free (val_str);
    max_p = true;
  }

  /*-- require setting both, and make sure the values are consistent --*/
  if (!min_p || !max_p || (min_p && max_p && max_val<min_val)) {
    range_unset (gg);
  } else {

    for (k=0; k<ncols; k++) {
      GtkTreeIter iter;
	  
      j = cols[k];
      var = ggobi_stage_get_variable(d, j);

      vartable_iter_from_varno(j, d, &model, &iter);
      
      var->lim_specified.min = min_val;
      var->lim_specified.max = max_val;

      /*gtk_tree_store_set(GTK_TREE_STORE(model), &iter, 
	VT_REAL_USER_MIN, min_val, VT_REAL_USER_MAX, max_val, -1);*/
      
      var->lim_specified_p = min_p && max_p;
      
      ggobi_stage_update_col(d, (guint) j);
    }
    ggobi_stage_flush_changes(d);
  }

  g_free (cols);
}

static void
range_unset_cb (GtkWidget *w, GGobiSession *gg)
{
  range_unset (gg);
}

/*static void rescale_cb (GtkWidget *w, GGobiSession *gg) {*/
  /*GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);*/

  /*limits_set (d, true, true, gg->lims_use_visible);  
  vartable_limits_set (d);
  vartable_stats_set (d);

  tform_to_world(d);
  displays_tailpipe (FULL, gg);*/
/*}*/


/*
 * open a dialog with two text entry widgets in it,
 * and fetch the range for the selected variables in
 * dialog_range_set.
*/

static void
open_range_set_dialog (GtkWidget *w, GGobiSession *gg)
{
  GtkWidget *frame, *vb, *hb, *btn, *lbl;
  GtkWidget *dialog, *umin, *umax;
  GtkWidget *radio1, *radio2;
  GSList *group;
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint *cols;
  gboolean ok = true;
  /*gint ncols =*/ selected_cols_get (&cols, d, gg);
  
  // FIXME: Disabling this for convenience now - pretty sure all this range
  // stuff is going to be removed soon
  /*for (k=0; k<ncols; k++) {
    if (ggobi_stage_transform_get_transform(d, k))
    {
      ok = false;
      quick_message ("Sorry, can't set the range for a transformed variable\n",
        false);
      break;
    }
  }*/
  g_free (cols);
  if (!ok)
	  return;

  dialog = gtk_dialog_new_with_buttons ("Range Dialog", NULL, 0, 
  			GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);

  /*-- frame for a pair of radio buttons --*/
  frame = gtk_frame_new ("Define rescaling behavior");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), frame);

  vb = gtk_vbox_new (true, 5);
  gtk_container_set_border_width (GTK_CONTAINER (vb), 5);
  gtk_container_add (GTK_CONTAINER (frame), vb);

  radio1 = gtk_radio_button_new_with_mnemonic (NULL, "Use _visible points");
  GTK_TOGGLE_BUTTON (radio1)->active = TRUE;
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), radio1,
    "When rescaling, use only the cases that are visible: ie, not hidden by brushing and not excluded by subsampling",
    NULL);
  g_signal_connect (G_OBJECT (radio1), "toggled",
                      G_CALLBACK (limits_type_cb), gg);
  gtk_box_pack_start (GTK_BOX (vb), radio1, false, false, 0);

  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));
  radio2 = gtk_radio_button_new_with_mnemonic (group, "Use _all points");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), radio2,
    "When rescaling, use all cases",
    NULL);
  gtk_box_pack_start (GTK_BOX (vb), radio2, TRUE, TRUE, 0);
  /*-- --*/


  /*-- frame for setting the user-specified limits --*/
  frame = gtk_frame_new ("Override default limits");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), frame);

  vb = gtk_vbox_new (true, 5);
  gtk_container_set_border_width (GTK_CONTAINER (vb), 5);
  gtk_container_add (GTK_CONTAINER (frame), vb);

  /*-- make an hbox to hold a label and a text entry widget --*/
  hb = gtk_hbox_new (true, 5);
  lbl = gtk_label_new_with_mnemonic ("M_inimum: ");
  gtk_box_pack_start (GTK_BOX (hb), lbl, true, true, 0);

  umin = gtk_entry_new ();
  gtk_entry_set_width_chars(GTK_ENTRY(umin), 10);
  gtk_label_set_mnemonic_widget(GTK_LABEL(lbl), umin);
  
  gtk_widget_set_name (umin, "umin_entry");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), umin,
    "Minimum for the selected variable(s)", NULL);
  gtk_box_pack_start (GTK_BOX (hb), umin, true, true, 2);

  gtk_container_add (GTK_CONTAINER (vb), hb);

  /*-- make another hbox --*/
  hb = gtk_hbox_new (true, 5);
  lbl = gtk_label_new_with_mnemonic ("M_aximum: ");
  gtk_box_pack_start (GTK_BOX (hb), lbl,
    true, true, 2);

  umax = gtk_entry_new ();
  gtk_entry_set_width_chars(GTK_ENTRY(umin), 10);
  gtk_label_set_mnemonic_widget(GTK_LABEL(lbl), umax);
  
  gtk_widget_set_name (umax, "umax_entry");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), umax,
    "Maximum for the selected variable(s)", NULL);
  gtk_box_pack_start (GTK_BOX (hb), umax, true, true, 2);

  gtk_container_add (GTK_CONTAINER (vb), hb);
  /*-- --*/

  /*-- frame for the unset range button --*/
  frame = gtk_frame_new ("Restore default limits");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), frame);
  vb = gtk_vbox_new (true, 5);
  gtk_container_set_border_width (GTK_CONTAINER (vb), 5);
  gtk_container_add (GTK_CONTAINER (frame), vb);

  btn = gtk_button_new_with_mnemonic ("_Clear user limits");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Unset user min and max for the selected variable(s)", NULL);
  gtk_box_pack_start (GTK_BOX (vb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (range_unset_cb), gg);
  /*-- --*/

  gtk_widget_show_all(GTK_DIALOG(dialog)->vbox);
  
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	  dialog_range_set(dialog, gg);

  gtk_widget_destroy(dialog);
}
#endif

void range_unset (GGobiSession *gg)
{
  GtkTreeModel *model;
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint *cols;
  gint ncols = selected_cols_get (&cols, d, gg);
  gint j, k;
  GGobiVariable *var;

  for (k=0; k<ncols; k++) {
    GtkTreeIter iter;
	  
    j = cols[k];
    var = ggobi_stage_get_variable(d, j);
	
    vartable_iter_from_varno(j, d, &model, &iter);
	  
    var->lim_specified_p = false;
    /*-- then null out the two entries in the table --*/
    /*gtk_tree_store_set(GTK_TREE_STORE(model), &iter, 
	  	VT_REAL_USER_MIN, 0.0, VT_REAL_USER_MAX, 0.0, -1);*/
	  
	  ggobi_stage_update_col(d, (guint) j);
  }
  ggobi_stage_flush_changes(d);
  g_free ((gchar *) cols);
}


/*-------------------------------------------------------------------------*/
/*------- Adding derived variables (other than cloning, for now) ----------*/
/*-------------------------------------------------------------------------*/

// FIXME: In my (Michael) opinion, this is broken. UI's should be registered
// for creating each type of variable. If no UI is registered for a type,
// then the 'New' button should not be displayed.
// Right now 'New' is displayed under categorical, but one can only make
// real variables.

/* I went ahead and moved this from varchange.c since it seemed specific
  to this GUI. Adding variable seems simple enough that this is not
  needed as a generic utility. */
  
typedef enum {ADDVAR_ROWNOS, ADDVAR_BGROUP} NewVariableType;

static guint
create_explicit_variable (GGobiStage * d, gchar * vname, NewVariableType vartype)
{
  guint jvar;
  
  d = ggobi_stage_get_root(d);
  jvar = ggobi_data_add_cols(GGOBI_DATA(d), 1);
  ggobi_stage_set_col_name(d, jvar, vname);

  GGOBI_STAGE_ATTR_INIT(d, cluster);
  guint i;

  for( i =  0; i < d->n_rows; i++) {
    switch(vartype) {
      case ADDVAR_ROWNOS:
        ggobi_stage_set_raw_value(d, i, jvar, (gdouble) (i + 1));
        break;
      case ADDVAR_BGROUP:
        ggobi_stage_set_raw_value(d, i, jvar, (gdouble) GGOBI_STAGE_GET_ATTR_CLUSTER(d, i));
        break;
    }
  }
  ggobi_stage_update_col(d, jvar);
  ggobi_stage_flush_changes(d);
  return jvar;
}

static void
dialog_newvar_add (GtkWidget *w, GGobiSession *gg) 
{
  GtkWidget *dialog = w;
  GtkWidget *entry, *radio_brush;
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint vartype;
  gchar *vname;

  /*-- retrieve the radio button for the brushing groups --*/
  radio_brush = widget_find_by_name (GTK_DIALOG(dialog)->vbox, "radio_brush");
  if (radio_brush == NULL || !GTK_IS_RADIO_BUTTON(radio_brush)) {
    g_printerr ("found the wrong widget; bail out\n");
    return;
  }
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_brush)))
    vartype = ADDVAR_BGROUP;
  else vartype = ADDVAR_ROWNOS;

  /*-- retrieve the entry widget and variable name --*/
  entry = widget_find_by_name (GTK_DIALOG(dialog)->vbox, "newvar_entry");
  if (entry == NULL || !GTK_IS_ENTRY(entry)) {
    g_printerr ("found the wrong widget; bail out\n");
    return;
  }
  vname = gtk_editable_get_chars (GTK_EDITABLE (entry), 0, -1);
  if (vname != NULL && strlen(vname) > 0) {
     create_explicit_variable(d, vname, vartype);

/* I think we still want to do this ... */
#ifdef FORMERLY
    /*-- scroll to the bottom to highlight the new variable --*/
    nb = GTK_NOTEBOOK (gg->vartable_ui.notebook);
    indx = gtk_notebook_get_current_page (nb);
    /*-- each notebook page's child is a scrolled window --*/
    swin = gtk_notebook_get_nth_page (nb, indx);
    adj = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (swin));
    adj->value += adj->page_increment;
    gtk_adjustment_value_changed (adj);
#endif

  }
}

static void
open_newvar_dialog (GtkWidget *w, GGobiSession *gg)
{
  GtkWidget *dialog;
  GtkWidget *frame, *vb, *hb, *lbl;
  GtkWidget *radio1, *radio2, *entry;
  GSList *radio_group;

  dialog = gtk_dialog_new_with_buttons ("Add New Variable", NULL, 0, 
    GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, 
    NULL);
  frame = gtk_frame_new ("Variable values");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), frame,
    false, false, 2);

  /*-- make a vb to hold the radio buttons --*/
  vb = gtk_vbox_new (false, 2);
  gtk_container_add (GTK_CONTAINER (frame), vb);

  radio1 = gtk_radio_button_new_with_mnemonic (NULL, "1:_n");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), radio1,
    "Add a variable whose values run from 1 to the number of cases",
    NULL);
  gtk_box_pack_start (GTK_BOX (vb), radio1, false, false, 2);

  radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

  radio2 = gtk_radio_button_new_with_mnemonic (radio_group, "Brushed _groups");
  gtk_widget_set_name (radio2, "radio_brush");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), radio2,
    "Add a variable whose values are based on the groups defined by brushing",
    NULL);
  gtk_box_pack_start (GTK_BOX (vb), radio2, false, false, 2);

  /*-- label and entry --*/
  hb = gtk_hbox_new (false, 2);
  lbl = gtk_label_new_with_mnemonic ("Variable _name: ");
  gtk_box_pack_start (GTK_BOX (hb), lbl,
    true, true, 2);
  entry = gtk_entry_new();
  gtk_label_set_mnemonic_widget(GTK_LABEL(lbl), entry);
  gtk_entry_set_text (GTK_ENTRY (entry), "foo");
  gtk_widget_set_name (entry, "newvar_entry");

  gtk_box_pack_start (GTK_BOX (hb), entry, true, true, 2);

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hb,
    false, false, 2);

	gtk_widget_show_all(GTK_DIALOG(dialog)->vbox);
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		dialog_newvar_add(dialog, gg);
	
  gtk_widget_destroy(dialog);
}


/*-------------------------------------------------------------------------*/
/*                         Rename one variable                             */
/*-------------------------------------------------------------------------*/

static void
dialog_rename_var (GtkWidget *w, GGobiSession *gg) 
{
  GtkWidget *dialog = w;
  GtkWidget *entry;
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gchar *vname;
  gint *selected_vars, nselected_vars = 0;
  gint jvar;

  /*-- find out what variables are selected in the var statistics panel --*/
  nselected_vars = selected_cols_get (&selected_vars, d, gg);
  if (nselected_vars == 0)
    return;

  /*-- retrieve the entry widget and variable name --*/
  entry = widget_find_by_name (GTK_DIALOG(dialog)->vbox, "rename_entry");
  if (entry == NULL || !GTK_IS_ENTRY(entry)) {
    g_printerr ("found the wrong widget; bail out\n");
    return;
  }

  jvar = selected_vars[0];
  vname = gtk_editable_get_chars (GTK_EDITABLE (entry), 0, -1);
  if (vname != NULL && strlen(vname) > 1) {
    ggobi_stage_set_col_name(d, jvar, vname);
  }
}

static void
open_rename_dialog (GtkWidget *w, GGobiSession *gg)
{
  GtkWidget *dialog, *hb, *entry, *lbl;
  GGobiStage *d = datad_get_from_notebook(gg->vartable_ui.notebook);
  gint *selected_vars, nselected_vars = 0;

  /*-- find out what variables are selected in the var statistics panel --*/
  nselected_vars = selected_cols_get (&selected_vars, d, gg);

  if (nselected_vars == 0) {
    gchar *message = g_strdup_printf ("You must select one variable.\n");
    quick_message (message, false);
    g_free (message);
	return;
  }

  dialog = gtk_dialog_new_with_buttons ("Rename One Variable", NULL, 0, 
  			GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
 
  /*-- label and entry --*/
  hb = gtk_hbox_new (false, 2);
  lbl = gtk_label_new ("Variable _name: ");
  gtk_box_pack_start (GTK_BOX (hb), lbl,
    true, true, 2);
  entry = gtk_entry_new();
  gtk_label_set_mnemonic_widget(GTK_LABEL(lbl), entry);
  /*-- label it with the name of the variable being renamed --*/
  gtk_entry_set_text (GTK_ENTRY (entry), ggobi_stage_get_col_name(d, selected_vars[0]));
  gtk_widget_set_name (entry, "rename_entry");

  gtk_box_pack_start (GTK_BOX (hb), entry, true, true, 2);

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hb,
    false, false, 2);
	
  gtk_widget_show_all(GTK_DIALOG(dialog)->vbox);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	  dialog_rename_var(dialog, gg);

  gtk_widget_destroy(dialog);
}

/*-------------------------------------------------------------------------*/

void select_all_cb (GtkWidget *w, GGobiSession *gg)
{
  GtkWidget *tree_view = vartable_tree_view_get (gg);
  GtkTreeSelection *tree_sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  gtk_tree_selection_select_all(tree_sel);
}
void deselect_all_cb (GtkWidget *w, GGobiSession *gg)
{
  GtkWidget *tree_view = vartable_tree_view_get (gg);
  GtkTreeSelection *tree_sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  gtk_tree_selection_unselect_all(tree_sel);
}

GtkWidget *
vartable_buttonbox_build (GGobiSession *gg) {
  GtkWidget *hbox, *hb, *btn;

  /*-- hbox for the buttons along the bottom --*/
  hbox = gtk_hbox_new (false, 12);

  /*-- Make and clear selections --*/
  hb = gtk_hbox_new (false, 2);

  btn = gtk_button_new_with_mnemonic ("_Select all");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Select all variables", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (select_all_cb), gg);

  btn = gtk_button_new_with_mnemonic ("Clear s_election");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Deselect all variables", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (deselect_all_cb), gg);

  gtk_box_pack_start (GTK_BOX (hbox), hb, true, false, 1);
  /*-- --*/

  /*-- Set and apply limits --*/
  /*hb = gtk_hbox_new (false, 2);*/

  /*-- set and clear variable ranges --*/
  /*btn = gtk_button_new_with_mnemonic ("_Limits ... ");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Set user min and max for the selected variable(s), and define rescaling behavior", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, true, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (open_range_set_dialog), gg);*/

  /*-- rescale after resetting variable ranges --*/
  /*btn = gtk_button_new_with_mnemonic ("Resc_ale");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Rescale plots using specified limits and scaling behavior", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (rescale_cb), gg);

  gtk_box_pack_start (GTK_BOX (hbox), hb, true, false, 1);*/
  /*--  --*/

  /*-- Clone, new, delete ... --*/
  hb = gtk_hbox_new (false, 2);
  /*-- Clone or delete selected variables --*/

  btn = gtk_button_new_with_mnemonic ("Cl_one");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Clone selected variables", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (clone_vars_cb), gg);

  /*-- New variable: index, derived from brushing, ... --*/
  btn = gtk_button_new_with_mnemonic ("_New");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Add a new variable", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (open_newvar_dialog), gg);
  /*-- --*/

/*
 * not yet implemented
  btn = gtk_button_new_with_label ("Delete");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Delete selected variables", NULL);
  gtk_box_pack_start (GTK_BOX (hb), btn, false, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (delete_vars_cb), gg);
  gtk_widget_set_sensitive (btn, false);
*/

  gtk_box_pack_start (GTK_BOX (hbox), hb, true, false, 1);
  /*-- --*/

  /*-- Rename one variable ... --*/
  btn = gtk_button_new_with_mnemonic ("Rena_me ...");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Rename one variable -- one variable must be selected", NULL);
  gtk_box_pack_start (GTK_BOX (hbox), btn, true, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (open_rename_dialog), gg);
  /*-- --*/

  btn = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Close the window", NULL);
  gtk_box_pack_start (GTK_BOX (hbox), btn, true, false, 1);
  g_signal_connect (G_OBJECT (btn), "clicked",
                      G_CALLBACK (close_btn_cb), gg);

  return hbox;
}
