/* vardata.c */

#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>
#include "vars.h"

void vardata_alloc ()
{
  if (gg.vardata != NULL)
    g_free ((gpointer) gg.vardata);

  gg.vardata = (vardatad *) g_malloc (gg.ncols * sizeof (vardatad));
}

void vardata_realloc (gint n)
{
  gg.vardata = (vardatad *) g_realloc ((gpointer) gg.vardata,
    n * sizeof (vardatad));
}

extern gfloat no_change (gfloat);

void vardata_init ()
{
  gint j;
  for (j=0; j<gg.ncols; j++) {
    gg.vardata[j].selected = false;
    gg.vardata[j].nmissing = 0;
    gg.vardata[j].mean = 0.0;
    gg.vardata[j].median = 0.0;
    gg.vardata[j].lim_raw.min = 0.0;
    gg.vardata[j].lim_raw.max = 0.0;
    gg.vardata[j].lim_tform.min = 0.0;
    gg.vardata[j].lim_tform.max = 0.0;

    gg.vardata[j].domain_incr = 0.;
    gg.vardata[j].domain_adj = no_change;
    gg.vardata[j].inv_domain_adj = no_change;
    gg.vardata[j].tform1 = NO_TFORM1;
    gg.vardata[j].param = 0.;
	gg.vardata[j].tform2 = NO_TFORM2;

    gg.vardata[j].jitter_factor = 0.0;

    gg.vardata[j].collab = NULL;
    gg.vardata[j].collab_tform = NULL;
    gg.vardata[j].groupid = 0;
  }
}

/*-------------------------------------------------------------------------*/
/*                 variable groups                                         */
/*-------------------------------------------------------------------------*/

/*
 * This assumes that the values of variable group ids
 * are {0,1,2,...,nvgroups-1}
*/
gint
nvgroups ()
{
  gint j, ngr = 0;

  for (j=0; j<gg.ncols; j++)
    if (gg.vardata[j].groupid > ngr)
      ngr = gg.vardata[j].groupid;

  return (ngr+1);
}

void
vgroups_sort () {
  gint maxid, id, newid, j;
  gboolean found;

  /*
   * Find maximum vgroup id.
  */
  maxid = 0;
  for (j=0; j<gg.ncols; j++) {
    if (gg.vardata[j].groupid > maxid)
      maxid = gg.vardata[j].groupid;
  }

  /*
   * Find minimum vgroup id, set it to 0.  Find next, set it to 1; etc.
  */
  id = 0;
  newid = -1;
  while (id <= maxid) {
    found = false;
    for (j=0; j<gg.ncols; j++) {
      if (gg.vardata[j].groupid == id) {
        newid++;
        found = true;
        break;
      }
    }
    if (found)
      for (j=0; j<gg.ncols; j++)
        if (gg.vardata[j].groupid == id)
          gg.vardata[j].groupid = newid;
    id++;
  }
}



/*-------------------------------------------------------------------------*/
/*                 finding the statistics for the table                    */
/*-------------------------------------------------------------------------*/

void
vardata_stats_print () {
  gint j;
  for (j=0; j<gg.ncols; j++) {
    g_printerr ("mean=%f, median=%f\n",
      gg.vardata[j].mean, gg.vardata[j].median);
    g_printerr ("lims: %7.2f %7.2f %7.2f %7.2f\n",
      gg.vardata[j].lim_raw.min, gg.vardata[j].lim_raw.max,
      gg.vardata[j].lim_tform.min, gg.vardata[j].lim_tform.max);
  }
}

void
vardata_stats_set () {
  gint j, i, m, np;
  gfloat *sumv = (gfloat *) g_malloc0 (gg.ncols * sizeof (gfloat));
  gfloat *x = (gfloat *) g_malloc (gg.nrows * sizeof (gfloat));
  extern gint fcompare (const void *, const void *);

  /*
   * this could be done with less code, but this 
   * minimizes looping and function calls, and should
   * be as as fast as we can make it
  */

  for (j=0; j<gg.ncols; j++) {
    gg.vardata[j].lim_raw.min = gg.raw.data[0][j];
    gg.vardata[j].lim_raw.max = gg.raw.data[0][j];
    gg.vardata[j].lim_tform.min = gg.tform2.data[0][j];
    gg.vardata[j].lim_tform.max = gg.tform2.data[0][j];
  }

  for (j=0; j<gg.ncols; j++) {
    np = 0;
    for (m=0; m<gg.nrows_in_plot; m++) {
      i = gg.rows_in_plot[m];
      if (gg.nmissing > 0 && MISSING_P(i,j))
        ;
      else {

        if (gg.raw.data[i][j] < gg.vardata[j].lim_raw.min)
          gg.vardata[j].lim_raw.min = gg.raw.data[i][j];
        else if (gg.raw.data[i][j] > gg.vardata[j].lim_raw.max)
          gg.vardata[j].lim_raw.max = gg.raw.data[i][j];

        if (gg.tform2.data[i][j] < gg.vardata[j].lim_tform.min)
          gg.vardata[j].lim_tform.min = gg.tform2.data[i][j];
        else if (gg.tform2.data[i][j] > gg.vardata[j].lim_tform.max)
          gg.vardata[j].lim_tform.max = gg.tform2.data[i][j];

        sumv[j] += gg.raw.data[i][j];
        x[np] = gg.raw.data[i][j];
        np++;
      }
    }
    gg.vardata[j].mean = sumv[j] / (gfloat) gg.nrows;

    /*-- median: sort the temporary vector, and find its center --*/
    qsort((void *) x, np, sizeof (gfloat), fcompare);
    gg.vardata[j].median = 
      ((np % 2) != 0) ?  x[(np-1)/2] : (x[np/2-1] + x[np/2])/2. ;
  }

  g_free ((gpointer) sumv);
  g_free ((gpointer) x);
}

