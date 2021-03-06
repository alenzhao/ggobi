/* tourcorr.c */
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

#include <gtk/gtk.h>
#ifdef USE_STRINGS_H
#include <strings.h>
#endif

#include <math.h>

#include "vars.h"
#include "externs.h"

#define CTON true
#define CTOFF false

static void tourcorr_active_horvar_set (gint jvar, GGobiStage *, displayd *, GGobiSession *);
static void tourcorr_active_vervar_set (gint jvar, GGobiStage *, displayd *, GGobiSession *);
static void tourcorr_active_var_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg);
void tourcorr_write_video(GGobiSession *gg);

void
display_tourcorr_init_null (displayd *dsp, GGobiSession *gg)
{
  arrayd_init_null(&dsp->tcorr1.Fa);
  arrayd_init_null(&dsp->tcorr1.Fz);
  arrayd_init_null(&dsp->tcorr1.F);

  arrayd_init_null(&dsp->tcorr1.Ga);
  arrayd_init_null(&dsp->tcorr1.Gz);
  arrayd_init_null(&dsp->tcorr1.G);

  arrayd_init_null(&dsp->tcorr1.Va);
  arrayd_init_null(&dsp->tcorr1.Vz);

  arrayd_init_null(&dsp->tcorr1.tv);

  vectori_init_null(&dsp->tcorr1.subset_vars);
  vectorb_init_null(&dsp->tcorr1.subset_vars_p);
  vectori_init_null(&dsp->tcorr1.active_vars);
  vectorb_init_null(&dsp->tcorr1.active_vars_p);

  vectord_init_null(&dsp->tcorr1.lambda);
  vectord_init_null(&dsp->tcorr1.tau);
  vectord_init_null(&dsp->tcorr1.tinc);

  /* manipulation controls */
  arrayd_init_null(&dsp->tc1_manbasis);
  arrayd_init_null(&dsp->tc2_manbasis);


  arrayd_init_null(&dsp->tcorr2.Fa);
  arrayd_init_null(&dsp->tcorr2.Fz);
  arrayd_init_null(&dsp->tcorr2.F);

  arrayd_init_null(&dsp->tcorr2.Ga);
  arrayd_init_null(&dsp->tcorr2.Gz);
  arrayd_init_null(&dsp->tcorr2.G);

  arrayd_init_null(&dsp->tcorr2.Va);
  arrayd_init_null(&dsp->tcorr2.Vz);

  arrayd_init_null(&dsp->tcorr2.tv);

  vectori_init_null(&dsp->tcorr2.subset_vars);
  vectorb_init_null(&dsp->tcorr2.subset_vars_p);
  vectori_init_null(&dsp->tcorr2.active_vars);
  vectorb_init_null(&dsp->tcorr2.active_vars_p);

  vectord_init_null(&dsp->tcorr2.lambda);
  vectord_init_null(&dsp->tcorr2.tau);
  vectord_init_null(&dsp->tcorr2.tinc);
}

void
alloc_tourcorr (displayd *dsp, GGobiSession *gg)
{
  GGobiStage *d = dsp->d;
  gint nc = d->n_cols;

  /* first index is the projection dimensions, second dimension is ncols */
  arrayd_alloc(&dsp->tcorr1.Fa, 1, nc);
  arrayd_alloc(&dsp->tcorr1.Fz, 1, nc);
  arrayd_alloc(&dsp->tcorr1.F, 1, nc);

  arrayd_alloc(&dsp->tcorr1.Ga, 1, nc);
  arrayd_alloc(&dsp->tcorr1.Gz, 1, nc);
  arrayd_alloc(&dsp->tcorr1.G, 1, nc);

  arrayd_alloc(&dsp->tcorr1.Va, 1, nc);
  arrayd_alloc(&dsp->tcorr1.Vz, 1, nc);

  arrayd_alloc(&dsp->tcorr1.tv, 1, nc);

  vectori_alloc(&dsp->tcorr1.subset_vars, nc);
  vectorb_alloc_zero(&dsp->tcorr1.subset_vars_p, nc);
  vectori_alloc(&dsp->tcorr1.active_vars, nc);
  vectorb_alloc_zero(&dsp->tcorr1.active_vars_p, nc);

  vectord_alloc(&dsp->tcorr1.lambda, nc);
  vectord_alloc_zero(&dsp->tcorr1.tau, nc);
  vectord_alloc(&dsp->tcorr1.tinc, nc);

  /* manipulation controls */
  arrayd_alloc(&dsp->tc1_manbasis, 2, nc);
  arrayd_alloc(&dsp->tc2_manbasis, 2, nc);

  /* first index is the projection dimensions, second dimension is ncols */
  arrayd_alloc(&dsp->tcorr2.Fa, 1, nc);
  arrayd_alloc(&dsp->tcorr2.Fz, 1, nc);
  arrayd_alloc(&dsp->tcorr2.F, 1, nc);

  arrayd_alloc(&dsp->tcorr2.Ga, 1, nc);
  arrayd_alloc(&dsp->tcorr2.Gz, 1, nc);
  arrayd_alloc(&dsp->tcorr2.G, 1, nc);

  arrayd_alloc(&dsp->tcorr2.Va, 1, nc);
  arrayd_alloc(&dsp->tcorr2.Vz, 1, nc);

  arrayd_alloc(&dsp->tcorr2.tv, 1, nc);

  vectori_alloc(&dsp->tcorr2.subset_vars, nc);
  vectorb_alloc_zero(&dsp->tcorr2.subset_vars_p, nc);
  vectori_alloc(&dsp->tcorr2.active_vars, nc);
  vectorb_alloc_zero(&dsp->tcorr2.active_vars_p, nc);

  vectord_alloc(&dsp->tcorr2.lambda, nc);
  vectord_alloc_zero(&dsp->tcorr2.tau, nc);
  vectord_alloc(&dsp->tcorr2.tinc, nc);
}

/*-- eliminate the nc columns contained in *cols --*/
void
tourcorr_realloc_down (GSList *cols, GGobiStage *d, GGobiSession *gg)
{
  displayd *dsp;
  GList *l;
  for (l=gg->displays; l; l=l->next) {
    dsp = (displayd *) l->data;
    if (dsp->d == d) {
      arrayd_delete_cols (&dsp->tcorr1.Fa, cols);
      arrayd_delete_cols (&dsp->tcorr1.Fz, cols);
      arrayd_delete_cols (&dsp->tcorr1.F, cols);
      arrayd_delete_cols (&dsp->tcorr1.Ga, cols);
      arrayd_delete_cols (&dsp->tcorr1.Gz, cols);
      arrayd_delete_cols (&dsp->tcorr1.G, cols);
      arrayd_delete_cols (&dsp->tcorr1.Va, cols);
      arrayd_delete_cols (&dsp->tcorr1.Vz, cols);
      arrayd_delete_cols (&dsp->tcorr1.tv, cols);

      vectori_delete_els (&dsp->tcorr1.subset_vars, cols);
      vectorb_delete_els (&dsp->tcorr1.subset_vars_p, cols);
      vectori_delete_els (&dsp->tcorr1.active_vars, cols);
      vectorb_delete_els (&dsp->tcorr1.active_vars_p, cols);

      vectord_delete_els (&dsp->tcorr1.lambda, cols);
      vectord_delete_els (&dsp->tcorr1.tau, cols);
      vectord_delete_els (&dsp->tcorr1.tinc, cols);

      arrayd_delete_cols (&dsp->tc1_manbasis, cols);
      arrayd_delete_cols (&dsp->tc2_manbasis, cols);

      arrayd_delete_cols (&dsp->tcorr2.Fa, cols);
      arrayd_delete_cols (&dsp->tcorr2.Fz, cols);
      arrayd_delete_cols (&dsp->tcorr2.F, cols);
      arrayd_delete_cols (&dsp->tcorr2.Ga, cols);
      arrayd_delete_cols (&dsp->tcorr2.Gz, cols);
      arrayd_delete_cols (&dsp->tcorr2.G, cols);
      arrayd_delete_cols (&dsp->tcorr2.Va, cols);
      arrayd_delete_cols (&dsp->tcorr2.Vz, cols);
      arrayd_delete_cols (&dsp->tcorr2.tv, cols);

      vectori_delete_els (&dsp->tcorr2.subset_vars, cols);
      vectorb_delete_els (&dsp->tcorr2.subset_vars_p, cols);
      vectori_delete_els (&dsp->tcorr2.active_vars, cols);
      vectorb_delete_els (&dsp->tcorr2.active_vars_p, cols);

      vectord_delete_els (&dsp->tcorr2.lambda, cols);
      vectord_delete_els (&dsp->tcorr2.tau, cols);
      vectord_delete_els (&dsp->tcorr2.tinc, cols);
    }
  }
}

