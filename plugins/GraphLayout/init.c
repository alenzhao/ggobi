#include <gtk/gtk.h>
#include "ggobi.h"
#include "externs.h"
#include "GGobiAPI.h"

#include "plugin.h"

#include "glayout.h"

void
glayout_init (glayoutd *gl) {

  arrayd_init_null (&gl->dist);
  arrayd_init_null (&gl->pos);

  gl->e = NULL;
  gl->dsrc = NULL;

  gl->centerNodeIndex = -1;
  gl->radial = NULL;

  gl->neato_dim = 2;
}
