#ifndef GGOBI_S_H
#define GGOBI_S_H

#include <stdio.h>
#ifndef DEFINES_H
#include "defines.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* sort +1 */
void       newvar_add (gint vtype, gchar *vname, datad *d, ggobid *gg);
void       missings_datad_cb (GtkWidget *w, ggobid *gg);
gint       symbol_table_populate (datad *d);
void       arrayd_add_cols (array_d *, gint);
void       arrayd_add_rows (array_d *, gint);
void       arrayd_alloc (array_d *, gint, gint);
void       arrayd_alloc_zero (array_d *, gint, gint);
void       arrayd_copy (array_d *, array_d *);
void       arrayd_delete_cols (array_d *, gint, gint *);
void       arrayd_free (array_d *, gint, gint);
void       arrayd_init_null (array_d *);
void       arrayd_zero (array_d *);
void       arrayf_add_cols (array_f *, gint);
void       arrayf_add_rows (array_f *, gint);
void       arrayf_alloc (array_f *, gint, gint);
void       arrayf_alloc_zero (array_f *, gint, gint);
void       arrayf_copy (array_f *, array_f *);
void       arrayf_delete_cols (array_f *, gint, gint *);
void       arrayf_free (array_f *, gint, gint);
void       arrayf_init_null (array_f *);
void       arrayf_zero (array_f *);
void       arrayl_add_cols (array_l *, gint);
void       arrayl_add_rows (array_l *, gint);
void       arrayl_alloc (array_l *, gint, gint);
void       arrayl_alloc_zero (array_l *, gint, gint);
void       arrayl_delete_cols (array_l *, gint, gint *);
void       arrayl_free (array_l *, gint, gint);
void       arrayl_init_null (array_l *);
void       arrayl_zero (array_l *);
gboolean   array_read (datad *, InputDescription *,  ggobid *);
void       arrays_add_cols (array_s *, gint);
void       arrays_add_rows (array_s *, gint);
void       arrays_alloc (array_s *, gint, gint);
void       arrays_alloc_zero (array_s *, gint, gint);
void       arrays_delete_cols (array_s *, gint, gint *);
void       arrays_free (array_s *, gint, gint);
void       arrays_init_null (array_s *);
void       arrays_zero (array_s *);
void       assign_points_to_bins (datad *, ggobid *);
void       br_color_ids_alloc (datad *, ggobid *);
void       br_color_ids_init (datad *, ggobid *);
void       br_edge_color_alloc (ggobid *);
void       br_edge_color_init (datad *d, ggobid *);
gboolean   br_edge_vectors_check_size (gint, datad *, ggobid *);
void       br_glyph_ids_alloc (datad *);
void       br_glyph_ids_init (datad *, ggobid *);
RedrawStyle brush_activate (gboolean, displayd *, ggobid *);
void       brush_draw_brush (splotd *, GdkDrawable *, datad *, ggobid *);
void       brush_draw_label (splotd *, GdkDrawable *, datad *, ggobid *);
void       brush_event_handlers_toggle (splotd *, gboolean);
void       brush_init (datad *, ggobid *);
void       brush_motion (icoords *, gboolean, gboolean, cpaneld *, splotd *, ggobid *);
void       brush_once_and_redraw (gboolean binningp, splotd *sp, displayd *display, ggobid *gg);
gboolean   brush_once (gboolean force, splotd *, ggobid *);
void       brush_options_cb (gpointer, guint, GtkCheckMenuItem *);
void       brush_pos_init (splotd *);
void       brush_prev_vectors_update (datad *, ggobid *);
void       brush_reset(ggobid *gg, gint action);
void       brush_set_pos (gint, gint, splotd *);
void       brush_undo (splotd *, datad *, ggobid *);
gboolean   build_symbol_vectors_by_var (cpaneld *, datad *, ggobid *);
gdouble    calc_norm (gdouble *x, gint n);
void       checkbox_delete_nth (gint jvar, datad *);
gboolean   checkequiv(gdouble **u0, gdouble **u1, gint nc, gint nd);
void       clone_vars (gint *cols, gint ncols, datad *, ggobid *);
void       cluster_free (gint, datad *, ggobid *);
void       clusters_set (datad *, ggobid *);
void       cluster_table_labels_update (datad *d, ggobid *gg);
void       cluster_table_update (datad *, ggobid *);
void       cluster_window_open (ggobid *);
gboolean   collabels_read (InputDescription *desc, gboolean, datad *, ggobid *);
void       collab_tform_update (gint j, datad *d, ggobid *gg);
void       colorscheme_init (colorschemed *scheme);
gboolean   colors_remap (colorschemed *scheme, gboolean force, ggobid *gg);
gchar*     computeTitle (gboolean, displayd *, ggobid *);
void       copy_mat(gdouble **, gdouble **, gint, gint);
void       cpanel_brush_init (cpaneld *, ggobid *);
void       cpanel_brush_make (ggobid *);
void       cpanel_brush_set (cpaneld *, ggobid *);
void       cpanel_ctour_make (ggobid *);
void       cpanel_edgeedit_init (cpaneld *, ggobid *);
void       cpanel_edgeedit_make (ggobid *);
void       cpanel_edgeedit_set (cpaneld *, ggobid *);
void       cpanel_identify_init (cpaneld *, ggobid *);
void       cpanel_identify_make (ggobid *);
void       cpanel_identify_set (cpaneld *, ggobid *);
void       cpanel_movepts_make (ggobid *);
void       cpanel_p1d_init (cpaneld *, ggobid *);
void       cpanel_p1dplot_make (ggobid *);
void       cpanel_p1d_set (cpaneld *, ggobid *);
void       cpanel_parcoords_init (cpaneld *, ggobid *);
void       cpanel_parcoords_make (ggobid *);
void       cpanel_parcoords_set (cpaneld *, ggobid *);
void       cpanel_rotation_init (cpaneld *, ggobid *);
void       cpanel_rotation_make (ggobid *);
void       cpanel_rotation_set (cpaneld *, ggobid *);
void       cpanel_scale_init (cpaneld *, ggobid *);
void       cpanel_scale_make (ggobid *);
void       cpanel_scale_set (cpaneld *, ggobid *);
void       cpanel_scatmat_init (cpaneld *, ggobid *);
void       cpanel_scatmat_make (ggobid *);
void       cpanel_scatmat_set (cpaneld *, ggobid *);
void       cpanel_set (displayd *, ggobid *);
void       cpanel_show_cb (GtkCheckMenuItem *w, guint action);
void       cpanel_t1d_init (cpaneld *, ggobid *);
void       cpanel_t2d_init (cpaneld *, ggobid *);
void       cpanel_tcorr_init (cpaneld *, ggobid *);
void       cpanel_tour1d_make (ggobid *);
void       cpanel_tour1d_set (cpaneld *, ggobid *);
void       cpanel_tour2d_make (ggobid *);
void       cpanel_tour2d_set (cpaneld *, ggobid *);
void       cpanel_tourcorr_set (cpaneld *, ggobid *);
void       cpanel_xyplot_init (cpaneld *, ggobid *);
void       cpanel_xyplot_make (ggobid *);
void       cpanel_xyplot_set (cpaneld *, ggobid *);
displayd * createDisplayFromDescription (ggobid *, GGobiDisplayDescription *desc);
GtkWidget* CreateMenuCheck (GtkWidget *, gchar *, GtkSignalFunc, gpointer, gboolean, ggobid *);
GtkWidget* CreateMenuItem (GtkWidget *, gchar *, gchar *, gchar *, GtkWidget *, GtkAccelGroup *, GtkSignalFunc, gpointer, ggobid *) ;
GtkWidget* create_variable_notebook (GtkWidget *box, GtkSelectionMode mode, GtkSignalFunc func, ggobid *);
void       ctour_event_handlers_toggle (splotd *, gboolean);
void       ctourpp_window_open (ggobid *);
gushort    datad_colors_used_get (gint *ncolors_used, gushort *colors_used, datad *, ggobid *); 
void       datad_free (datad *, ggobid *);
datad*     datad_get_from_notebook (GtkWidget *notebook, ggobid *);
displayd*  datad_init (datad *, ggobid *, gboolean);
datad *    datad_new(datad *d, ggobid *gg);
gchar*     datasetName (datad *d, ggobid *gg);
colorschemed* default_scheme_init ();
gint       delete_vars (gint *, gint, datad *, ggobid *);
void       disconnect_button_press_signal (splotd *sp);
void       disconnect_button_release_signal (splotd *sp);
void       disconnect_key_press_signal (splotd *sp);
void       disconnect_motion_signal (splotd *sp);
gint       display_add(displayd *display, ggobid *);
displayd*  display_alloc_init (enum displaytyped, gboolean, datad *, ggobid *);
void       display_close_cb (displayd *d, guint, GtkWidget *);
displayd*  display_create (gint displaytype, gboolean missing_p, datad *, ggobid *);
void       display_delete_cb (GtkWidget *, GdkEvent *, displayd *);
void       display_free_all (ggobid *);
void       display_free (displayd *, gboolean force, ggobid *);
void       display_menu_build (ggobid *);
void       display_menu_init (ggobid *);
void       display_new (ggobid *, guint action, GtkWidget *widget);
void       display_options_cb (GtkCheckMenuItem *w, guint action);
void       display_plot (displayd *display, RedrawStyle type, ggobid *);
void       display_print_cb (displayd *d, guint, GtkWidget *);
void       display_set_current (displayd *, ggobid *);
void       display_set_position (displayd *d, ggobid *gg);
void       displays_plot (splotd *, RedrawStyle, ggobid *);
void       displays_tailpipe (RedrawStyle, ggobid *);
void       display_t1d_init (displayd *, ggobid *);
void       display_t2d_init (displayd *, ggobid *);
void       display_tailpipe (displayd *, RedrawStyle, ggobid *);
void       display_tour1d_init (displayd *dsp, ggobid *gg);
void       display_tour1d_init_null (displayd *dsp, ggobid *gg);
void       display_tour2d_init (displayd *dsp, ggobid *gg);
void       display_tour2d_init_null (displayd *dsp, ggobid *gg);
void       display_tourcorr_init (displayd *dsp, ggobid *gg);
void       display_tourcorr_init_null (displayd *dsp, ggobid *gg);
gboolean   display_type_handles_action (displayd *, PipelineMode m);
void       display_window_init (displayd *, gint, ggobid *);
void       display_write_svg (ggobid *);
gint       do_ash1d (gfloat *, gint, gint, gint, gfloat *, gfloat *, gfloat *, gfloat *);
void       draw_glyph (GdkDrawable *, glyphd *, icoords *, gint, ggobid *);
gint       dsvd (gdouble **a, gint m, gint n, gfloat *w, gdouble **v);
void       edgeedit_event_handlers_toggle (splotd *, gboolean);
void       edges_alloc (gint, datad *);
void       edges_create (datad *, ggobid *);
void       edgeset_add_cb (GtkWidget *w, datad *e);
gboolean   edgeset_add (displayd *);
void       edges_free (datad *, ggobid *);
gboolean   edges_read (InputDescription *desc, gboolean, datad *, ggobid *);
void       eigenvals_get (gfloat *, datad *);
gint       fcompare (const void *x1, const void *x2);
void       filename_get_r (ggobid *, guint, GtkWidget *);
void       filename_get_w (GtkWidget *, ggobid *);
gboolean   fileset_read (const gchar *, ggobid *);
gboolean   fileset_read_init (const gchar *ldata_in, ggobid *);
void       find_glyph_type_and_size (gint, glyphd *);
gint       find_keepers (gint ncols_current, gint nc, gint *cols, gint *keepers);
gint       find_nearest_point (icoords *, splotd *, datad *, ggobid *);
GtkWidget *get_clist_from_object (GtkObject *);
void       get_extended_brush_corners (icoords *, icoords *, datad *, splotd *);
GtkItemFactory *get_main_menu (GtkItemFactoryEntry[], gint, GtkAccelGroup *, GtkWidget  *, GtkWidget **, gpointer);
gint       get_one_selection_from_clist (GtkWidget *clist);
gint       get_selections_from_clist (gint maxnvars, gint *vars, GtkWidget *clist);
GtkWidget *GGobi_addToolsMenuItem (gchar *label, ggobid *gg);
ggobid*    ggobi_alloc (void);
gboolean   ggobi_file_set_create (gchar *rootname, datad *, ggobid *);
ggobid*    GGobiFromDisplay (displayd *display);
ggobid*    GGobiFromSPlot (splotd *sp);
ggobid*    GGobiFromWidget (GtkWidget *w, gboolean);
ggobid*    GGobiFromWindow (GdkWindow *w);
ggobid*    ggobi_get (gint);
gint       ggobi_getIndex(ggobid *gg);
void       GGobi_widget_set (GtkWidget *, ggobid *gg, gboolean isWindow);
GList*     g_list_remove_nth (GList *, gint);
GList*     g_list_replace_nth (GList *, gpointer, gint);
void       globals_init (ggobid *);
void       gt_basis (array_d, gint, vector_i, gint, gint);
void       hidden_alloc (datad *);
void       hidden_init (datad *, ggobid *);
gboolean   hidden_read (InputDescription *desc, gboolean, datad *, ggobid *);
RedrawStyle identify_activate (gint, displayd *, ggobid *);
void       identify_event_handlers_toggle (splotd *, gboolean);
void       identify_link_by_id (gint k, datad *source_d, ggobid *gg);
gboolean   impute_fixed (gint, gint nvars, gint *vars, datad *, ggobid *);
void       impute_random (datad *, gint nvars, gint *vars, ggobid *);
void       impute_window_open (ggobid *);
void       init_plot_GC (GdkWindow *, ggobid *);
void       init_var_GCs (GtkWidget *, ggobid *);
gboolean   isCrossed (gdouble, gdouble, gdouble, gdouble, gdouble, gdouble, gdouble, gdouble);
gfloat     jitter_randval (gint);
void       jitter_value_set (gfloat, datad *, ggobid *);
void       jitter_vars_init (datad *, ggobid *);
void       jitter_window_open (ggobid *);
void       limits_adjust (gfloat *, gfloat *);
void       limits_set_by_var (gint, gboolean do_raw, gboolean do_tform, datad *, ggobid *);  
void       limits_set (gboolean do_raw, gboolean do_tform, datad *, ggobid *);  
gint       lines_intersect (glong, glong, glong, glong, glong, glong, glong, glong);
void       linking_method_set (displayd *, datad *, ggobid *);
void       make_ggobi (GGobiOptions *, gboolean, ggobid *);
void       make_symbol_window (ggobid *);
void       make_ui (ggobid *);
GlyphType  mapGlyphName (const gchar *gtype);
void       missing_arrays_add_cols (datad *d, ggobid *);
gboolean   missing_values_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   mouseinwindow (splotd *);
void       mousepos_get_motion (GtkWidget *, GdkEventMotion *, gboolean *, gboolean *, splotd *);
void       mousepos_get_pressed (GtkWidget *, GdkEventButton *, gboolean *, gboolean *, splotd *);
void       move_pt (gint id, gint x, gint y, splotd *sp, datad *d, ggobid *);
void       movepts_event_handlers_toggle (splotd *, gboolean);
void       movepts_history_add (gint id, splotd *sp, datad *, ggobid *);
void       movepts_history_delete_last (datad *, ggobid *);
gdouble    myrint (gdouble x);
gint       ndatad_with_vars_get (ggobid *gg);
void       newvar_add (gint vtype, gchar *vname, datad *, ggobid *);
void       newvar_add_with_values (gdouble *, gint, gchar *, datad *, ggobid *);
gint       option_menu_index (GtkOptionMenu *);
RedrawStyle p1d_activate (gint, displayd *, ggobid *);
void       p1d_event_handlers_toggle (splotd *, gboolean);
void       p1d_reproject (splotd *, glong **, datad *, ggobid *);
gboolean   p1d_varsel (splotd *, gint, gint *, gint);
void       pan_by_drag (splotd *, ggobid *);
void       pan_step (splotd *, gint, ggobid *);
void       parcoords_cpanel_init (cpaneld*, ggobid *);
void       parcoords_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *, gboolean);
displayd*  parcoords_new (gboolean, gint, gint *, datad *, ggobid *);
void       parcoords_reset_arrangement (displayd *, gint, ggobid *);
gboolean   parcoords_varsel (cpaneld *, splotd *, gint, gint *, ggobid *);
gboolean   pca_calc (datad *, ggobid *);
void       pca_diagnostics_set (datad *d, ggobid *);
gint       pcompare (const void *, const void *);
void       pipeline_arrays_add_column (gint, datad *, ggobid *);
void       pipeline_arrays_alloc (datad *, ggobid *);
void       pipeline_arrays_free (datad *d, ggobid *);
void       pipeline_init (datad *, ggobid *);
gint       plotted_cols_get (gint *, datad *, ggobid *);
gboolean   point_colors_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   point_glyphs_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   point_in_which_bin (gint, gint, gint *, gint *, datad *, splotd *);
void       populate_option_menu (GtkWidget *, gchar **, gint, GtkSignalFunc, ggobid *);
void       position_popup_menu (GtkMenu *menu, gint *px, gint *py, gpointer);
PipelineMode projection_get (ggobid *);
void       quick_message (gchar *, gboolean);
void       quit_ggobi(ggobid *gg, gint action, GtkWidget *w);
gdouble    randvalue (void);
void       range_unset (ggobid *gg);
void       raw_to_tform_copy (void);
gboolean   read_ascii_data(InputDescription *desc, ggobid *gg);
void       reinit_transient_brushing (displayd *, ggobid *);
void       rejitter (gint *, gint, datad *, ggobid *);
void       rnorm2 (gdouble *, gdouble *);
void       rotation_event_handlers_toggle (splotd *, gboolean);
void       rowids_alloc (datad *);
void       rowids_init_null (datad *d);
void       rowidv_init (datad *d);
void       rowlabels_alloc (datad *d, ggobid *) ;
void       rowlabels_free (datad *d, ggobid *);
gboolean   rowlabels_read (InputDescription *desc, gboolean, datad *, ggobid *);
void       rows_in_plot_set (datad *d, ggobid *);
void       ruler_ranges_set (gboolean force, displayd *, splotd *, ggobid *);
void       scale_click_init (splotd *sp, ggobid *gg);
void       scale_event_handlers_toggle (splotd *, gboolean);
void       scale_set_default_values (GtkScale *scale);
void       scaling_visual_cues_draw (splotd *, GdkDrawable *, ggobid *);
void       scatmat_cpanel_init (cpaneld *, ggobid *);
void       scatmat_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd*  scatmat_new (gboolean, gint, gint *, gint, gint *, datad *, ggobid *);
gboolean   scatmat_varsel (cpaneld *, splotd *, gint, gint *, gint, gboolean, ggobid *);
gboolean   scatmat_varsel_simple (cpaneld *, splotd *, gint, gint *, ggobid *);
void       scatterplot_cpanel_init (cpaneld *, PipelineMode, ggobid *);
void       scatterplot_display_edge_menu_update (displayd *, GtkAccelGroup *accel_group, GtkSignalFunc func, ggobid *gg);
void       scatterplot_display_menus_make (displayd *display, GtkAccelGroup *, GtkSignalFunc, ggobid *);
void       scatterplot_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd*  scatterplot_new (gboolean, splotd *sp, datad *d, ggobid *);
void       scatterplot_show_hrule (displayd *, gboolean show);
void       scatterplot_show_rulers (displayd *, gint);
void       scatterplot_show_vrule (displayd *, gboolean show);
gboolean   scree_mapped_p (ggobid *);
void       scree_plot_make (ggobid *);
gint       selected_cols_get (gint *, datad *d, ggobid *);
void       smooth_window_open (ggobid *);
void       sp_event_handlers_toggle (splotd *, gboolean);
void       sphere_condnum_set (gfloat x, ggobid *);
void       sphere_enable (gboolean sens, ggobid *);
void       sphere_free (datad *);
void       sphere_init (datad *);
gint       sphere_npcs_get (datad *, ggobid *);
void       sphere_npcs_range_set (gint n, ggobid *gg);
void       sphere_npcs_set (gint, datad *, ggobid *);
void       sphere_panel_open (ggobid *);
void       sphere_varcovar_set (datad *, ggobid *);
void       sphere_variance_set (gfloat x, datad *, ggobid*);
void       spherevars_set (ggobid *);
void       spherize_data (vector_i *svars, vector_i *pcvars, datad *, ggobid *);
gboolean   spherize_set_pcvars (datad *, ggobid *);
void       splash_show (ggobid *gg, guint action, GtkWidget *w);
void       splot_cursor_set (gint jcursor, splotd *sp);
void       splot_dimension_set(splotd* sp, gint width, gint height);
void       splot_edges_realloc (splotd *, datad *, ggobid *);
gboolean   splot_event_handled (GtkWidget *, GdkEventKey *, cpaneld *, splotd *, ggobid *);
void       splot_expose (splotd *);
void       splot_free (splotd *, displayd *, ggobid *);
void       splot_get_dimensions (splotd *, gint *, gint *);
splotd*    splot_new (displayd *, gint, gint, ggobid *);
void       splot_pixmap0_to_pixmap1 (splotd *, gboolean, ggobid *);
void       splot_pixmap1_to_window (splotd *, ggobid *);
void       splot_plane_to_screen (displayd *, cpaneld *, splotd *, ggobid *);
void       splot_plane_to_world (splotd *, gint, ggobid *);
gboolean   splot_plot_case (gint m, datad *, splotd *, displayd *, ggobid *);
void       splot_redraw (splotd *sp, RedrawStyle, ggobid *);
void       splot_reverse_pipeline (splotd *, gint, lcoords *, gboolean, gboolean, ggobid *);
void       splot_screen_to_tform (cpaneld *, splotd *, icoords *, fcoords *, ggobid *);
void       splot_set_current (splotd *, gboolean, ggobid *);
void       splot_set_plot_center (splotd *);
void       splot_world_to_plane (cpaneld *, splotd *, ggobid *);
void       splot_zoom (splotd *sp, gfloat xsc, gfloat ysc, ggobid *) ;
void       sp_whiskers_make (splotd *, displayd *, ggobid *);
gint       sqdist (gint, gint, gint, gint);
void       sticky_id_toggle (datad *, ggobid *);
void       strip_suffixes (ggobid *);
gint       strToInteger (const gchar *tmp);
void       submenu_append (GtkWidget *, GtkWidget *);
void       submenu_destroy (GtkWidget *);
void       submenu_insert (GtkWidget *, GtkWidget *, gint);
GtkWidget* submenu_make (gchar *, guint, GtkAccelGroup *);
void       subset_apply (datad *, ggobid *);
gboolean   subset_block (gint, gint, datad *, ggobid *);
gboolean   subset_everyn (gint, gint, datad *, ggobid *);
void       subset_include_all (datad *, ggobid *);
gboolean   subset_random (gint, datad *, ggobid *);
gboolean   subset_rowlab (gchar *, datad *, ggobid *);
gboolean   subset_sticky (datad *, ggobid *);
void       subset_window_open (ggobid *, guint, GtkWidget *);
void       symbol_link_by_id (gint k, datad *source_d, ggobid *gg);
void       symbol_window_redraw (ggobid *);
gboolean   t2d_switch_index(gint indxtype, gint basismeth, ggobid *gg);
void       textur (gfloat *, gfloat *, gint, gint, gfloat, gint, ggobid *);
void       tform1_to_tform2_copy (void);
void       tform_label_update (gint, datad *, ggobid *);
void       tform_to_world_by_var (gint j, datad *, ggobid *);
void       tform_to_world (datad *, ggobid *);
void       tooltips_show_cb (GtkCheckMenuItem *w, guint action);
void       tour1d_event_handlers_toggle (splotd *, gboolean);
void       tour1d_fade_vars_cb (GtkCheckMenuItem *w, guint action);
void       tour1d_func (gboolean, displayd *, ggobid *);
void       tour1d_io_cb (GtkWidget *w, gpointer *cbd);
void       tour1dpp_window_open (ggobid *);
void       tour1d_projdata (splotd *, glong **, datad *, ggobid *);
void       tour1d_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void       tour1d_realloc_up (gint nc, datad *d, ggobid *gg);
void       tour1d_varsel (gint jvar, gint btn, datad *, ggobid *);
void       tour1d_vert (cpaneld *, gboolean);
void       tour2d_event_handlers_toggle (splotd *, gboolean);
void       tour2d_fade_vars_cb (GtkCheckMenuItem *w, guint action);
void       tour2d_func (gboolean, displayd *, ggobid *);
void       tour2d_io_cb (GtkWidget *w, gpointer *cbd);
void       tour2dpp_window_open (ggobid *);
void       tour2d_projdata (splotd *, glong **, datad *, ggobid *);
void       tour2d_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void       tour2d_realloc_up (gint nc, datad *d, ggobid *gg);
void       tour2d_varsel (gint jvar, gint btn, datad *, ggobid *);
void       tourcorr_fade_vars_cb (GtkCheckMenuItem *w, guint action);
void       tourcorr_func (gboolean, displayd *, ggobid *);
void       tourcorr_io_cb (GtkWidget *w, gpointer *cbd);
void       tourcorr_projdata (splotd *, glong **, datad *, ggobid *);
void       tourcorr_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void       tourcorr_realloc_up (gint nc, datad *d, ggobid *gg);
void       tourcorr_varsel (gint jvar, gint btn, datad *, ggobid *);
void       tour_do_step (displayd *, ggobid *);
void       tour_manip_colors_init (ggobid *);
void       transform0_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void       transform0_values_set (gint, gint, datad *, ggobid *);
gboolean   transform1_apply (gint, datad *, ggobid *);
void       transform1_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void       transform1_values_set (gint, gfloat, gint, datad *, ggobid *);
gboolean   transform2_apply (gint, datad *, ggobid *);
void       transform2_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void       transform2_values_set (gint, gint, datad *, ggobid *);
void       transform (gint, gint, gfloat, gint *, gint, datad *, ggobid *);
void       transform_values_init (gint j, datad *, ggobid *);
void       transform_variable (gint, gint, gfloat, gint, datad *, ggobid *);
void       transform_window_open (ggobid *);
datad*     ValidateDatadRef (datad *d, ggobid *gg, gboolean fatal);
displayd*  ValidateDisplayRef(displayd *d, ggobid *gg, gboolean fatal);
ggobid*    ValidateGGobiRef (ggobid *gg, gboolean fatal);
void       varcircle_label_set (gint, datad *);
void       varcircle_label_set (gint jvar, datad *d);
void       varcircles_add (gint ncols, datad *, ggobid *);
void       varcircles_cursor_set_default (datad *d);
void       varcircles_delete_nth (gint j, datad *);
void       varcircles_layout_cb (GtkCheckMenuItem *w, guint action);
void       varcircles_layout_reset (gint ncols, datad *, ggobid *);
void       varcircles_populate (datad *, ggobid *);
void       varcircles_refresh (datad *, ggobid *);
void       vardialog_open (ggobid *, gchar *title);
void       variable_clone (gint, const gchar *, gboolean, datad *, ggobid *);
void       variable_notebook_varchange_cb (GtkObject *obj, vartabled *vt, ggobid *gg, GtkWidget *notebook);
void       variable_notebook_subwindow_add (datad *d, GtkSignalFunc func, GtkWidget *notebook, ggobid *gg); gint alloc_optimize0_p (optimize0_param *op, gint nrows, gint ncols, gint ndim);
void       varlabel_set (gint, datad *);
void       varpanel_checkboxes_add (gint, datad *d, ggobid *gg);
void       varpanel_checkboxes_populate (datad *, ggobid *);
void       varpanel_clear (datad *, ggobid *);
void       varpanel_make (GtkWidget *, ggobid *);
void       varpanel_refresh (ggobid *);
void       varpanel_reinit (ggobid *gg);
void       varpanel_show_page (displayd*, ggobid*);
void       varpanel_tooltips_set (ggobid *);
void       varsel (cpaneld *, splotd *, gint jvar, gint btn, gint alt_mod, gint ctrl_mod, gint shift_mod, datad *, ggobid *);
void       vars_stdized_send_event (datad *d, ggobid *gg);
void       vartable_alloc (datad *);
void       vartable_cells_set_by_var (gint j, datad *d);
void       vartable_collab_set_by_var (gint, datad *);
void       vartable_collab_tform_set_by_var (gint j, datad *d);
vartabled *vartable_element_get_by_name (gchar *collab, datad *d);
vartabled *vartable_element_get (gint j, datad *d);
void       vartable_element_remove (gint, datad *);
gint       vartable_index_get_by_name (gchar *collab, datad *d);
void       vartable_init (datad *d);
void       vartable_limits_set_by_var (gint j, datad *d);
void       vartable_limits_set (datad *);
void       vartable_open (ggobid *);
void       vartable_realloc (gint, datad *);
void       vartable_row_append (datad *, ggobid *);
void       vartable_select_var (gint, gboolean, datad *, ggobid *);
void       vartable_show_page (displayd*, ggobid*);
void       vartable_stats_set_by_var (gint j, datad *);
void       vartable_stats_set (datad *);
void       vectorb_alloc (vector_b *, gint);
void       vectorb_copy (vector_b *, vector_b *);
void       vectorb_free (vector_b *);
void       vectorb_init_null (vector_b *);
void       vectorb_realloc (vector_b *, gint);
void       vectorb_realloc_zero (vector_b *, gint);
void       vectorb_zero (vector_b *vecp);
void       vectorf_alloc (vector_f *, gint);
void       vectorf_alloc_zero (vector_f *, gint);
void       vectorf_delete_els (vector_f *vecp, gint nels, gint *els);
void       vectorf_free (vector_f *);
void       vectorf_init_null (vector_f *);
void       vectorf_realloc (vector_f *, gint);
void       vectorf_zero (vector_f *vecp);
void       vectorg_alloc (vector_g *, gint);
void       vectorg_copy (vector_g *, vector_g *);
void       vectorg_free (vector_g *);
void       vectorg_init_null (vector_g *);
void       vectorg_realloc (vector_g *, gint);
void       vectorg_realloc_zero (vector_g *, gint);
void       vectori_alloc (vector_i *, gint);
void       vectori_alloc_zero (vector_i *, gint);
void       vectori_copy (vector_i *, vector_i *);
void       vectori_delete_els (vector_i *vecp, gint nels, gint *els);
void       vectori_free (vector_i *);
void       vectori_init_null (vector_i *);
void       vectori_realloc (vector_i *, gint);
void       vectors_copy (vector_s *, vector_s *);
void       vectors_free (vector_s *);
void       vectors_init_null (vector_s *);
void       vectors_realloc (vector_s *, gint);
void       vectors_realloc_zero (vector_s *, gint);
RedrawStyle  viewmode_activate (splotd *, PipelineMode, gboolean, ggobid *);
PipelineMode viewmode_get (ggobid *);
void       viewmode_set_cb (GtkWidget  *, gint);
void       viewmode_set (PipelineMode, ggobid *);
void       viewmode_submenus_update (PipelineMode prev_mode, ggobid *);
GtkWidget* widget_find_by_name (GtkWidget *, gchar *);
gboolean   widget_initialized (GtkWidget *w);
void       widget_initialize (GtkWidget *w, gboolean initd);
void       world_to_raw (gint, splotd *, datad *, ggobid *);
void       writeall_window_open (ggobid *);
void       wvis_init (ggobid *gg);
void       wvis_window_open (ggobid *gg);
RedrawStyle xyplot_activate (gint, displayd *, ggobid *);
void       xyplot_cycle_activate (gboolean state, cpaneld *cpanel, ggobid *gg);
void       xyplot_event_handlers_toggle (splotd *, gboolean);
gboolean   xyplot_varsel (splotd *, gint, gint *, gint);
void       xy_reproject (splotd *, glong **, datad *, ggobid *);
void       zoom_by_drag (splotd *, ggobid *);
void       zoom_step (splotd *, gint, gint, rectd *, ggobid*);