void
free_tourcorr(displayd *dsp)
{
  /*  gint k;*/
  /*  GGobiStage *d = dsp->d;*/
  /*  gint nc = d->n_cols;*/

  vectori_free(&dsp->tcorr1.subset_vars);
  vectorb_free(&dsp->tcorr1.subset_vars_p);
  vectori_free(&dsp->tcorr1.active_vars);
  vectorb_free(&dsp->tcorr1.active_vars_p);

  vectord_free(&dsp->tcorr1.lambda);
  vectord_free(&dsp->tcorr1.tau);
  vectord_free(&dsp->tcorr1.tinc);

  arrayd_free (&dsp->tcorr1.Fa); 
  arrayd_free (&dsp->tcorr1.Fz); 
  arrayd_free (&dsp->tcorr1.F); 

  arrayd_free (&dsp->tcorr1.Ga); 
  arrayd_free (&dsp->tcorr1.Gz); 
  arrayd_free (&dsp->tcorr1.G); 

  arrayd_free (&dsp->tcorr1.Va); 
  arrayd_free (&dsp->tcorr1.Vz); 
  arrayd_free (&dsp->tcorr1.tv); 

  arrayd_free (&dsp->tc1_manbasis); 
  arrayd_free (&dsp->tc2_manbasis); 

  vectori_free(&dsp->tcorr2.subset_vars);
  vectorb_free(&dsp->tcorr2.subset_vars_p);
  vectori_free(&dsp->tcorr2.active_vars);
  vectorb_free(&dsp->tcorr2.active_vars_p);

  vectord_free(&dsp->tcorr2.lambda);
  vectord_free(&dsp->tcorr2.tau);
  vectord_free(&dsp->tcorr2.tinc);

  arrayd_free (&dsp->tcorr2.Fa); 
  arrayd_free (&dsp->tcorr2.Fz); 
  arrayd_free (&dsp->tcorr2.F); 

  arrayd_free (&dsp->tcorr2.Ga); 
  arrayd_free (&dsp->tcorr2.Gz); 
  arrayd_free (&dsp->tcorr2.G); 

  arrayd_free (&dsp->tcorr2.Va); 
  arrayd_free (&dsp->tcorr2.Vz); 
  arrayd_free (&dsp->tcorr2.tv); 

}

void 
display_tourcorr_init (displayd *dsp, GGobiSession *gg) {
  gint i, j;
  GGobiStage *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  gint nc = d->n_cols;
  gint nvert, nhoriz;

  if (nc < MIN_NVARS_FOR_COTOUR)
    return;

  alloc_tourcorr(dsp, gg);

  if (nc == 3) {  /*-- ... which is equal to MIN_NVARS_FOR_COTOUR --*/
    nhoriz = 2;
    nvert = 1;
  } else if (nc < 8) {
    nhoriz = nc-2;
    nvert = 2;
  } else {
    nhoriz = 3;
    nvert = 2;
  }

/*-------------------------------------------------------------------*/

  /*-- vertical --*/
  dsp->tcorr2.nsubset = dsp->tcorr2.nactive = nvert;
  for (j=0; j<nvert; j++) {
    dsp->tcorr2.subset_vars.els[j] = dsp->tcorr2.active_vars.els[j] = j;
    dsp->tcorr2.subset_vars_p.els[j] = dsp->tcorr2.active_vars_p.els[j] = true;
  }

  /*-- horizontal --*/
  dsp->tcorr1.nsubset = dsp->tcorr1.nactive = nhoriz;
  for (j=0; j<nhoriz; j++) {
    dsp->tcorr1.subset_vars.els[j] = dsp->tcorr1.active_vars.els[j] = j+nvert;
    dsp->tcorr1.subset_vars_p.els[j+nvert] =
      dsp->tcorr1.active_vars_p.els[j+nvert] = true;
  }

/*
  if (nc < 8) {
    dsp->tcorr2.nsubset = dsp->tcorr2.nactive = nc-2;
    for (j=0; j<2; j++) {
      dsp->tcorr2.subset_vars_p.els[j] = dsp->tcorr2.active_vars_p.els[j] = false;
    }
    for (j=0; j<nc-2; j++) {
      dsp->tcorr2.subset_vars.els[j] = dsp->tcorr2.active_vars.els[j] = j+2;
      dsp->tcorr2.subset_vars_p.els[j+2] = dsp->tcorr2.active_vars_p.els[j+2] = true;
    }
  }
  else {
    dsp->tcorr2.nsubset = dsp->tcorr2.nactive = 3;
    for (j=0; j<3; j++) {
      dsp->tcorr2.subset_vars.els[j] = dsp->tcorr2.active_vars.els[j] = j+2;
      dsp->tcorr2.subset_vars_p.els[j+2] = dsp->tcorr2.active_vars_p.els[j+2] = true;
    }
    for (j=3; j<nc-2; j++) {
      dsp->tcorr2.subset_vars.els[j] = dsp->tcorr2.active_vars.els[j] = 0;
      dsp->tcorr2.subset_vars_p.els[j+2] = dsp->tcorr2.active_vars_p.els[j+2] = false;
    }
  }
*/

  /* declare starting horizontal base as first horizontal variable */
  for (i=0; i<1; i++)
    for (j=0; j<nc; j++)
      dsp->tcorr1.Fa.vals[i][j] = dsp->tcorr1.Fz.vals[i][j] = 
        dsp->tcorr1.F.vals[i][j] = 
        dsp->tcorr1.Ga.vals[i][j] = dsp->tcorr1.Gz.vals[i][j] = 0.0;
  dsp->tcorr1.F.vals[0][dsp->tcorr1.active_vars.els[0]] = 1.0;

  /* declare starting vertical base as first vertical variable */
  for (i=0; i<1; i++)
    for (j=0; j<nc; j++)
      dsp->tcorr2.Fa.vals[i][j] = dsp->tcorr2.Fz.vals[i][j] = 
        dsp->tcorr2.F.vals[i][j] = 
        dsp->tcorr2.Ga.vals[i][j] = dsp->tcorr2.Gz.vals[i][j] = 0.0;
  dsp->tcorr2.F.vals[0][dsp->tcorr2.active_vars.els[0]] = 1.0;

  /* horizontal */
  dsp->tcorr1.dist_az = 0.0;
  dsp->tcorr1.delta = cpanel->tcorr1.step*M_PI_2/10.0;
  dsp->tcorr1.tang = 0.0;
  dsp->tcorr1.idled = 0;
  dsp->tcorr1.get_new_target = true;

  dsp->tourcorr_video = false;

  /* vertical */
  dsp->tcorr2.dist_az = 0.0;
  dsp->tcorr2.delta = cpanel->tcorr2.step*M_PI_2/10.0; 
  dsp->tcorr2.tang = 0.0;
  dsp->tcorr2.idled = 0;
  dsp->tcorr2.get_new_target = true;

  /* manip */
  dsp->tc1_manip_var = dsp->tcorr1.active_vars.els[0];
  dsp->tc2_manip_var = dsp->tcorr2.active_vars.els[0];

  /* pp */
  dsp->tcorr1.target_selection_method = 0;
  dsp->tcorr2.target_selection_method = 0;
  dsp->tcorr_axes = true;
}

