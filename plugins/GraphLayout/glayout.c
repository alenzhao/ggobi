#include <gtk/gtk.h>
#include "ggobi.h"
#include "externs.h"
#include "GGobiAPI.h"

#include <stdio.h>

#include "plugin.h"
#include "glayout.h"

#include "GGStructSizes.c"

void       close_glayout_window(GtkWidget *w, PluginInstance *inst);
GtkWidget *create_glayout_window(ggobid *gg, PluginInstance *inst);
void       show_glayout_window (GtkWidget *widget, PluginInstance *inst);

void       neato_dim_cb (GtkAdjustment *adj, PluginInstance *inst);

gboolean
addToToolsMenu(ggobid *gg, GGobiPluginInfo *plugin, PluginInstance *inst)
{
  GtkWidget *entry;
  const gchar *lbl = "Graph layout ...";

  inst->data = NULL;
  inst->info = plugin;
  inst->gg = gg;

  entry = GGobi_addToolsMenuItem ((gchar *)lbl, gg);
  gtk_signal_connect (GTK_OBJECT(entry), "activate",
                      GTK_SIGNAL_FUNC (show_glayout_window), inst);
  return(true);
}


void
show_glayout_window (GtkWidget *widget, PluginInstance *inst)
{
  if (g_slist_length(inst->gg->d) < 1) {
    g_printerr ("No datasets to show\n");
    return;
  }

  if (inst->data == NULL) {
    GtkWidget *window;
    glayoutd *gl = (glayoutd *) g_malloc (sizeof (glayoutd));

    glayout_init (gl);
    inst->data = gl;

    window = create_glayout_window (inst->gg, inst);
    gtk_object_set_data (GTK_OBJECT (window), "glayoutd", gl);

#ifdef HIGHLIGHTSTICKY
/*-- Can't do this here until I have an agnostic highlight function --*/
void highlight_edges_cb (GtkButton *button, PluginInstance *inst);
  gtk_signal_connect (GTK_OBJECT(inst->gg),
    "sticky_point_added", highlight_sticky_edges, inst);
  gtk_signal_connect (GTK_OBJECT(inst->gg),
    "sticky_point_removed", highlight_sticky_edges, inst);
#endif


  } else {
    gtk_widget_show_now ((GtkWidget*) inst->data);
  }
}

glayoutd *
glayoutFromInst (PluginInstance *inst)
{
  glayoutd *gl = (glayoutd *) inst->data;
  return gl;
}

void
scale_array_max (array_d *dist, gint nr, gint nc)
{
  /*extern gdouble delta;*/  /* in mds.c */
  gdouble max;
  gint i, j;
  gdouble **d = dist->vals;

  if (dist->nrows < nr || dist->ncols < nc)
    g_printerr ("This array is smaller than nr or nc\n");
  else {

    max = 0.0;
    for (j=0; j<nc; j++) {
      for (i=0; i<nr; i++) {
        if (d[i][j] != DBL_MAX) {
          if (d[i][j] > max) max = d[i][j];
          if (d[i][j] < 0.0) 
            g_printerr ("Negative value %e in dist array at i=%d, j=%d\n",
              d[i][j], i, j);
        }
      }
    }

    if (max < 1E-10) 
      printf("Range of dist array too small: max=%e\n", max);

    for (j=0; j<nc; j++) {
      for (i=0; i<nr; i++) {
        if(d[i][j] != DBL_MAX)
          d[i][j] /= max;
      }
    }
  }
}

static void
glayout_datad_set_cb (GtkWidget *cl, gint row, gint column,
  GdkEventButton *event, PluginInstance *inst)
{
  ggobid *gg = inst->gg;
  glayoutd *gl = glayoutFromInst (inst);
  gchar *dname;
  datad *d;
  GSList *l;
  gchar *clname = gtk_widget_get_name (GTK_WIDGET(cl));

  gtk_clist_get_text (GTK_CLIST (cl), row, 0, &dname);
  for (l = gg->d; l; l = l->next) {
    d = l->data;
    if (strcmp (d->name, dname) == 0) {
      if (strcmp (clname, "nodeset") == 0) {
        gl->dsrc = d;
      } else if (strcmp (clname, "edgeset") == 0) {
        gl->e = d;
      }
      break;
    }
  }
  /* Don't free either string; they're just pointers */
}
static void 
glayout_clist_datad_added_cb (ggobid *gg, datad *d, void *clist)
{
  gchar *row[1];
  GtkWidget *swin = (GtkWidget *)
    gtk_object_get_data (GTK_OBJECT (clist), "datad_swin");
  gchar *clname = gtk_widget_get_name (GTK_WIDGET(clist));

  if (strcmp (clname, "nodeset") == 0 && d->rowid.idv.nels > 0) {
    row[0] = g_strdup (d->name);
    gtk_clist_append (GTK_CLIST (GTK_OBJECT(clist)), row);
    g_free (row[0]);
  }
  if (strcmp (clname, "edgeset") == 0 && d->edge.n > 0) {
    row[0] = g_strdup (d->name);
    gtk_clist_append (GTK_CLIST (GTK_OBJECT(clist)), row);
    g_free (row[0]);
  }

  gtk_widget_show_all (swin);
}

