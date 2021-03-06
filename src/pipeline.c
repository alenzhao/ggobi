/*-- pipeline.c --*/
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

#include "pipeline.h"


void
tform_to_world_by_var (GGobiStage * d, guint j)
{
  guint i;

  for( i =  0; i < d->n_rows; i++) {
    gdouble ftmp = -1.0 + 2.0 * ggobi_stage_get_raw_value(d, i, j);
    d->world.vals[i][j] = ftmp;
  }
}

void
tform_to_world (GGobiStage * d)
{
  guint j;

  for (j = 0; j < d->n_cols; j++)
    tform_to_world_by_var (d, j);
}



/*-------------------------------------------------------------------------*/
/*                     reverse pipeline                                    */
/*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/* Reverse pipeline code for populating the table of variable values  */
/*--------------------------------------------------------------------*/
/*
 * I want these routines to work for point motion (movepts.c) and
 * for line editing (lineedit.c).  They require different arguments.
 * And they all need to be moved now, too, maybe some to splot.c or
 * maybe all to pipeline.c.  ... and what about the class-based
 * methodology?  Hmm.
*/

void
pt_screen_to_plane (icoords * screen, gint id, gboolean horiz, gboolean vert,
                    gcoords * eps, gcoords * planar, splotd * sp)
{
  gcoords prev_planar;
  prev_planar.x = 0;
  prev_planar.y = 0;
  
  gdouble scale_x, scale_y;

  scale_x = sp->scale.x;
  scale_y = sp->scale.y;
  scale_x /= 2;
  sp->iscale.x = (gdouble) sp->max.x * scale_x;
  scale_y /= 2;
  sp->iscale.y = -1 * (gdouble) sp->max.y * scale_y;

  if (id >= 0) {                /* when moving points, initialize new planar values */
    eps->x = 0;
    eps->y = 0;
    planar->x = prev_planar.x = sp->planar[id].x;
    planar->y = prev_planar.y = sp->planar[id].y;
  }

  if (horiz) {                  /* relevant distinction for moving points */
    screen->x -= sp->max.x / 2;
    planar->x = (gdouble) screen->x / sp->iscale.x;
    planar->x += (gdouble) sp->pmid.x;
  }

  if (vert) {                   /* relevant distinction for moving points */
    screen->y -= sp->max.y / 2;
    planar->y = (gdouble) screen->y / sp->iscale.y;
    planar->y += (gdouble) sp->pmid.y;
  }

  if (id >= 0) {                /* when moving points */
    if (horiz)
      eps->x = planar->x - prev_planar.x;
    if (vert)
      eps->y = planar->y - prev_planar.y;
  }
}

void
pt_plane_to_world (splotd * sp, gcoords * planar, gcoords * eps,
                   gdouble * world)
{
  displayd *display = (displayd *) sp->displayptr;
  cpaneld *cpanel = &display->cpanel;
  gint j, var;

  switch (cpanel->pmode) {      /* only valid for scatterplots? */
  case P1PLOT:
    if (display->p1d_orientation == VERTICAL)
      world[sp->p1dvar] = planar->y;
    else
      world[sp->p1dvar] = planar->x;
    break;
  case XYPLOT:
    world[sp->xyvars.x] = planar->x;
    world[sp->xyvars.y] = planar->y;
    break;
  case TOUR1D:
    /*if (!gg->is_pp) { */
    for (j = 0; j < display->t1d.nactive; j++) {
      var = display->t1d.active_vars.els[j];
      world[var] += (eps->x * (gdouble) display->t1d.F.vals[0][var]);
    }
    /*} */
    break;
  case TOUR2D3:
    for (j = 0; j < display->t2d3.nactive; j++) {
      var = display->t2d3.active_vars.els[j];
      world[var] +=
        (eps->x * (gdouble) display->t2d3.F.vals[0][var] +
         eps->y * (gdouble) display->t2d3.F.vals[1][var]);
    }
    break;
  case TOUR2D:
    /*if (!gg->is_pp) { */
    for (j = 0; j < display->t2d.nactive; j++) {
      var = display->t2d.active_vars.els[j];
      world[var] +=
        (eps->x * (gdouble) display->t2d.F.vals[0][var] +
         eps->y * (gdouble) display->t2d.F.vals[1][var]);
    }
    /*} */
    break;
  case COTOUR:
    /*if (!gg->is_pp) { */
    for (j = 0; j < display->tcorr1.nactive; j++) {
      var = display->tcorr1.active_vars.els[j];
      world[var] += (eps->x * (gdouble) display->tcorr1.F.vals[0][var]);
    }
    for (j = 0; j < display->tcorr2.nactive; j++) {
      var = display->tcorr2.active_vars.els[j];
      world[var] += (eps->y * (gdouble) display->tcorr2.F.vals[0][var]);
    }
    /*} */
    break;
  default:
    g_printerr ("reverse pipeline not yet implemented for this projection\n");
  }
}

void
pt_world_to_raw_by_var (gint j, gdouble * world, gdouble * raw, GGobiStage * d)
{
  gdouble rdiff;
  gdouble x;
  GGobiVariable *var = ggobi_stage_get_variable(d, j);

  rdiff = ggobi_variable_get_range(var);

  x = (world[j] + 1.0) * .5 * rdiff;
  x += ggobi_variable_get_min(var);

  raw[j] = x;
}

void
pt_screen_to_raw (icoords * screen, gint id, gboolean horiz, gboolean vert,
                  gdouble * raw, gcoords * eps, GGobiStage * d, splotd * sp,
                  GGobiSession * gg)
{
  gint j;
  gcoords planar;
  gdouble *world = (gdouble *) g_malloc0 (d->n_cols * sizeof (gdouble));

  pt_screen_to_plane (screen, id, horiz, vert, eps, &planar, sp);
  pt_plane_to_world (sp, &planar, &planar, world);

  for (j = 0; j < d->n_cols; j++)
    pt_world_to_raw_by_var (j, world, raw, d);

  g_free (world);
}