void
tourcorr_fade_vars (gboolean fade, GGobiSession *gg) 
{
  gg->tourcorr.fade_vars = fade;
}

void tourcorr_speed_set(gdouble slidepos, GGobiSession *gg) {
  displayd *dsp = gg->current_display; 
  cpaneld *cpanel = &dsp->cpanel;

  speed_set(slidepos, &cpanel->tcorr1.step, &dsp->tcorr1.delta);

  speed_set(slidepos, &cpanel->tcorr2.step, &dsp->tcorr2.delta);

  cpanel->tcorr.slidepos = slidepos;
}

void tourcorr_pause (cpaneld *cpanel, gboolean state, GGobiSession *gg)
{
  cpanel->tcorr1.paused = state;
  cpanel->tcorr2.paused = state;

  tourcorr_func (!cpanel->tcorr1.paused, gg->current_display, gg);
  tourcorr_func (!cpanel->tcorr2.paused, gg->current_display, gg);

  if (cpanel->tcorr1.paused && cpanel->tcorr2.paused) {
    /*-- whenever motion stops, we need a FULL redraw --*/
    display_tailpipe (gg->current_display, FULL, gg);
  }
}

gboolean
tourcorr_subset_horvar_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg)
{
  gboolean in_subset = dsp->tcorr1.subset_vars_p.els[jvar];
  gint j, k;
  gboolean changed = false;
  gboolean fade = gg->tourcorr.fade_vars;

  gg->tourcorr.fade_vars = false;

  /*
   * if it's in the vertical subset, and there are at least two
   * variables in that subset, then remove jvar from the vertical
   * subset before adding it to the horizontal one
  */
  if (dsp->tcorr2.subset_vars_p.els[jvar] && dsp->tcorr2.nsubset > 1) {

  /*-- removing from tcorr2 --*/
    dsp->tcorr2.subset_vars_p.els[jvar] = false;
    dsp->tcorr2.nsubset -= 1;

    /*-- reset the horizontal subset_vars based on subset_vars_p --*/
    for (j=0, k=0; j<d->n_cols; j++)
      if (dsp->tcorr2.subset_vars_p.els[j])
        dsp->tcorr2.subset_vars.els[k++] = j;

    /*-- jump jvar out of the vertical active set --*/
    tourcorr_active_vervar_set (jvar, d, dsp, gg);

  /*-- adding to tcorr1 --*/
 
    /*-- add jvar to the horizontal subset --*/
    dsp->tcorr1.subset_vars_p.els[jvar] = true;
    dsp->tcorr1.nsubset += 1;

    /*-- jump jvar into the horizontal active set --*/
    tourcorr_active_horvar_set (jvar, d, dsp, gg);

    changed = true;

    /*-- reset subset_vars based on subset_vars_p --*/
    for (j=0, k=0; j<d->n_cols; j++)
      if (dsp->tcorr1.subset_vars_p.els[j])
        dsp->tcorr1.subset_vars.els[k++] = j;

    varcircles_visibility_set (dsp, gg);

  } else {  /*-- simple adding or removing, no swaps --*/

    if (in_subset) {
      /*-- allow the change if there are more than 3 cotour variables,
           and if there's more than 1 horizontal variables --*/
      if (dsp->tcorr1.nsubset + dsp->tcorr2.nsubset > MIN_NVARS_FOR_COTOUR &&
          dsp->tcorr1.nsubset > 1)
      {
        dsp->tcorr1.subset_vars_p.els[jvar] = false;
        dsp->tcorr1.nsubset -= 1;
        changed = true;
      }
    } else {
      dsp->tcorr1.subset_vars_p.els[jvar] = true;
      dsp->tcorr1.nsubset += 1;
      changed = true;
    }

    if (changed) {
      dsp->tc1_manipvar_inc = false;
      /*-- reset subset_vars based on subset_vars_p --*/
      for (j=0, k=0; j<d->n_cols; j++)
        if (dsp->tcorr1.subset_vars_p.els[j]) {
          dsp->tcorr1.subset_vars.els[k++] = j;
          if (j == dsp->tc1_manip_var)
            dsp->tc1_manipvar_inc = true;
	}

      /*-- Manip var needs to be one of the active vars --*/
      if (!dsp->tc1_manipvar_inc) {
        dsp->tc1_manip_var = dsp->tcorr1.subset_vars.els[0];
      }
      
      zero_tau(dsp->tcorr1.tau, 1);
      dsp->tcorr1.get_new_target = true;

      varcircles_visibility_set (dsp, gg);
      /*-- add/remove jvar to/from the horizontal active set --*/
      tourcorr_active_horvar_set (jvar, d, dsp, gg);
    }
  }

  gg->tourcorr.fade_vars = fade;
  return changed;
}

static void 
tourcorr_active_horvar_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg)
{
  gint j, k;
  gboolean in_subset = dsp->tcorr1.subset_vars_p.els[jvar];
  gboolean active = dsp->tcorr1.active_vars_p.els[jvar];

  /*
   * This covers the case where we've just removed a variable
   * from the subset and then called tour2d_active_var_set ..
   * but the variable is already inactive, so we don't need to
   * do anything.
  */
  if (!active && !in_subset)
/**/return;

  /* deselect var if tcorr1.nactive > 1 */
  if (active) {
    if (dsp->tcorr1.nactive > 1) {
      for (j=0; j<dsp->tcorr1.nactive; j++) {
        if (jvar == dsp->tcorr1.active_vars.els[j]) 
          break;
      }
      if (j<dsp->tcorr1.nactive-1) {
        for (k=j; k<dsp->tcorr1.nactive-1; k++){
          dsp->tcorr1.active_vars.els[k] = dsp->tcorr1.active_vars.els[k+1];
        }
      }
      dsp->tcorr1.nactive--;
      if (!gg->tourcorr.fade_vars) /* set current position without sel var */
      {
        gt_basis(dsp->tcorr1.Fa, dsp->tcorr1.nactive, dsp->tcorr1.active_vars, 
          d->n_cols, (gint) 1);
        arrayd_copy(&dsp->tcorr1.Fa, &dsp->tcorr1.F);
	/*        copy_mat(dsp->tcorr1.F.vals, dsp->tcorr1.Fa.vals, d->n_cols, 1);*/
      }
      dsp->tcorr1.active_vars_p.els[jvar] = false;
    }
  }
  else { /* not active, so add the variable */
    if (jvar > dsp->tcorr1.active_vars.els[dsp->tcorr1.nactive-1]) {
      dsp->tcorr1.active_vars.els[dsp->tcorr1.nactive] = jvar;
    }
    else if (jvar < dsp->tcorr1.active_vars.els[0]) {
      for (j=dsp->tcorr1.nactive; j>0; j--) {
          dsp->tcorr1.active_vars.els[j] = dsp->tcorr1.active_vars.els[j-1];
      }
      dsp->tcorr1.active_vars.els[0] = jvar;
    }
    else {
      gint jtmp = dsp->tcorr1.nactive;
      for (j=0; j<dsp->tcorr1.nactive-1; j++) {
        if (jvar > dsp->tcorr1.active_vars.els[j] && jvar < 
          dsp->tcorr1.active_vars.els[j+1]) {
          jtmp = j+1;
          break;
        }
      }
      for (j=dsp->tcorr1.nactive-1;j>=jtmp; j--) 
          dsp->tcorr1.active_vars.els[j+1] = dsp->tcorr1.active_vars.els[j];
      dsp->tcorr1.active_vars.els[jtmp] = jvar;
    }
    dsp->tcorr1.nactive++;
    dsp->tcorr1.active_vars_p.els[jvar] = true;
  }

  dsp->tcorr1.get_new_target = true;
}

