#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>

#include "base_defs.h"
#include "start_screen.h"
#include "login_screen.h"   // for cross-nav
#include "home_page.h"
#include "create_screen.h"

int three_dbg=0;

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

// static void c_goto_home( GtkButton *button , gpointer data ) {
//   phdl_grp all_hdls = (phdl_grp)data;
//   hide_create_screen( all_hdls );
//   create_home_screen ( all_hdls );
// }

static gboolean is_blank(const char *input) {
    if (!input) return TRUE;
    while (*input) {
        if (!g_ascii_isspace((guchar)*input)) 
        return FALSE;
        input++;
    }
    return TRUE;
}

static void on_submit_create ( GtkButton *button , gpointer data ) {
 
  phdl_grp all_hdls = (phdl_grp)data;

  GtkEntry *u = g_object_get_data(G_OBJECT(button), "u");
  GtkEntry *p = g_object_get_data(G_OBJECT(button), "p");

  const char *user = gtk_entry_get_text(GTK_ENTRY(u));
  const char *pass = gtk_entry_get_text(GTK_ENTRY(p));
  
  if (is_blank(user) || is_blank(pass)) {
    GtkWidget *dlg = gtk_message_dialog_new (
      GTK_WINDOW(all_hdls->parentWin),
      GTK_DIALOG_MODAL,
      GTK_MESSAGE_WARNING,
      GTK_BUTTONS_OK,  
      "Please enter a username and a password to create account."
    );
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
    return;
  } 

  g_free(all_hdls->current_username);
  g_free(all_hdls->current_password);

  all_hdls->current_username = g_strdup(user);
  all_hdls->current_password = g_strdup(pass);

  hide_create_screen(all_hdls);
  create_home_screen(all_hdls);

  g_print("[Create] username='%s' password='%s'\n", user, pass);
}

int create_create_screen ( phdl_grp pall_hdls ) {
  GtkWidget *hbox , *hbox2;
  GtkWidget *label, *button;
  GtkWidget *name_entry, *email_entry, *user_entry, *pass_entry, *phone_entry;

  if ( pall_hdls == NULL ) {
    printf( "  >> E %s  , all_hdls = NULL !!! \n" , __FUNCTION__ );
    return -1;
  }

  if ( three_dbg ) {
    printf( "  >> E %s \n" , __FUNCTION__ );
    printf( "    parentWin = %p \n" , pall_hdls->parentWin );
  }

  if ( pall_hdls->vbox_create_page == NULL ) {
    pall_hdls->vbox_create_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    // Title row
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    label = gtk_label_new (" CREATE ACCOUNT ");
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);
    gtk_widget_show_all (hbox);
    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_create_page ), hbox );

    // Inputs
    GtkWidget *v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(v), 12);

    name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Full Name");
    gtk_box_pack_start(GTK_BOX(v), name_entry, FALSE, FALSE, 0);

    email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "Email");
    gtk_box_pack_start(GTK_BOX(v), email_entry, FALSE, FALSE, 0);
    
    user_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(user_entry), "Username");
    gtk_box_pack_start(GTK_BOX(v), user_entry, FALSE, FALSE, 0);

    pass_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pass_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(pass_entry), 0x2022);
    gtk_box_pack_start(GTK_BOX(v), pass_entry, FALSE, FALSE, 0);

    phone_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(phone_entry), "Phone Number");
    gtk_box_pack_start(GTK_BOX(v), phone_entry, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(pall_hdls->vbox_create_page), v);

    // Buttons
    hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

    // Back to Start
    button = gtk_button_new_with_label ("Back");
    g_signal_connect (button, "clicked",  G_CALLBACK ( back_to_start ), (gpointer) pall_hdls );
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    // Go to Login (direct switch)
    button = gtk_button_new_with_label ("Go to Login");
    g_signal_connect (button, "clicked",  G_CALLBACK ( goto_login ), (gpointer) pall_hdls );
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    // Submit
    button = gtk_button_new_with_label ("Submit");
    g_object_set_data(G_OBJECT(button), "u", user_entry);
    g_object_set_data(G_OBJECT(button), "p", pass_entry);
    // g_signal_connect (button, "clicked",  G_CALLBACK ( c_goto_home ), (gpointer) pall_hdls );
    g_signal_connect (button, "clicked", G_CALLBACK(on_submit_create), (gpointer)pall_hdls);
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    g_object_ref ( pall_hdls->vbox_create_page );
    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_create_page ), hbox2 );
  } else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_create_page );
  }

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
