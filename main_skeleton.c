#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "data_types.h"
#include "home_page.h"
#include "setting_page.h"

#include "params.h"

static app_flags flgs;

static void destroy( GtkWidget *widget,    gpointer   data );

int main(int argc, char* argv[]) {
  int rc = 0;

  rc = parse_cmdline( argc, argv , &flgs );
  if ( rc == 0 ) {
    GtkWidget* window;
    hdl_grp   all_hdls;
    phdl_grp  pall_hdls = NULL;

    pall_hdls = &all_hdls;

    pall_hdls->flg = &flgs;

    gtk_init(&argc, &argv);

    // code
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    pall_hdls->parentWin = window;

    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), WIN_W, WIN_H );

    if ( pall_hdls->flg->dbg ) {
      printf( "  window = %p \n" , window );
    }
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    gtk_widget_show_all ( window );

    gtk_main();  // blocks until GTK terminates
  }
  return rc;
}

static void destroy( GtkWidget *widget, gpointer   data ) {
  gtk_main_quit ();
}