gboolean
tourcorr_subset_vervar_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg)
{
  gboolean in_subset = dsp->tcorr2.subset_vars_p.els[jvar];
  gint j, k;
  gboolean changed = false;
  gboolean fade = gg->tourcorr.fade_vars;

  gg->tourcorr.fade_vars = false;

  /*
   * if it's in the horiz subset, and there are at least two
   * variables in that subset, then remove jvar from the horiz
   * subset before adding it to the vertical one
  */
  if (dsp->tcorr1.subset_vars_p.els[jvar] && dsp->tcorr1.nsubset > 1) {

  /*-- removing from tcorr1 --*/
    dsp->tcorr1.subset_vars_p.els[jvar] = false;
    dsp->tcorr1.nsubset -= 1;

    /*-- reset the vertical subset_vars based on subset_vars_p --*/
    for (j=0, k=0; j<d->n_cols; j++)
      if (dsp->tcorr1.subset_vars_p.els[j])
        dsp->tcorr1.subset_vars.els[k++] = j;

    /*-- jump jvar out of the horizontal active set --*/
    tourcorr_active_horvar_set (jvar, d, dsp, gg);

  /*-- adding to tcorr2 --*/
 
    /*-- add jvar to the vertical subset --*/
    dsp->tcorr2.subset_vars_p.els[jvar] = true;
    dsp->tcorr2.nsubset += 1;

    /*-- jump jvar into the vertical active set --*/
    tourcorr_active_vervar_set (jvar, d, dsp, gg);

    changed = true;

    /*-- reset subset_vars based on subset_vars_p --*/
    for (j=0, k=0; j<d->n_cols; j++)
      if (dsp->tcorr2.subset_vars_p.els[j])
        dsp->tcorr2.subset_vars.els[k++] = j;

    varcircles_visibility_set (dsp, gg);

  } else {  /*-- simple adding or removing, no swaps --*/

    if (in_subset) {
      /*-- allow the change if there are more than 3 cotour variables,
           and if there's more than 1 vertical variables --*/
      if (dsp->tcorr2.nsubset + dsp->tcorr1.nsubset > MIN_NVARS_FOR_COTOUR &&
          dsp->tcorr2.nsubset > 1)
      {
        dsp->tcorr2.subset_vars_p.els[jvar] = false;
        dsp->tcorr2.nsubset -= 1;
        changed = true;
      }
    } else {
      dsp->tcorr2.subset_vars_p.els[jvar] = true;
      dsp->tcorr2.nsubset += 1;
      changed = true;
    }

    if (changed) {
      dsp->tc2_manipvar_inc = false;
      /*-- reset subset_vars based on subset_vars_p --*/
      for (j=0, k=0; j<d->n_cols; j++)
        if (dsp->tcorr2.subset_vars_p.els[j]) {
          dsp->tcorr2.subset_vars.els[k++] = j;
          if (j == dsp->tc2_manip_var)
            dsp->tc2_manipvar_inc = true;
	}
      /*-- Manip var needs to be one of the active vars --*/
      if (!dsp->tc2_manipvar_inc) {
        dsp->tc2_manip_var = dsp->tcorr2.subset_vars.els[0];
      }
      
      zero_tau(dsp->tcorr2.tau, 1);
      dsp->tcorr2.get_new_target = true;

      varcircles_visibility_set (dsp, gg);
      /*-- add/remove jvar to/from the horizontal active set --*/
      tourcorr_active_vervar_set (jvar, d, dsp, gg);
    }
  }

  gg->tourcorr.fade_vars = fade;
  return changed;
}

#ifdef OVER
gboolean
tourcorr_subset_vervar_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg)
{
  gboolean in_subset = dsp->tcorr2.subset_vars_p.els[jvar];
  gint j, k;
  gboolean changed = false;
  gboolean fade = gg->tourcorr.fade_vars;

  gg->tourcorr.fade_vars = false;

  if (dsp->tcorr1.subset_vars_p.els[jvar])
    return false;

  if (in_subset) {
    if (dsp->tcorr1.nsubset + dsp->tcorr2.nsubset > MIN_NVARS_FOR_COTOUR &&
        dsp->tcorr2.nsubset > 1)
    {
      dsp->tcorr2.subset_vars_p.els[jvar] = false;
      dsp->tcorr2.nsubset -= 1;
      changed = true;
    }
  } else {
    dsp->tcorr2.subset_vars_p.els[jvar] = true;
    dsp->tcorr2.nsubset += 1;
    changed = true;
  }

  /*-- reset subset_vars based on subset_vars_p --*/
  if (changed) {
    for (j=0, k=0; j<d->n_cols; j++)
      if (dsp->tcorr2.subset_vars_p.els[j])
        dsp->tcorr2.subset_vars.els[k++] = j;

    varcircles_visibility_set (dsp, gg);
    /*-- add/remove jvar to/from the vertical active set --*/
    tourcorr_active_vervar_set (jvar, d, dsp, gg);
  }

  gg->tourcorr.fade_vars = fade;
  return changed;
}
#endif

static void 
tourcorr_active_vervar_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg)
{
  gint j, k;
  gboolean in_subset = dsp->tcorr2.subset_vars_p.els[jvar];
  gboolean active = dsp->tcorr2.active_vars_p.els[jvar];

  /*
   * This covers the case where we've just removed a variable
   * from the subset and then called tour2d_active_var_set ..
   * but the variable is already inactive, so we don't need to
   * do anything.
  */
  if (!active && !in_subset)
/**/return;

  /* deselect var if tcorr2.nactive > 1 */
  if (active) {
    if (dsp->tcorr2.nactive > 1) {
      for (j=0; j<dsp->tcorr2.nactive; j++) {
        if (jvar == dsp->tcorr2.active_vars.els[j]) 
          break;
      }
      if (j<dsp->tcorr2.nactive-1) {
        for (k=j; k<dsp->tcorr2.nactive-1; k++){
          dsp->tcorr2.active_vars.els[k] = dsp->tcorr2.active_vars.els[k+1];
        }
      }
      dsp->tcorr2.nactive--;

      if (!gg->tourcorr.fade_vars) /* set current position without sel var */
      {
        gt_basis(dsp->tcorr2.Fa, dsp->tcorr2.nactive, dsp->tcorr2.active_vars, 
          d->n_cols, (gint) 1);
        arrayd_copy(&dsp->tcorr2.Fa, &dsp->tcorr2.F);
	/*        copy_mat(dsp->tcorr2.F.vals, dsp->tcorr2.Fa.vals, d->n_cols, 1);*/
      }
      dsp->tcorr2.active_vars_p.els[jvar] = false;

    }
  }
  else { /* not active, so add the variable */
    if (jvar > dsp->tcorr2.active_vars.els[dsp->tcorr2.nactive-1]) {
      dsp->tcorr2.active_vars.els[dsp->tcorr2.nactive] = jvar;
    }
    else if (jvar < dsp->tcorr2.active_vars.els[0]) {
      for (j=dsp->tcorr2.nactive; j>0; j--) {
          dsp->tcorr2.active_vars.els[j] = dsp->tcorr2.active_vars.els[j-1];
      }
      dsp->tcorr2.active_vars.els[0] = jvar;
    }
    else {
      gint jtmp = dsp->tcorr2.nactive;
      for (j=0; j<dsp->tcorr2.nactive-1; j++) {
        if (jvar > dsp->tcorr2.active_vars.els[j] && jvar < dsp->tcorr2.active_vars.els[j+1])
        {
          jtmp = j+1;
          break;
        }
      }
      for (j=dsp->tcorr2.nactive-1;j>=jtmp; j--) 
          dsp->tcorr2.active_vars.els[j+1] = dsp->tcorr2.active_vars.els[j];
      dsp->tcorr2.active_vars.els[jtmp] = jvar;
    }
    dsp->tcorr2.nactive++;
    dsp->tcorr2.active_vars_p.els[jvar] = true;
  }

  dsp->tcorr2.get_new_target = true;
}

