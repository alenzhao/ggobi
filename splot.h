#ifndef DEFINES_H
#include "defines.h"
#endif

typedef struct {

 gpointer displayptr;  /* a pointer to the enclosing display */

 GtkWidget *da;      /* drawing_area */
 GdkPixmap *pixmap0, *pixmap1;  /* 2-stage drawing */

 gint redraw_style;

 /* Drawing area dimensions */
 icoords max;

 /*
  * line segments in scatterplot and scatmat
 */
 GdkSegment *arrowheads;
 GdkSegment *segments;

 /*
  * line segments in parallel coordinates plot
 */
 GdkSegment *whiskers;

 lcoords *planar;
 icoords *screen;

 /*
  * shift and scale
 */
 fcoords scale, tour_scale;
 lcoords iscale;
 icoords ishift;


/*
 * button and key event information
*/
 gint motion_id, press_id, release_id, key_press_id;

/*
 * plot1d  (used in parcoords as well as scatterplot)
*/
 gint p1dvar;
 gfloat *p1d_data;  /* the spreading data */
 lims p1d_lim;      /* limits of the spreading data */

/*
 * xyplot
*/
 icoords xyvars;

/*
 * rotation
*/
 struct {gint x, y, z;} spinvars;

/*
 * tour
*/
 gint n_tourvars;  /*number of variables touring */
 gint *tourvars;   /*numbers of chosen variables */

/*
 * correlation tour
*/
 gint n_corrvars_x, *corrvars_x, n_corrvars_y, *corrvars_y;

/*
 * brushing
*/

} splotd;

#define SPLOT_H