/*tsplot functions*/
void      cpanel_tsplot_init (cpaneld *, ggobid *);
void      cpanel_tsplot_make (ggobid *);
void      cpanel_tsplot_set (cpaneld *, ggobid *);
void      tsplot_cpanel_init (cpaneld*, ggobid *);
void      tsplot_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd* tsplot_new (gboolean, gint, gint *, datad *, ggobid *);
void      tsplot_reset_arrangement (displayd *, gint, ggobid *);
gboolean  tsplot_varsel (cpaneld *, splotd *, gint, gint, gint *, ggobid *);
void      tsplot_whiskers_make (splotd *, displayd *, ggobid *);

/* The new way of handling window closures, so that we don't just exit. */
gboolean  ggobi_close (GtkObject *w, GdkEvent *ev, ggobid *gg);
gint      ggobi_remove_by_index (ggobid *gg, gint which);
gint      ggobi_remove (ggobid *);
void      subset_init (datad *d, ggobid *gg);


#ifdef BARCHART_IMPLEMENTED
gboolean barchart_active_paint_points (splotd *sp, datad *d); 
void barchart_add_bar_cues (splotd *sp, GdkDrawable *drawable, ggobid *gg);
void barchart_clean_init (splotd *sp);
void barchart_cpanel_init (cpaneld *, ggobid *);
void barchart_event_handlers_toggle (splotd *, gboolean state);
void barchart_free_structure (splotd *sp);
gboolean barchart_identify_bars (icoords mousepos, splotd *sp, datad *d, ggobid *gg);
void barchart_init_vectors(splotd *sp);
void barchart_mode_menu_make (GtkAccelGroup *accel_group, GtkSignalFunc func, ggobid *gg, gboolean useIds);
displayd *barchart_new (gboolean missing_p, splotd *sp, datad *d, ggobid *gg);
void barchart_recalc_counts (splotd *sp, datad *d, ggobid *gg);
void barchart_recalc_dimensions (splotd *sp, datad *d, ggobid *gg);
void barchart_recalc_group_dimensions (splotd *sp, ggobid *gg);
void barchart_redraw (splotd *sp, datad *d, ggobid *gg);
void barchart_splot_add_plot_labels (splotd *, GdkDrawable *, ggobid *);
void cpanel_barchart_make (ggobid *gg);
void cpanel_barchart_set (cpaneld *cpanel, ggobid *gg);
#endif

#ifdef WIN32
void win32_draw_to_pixmap_binned (icoords *, icoords *, gint, splotd *, ggobid *gg);
void win32_draw_to_pixmap_unbinned (gint, splotd *, ggobid *gg);
#endif

void showInputDescription(InputDescription *desc, ggobid *gg);
void initSessionOptions(void);
/*gint parse_command_line (gint *argc, gchar **av, ggobid *gg);*/
gint parse_command_line (gint *argc, gchar **av);
#ifdef __cplusplus
}
#endif

#include "GGobiAPI.h"

#define CHECK_GG(a) ValidateGGobiRef(a, true)

#endif