static void 
tourcorr_active_var_set (gint jvar, GGobiStage *d, displayd *dsp, GGobiSession *gg)
{
  gboolean in_subsethor = dsp->tcorr1.subset_vars_p.els[jvar];
  gboolean in_subsetver = dsp->tcorr2.subset_vars_p.els[jvar];
  gboolean activehor = dsp->tcorr1.active_vars_p.els[jvar];
  gboolean activever = dsp->tcorr2.active_vars_p.els[jvar];
  gint j, k;

  /* Taking care of horizontal variables first */ 
  if (in_subsethor && activehor && (dsp->tcorr1.nactive>1)) {
    /* remove variable */
    for (j=0; j<dsp->tcorr1.nactive; j++) {
      if (jvar == dsp->tcorr1.active_vars.els[j]) 
        break;
    }
    if (j<dsp->tcorr1.nactive-1) {
      for (k=j; k<dsp->tcorr1.nactive-1; k++){
        dsp->tcorr1.active_vars.els[k] = dsp->tcorr1.active_vars.els[k+1];
      }
    }
    dsp->tcorr1.nactive--;
    if (!gg->tourcorr.fade_vars) /* set current position without sel var */
    {
      gt_basis(dsp->tcorr1.Fa, dsp->tcorr1.nactive, dsp->tcorr1.active_vars, 
        d->n_cols, (gint) 1);
      arrayd_copy(&dsp->tcorr1.Fa, &dsp->tcorr1.F);
    }
    dsp->tcorr1.active_vars_p.els[jvar] = false;
  }
  else if (in_subsethor && !activehor) {
    /* add variable */
    if (jvar > dsp->tcorr1.active_vars.els[dsp->tcorr1.nactive-1]) {
      dsp->tcorr1.active_vars.els[dsp->tcorr1.nactive] = jvar;
    }
    else if (jvar < dsp->tcorr1.active_vars.els[0]) {
      for (j=dsp->tcorr1.nactive; j>0; j--) {
          dsp->tcorr1.active_vars.els[j] = dsp->tcorr1.active_vars.els[j-1];
      }
      dsp->tcorr1.active_vars.els[0] = jvar;
    }
    else {
      gint jtmp = dsp->tcorr1.nactive;
      for (j=0; j<dsp->tcorr1.nactive-1; j++) {
        if (jvar > dsp->tcorr1.active_vars.els[j] && jvar < 
          dsp->tcorr1.active_vars.els[j+1]) {
          jtmp = j+1;
          break;
        }
      }
      for (j=dsp->tcorr1.nactive-1;j>=jtmp; j--) 
          dsp->tcorr1.active_vars.els[j+1] = dsp->tcorr1.active_vars.els[j];
      dsp->tcorr1.active_vars.els[jtmp] = jvar;
    }
    dsp->tcorr1.nactive++;
    dsp->tcorr1.active_vars_p.els[jvar] = true;
  }

  /* Now vertical variables */
  if (in_subsetver && activever && (dsp->tcorr2.nactive>1)) {
    /* remove variable */
    for (j=0; j<dsp->tcorr2.nactive; j++) {
      if (jvar == dsp->tcorr2.active_vars.els[j]) 
        break;
    }
    if (j<dsp->tcorr2.nactive-1) {
      for (k=j; k<dsp->tcorr2.nactive-1; k++) {
        dsp->tcorr2.active_vars.els[k] = dsp->tcorr2.active_vars.els[k+1];
      }
    }
    dsp->tcorr2.nactive--;

    if (!gg->tourcorr.fade_vars) /* set current position without sel var */
    {
      gt_basis(dsp->tcorr2.Fa, dsp->tcorr2.nactive, dsp->tcorr2.active_vars, 
        d->n_cols, (gint) 1);
      arrayd_copy(&dsp->tcorr2.Fa, &dsp->tcorr2.F);
    }
    dsp->tcorr2.active_vars_p.els[jvar] = false;
  }
  else if (in_subsetver && !activever) {
    /* add variable */
    if (jvar > dsp->tcorr2.active_vars.els[dsp->tcorr2.nactive-1]) {
      dsp->tcorr2.active_vars.els[dsp->tcorr2.nactive] = jvar;
    }
    else if (jvar < dsp->tcorr2.active_vars.els[0]) {
      for (j=dsp->tcorr2.nactive; j>0; j--) {
          dsp->tcorr2.active_vars.els[j] = dsp->tcorr2.active_vars.els[j-1];
      }
      dsp->tcorr2.active_vars.els[0] = jvar;
    }
    else {
      gint jtmp = dsp->tcorr2.nactive;
      for (j=0; j<dsp->tcorr2.nactive-1; j++) {
        if (jvar > dsp->tcorr2.active_vars.els[j] && jvar < 
          dsp->tcorr2.active_vars.els[j+1]) {
          jtmp = j+1;
          break;
        }
      }
      for (j=dsp->tcorr2.nactive-1;j>=jtmp; j--) 
          dsp->tcorr2.active_vars.els[j+1] = dsp->tcorr2.active_vars.els[j];
      dsp->tcorr2.active_vars.els[jtmp] = jvar;
    }
    dsp->tcorr2.nactive++;
    dsp->tcorr2.active_vars_p.els[jvar] = true;
  }

  dsp->tcorr1.get_new_target = true;
  dsp->tcorr2.get_new_target = true;
}

void
tourcorr_manip_var_set (gint j, gint btn, GGobiSession *gg)
{
  displayd *dsp = gg->current_display;

  if (btn == 1)
    dsp->tc1_manip_var = j;    
  if (btn == 2)
    dsp->tc2_manip_var = j;
}

gboolean
tourcorr_varsel (GtkWidget *w, gint jvar, gint toggle, gint mouse, GGobiStage *d, GGobiSession *gg)
{
  displayd *dsp = gg->current_display;
  gboolean changed = true;

  if (GTK_IS_TOGGLE_BUTTON(w)) {
    /*-- w is either one of the x or y toggle buttons --*/
    gboolean isx = (w == varpanel_widget_get_nth (VARSEL_X, jvar, d));

    /* add/remove jvar to/from the subset of variables that <may> be active */
    if (isx) 
      changed = tourcorr_subset_horvar_set (jvar, d, dsp, gg);
    else
      changed = tourcorr_subset_vervar_set (jvar, d, dsp, gg);

  } else if (GTK_IS_BUTTON(w)) {
    /*-- w is the label --*/
    if (mouse == 1) 
      changed = tourcorr_subset_horvar_set (jvar, d, dsp, gg);
    else
      changed = tourcorr_subset_vervar_set (jvar, d, dsp, gg);

  } else if (GTK_IS_DRAWING_AREA(w)) {

    if (d->vcirc_ui.jcursor == GDK_HAND2) {
      tourcorr_manip_var_set (jvar, mouse, gg);
      d->vcirc_ui.jcursor = (gint) NULL;
    }
    else {
      tourcorr_active_var_set (jvar, d, dsp, gg);
    }
  }

  return changed;
}

