#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "data_types.h"

#include "params.h"
#include "home_page.h"
#include "login_screen.h"
#include "create_screen.h"
#include "start_screen.h"

static app_flags flgs;

static void destroy( GtkWidget *widget,    gpointer   data ) {
  gtk_main_quit ();
}

int main(int argc, char* argv[]) {
  int rc = 0;

  GtkWidget* window;
  hdl_grp   all_hdls;
  phdl_grp  pall_hdls = NULL;

  pall_hdls = &all_hdls;

  gtk_init(&argc, &argv);

  // init screen containers
  pall_hdls->vbox_one   = NULL;  // Start
  pall_hdls->vbox_two   = NULL;  // Login
  pall_hdls->vbox_three = NULL;  // Create
  pall_hdls->vbox_four  = NULL;  // Home

  // window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  pall_hdls->parentWin = window;

  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), WIN_W, WIN_H );

  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);

  // Start on the Start Screen
  create_start_screen_rtn(  &pall_hdls );
  create_login_screen_rtn(  &pall_hdls );
  create_create_screen_rtn(  &pall_hdls );

  gtk_widget_show_all ( window );

  gtk_main();  // blocks until GTK terminates
  return rc;
}