GtkWidget *
create_glayout_window(ggobid *gg, PluginInstance *inst)
{
  GtkWidget *window, *main_vbox, *notebook, *label, *frame, *vbox, *btn;
  GtkWidget *hb, *entry, *hscale;
  GtkTooltips *tips = gtk_tooltips_new ();
  /*-- for lists of datads --*/
  gchar *clist_titles[2] = {"node sets", "edge sets"};
  datad *d;
  GtkWidget *hbox, *swin, *clist;
  gchar *row[1];
  GSList *l;
  glayoutd *gl = glayoutFromInst (inst);
  GtkObject *adj;

  /*-- I will probably have to get hold of this window, after which
       I can name all the other widgets --*/
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gl->window = window;

  gtk_window_set_title(GTK_WINDOW(window), "glayout");
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
                      GTK_SIGNAL_FUNC (close_glayout_window), inst);

  main_vbox = gtk_vbox_new (FALSE,1);
  gtk_container_set_border_width (GTK_CONTAINER(main_vbox), 5); 
  gtk_container_add (GTK_CONTAINER(window), main_vbox);

  notebook = gtk_notebook_new ();
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook),
    GTK_POS_TOP);
  gtk_box_pack_start (GTK_BOX (main_vbox), notebook, false, false, 2);

/*-- "Specify datasets" list widgets --*/
/*-- this is exactly the same code that appears in ggvis.c --*/

  hbox = gtk_hbox_new (true, 10);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);

/*
 * node sets
*/
  /* Create a scrolled window to pack the CList widget into */
  swin = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swin),
    GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  clist = gtk_clist_new_with_titles (1, &clist_titles[0]);
  gtk_widget_set_name (GTK_WIDGET(clist), "nodeset");
  gtk_clist_set_selection_mode (GTK_CLIST (clist),
    GTK_SELECTION_SINGLE);
  gtk_object_set_data (GTK_OBJECT (clist), "datad_swin", swin);
  gtk_signal_connect (GTK_OBJECT (clist), "select_row",
    (GtkSignalFunc) glayout_datad_set_cb, inst);
  gtk_signal_connect (GTK_OBJECT (gg), "datad_added",
    (GtkSignalFunc) glayout_clist_datad_added_cb, GTK_OBJECT (clist));
  /*-- --*/

  for (l = gg->d; l; l = l->next) {
    d = (datad *) l->data;
    if (d->rowid.idv.nels != 0) {  /*-- node sets --*/
      row[0] = g_strdup (d->name);
      gtk_clist_append (GTK_CLIST (clist), row);
      g_free (row[0]);
    }
  }
  gtk_clist_select_row (GTK_CLIST(clist), 0, 0);
  gtk_container_add (GTK_CONTAINER (swin), clist);
  gtk_box_pack_start (GTK_BOX (hbox), swin, true, true, 2);

/*
 * edge sets
*/
  /* Create a scrolled window to pack the CList widget into */
  swin = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swin),
    GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  clist = gtk_clist_new_with_titles (1, &clist_titles[1]);
  gtk_widget_set_name (GTK_WIDGET(clist), "edgeset");
  gtk_clist_set_selection_mode (GTK_CLIST (clist),
    GTK_SELECTION_SINGLE);
  gtk_object_set_data (GTK_OBJECT (clist), "datad_swin", swin);
  gtk_signal_connect (GTK_OBJECT (clist), "select_row",
    (GtkSignalFunc) glayout_datad_set_cb, inst);
  gtk_signal_connect (GTK_OBJECT (gg), "datad_added",
    (GtkSignalFunc) glayout_clist_datad_added_cb, GTK_OBJECT (clist));
  /*-- --*/

  for (l = gg->d; l; l = l->next) {
    d = (datad *) l->data;
    if (d->edge.n != 0) {  /*-- edge sets --*/
      row[0] = g_strdup (d->name);
      gtk_clist_append (GTK_CLIST (clist), row);
      g_free (row[0]);
    }
  }
  gtk_clist_select_row (GTK_CLIST(clist), 0, 0);
  gtk_container_add (GTK_CONTAINER (swin), clist);
  gtk_box_pack_start (GTK_BOX (hbox), swin, true, true, 2);

  label = gtk_label_new ("Specify datasets");
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), hbox, label);

  /*-- radial tab --*/
  frame = gtk_frame_new ("Radial layout");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);

  vbox = gtk_vbox_new (false, 5);
  gtk_container_set_border_width (GTK_CONTAINER(vbox), 5); 
  gtk_container_add (GTK_CONTAINER(frame), vbox);

  /*-- Label of the center node: passive display --*/
  hb = gtk_hbox_new (false, 2);
  gtk_box_pack_start (GTK_BOX (vbox), hb, false, false, 2);

  gtk_box_pack_start (GTK_BOX (hb), gtk_label_new ("Center node"),
    false, false, 2);
  entry = gtk_entry_new ();
  gtk_entry_set_editable (GTK_ENTRY (entry), false);
  gtk_object_set_data (GTK_OBJECT(window), "CENTERNODE", entry);
  if (gl->dsrc)
    gtk_entry_set_text (GTK_ENTRY (entry),
      (gchar *) g_array_index (gl->dsrc->rowlab, gchar *, 0));
  gtk_signal_connect (GTK_OBJECT(gg),
    "sticky_point_added", radial_center_set_cb, inst);
  gtk_tooltips_set_tip (GTK_TOOLTIPS (tips), entry,
    "To reset the center node, use sticky identification in ggobi", NULL);
  gtk_box_pack_start (GTK_BOX (hb), entry, true, true, 2);
  
  btn = gtk_button_new_with_label ("apply");
  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
                      GTK_SIGNAL_FUNC (radial_cb), inst);
  gtk_box_pack_start (GTK_BOX (vbox), btn, false, false, 3);

  /*-- highlight the edges connected to nodes with sticky labels --*/
