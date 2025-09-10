#ifndef _BASE_DEFS_H

#define _BASE_DEFS_H 1

#include <gtk/gtk.h>
#include <gmodule.h>

#include "data_types.h"

#define WIN_W 720
#define WIN_H 480

// keep at bottom
typedef struct _hdl_grp {
  GtkWidget *parentWin;

  papp_flags flg;
} hdl_grp, *phdl_grp;


#endif
