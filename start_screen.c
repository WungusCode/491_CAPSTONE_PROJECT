#include <stdio.h>
#include <gtk/gtk.h>

#include "base_defs.h"
#include "start_screen.h"
#include "login_screen.h"   // renamed
#include "create_screen.h"  // renamed
#include "home_page.h"

int start_dbg = 0;

static void hide_start_screen ( phdl_grp all_hdls ) {
  gtk_widget_hide( all_hdls->vbox_start_page );
  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_start_page );
}

static void end_program ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_start_screen( all_hdls );
  gtk_main_quit ();
}

static void goto_login ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_start_screen( all_hdls );
  create_login_screen ( all_hdls );     // renamed
}

static void goto_create ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_start_screen( all_hdls );
  create_create_screen ( all_hdls );    // renamed
}

int create_start_screen ( phdl_grp pall_hdls ) {
  GtkWidget *hbox , *hbox2 ;
  GtkWidget *label;
  GtkWidget *button;

  if ( pall_hdls == NULL ) {
    printf( "  >> E %s  , all_hdls = NULL !!! \n" , __FUNCTION__ );
    return -1;
  }

  if ( start_dbg ) {
    printf( "  >> E %s \n" , __FUNCTION__ );
    printf( "    parentWin = %p \n" , pall_hdls->parentWin );
  }

  if ( pall_hdls->vbox_start_page == NULL ) {
    pall_hdls->vbox_start_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_start_page );

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    label = gtk_label_new (" START SCREEN ");
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);
    gtk_widget_show_all (hbox);
    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_start_page ), hbox );

    hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

    // Exit
    button = gtk_button_new_with_label ("Exit");
    gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
    g_object_set ( button , "tooltip-text", "Click to End program", NULL);
    g_signal_connect (button, "clicked",  G_CALLBACK ( end_program ), (gpointer) pall_hdls );
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    // Login
    button = gtk_button_new_with_label ("Login");
    gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
    g_object_set ( button , "tooltip-text", "Go to Login", NULL);
    g_signal_connect (button, "clicked",  G_CALLBACK ( goto_login ), (gpointer) pall_hdls );
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    // Create Account
    button = gtk_button_new_with_label ("Create Account");
    gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
    g_object_set ( button , "tooltip-text", "Go to Create Account", NULL);
    g_signal_connect (button, "clicked",  G_CALLBACK ( goto_create ), (gpointer) pall_hdls );
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_start_page ), hbox2 );
    g_object_ref ( pall_hdls->vbox_start_page );
  } else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_start_page );
  }

  gtk_widget_show_all ( pall_hdls->vbox_start_page );
  return 0;
}

int create_start_screen_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;
  rc = create_start_screen( pall_hdls );
  *all_hdls = pall_hdls;
  return rc;
}

