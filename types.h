/*-- types.h --*/
/*
    This software may only be used by you under license from AT&T Corp.
    ("AT&T").  A copy of AT&T's Source Code Agreement is available at
    AT&T's Internet website having the URL:
    <http://www.research.att.com/areas/stat/ggobi/license.html>
    If you received this software without first entering into a license
    with AT&T, you have an infringing copy of this software and cannot use
    it without violating AT&T's intellectual property rights.
*/
#ifndef TYPES_H
#define TYPES_H

#include <gtk/gtk.h>

typedef enum {ascii_data, binary_data, Sprocess_data, xml_data, mysql_data, url_data, unknown_data, num_data_modes} DataMode;

typedef enum {NONE, EXPOSE, QUICK, BINNED, FULL, FULL_1PIXMAP} RedrawStyle;


typedef struct {
  gint type, size;
} glyphd;

typedef struct {
    glong x, y;
} lcoords;
typedef struct {
    gint x, y;
} icoords;
typedef struct {
    gfloat x, y;
} fcoords;
typedef struct {
    gfloat min, max;
} lims;
typedef struct {
    gint a, b;
} endpointsd;

/*-- arrays --*/
/*-- double: some plugins will want these --*/
typedef struct {
  gdouble **vals;
  guint nrows, ncols;
} array_d;
/*-- floating point: for gg.raw_data, tform1, tform2 --*/
typedef struct {
  gfloat **vals;
  guint nrows, ncols;
} array_f;
/*-- short: for gg.missing --*/
typedef struct {
  gshort **vals;
  guint nrows, ncols;
} array_s;
/*-- long: for gg.world, jitdata --*/
typedef struct {
  glong **vals;
  guint nrows, ncols;
} array_l;

/*-- vectors --*/
typedef struct {
  gfloat *els;
  guint nels;
} vector_f;
typedef struct {
  gint *els;
  guint nels;
} vector_i;
typedef struct {
  gshort *els;
  guint nels;
} vector_s;
typedef struct {
  gboolean *els;
  guint nels;
} vector_b;
typedef struct {
  glyphd *els;
  guint nels;
} vector_g;

typedef struct {  /*-- used for obtaining ranks --*/
  gfloat f;
  gint indx;
} paird;


/*-- used to keep track of history in moving points --*/
typedef struct {
  gint i, j;
  gfloat val;
} celld;

/*-- tour elements --*/
typedef struct {
  gint datadim, projdim, nactive;
  vector_i active_vars;
  array_d Fa, Fz, F, Ga, Gz, G, Va, Vz, tv;
  vector_f lambda, tau, tinc;
  gfloat dist_az, delta, tang;
  gint target_selection_method;
  gint idled;
  gboolean get_new_target;
  gint nsteps, stepcntr;
  gint index; /* this is for counting planes passed */
  gfloat ppval; /* for projection pursuit */
} tour;

/* Sigbert's code for pp */
typedef struct {
  gint min_neighbour, max_neighbour, dim, data_step, neighbour_step;
  /* temporary space */
  gfloat *dist, *mean, *nmean, *fv1, *fv2, *ew, *ev, *cov;
  gint *index;
} subd_param;

typedef struct {
  gint *group, *ngroup, groups;
  /* temporary space */
  gfloat *cov, *mean, *ovmean, *a, *work;
  gint *kpvt;

} discriminant_param;

typedef struct {
  gint *ngroup, *group, groups;

  /* temporary space */
  gint *nright, *index;
  gfloat *x;

} cartgini_param;

typedef struct {
  gint *ngroup, *group, groups;

  /* temporary space */
  gint *nright, *index;
  gfloat *x;

} cartentropy_param;

typedef struct {
  gfloat *y;

  /* temporary space */
  gfloat *x;
  gint *index;

} cartvariance_param;

typedef struct
{ gfloat temp_start, temp_end, cooling, heating, temp, index_best;
  gint restart, maxproj, success;
  array_f proj_best, data;
} optimize0_param; 
/* end Sigbert's code */

/* pp */
typedef struct
{
  gint nrows, ncols;
  gfloat *h0, *h1;
  gfloat acoefs;
  gfloat **derivs;
  gfloat *alpha, *beta;
} holes_param;
/* end pp */

/*
 * display options
*/
typedef struct {
 gboolean points_show_p;              /* scatterplot, scatmat, parcoords */
 
 /*-- two options here may be plenty --*/
 gboolean edges_undirected_show_p;    /* scatterplot */
 gboolean edges_arrowheads_show_p;    /* scatterplot */
 gboolean edges_directed_show_p;      /* scatterplot: both of the above */
 /*-- --*/

 gboolean whiskers_show_p;            /* parcoords, time series */

 gboolean missings_show_p;            /* scatterplot, scatmat, parcoords */

 gboolean axes_show_p;                /* scatterplot, scatmat, parcoords */
 gboolean axes_label_p;               /* scatterplot */
/* unused
 gboolean axes_center_p;              * scatterplot *
 gboolean double_buffer_p;            * parcoords *
 gboolean link_p;                     * scatterplot, scatmat, parcoords *
*/
} DisplayOptions;

extern DisplayOptions DefaultDisplayOptions;

#ifdef BARCHART_IMPLEMENTED
/* bin struct for bins in Barcharts, histograms, ... */
typedef struct
{
  glong count;
  gint index;
  lcoords planar;
  GdkRectangle rect;
} bind;
/* end bind */

/* structure for a barchart, in splotd only a pointer to this structure is made to save memory */
typedef struct
{

 gboolean is_histogram;  /* true if variable not categorical */
 gboolean is_spine;      /* false by default */
 gint nbins;
 gint new_nbins;

 gint ncolors;
 gint maxbincounts;
 bind  *bins;
 bind  **cbins;
 gfloat *breaks;

 GdkPoint anchor_rgn[4];
 GdkPoint offset_rgn[4];
 gboolean anchor_drag;
 gboolean width_drag;
 gfloat offset;

/* whenever points in histograms "overflow" to the left or right of the first or last bin, they need a place to stay */
 gboolean high_pts_missing;
 gboolean low_pts_missing;
 bind *high_bin;
 bind *low_bin;
 bind *col_high_bin;
 bind *col_low_bin;

/* identify bars */
 gboolean *bar_hit;
 gboolean *old_bar_hit;
 gboolean same_hits;

 gint *index_to_rank;
} barchartd;

#endif


#define TYPES_H

#endif
