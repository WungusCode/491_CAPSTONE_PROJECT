#include <stdio.h>
#include <gtk/gtk.h>

#include "base_defs.h"
#include "start_screen.h"
#include "login_screen.h"   // for cross-nav
#include "home_page.h"
#include "create_screen.h"

int three_dbg=0;

// Added a tiny helper to center content in the window using a GtkGrid layout
static GtkWidget* center_in_page(GtkWidget *content) {
  GtkWidget *page = gtk_grid_new();

  GtkWidget *top = gtk_label_new(NULL), *bottom = gtk_label_new(NULL);
  GtkWidget *left = gtk_label_new(NULL), *right = gtk_label_new(NULL);

  /* spacers expand; content stays compact */
  gtk_widget_set_hexpand(top, TRUE);    gtk_widget_set_vexpand(top, TRUE);
  gtk_widget_set_hexpand(bottom, TRUE); gtk_widget_set_vexpand(bottom, TRUE);
  gtk_widget_set_hexpand(left, TRUE);   gtk_widget_set_vexpand(left, TRUE);
  gtk_widget_set_hexpand(right, TRUE);  gtk_widget_set_vexpand(right, TRUE);

  gtk_widget_set_hexpand(content, FALSE);
  gtk_widget_set_vexpand(content, FALSE);

  gtk_grid_attach(GTK_GRID(page), top,     0, 0, 3, 1);
  gtk_grid_attach(GTK_GRID(page), left,    0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(page), content, 1, 1, 1, 1);  /* centered cell */
  gtk_grid_attach(GTK_GRID(page), right,   2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(page), bottom,  0, 2, 3, 1);
  return page;
}


static void hide_create_screen ( phdl_grp all_hdls ) {
  gtk_widget_hide( all_hdls->vbox_create_page );
  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_create_page );
}

static void back_to_start ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_create_screen( all_hdls );
  create_start_screen ( all_hdls );
}

static void goto_login ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_create_screen( all_hdls );
  create_login_screen ( all_hdls );
}

static void c_goto_home( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;
  hide_create_screen( all_hdls );
  create_home_screen ( all_hdls );
}

#if 0
static void on_submit_create ( GtkButton *button , gpointer data ) {
  GtkEntry *u = g_object_get_data(G_OBJECT(button), "u");
  GtkEntry *p = g_object_get_data(G_OBJECT(button), "p");
  const char *user = gtk_entry_get_text(GTK_ENTRY(u));
  const char *pass = gtk_entry_get_text(GTK_ENTRY(p));
  g_print("[Create] username='%s' password='%s'\n", user, pass);
}
#endif

int create_create_screen ( phdl_grp pall_hdls ) {
  GtkWidget *hbox , *hbox2 ;
  GtkWidget *label, *button;
  GtkWidget *user_entry, *pass_entry;

  if ( pall_hdls == NULL ) {
    printf( "  >> E %s  , all_hdls = NULL !!! \n" , __FUNCTION__ );
    return -1;
  }

  if ( three_dbg ) {
    printf( "  >> E %s \n" , __FUNCTION__ );
    printf( "    parentWin = %p \n" , pall_hdls->parentWin );
  }

  if ( pall_hdls->vbox_create_page == NULL ) {
    /* Build compact content (title + inputs + buttons) */
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(content), 16);

    /* Title row */
    GtkWidget *hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *label = gtk_label_new(" CREATE ACCOUNT ");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content), hbox, FALSE, FALSE, 0);

    /* Inputs */
    GtkWidget *v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(v), 12);

    GtkWidget *user_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(user_entry), "Username");
    gtk_box_pack_start(GTK_BOX(v), user_entry, FALSE, FALSE, 0);

    GtkWidget *pass_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pass_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(pass_entry), 0x2022);
    gtk_box_pack_start(GTK_BOX(v), pass_entry, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), v, FALSE, FALSE, 0);

    /* Buttons row */
    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

    // Back to Start
    GtkWidget *button = gtk_button_new_with_label("Back");
    g_signal_connect(button, "clicked", G_CALLBACK(back_to_start), (gpointer)pall_hdls);
    gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 0);

    // Go to Login
    button = gtk_button_new_with_label("Go to Login");
    g_signal_connect(button, "clicked", G_CALLBACK(goto_login), (gpointer)pall_hdls);
    gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 0);

    // Submit
    button = gtk_button_new_with_label("Submit");
    g_object_set_data(G_OBJECT(button), "u", user_entry);
    g_object_set_data(G_OBJECT(button), "p", pass_entry);
    g_signal_connect(button, "clicked", G_CALLBACK(c_goto_home), (gpointer)pall_hdls);
    gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), hbox2, FALSE, FALSE, 0);

    /* Wrap content in centered page and ATTACH TO WINDOW */
    pall_hdls->vbox_create_page = center_in_page(content);
    gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_create_page);

    /* Keep a ref so you can remove/re-add it during navigation */
    g_object_ref(pall_hdls->vbox_create_page);

} else {
    /* When returning, just re-attach the existing page */
    gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_create_page);
}

//   if ( pall_hdls->vbox_create_page == NULL ) {
//     pall_hdls->vbox_create_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

//     // Title row
//     hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
//     label = gtk_label_new (" CREATE ACCOUNT ");
//     gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);
//     gtk_widget_show_all (hbox);
//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_create_page ), hbox );

//     // Inputs
//     GtkWidget *v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
//     gtk_container_set_border_width(GTK_CONTAINER(v), 12);

//     user_entry = gtk_entry_new();
//     gtk_entry_set_placeholder_text(GTK_ENTRY(user_entry), "Username");
//     gtk_box_pack_start(GTK_BOX(v), user_entry, FALSE, FALSE, 0);

//     pass_entry = gtk_entry_new();
//     gtk_entry_set_placeholder_text(GTK_ENTRY(pass_entry), "Password");
//     gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
//     gtk_entry_set_invisible_char(GTK_ENTRY(pass_entry), 0x2022);
//     gtk_box_pack_start(GTK_BOX(v), pass_entry, FALSE, FALSE, 0);

//     gtk_container_add(GTK_CONTAINER(pall_hdls->vbox_create_page), v);

//     // Buttons
//     hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

//     // Back to Start
//     button = gtk_button_new_with_label ("Back");
//     g_signal_connect (button, "clicked",  G_CALLBACK ( back_to_start ), (gpointer) pall_hdls );
//     gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

//     // Go to Login (direct switch)
//     button = gtk_button_new_with_label ("Go to Login");
//     g_signal_connect (button, "clicked",  G_CALLBACK ( goto_login ), (gpointer) pall_hdls );
//     gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

//     // Submit
//     button = gtk_button_new_with_label ("Submit");
//     g_object_set_data(G_OBJECT(button), "u", user_entry);
//     g_object_set_data(G_OBJECT(button), "p", pass_entry);
//     g_signal_connect (button, "clicked",  G_CALLBACK ( c_goto_home ), (gpointer) pall_hdls );
//     gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

//     g_object_ref ( pall_hdls->vbox_create_page );
//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_create_page ), hbox2 );
//   } else {
//     gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_create_page );
//   }

  gtk_widget_show_all ( pall_hdls->vbox_create_page );
  return 0;
}


int create_create_screen_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;
  rc = create_create_screen( pall_hdls );
  *all_hdls = pall_hdls;
  return rc;
}

