#ifndef GGOBI_H
#define GGOBI_H

#include "defines.h"
#include "types.h"
#include "display.h"
#include "display_tree.h"

#include "varseldata.h"

/*
 These are hooks for other applications (e.g. R) to 
 facilitate callbacks at a higher level that GTK events/signals.
 This one is used for responding to identifying points.
 */
typedef void (*IdentifyProc)(void *user_data, gint id, splotd *sp,
  GtkWidget *w, ggobid *gg);
typedef struct {
  IdentifyProc handler;
  void *user_data;
} IdentifyHandler;

struct _ggobid;

typedef struct /*-- ggobi --*/ {

 struct _ggobid *thisGG;

 /*-- used in identification, line editing, and point motion --*/
 gint nearest_point, nearest_point_prev; 

 GtkWidget *scatmat_mode_menu;
 GtkWidget *scatterplot_mode_menu;
 GtkWidget *rotation_io_menu;
 GtkAccelGroup *sp_accel_group;

} GGobiApp;

struct _ggobid {

 DisplayTree display_tree;
 GList *displays;
 displayd *current_display;
 splotd *current_splot; 

 GGobiApp app;

 GSList *d;  /*-- first is default: cases, nodes; second might be edges --*/

 /* main_ui */
 GtkWidget *control_panel[NMODES];
 GtkWidget *main_window, *main_menubar;
 GtkAccelGroup *main_accel_group;
 GtkWidget *mode_frame, *mode_item;
 GtkTooltips *tips;
 gboolean firsttime;  

 gboolean close_pending;
 IdentifyHandler identify_handler;

/*--------------------------------------------------------------------*/
/*                      reading in the data                           */
/*--------------------------------------------------------------------*/

 DataMode data_mode;
 gchar *data_in;
 gchar *filename;      /* full file name, including path */
 gchar *fname;         /* file name without suffix: .dat, .missing */

/*----------------------- pipeline ---------------------------------*/

 gint mode, prev_mode;
 gint projection, prev_projection;

 GtkWidget *vardata_window;

/*----------------------- missing values ---------------------------*/

/*---------------- deleting the hidden points; subsetting ----------*/

/*--------------- clusters: hiding, excluding ----------------------*/
 
/*----------------------- row grouping -----------------------------*/

/*----------------- segments in scatterplots -----------------------------*/

 gint nedges;
 endpointsd *edge_endpoints;

 /*-- line brushing --*/
 struct _LineData {
   gint *nxed_by_brush;
   vector_b xed_by_brush;
   vector_s color, color_now, color_prev;
   vector_b hidden, hidden_now, hidden_prev;
 } line;

 /*-- line groups --*/
 gint nlgroups;
 gint *lgroup_ids;
 rgroupd *lgroups;  /* id, nels, *els */


/*--------------------------------------------------------------------*/
/*                         color                                      */
/*--------------------------------------------------------------------*/

 gint ncolors;
 GdkColor *default_color_table, *color_table;  /* brushing colors */
 GdkColor bg_color;          /* background color */
 GdkColor accent_color;      /* color for axes and labels */
 gshort color_id, color_0;   /* 0:ncolors-1 */
 gboolean mono_p;

 struct _Color_UI {
   GtkWidget *symbol_window;
   GtkWidget *symbol_display;

   GtkWidget *colorseldlg;
   GtkWidget *bg_da, *accent_da, *fg_da[NCOLORS], *current_da;

   gint spacing;
   gint margin;  /* between glyphs in the symbol_display */
 } color_ui;

/*---------------------- graphics contexts -----------------------------*/

 GdkGC *plot_GC;
 GdkGC *selvarfg_GC, *selvarbg_GC;     /* white background, thick lines */
 GdkGC *unselvarfg_GC, *unselvarbg_GC; /* grey background, thin lines */

/*--------------------------- jittering --------------------------------*/

/*------------------------- writing out data ---------------------------*/

 struct _Save {
   gint format, stage, row_ind, column_ind, missing_ind;
   gboolean jitter_p, lines_p;
 } save;

/*---------------------- 1d plotting -----------------------------------*/

 struct _Ash {
   GtkWidget *type_opt;
   GtkObject *smoothness_adj;
 } ash;
 struct _P1D {
   GtkObject *cycle_speed_adj;
   gboolean cycle_p;
   /*-- texture --*/
   gfloat *gy;
 } p1d;

/*-------------------- 2d plotting -----------------------------------*/