void
tourcorr_projdata(splotd *sp, gdouble **world_data, GGobiStage *d, GGobiSession *gg) {
  gint j, m;
  displayd *dsp = (displayd *) sp->displayptr;
  gdouble tmpf, maxx, maxy;

  if (sp->tourcorr.initmax) {
    sp->tourcorr.maxscreen = 1;
    sp->tourcorr.initmax = false;
  }

  tmpf = 1/sp->tourcorr.maxscreen;
  maxx = sp->tourcorr.maxscreen;
  maxy = sp->tourcorr.maxscreen;
  for (m=0; m<d->n_rows; m++)
  {
    sp->planar[m].x = 0;
    sp->planar[m].y = 0;
    for (j=0; j<d->n_cols; j++)
    {
      sp->planar[m].x += (gdouble)(dsp->tcorr1.F.vals[0][j]*world_data[m][j]);
      sp->planar[m].y += (gdouble)(dsp->tcorr2.F.vals[0][j]*world_data[m][j]);
    }
    sp->planar[m].x *= tmpf;
    sp->planar[m].y *= tmpf;
    if (fabs(sp->planar[m].x) > maxx)
      maxx = fabs(sp->planar[m].x);
    if (fabs(sp->planar[m].y) > maxy)
      maxy = fabs(sp->planar[m].y);
  }

  if ((maxx > 1) || (maxy > 1)) {
    sp->tourcorr.maxscreen = (maxx > maxy) ? maxx : maxy;
    tmpf = 1/tmpf;
  }
}

void tourcorr_snap(GGobiSession *gg)
{
  displayd *dsp = gg->current_display;
  splotd *sp = gg->current_splot;
  GGobiStage *d = dsp->d;
  gint j;
  gdouble rnge;

  for (j=0; j<d->n_cols; j++) {
    rnge = ggobi_variable_get_range(ggobi_stage_get_variable(d, j));
    g_printerr("%f %f\n", dsp->tcorr1.F.vals[0][j]/rnge*sp->scale.x,
	     dsp->tcorr2.F.vals[0][j]/rnge*sp->scale.y);
  }
}

void tourcorr_video(GGobiSession *gg)
{
  displayd *dsp = gg->current_display;
  if (dsp == NULL)
    return;

  dsp->tourcorr_video = !dsp->tourcorr_video;
}

void tourcorr_write_video(GGobiSession *gg) 
{
  displayd *dsp = gg->current_display;
  splotd *sp = gg->current_splot;
  GGobiStage *d = dsp->d;
  gint j;
  GGobiVariable *var;

  g_printerr("%f %f\n",sp->scale.x, sp->scale.y);
  for (j=0; j<d->n_cols; j++) {
    var = ggobi_stage_get_variable(d, j);
    g_printerr("%f %f %f %f\n", dsp->tcorr1.F.vals[0][j], 
      dsp->tcorr2.F.vals[0][j], 
      ggobi_variable_get_min(var), ggobi_variable_get_max(var)
      );
  }
}