/*
  btn = gtk_button_new_with_label ("highlight edges");
  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
                      GTK_SIGNAL_FUNC (highlight_edges_cb), inst);
  gtk_box_pack_start (GTK_BOX (vbox), btn, false, false, 3);
*/

  label = gtk_label_new ("Radial");
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
                            frame, label);
  /*-- --*/

  /*-- graphviz tab: dot and neato --*/
  frame = gtk_frame_new ("Graphviz layouts");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);

  vbox = gtk_vbox_new (false, 5);
  gtk_container_set_border_width (GTK_CONTAINER(vbox), 5); 
  gtk_container_add (GTK_CONTAINER(frame), vbox);

  btn = gtk_button_new_with_label ("dot");
  gtk_widget_set_name (btn, "dot");
#ifdef GRAPHVIZ
  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
                      GTK_SIGNAL_FUNC (dot_neato_layout_cb), (gpointer) inst);
#else
  gtk_widget_set_sensitive (btn, false);
#endif
  gtk_box_pack_start (GTK_BOX (vbox), btn, false, false, 3);


  hbox = gtk_hbox_new (true, 10);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 2);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, true, true, 3);

  btn = gtk_button_new_with_label ("neato");
  gtk_widget_set_name (btn, "neato");
#ifdef GRAPHVIZ
  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
                      GTK_SIGNAL_FUNC (dot_neato_layout_cb), (gpointer) inst);
#else
  gtk_widget_set_sensitive (btn, false);
#endif
  gtk_box_pack_start (GTK_BOX (hbox), btn, false, false, 3);


  /*-- neato scale --*/
  adj = gtk_adjustment_new ((gfloat)gl->neato_dim, 2.0, 11.0, 1.0, 1.0, 1.0);
  gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
    GTK_SIGNAL_FUNC (neato_dim_cb), inst);
  hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj));
  gtk_widget_set_usize (GTK_WIDGET (hscale), 150, 30);

  gtk_range_set_update_policy (GTK_RANGE (hscale), GTK_UPDATE_CONTINUOUS);
  gtk_scale_set_digits (GTK_SCALE(hscale), 0);
  gtk_scale_set_value_pos (GTK_SCALE(hscale), GTK_POS_BOTTOM);
  gtk_scale_set_draw_value (GTK_SCALE(hscale), TRUE);

  gtk_scale_set_digits (GTK_SCALE(hscale), 0);
  gtk_box_pack_start (GTK_BOX (hbox), hscale, false, false, 3);
  /*-- --*/


  label = gtk_label_new ("Graphviz");
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
                            frame, label);

  gtk_widget_show_all (window);

  return(window);
}


void close_glayout_window(GtkWidget *w, PluginInstance *inst)
{
  inst->data = NULL;
}

void closeWindow(ggobid *gg, PluginInstance *inst)
{
  if(inst->data) {
    gtk_signal_disconnect_by_func(GTK_OBJECT(inst->data),
      GTK_SIGNAL_FUNC (close_glayout_window), inst);
    gtk_widget_destroy((GtkWidget*) inst->data);
  }
}

gint
visible_set (glong *visible, datad *d)
{
  gint i, m;
  gint nvisible = 0;

  for (m=0; m<d->nrows_in_plot; m++) {
    i = d->rows_in_plot[m];
    if (!d->hidden.els[i]) {
      visible[nvisible++] = i;
    }
  }

  return nvisible;
}
