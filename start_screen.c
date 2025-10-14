#include <stdio.h>
#include <gtk/gtk.h>

#include "base_defs.h"
#include "start_screen.h"
#include "login_screen.h"   // renamed
#include "create_screen.h"  // renamed
#include "home_page.h"

int start_dbg = 0;

// New function to center content in the window using a GtkGrid layout
static GtkWidget* center_in_page(GtkWidget *content) {
  GtkWidget *page = gtk_grid_new();

  GtkWidget *top = gtk_label_new(NULL), *bottom = gtk_label_new(NULL);
  GtkWidget *left = gtk_label_new(NULL), *right = gtk_label_new(NULL);

  gtk_widget_set_hexpand(top, TRUE);    gtk_widget_set_vexpand(top, TRUE);
  gtk_widget_set_hexpand(bottom, TRUE); gtk_widget_set_vexpand(bottom, TRUE);
  gtk_widget_set_hexpand(left, TRUE);   gtk_widget_set_vexpand(left, TRUE);
  gtk_widget_set_hexpand(right, TRUE);  gtk_widget_set_vexpand(right, TRUE);

  gtk_widget_set_hexpand(content, FALSE);
  gtk_widget_set_vexpand(content, FALSE);

  gtk_grid_attach(GTK_GRID(page), top,     0, 0, 3, 1);
  gtk_grid_attach(GTK_GRID(page), left,    0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(page), content, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(page), right,   2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(page), bottom,  0, 2, 3, 1);

  return page;
}

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
  create_login_screen_rtn( &all_hdls );     // using _rtn now
}

static void goto_create ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_start_screen( all_hdls );
  create_create_screen_rtn (&all_hdls );    // using _rtn now
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

if (pall_hdls->vbox_start_page == NULL) {
    /* Build a compact content block (title + buttons row) */
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(content), 16);

    /* Title */
    GtkWidget *title_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *label     = gtk_label_new(" START SCREEN ");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(title_row), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content),   title_row, FALSE, FALSE, 0);

    /* Buttons row */
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 24);

    // Exit
    GtkWidget *button = gtk_button_new_with_label("Exit");
    pall_hdls->vbx_hdls->sp_exit_btn = button;
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    g_object_set(button, "tooltip-text", "Click to End program", NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(end_program), pall_hdls);
    gtk_box_pack_start(GTK_BOX(row), button, FALSE, FALSE, 0);

    // Login
    button = gtk_button_new_with_label("Login");
    pall_hdls->vbx_hdls->sp_login_btn = button;
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    g_object_set(button, "tooltip-text", "Go to Login", NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(goto_login), pall_hdls);
    gtk_box_pack_start(GTK_BOX(row), button, FALSE, FALSE, 0);

    // Create Account
    button = gtk_button_new_with_label("Create Account");
    pall_hdls->vbx_hdls->sp_create_btn = button;
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    g_object_set(button, "tooltip-text", "Go to Create Account", NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(goto_create), pall_hdls);
    gtk_box_pack_start(GTK_BOX(row), button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), row, FALSE, FALSE, 0);

    /* Wrap content in a centered grid and attach to window */
    pall_hdls->vbox_start_page = center_in_page(content);
    gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_start_page);
    g_object_ref(pall_hdls->vbox_start_page);

} else {
    gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_start_page);
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

