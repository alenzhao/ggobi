/* missing.c */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#include <gtk/gtk.h>
#include "vars.h"
#include "externs.h"

/*--------------------------------------------------------------------*/
/*             Memory management routines                             */
/*--------------------------------------------------------------------*/

void
missing_world_free ()
{
  arrayl_free (&gg.missing_world, 0, 0);
  arrayl_free (&gg.missing_jitter, 0, 0);
}

void
missing_world_alloc ()
{
  if (gg.missing_world.data != NULL) missing_world_free ();

  arrayl_alloc (&gg.missing_world, gg.nrows, gg.ncols);
  arrayl_alloc (&gg.missing_jitter, gg.nrows, gg.ncols);
}

void
missing_arrays_add_column (gint jvar)
{
  gint nc = gg.ncols + 1;

  if (gg.nmissing > 0) {
    arrays_add_cols (&gg.missing, nc);
    arrayl_add_cols (&gg.missing_jitter, nc);
    arrayl_add_cols (&gg.missing_world, nc);
  }
}


/*------------------------------------------------------------------*/
/*      Scaling and jittering missing value plots                   */
/*------------------------------------------------------------------*/

void
missing_lim_set ()
{
  gint i, j;
  gshort min, max;

  min = max = gg.missing.data[0][0];
  for (i=0; i<gg.nrows; i++) {
    for (j=0; j<gg.ncols; j++) {
      if (gg.missing.data[i][j] < min) min = gg.missing.data[i][j];
      if (gg.missing.data[i][j] > max) max = gg.missing.data[i][j];
    }
  }
  gg.missing_lim.min = (gfloat) min;
  gg.missing_lim.max = (gfloat) max;
}

/*
 * This jitters the missings indicator vector for one variable
*/
void
missing_jitter_variable (gint jcol)
{
  gint i, m;
  gfloat precis = (gfloat) PRECISION1;
  gfloat frand, fworld, fjit;

  for (i=0; i<gg.nrows; i++) {
    if ((m = gg.rows_in_plot[i]) >= gg.nlinkable)
      break;
    else {

      frand = jitter_randval (gg.jitter_type) * precis;

      if (gg.jitter_convex) {
        fworld = (gfloat)
          (gg.missing_world.data[m][jcol] - gg.missing_jitter.data[m][jcol]);
        fjit = gg.missing_jitter_factor * (frand - fworld);
      } else
        fjit = gg.missing_jitter_factor * frand;

      gg.missing_jitter.data[m][jcol] = (glong) fjit;
    }
  }
}

void
missing_jitter_value_set (gfloat value) {
  gg.missing_jitter_factor = (value > 0) ? value : 0;
}

static void
missing_jitter_init ()
{
  gint j;

  missing_jitter_value_set (.2);
  for (j=0; j<gg.ncols; j++)
    missing_jitter_variable (j);
}

void
missing_to_world ()
{
  gint i, j, m;
  gfloat ftmp;
  gfloat precis = PRECISION1;
  gfloat min = gg.missing_lim.min;
  gfloat range = gg.missing_lim.max - gg.missing_lim.min;
  static gboolean initd = false;

  if (!initd) {
    missing_jitter_init ();
    initd = true;
  }

  for (j=0; j<gg.ncols; j++) {
    for (i=0; i<gg.nrows_in_plot; i++) {
      m = gg.rows_in_plot[i];
      ftmp = -1.0 + 2.0*(gg.missing.data[m][j] - min) / range;
      gg.missing_world.data[m][j] = (glong) (precis * ftmp);

      /* Add in the jitter values */
      gg.missing_world.data[m][j] += gg.missing_jitter.data[m][j];
    }
  }
}

void
missing_rejitter () {
  gint j;

/*
 * This rejitters <everything> which is perhaps excessive, no?
*/
  for (j=0; j<gg.ncols; j++)
    missing_jitter_variable (j);

  missing_to_world ();

  /*-- redisplay only the missings displays --*/
  displays_tailpipe (REDISPLAY_MISSING); 
}
