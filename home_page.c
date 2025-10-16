#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "indent_print.h"

#include "transact_page.h"
#include "pie_page.h"

#include "transaction_list_page.h"

#include "setting_page.h"

static void hide_home_page( phdl_grp all_hdls ) {
  gtk_widget_hide( all_hdls->vbox_home_page );

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );

}

static void chart_clicked ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;

  hide_home_page( all_hdls );

  create_pie_chart_page( all_hdls );
}

static void transact_clicked ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;

  hide_home_page( all_hdls );

  create_transaction_page( all_hdls );
}

static void list_transact_clicked ( GtkButton *button , gpointer data ) {
  phdl_grp all_hdls = (phdl_grp)data;

  hide_home_page( all_hdls );

  create_transaction_history_page ( all_hdls );
}

static void on_settings_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn; (void)user_data; // unused
    create_setting_page();
}

int create_home_screen ( phdl_grp pall_hdls ) {

  GtkWidget *hbox , *hbox2 , *hbox3, *hbox_spc;
  GtkWidget *label, *blnk_label;
  GtkWidget *button;

  int rc = 0;

  if ( pall_hdls != NULL ) {
    if ( pall_hdls->flg->dbg ) {
      LOG_BLOCK_START ( "  >> E %s \n" , __FUNCTION__ );
      LOG_INDENTED ( "    flgs->dbg = %d \n" , pall_hdls->flg->dbg );
      LOG_INDENTED ( "    parentWin = %p \n" , pall_hdls->parentWin );
    }
  }
  else {
     LOG_BLOCK_START ( "  >> E %s  , all_hdls = NULL !!! \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_home_page == NULL ) {
    pall_hdls->vbox_home_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    if ( pall_hdls->flg->dbg  ) printf( "    vbox = %p  , %s L%4d \n" , pall_hdls->vbox_home_page , __FILE__ , __LINE__  );

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

    label = gtk_label_new (" $3,261 ");

    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

    gtk_widget_show_all (hbox);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox );

    hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

    // simple button , TODO make a text_view
    button = gtk_button_new_with_label ("Show more details");
    gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
    gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox2 );

    hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    blnk_label = gtk_label_new ("");
    gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox_spc );

    hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    blnk_label = gtk_label_new ("");
    gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox_spc );

    hbox3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

    button = gtk_button_new_with_label ("");

    GtkWidget *image = gtk_image_new_from_file("./resources/libreoffice-chart.png");
    gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
    gtk_button_set_image( GTK_BUTTON( button ) , image);

    pall_hdls->vbx_hdls->hp_chart_btn = button;
    g_object_set ( button , "tooltip-text", "Click to display Pie Chart", NULL);
    g_signal_connect (button, "clicked",  G_CALLBACK ( chart_clicked ), (gpointer) pall_hdls );

    gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

    button = gtk_button_new_with_label ("");
    image = gtk_image_new_from_file("./resources/transaction_64x64.png");
    gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
    gtk_button_set_image( GTK_BUTTON( button ) , image);

    pall_hdls->vbx_hdls->hp_plus_trans_btn = button;

    g_object_set ( button , "tooltip-text", "Click to add or delete a transaction", NULL);
    g_signal_connect (button, "clicked",  G_CALLBACK ( transact_clicked ), (gpointer) pall_hdls );

    gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

    button = gtk_button_new_with_label ("");
    image = gtk_image_new_from_file("./resources/list_transactions_64x64.png");
    gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
    gtk_button_set_image( GTK_BUTTON( button ) , image);

    pall_hdls->vbx_hdls->hp_list_trans_btn = button;

    g_object_set ( button , "tooltip-text", "Click to show all transactions", NULL);
    g_signal_connect (button, "clicked",  G_CALLBACK ( list_transact_clicked ), (gpointer) pall_hdls );

    gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

    button = gtk_button_new_with_label ("");
    image = gtk_image_new_from_file("./resources/settings.png");
    gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
    gtk_button_set_image( GTK_BUTTON( button ) , image);

    g_signal_connect(button, "clicked", G_CALLBACK(on_settings_clicked), NULL);
    g_object_set ( button , "tooltip-text", "Click to change app settings", NULL);

    gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox3 );

  }  // if !all_hdls->vbox_home_page
  else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_home_page );
  }

  g_object_ref ( pall_hdls->vbox_home_page );
  gtk_widget_show_all ( pall_hdls->vbox_home_page );

  if ( pall_hdls->flg->dbg ) {
    LOG_BLOCK_END ( "  << Lv %s \n" , __FUNCTION__ );
  }

  return rc;
}  // create_home_screen

int create_home_screen_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  printf( "  E  %s  *all_hdls = %p pall_hdls =%p \n" , __func__ , *all_hdls , pall_hdls );
  rc = create_home_screen( pall_hdls );

  if ( pall_hdls != NULL ) {
    printf( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_home_page );
  }
  *all_hdls = pall_hdls;

  printf( "  Lv  %s *all_hdls = %p pall_hdls =%p \n" , __func__, *all_hdls , pall_hdls );
  return rc;
}