 struct _XYPlot {
   gboolean cycle_p;
   gint direction;
 } xyplot;

/*---------------------- touring -------------------------------------*/

 struct _Tour2d {
   GtkWidget *io_menu;
   gint idled; 
 } tour2d;

/*-------------------- parallel coordinates --------------------------*/

 struct _Parcoords {
   GtkAccelGroup *pc_accel_group;
   GtkWidget *arrangement_box;
   GtkWidget *mode_menu;
 } parcoords;

/*------------------------ brushing ----------------------------------*/

 glyphv glyph_id, glyph_0;

 struct _Brush_UI {
   GtkWidget *reset_menu;
   GtkWidget *link_menu;
   GtkWidget *mode_opt;
   GtkWidget *cg_opt;
   GtkWidget *scope_opt;
   GtkWidget *brush_on_btn;
   brush_coords brush_pos ;  

   gboolean firsttime;
 } brush;

 struct _Plot {
  /*
   * Part of binning: the corners of the bin to be copied from
   * pixmap0 to pixmap1.
   * They're defined in splot_draw_to_pixmap0_binned and used in
   * splot_pixmap0_to_pixmap1 when binned == true.
  */
  icoords bin0, bin1;
  icoords loc0, loc1;
 } plot;

/*---------------------- identification ------------------------------*/

 struct _Identify {
/*
   GSList *sticky_ids;
*/
   GtkWidget *link_menu;
 } identify;

/*--------------------- submenu management ---------------------------*/

 struct _Mode_SubMenu {
   GtkWidget *reset_item;
   GtkWidget *link_item;
   GtkWidget *io_item;
 
   gboolean firsttime_reset;
   gboolean firsttime_link;
   gboolean firsttime_io;
  
 } mode_menu;

/*-------------------- transformation --------------------------------*/

/*
 gint std_type;
*/

 struct _Transformation {
   GtkWidget *stage0_opt, *stage1_opt, *stage2_opt;
   GtkAdjustment *boxcox_adj;
 } tform;

 struct _Sphere {
   GtkWidget *window;
   GtkAdjustment *npcs_adj;
   GtkWidget *totvar_entry, *condnum_entry;
   GtkWidget *sphere_apply_btn;
 } sphere_ui;

/*-------------------- subsetting ------------------------------------*/

 struct _SubsetUI {
   gboolean rescale_p;
   GtkWidget *window;
   GtkWidget *notebook;
   /*-- entry widgets from which to get values for sample, rowlab --*/
   GtkWidget *random_entry, *nrows_entry, *rowlab_entry;
   /*-- spinners --*/
   GtkWidget *bstart, *bsize;
   GtkWidget *bstart_incr, *bsize_incr;
   GtkWidget *estart, *estep;
 } subset_ui;

/*-------------------- scaling ---------------------------------------*/

 struct _Scale {
   GtkWidget *scale_reset_menu;
   /*-- widgets whose sensitivity needs to be turned on and off --*/
   GtkWidget *pan_opt, *zoom_opt, *pan_radio, *zoom_radio;
   rectd click_rect;
 } scale;

/*-------------------- imputation ------------------------------------*/

 struct _Impute {
   gboolean rescale_p, vgroup_p;
   gint whichvars;
   GtkWidget *window;
   GtkWidget *notebook;
   /*-- the entry widgets from which to get values to impute --*/
   GtkWidget *entry_above, *entry_below, *entry_val;
 } impute;

/*-------------------- moving points ---------------------------------*/

 struct _MovePts {
   gboolean cluster_p;
   enum directiond direction;
   lcoords eps;
/*
   GSList *history;
*/
 } movepts;


/*----------------- variable selection panel -------------------------*/

 struct _Varpanel_ui {
   GtkWidget *varpanel;
   GtkWidget *scrolled_window;
   GtkAccelGroup *varpanel_accel_group;
 } varpanel_ui;

/*----------------- variable selection menus -------------------------*/

 struct {
  varseldatad vdata0, vdata1;
 } p1d_menu;

 struct {
  varseldatad vdata0, vdata1;
 } parcoords_menu;

 struct {
  varseldatad vdata0, vdata1;
 } xyplot_menu;

 struct {
  varseldatad vdata0, vdata1, vdata2;
 } rotation_menu;

 struct {
  varseldatad vdata0, vdata1, vdata2;
 } tour2d_menu;

 struct {
  varseldatad vdata0, vdata1, vdata2, vdata3;
 } scatmat_menu;

/*--------------------------------------------------------------------*/

}; /*  ggobid; */

#define GGOBI_H

#endif