/* Variable manipulation */
void
tourcorr_manip_init(gint p1, gint p2, splotd *sp) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  GGobiStage *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  GGobiSession *gg = GGobiFromSPlot(sp);
  gint j;
  gint n1vars = dsp->tcorr1.nactive, n2vars = dsp->tcorr2.nactive;
  gboolean dontdoit = false;

  /* need to turn off tour */
  if (!cpanel->tcorr1.paused && !cpanel->tcorr2.paused) {
    tourcorr_func(CTOFF, gg->current_display, gg);
  }

  dsp->tc1_phi = 0.;
  dsp->tc2_phi = 0.;

  /* gets mouse position */
  dsp->tc1_pos = dsp->tc1_pos_old = p1;
  dsp->tc2_pos = dsp->tc2_pos_old = p2;

  /* initializes indicator for manip var being one of existing vars */
  dsp->tc1_manipvar_inc = false;
  dsp->tc2_manipvar_inc = false;

  /* check if manip var is one of existing vars */
  /* n1vars, n2vars is the number of variables, excluding the
     manip var in hor and vert directions */
  for (j=0; j<dsp->tcorr1.nactive; j++)
    if (dsp->tcorr1.active_vars.els[j] == dsp->tc1_manip_var) {
      dsp->tc1_manipvar_inc = true;
      n1vars--;
    }
  for (j=0; j<dsp->tcorr2.nactive; j++)
    if (dsp->tcorr2.active_vars.els[j] == dsp->tc2_manip_var) {
      dsp->tc2_manipvar_inc = true;
      n2vars--;
    }

  /* make manip basis, from existing projection */
  /* 0 will be the remainder of the projection, and
     1 will be the indicator vector for the manip var */
  for (j=0; j<d->n_cols; j++) {
    dsp->tc1_manbasis.vals[0][j] = dsp->tcorr1.F.vals[0][j];
    dsp->tc1_manbasis.vals[1][j] = 0.;
  }
  for (j=0; j<d->n_cols; j++) {
    dsp->tc2_manbasis.vals[0][j] = dsp->tcorr2.F.vals[0][j];
    dsp->tc2_manbasis.vals[1][j] = 0.;
  }
  dsp->tc1_manbasis.vals[1][dsp->tc1_manip_var]=1.;
  dsp->tc2_manbasis.vals[1][dsp->tc2_manip_var]=1.;

  if (n1vars > 0)
  {
    while (!gram_schmidt(dsp->tc1_manbasis.vals[0],  dsp->tc1_manbasis.vals[1],
      d->n_cols))
    {
       gt_basis(dsp->tcorr1.tv, dsp->tcorr1.nactive, dsp->tcorr1.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->tc1_manbasis.vals[1][j] = dsp->tcorr1.tv.vals[0][j];
    }
  }
  if (n2vars > 0)
  {
    while (!gram_schmidt(dsp->tc2_manbasis.vals[0],  dsp->tc2_manbasis.vals[1],
      d->n_cols))
    {
       gt_basis(dsp->tcorr2.tv, dsp->tcorr2.nactive, dsp->tcorr2.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->tc2_manbasis.vals[1][j] = dsp->tcorr2.tv.vals[0][j];
    }
  } 

  if (dontdoit) {
    disconnect_motion_signal (sp);
  }
}

void
tourcorr_run(displayd *dsp, GGobiSession *gg)
{
  GGobiStage *d = dsp->d;
  /*  cpaneld *cpanel = &dsp->cpanel;*/
  gint i, nv;
  gint pathprob = 0;

  if (!dsp->tcorr1.get_new_target && 
      !reached_target(dsp->tcorr1.tang, dsp->tcorr1.dist_az, 0, 0, 0)) {

    increment_tour(dsp->tcorr1.tinc, dsp->tcorr1.tau, 
      dsp->tcorr1.dist_az, dsp->tcorr1.delta, 
      &dsp->tcorr1.tang, (gint) 1);

    tour_reproject(dsp->tcorr1.tinc, dsp->tcorr1.G, dsp->tcorr1.Ga, 
      dsp->tcorr1.Gz, dsp->tcorr1.F, dsp->tcorr1.Va, d->n_cols, (gint) 1);
  }
  else { /* do final clean-up and get new target */
    if (!dsp->tcorr1.get_new_target) {
      do_last_increment(dsp->tcorr1.tinc, dsp->tcorr1.tau, 
        dsp->tcorr1.dist_az, (gint) 1);
      tour_reproject(dsp->tcorr1.tinc, dsp->tcorr1.G, dsp->tcorr1.Ga, 
        dsp->tcorr1.Gz, dsp->tcorr1.F, dsp->tcorr1.Va, d->n_cols, (gint) 1);
      }
    
    arrayd_copy(&dsp->tcorr1.F, &dsp->tcorr1.Fa);
    /*    copy_mat(dsp->tcorr1.Fa.vals, dsp->tcorr1.F.vals, d->n_cols, 1);*/
    nv = 0;
    for (i=0; i<d->n_cols; i++) 
      if (fabs(dsp->tcorr1.Fa.vals[0][i]) > 0.01) {
        nv++;
      }
    if (nv == 1 && dsp->tcorr1.nactive == 1) /* only generate new dir if num of
                                           active/used variables is > 1 */
      dsp->tcorr1.get_new_target = true;
    else {
      gt_basis(dsp->tcorr1.Fz, dsp->tcorr1.nactive, 
        dsp->tcorr1.active_vars, d->n_cols, 
        (gint) 1);
      pathprob = tour_path(dsp->tcorr1.Fa, dsp->tcorr1.Fz, dsp->tcorr1.F, 
        d->n_cols, (gint) 1, 
        dsp->tcorr1.Ga, dsp->tcorr1.Gz, dsp->tcorr1.G, dsp->tcorr1.lambda, 
        dsp->tcorr1.tv, dsp->tcorr1.Va, dsp->tcorr1.Vz,
        dsp->tcorr1.tau, dsp->tcorr1.tinc, 
        &dsp->tcorr1.dist_az, &dsp->tcorr1.tang);
      if (pathprob == 0) 
        dsp->tcorr1.get_new_target = false;
      else if (pathprob == 1) { /* problems with Fa so need to force a jump */
        tourcorr_scramble(gg);
        pathprob = tour_path(dsp->tcorr1.Fa, dsp->tcorr1.Fz, dsp->tcorr1.F, 
          d->n_cols, (gint) 1, 
          dsp->tcorr1.Ga, dsp->tcorr1.Gz, dsp->tcorr1.G, dsp->tcorr1.lambda, 
          dsp->tcorr1.tv, dsp->tcorr1.Va, dsp->tcorr1.Vz,
          dsp->tcorr1.tau, dsp->tcorr1.tinc, 
          &dsp->tcorr1.dist_az, &dsp->tcorr1.tang);
      }
      else if (pathprob == 2 || pathprob == 3) { /* problems with Fz,
				    so will force a new choice of Fz */
        dsp->tcorr1.get_new_target = true;
      }
    }
  }

  if (!dsp->tcorr2.get_new_target && 
      !reached_target(dsp->tcorr2.tang, dsp->tcorr2.dist_az, 0, 0, 0)) {
    increment_tour(dsp->tcorr2.tinc, dsp->tcorr2.tau, 
      dsp->tcorr2.dist_az, dsp->tcorr2.delta, 
      &dsp->tcorr2.tang, (gint) 1);

    tour_reproject(dsp->tcorr2.tinc, dsp->tcorr2.G, dsp->tcorr2.Ga, 
      dsp->tcorr2.Gz, dsp->tcorr2.F, dsp->tcorr2.Va, d->n_cols, (gint) 1);
  }
  else { /* do final clean-up and get new target */
    if (!dsp->tcorr2.get_new_target) {
      do_last_increment(dsp->tcorr2.tinc, dsp->tcorr2.tau, 
        dsp->tcorr2.dist_az, (gint) 1);
      tour_reproject(dsp->tcorr2.tinc, dsp->tcorr2.G, dsp->tcorr2.Ga, 
        dsp->tcorr2.Gz, dsp->tcorr2.F, dsp->tcorr2.Va, d->n_cols, (gint) 1);
    }
    arrayd_copy(&dsp->tcorr2.F, &dsp->tcorr2.Fa);
    /*    copy_mat(dsp->tcorr2.Fa.vals, dsp->tcorr2.F.vals, d->n_cols, 1);*/
    nv = 0;
    for (i=0; i<d->n_cols; i++) 
      if (fabs(dsp->tcorr2.Fa.vals[0][i]) > 0.01) {
        nv++;
      }
    if (nv == 1 && dsp->tcorr2.nactive == 1) /* only generate new dir if num of
                                           active/used variables is > 1 */
      dsp->tcorr2.get_new_target = true;
    else {
      gt_basis(dsp->tcorr2.Fz, dsp->tcorr2.nactive, dsp->tcorr2.active_vars, 
        d->n_cols, (gint) 1);
      pathprob = tour_path(dsp->tcorr2.Fa, dsp->tcorr2.Fz, dsp->tcorr2.F, 
        d->n_cols, (gint) 1, 
        dsp->tcorr2.Ga, dsp->tcorr2.Gz, dsp->tcorr2.G, dsp->tcorr2.lambda, 
        dsp->tcorr2.tv, dsp->tcorr2.Va, dsp->tcorr2.Vz,
        dsp->tcorr2.tau, dsp->tcorr2.tinc, &dsp->tcorr2.dist_az, 
        &dsp->tcorr2.tang);
      if (pathprob == 0) 
        dsp->tcorr2.get_new_target = false;
      else if (pathprob == 1) { /* problems with Fa so need to force a jump */
        tourcorr_scramble(gg);
        pathprob = tour_path(dsp->tcorr2.Fa, dsp->tcorr2.Fz, dsp->tcorr2.F, 
          d->n_cols, (gint) 1, 
          dsp->tcorr2.Ga, dsp->tcorr2.Gz, dsp->tcorr2.G, dsp->tcorr2.lambda, 
          dsp->tcorr2.tv, dsp->tcorr2.Va, dsp->tcorr2.Vz,
          dsp->tcorr2.tau, dsp->tcorr2.tinc, 
          &dsp->tcorr2.dist_az, &dsp->tcorr2.tang);
      }
      else if (pathprob == 2 || pathprob == 3) { /* problems with Fz,
				    so will force a new choice of Fz */
        dsp->tcorr2.get_new_target = true;
      }
    }
  }
  
  display_tailpipe (dsp, FULL, gg);

  varcircles_refresh (d, gg);
  if (dsp->tourcorr_video) tourcorr_write_video(gg);
}

void
tourcorr_do_step(displayd *dsp, GGobiSession *gg)
{
  tourcorr_run(dsp, gg);
}

gint
tourcorr_idle_func (displayd *dsp)
{
  GGobiSession *gg = GGobiFromDisplay (dsp);
  cpaneld *cpanel = &dsp->cpanel;
  gboolean doit = !cpanel->tcorr1.paused;

  if (doit) {
    tourcorr_run(dsp, gg);
    gdk_flush ();
  }

  return (doit);
}

void tourcorr_func (gboolean state, displayd *dsp, GGobiSession *gg)
{
  splotd *sp = (splotd *) g_list_nth_data (dsp->splots, 0);

  if (state) {
    if (dsp->tcorr1.idled == 0) {
      dsp->tcorr1.idled = g_idle_add_full (G_PRIORITY_LOW,
        (GSourceFunc) tourcorr_idle_func, dsp, NULL);
    }
    gg->tourcorr.idled = 1;
  } else {
    if (dsp->tcorr1.idled) {
      g_source_remove (dsp->tcorr1.idled);
      dsp->tcorr1.idled = 0;
    }
    gg->tourcorr.idled = 0;
  }

  splot_connect_expose_handler (dsp->tcorr1.idled, sp);
}

void tourcorr_reinit(GGobiSession *gg)
{
  int j, m;
  displayd *dsp = gg->current_display;
  GGobiStage *d = dsp->d;
  splotd *sp = gg->current_splot;

  for (j=0; j<d->n_cols; j++) {
    dsp->tcorr1.F.vals[0][j] = 0.;
    dsp->tcorr1.Fa.vals[0][j] = 0.;
  }
  m = dsp->tcorr1.active_vars.els[0];
  dsp->tcorr1.F.vals[0][m] = 1.;
  dsp->tcorr1.Fa.vals[0][m] = 1.;

  dsp->tcorr1.get_new_target = true;

  for (j=0; j<d->n_cols; j++) {
    dsp->tcorr2.F.vals[0][j] = 0.;
    dsp->tcorr2.Fa.vals[0][j] = 0.;
  }
  m = dsp->tcorr2.active_vars.els[0];
  dsp->tcorr2.F.vals[0][m] = 1.;
  dsp->tcorr2.Fa.vals[0][m] = 1.;

  dsp->tcorr2.get_new_target = true;

  sp->tourcorr.initmax = true;

  display_tailpipe (dsp, FULL, gg);

  varcircles_refresh (d, gg);

}

void tourcorr_scramble(GGobiSession *gg)
{
  gint j;
  displayd *dsp = gg->current_display;
  GGobiStage *d = dsp->d;
  /*gint i, nc = d->n_cols;*/

  for (j=0; j<d->n_cols; j++) {
    dsp->tcorr1.F.vals[0][j] = 0.;
    dsp->tcorr1.Fa.vals[0][j] = 0.;
  }

  for (j=0; j<d->n_cols; j++) {
    dsp->tcorr2.F.vals[0][j] = 0.;
    dsp->tcorr2.Fa.vals[0][j] = 0.;
  }

  gt_basis(dsp->tcorr1.Fa, dsp->tcorr1.nactive, dsp->tcorr1.active_vars, 
    d->n_cols, (gint) 1);
  arrayd_copy(&dsp->tcorr1.Fa, &dsp->tcorr1.F);
  /*  copy_mat(dsp->tcorr1.F.vals, dsp->tcorr1.Fa.vals, d->n_cols, 1);*/

  gt_basis(dsp->tcorr2.Fa, dsp->tcorr2.nactive, dsp->tcorr2.active_vars, 
    d->n_cols, (gint) 1);
  arrayd_copy(&dsp->tcorr2.Fa, &dsp->tcorr2.F);
  /*  copy_mat(dsp->tcorr2.F.vals, dsp->tcorr2.Fa.vals, d->n_cols, 1);*/

  dsp->tcorr1.get_new_target = true;
  dsp->tcorr2.get_new_target = true;

  display_tailpipe (dsp, FULL, gg);

  varcircles_refresh (d, gg);
}

void
tourcorr_manip(gint p1, gint p2, splotd *sp, GGobiSession *gg) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  GGobiStage *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  gdouble xcosphi=1., xsinphi=0., ycosphi=1., ysinphi=0.;
  gdouble distx = 0., disty = 0.;
  gdouble denom = (gdouble) MIN(sp->max.x, sp->max.y)/2.;
  gint actual_nxvars = dsp->tcorr1.nactive, actual_nyvars = 
    dsp->tcorr2.nactive;
  gint j;
  gboolean offscreen = false;

  /* check if off the plot window */
  if (p1 > sp->max.x || p1 < 0 ||
      p2 > sp->max.y || p2 <0)
    offscreen = true;

  if (dsp->tc1_manipvar_inc)
    actual_nxvars = dsp->tcorr1.nactive-1;
  if (dsp->tc2_manipvar_inc)
    actual_nyvars = dsp->tcorr2.nactive-1;

  if (!offscreen) {
    dsp->tc1_pos_old = dsp->tc1_pos;
    dsp->tc2_pos_old = dsp->tc2_pos;
  
    dsp->tc1_pos = p1;
    dsp->tc2_pos = p2;

    if (actual_nxvars > 0 || actual_nyvars > 0)
    {
      if (cpanel->tcorr.manip_mode == CMANIP_VERT)
      {
        distx = 0.;
        if (actual_nyvars > 0)
          disty = dsp->tc2_pos_old - dsp->tc2_pos;
      }
      else if (cpanel->tcorr.manip_mode == CMANIP_HOR)
      {
        if (actual_nxvars > 0)
          distx = dsp->tc1_pos - dsp->tc1_pos_old;
        disty = 0.;
      }
      else if (cpanel->tcorr.manip_mode == CMANIP_COMB)
      {
        if (actual_nxvars > 0)
          distx = dsp->tc1_pos - dsp->tc1_pos_old;
        if (actual_nyvars > 0)
          disty = dsp->tc2_pos_old - dsp->tc2_pos;
      }
      else if (cpanel->tcorr.manip_mode == CMANIP_EQUAL)
      {
        if (actual_nxvars > 0)
          distx = dsp->tc1_pos - dsp->tc1_pos_old;
        if (actual_nyvars > 0)
          disty = dsp->tc2_pos_old - dsp->tc2_pos;
        if (fabs(distx) != fabs(disty))
        {
          distx = (distx+disty)/1.414214;
          disty = distx;
        }
      }

      dsp->tc1_phi = dsp->tc1_phi + distx / denom;
      dsp->tc2_phi = dsp->tc2_phi + disty / denom;
  
      xcosphi = (gdouble) cos((gdouble) dsp->tc1_phi);
      xsinphi = (gdouble) sin((gdouble) dsp->tc1_phi);
      if (xcosphi > 1.0)
      {
        xcosphi = 1.0;
        xsinphi = 0.0;
      }
      else if (xcosphi < -1.0)
      {
        xcosphi = -1.0;
        xsinphi = 0.0;
      }
      ycosphi = (gdouble) cos((double) dsp->tc2_phi);
      ysinphi = (gdouble) sin((double) dsp->tc2_phi);
      if (ycosphi > 1.0)
      {
        ycosphi = 1.0;
        ysinphi = 0.0;
      }
      else if (ycosphi < -1.0)
      {
        ycosphi = -1.0;
        ysinphi = 0.0;
      }
    }

    /* generate the projection basis */
    if (actual_nxvars > 0) 
    {
      for (j=0; j<d->n_cols; j++)
        dsp->tcorr1.F.vals[0][j] = xcosphi * dsp->tc1_manbasis.vals[0][j] + 
         xsinphi * dsp->tc1_manbasis.vals[1][j];
    }
 
    if (actual_nyvars > 0)
    {
      for (j=0; j<d->n_cols; j++)
        dsp->tcorr2.F.vals[0][j] = ycosphi * dsp->tc2_manbasis.vals[0][j] + 
         ysinphi * dsp->tc2_manbasis.vals[1][j];
    }

    display_tailpipe (dsp, FULL, gg);
    varcircles_refresh (d, gg);
  }
  else {
    disconnect_motion_signal (sp);
    arrayd_copy(&dsp->tcorr1.F, &dsp->tcorr1.Fa);
    /*    copy_mat(dsp->tcorr1.Fa.vals, dsp->tcorr1.F.vals, d->n_cols, 1);*/
    arrayd_copy(&dsp->tcorr2.F, &dsp->tcorr2.Fa);
    /*    copy_mat(dsp->tcorr2.Fa.vals, dsp->tcorr2.F.vals, d->n_cols, 1);*/
    dsp->tcorr1.get_new_target = true;
    dsp->tcorr2.get_new_target = true;
    if (!cpanel->tcorr1.paused && !cpanel->tcorr2.paused)
      tourcorr_func(CTON, gg->current_display, gg);
  }
}

void
tourcorr_manip_end(splotd *sp) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  cpaneld *cpanel = &dsp->cpanel;
  GGobiSession *gg = GGobiFromSPlot(sp);

  disconnect_motion_signal (sp);

  arrayd_copy(&dsp->tcorr1.F, &dsp->tcorr1.Fa);
  arrayd_copy(&dsp->tcorr2.F, &dsp->tcorr2.Fa);
  dsp->tcorr1.get_new_target = true;
  dsp->tcorr2.get_new_target = true;

  /* need to turn on tour? */
  if (!cpanel->tcorr1.paused && !cpanel->tcorr2.paused) {
    tourcorr_func(CTON, gg->current_display, gg);

    /*-- whenever motion stops, we need a FULL redraw --*/
    display_tailpipe (gg->current_display, FULL, gg);
  }

}

#undef CTON
#undef CTOFF
