#ifndef _BASE_DEFS_H

#define _BASE_DEFS_H 1

#include <gtk/gtk.h>
#include <gmodule.h>

#include "data_types.h"
//Window Size
#define WIN_W 720
#define WIN_H 480

// Shared handle passed between screens
typedef struct _hdl_grp {
  GtkWidget *parentWin;  // main window (set in main.c)

  // Screen containers (created on first visit, re-used later)
  GtkWidget *vbox_one;   // Start Screen (was "screen_one")
  GtkWidget *vbox_two;   // Login Screen
  GtkWidget *vbox_three; // Create Account Screen
  GtkWidget *vbox_four;  // Home Page
  
  papp_flags flg;
} hdl_grp, *phdl_grp;


#endif
