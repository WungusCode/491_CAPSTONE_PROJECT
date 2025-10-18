#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "indent_print.h"

#include "transact_page.h"
#include "pie_page.h"

#include "transaction_list_page.h"
#include "home_page.h"
#include "setting_page.h"
#include "start_screen.h"

// Added helper to center content in the window using a GtkGrid layout
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

GtkWidget* center_in_page(GtkWidget *child) {
    GtkWidget *alignment = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), child);
    return alignment;
}

 static void on_LogOut_clicked(GtkButton *button, gpointer user_data) {
    phdl_grp all_hdls = (phdl_grp)user_data;
    g_print("[Settings] Log Out clicked\n");
    hide_home_page( all_hdls );
    create_start_screen( all_hdls );
 }

int create_home_screen ( phdl_grp pall_hdls ) {
 if (!pall_hdls) return -1;
    GdkRGBA dark_green = {0.0, 0.4, 0.0, 1.0};

    if (pall_hdls->vbox_home_page == NULL) {
        GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
        gtk_container_set_border_width(GTK_CONTAINER(content), 16);

        GtkWidget *title_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        GtkWidget *title_lbl = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(title_lbl), "<span weight='bold' size='large'>Current Budget</span>");
        gtk_widget_set_halign(title_lbl, GTK_ALIGN_START);
        gtk_widget_set_margin_start(title_lbl, 16);
        gtk_widget_set_margin_bottom(title_lbl, 4);
        gtk_box_pack_start(GTK_BOX(title_row), title_lbl, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(content), title_row, FALSE, FALSE, 0);

        GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        GtkWidget *label = gtk_label_new(" $3,261 ");
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(label, 16);
        gtk_box_pack_start(GTK_BOX(row1), label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(content), row1, FALSE, FALSE, 0);

        GtkWidget *row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        GtkWidget *button = gtk_button_new_with_label("Show more details");
        gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(row2), button, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(content), row2, FALSE, FALSE, 0);

        // Log out button
        GtkWidget *btn_logout = gtk_button_new_with_label("Sign out");
        gtk_widget_set_valign(btn_logout, GTK_ALIGN_CENTER);
        g_signal_connect(btn_logout, "clicked", G_CALLBACK(on_LogOut_clicked), pall_hdls);
        gtk_box_pack_start(GTK_BOX(row2), btn_logout, FALSE, FALSE, 0);

        GtkWidget *row3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

        GtkWidget *chart_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        GtkWidget *chart_img = gtk_image_new_from_file("./resources/libreoffice-chart.png");
        GtkWidget *chart_lbl = gtk_label_new("Pie Chart");
        gtk_widget_set_halign(chart_img, GTK_ALIGN_CENTER);
        gtk_widget_set_vexpand(chart_img, TRUE);
        gtk_widget_set_halign(chart_lbl, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(chart_lbl, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(chart_box), chart_img, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(chart_box), chart_lbl, FALSE, FALSE, 0);
        GtkWidget *chart_btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(chart_btn), chart_box);
        gtk_widget_set_size_request(chart_btn, 200, 200);
        gtk_widget_override_background_color(chart_btn, GTK_STATE_FLAG_NORMAL, &dark_green);
        pall_hdls->vbx_hdls->hp_chart_btn = chart_btn;
        g_object_set(chart_btn, "tooltip-text", "Click to display Pie Chart", NULL);
        g_signal_connect(chart_btn, "clicked", G_CALLBACK(chart_clicked), pall_hdls);
        gtk_box_pack_start(GTK_BOX(row3), chart_btn, FALSE, FALSE, 0);

        GtkWidget *add_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        GtkWidget *add_img = gtk_image_new_from_file("./resources/transaction_64x64.png");
        GtkWidget *add_lbl = gtk_label_new("Add Transaction");
        gtk_widget_set_halign(add_img, GTK_ALIGN_CENTER);
        gtk_widget_set_vexpand(add_img, TRUE);
        gtk_widget_set_halign(add_lbl, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(add_lbl, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(add_box), add_img, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(add_box), add_lbl, FALSE, FALSE, 0);
        GtkWidget *add_btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(add_btn), add_box);
        gtk_widget_set_size_request(add_btn, 200, 200);
        gtk_widget_override_background_color(add_btn, GTK_STATE_FLAG_NORMAL, &dark_green);
        pall_hdls->vbx_hdls->hp_plus_trans_btn = add_btn;
        g_object_set(add_btn, "tooltip-text", "Click to add or delete a transaction", NULL);
        g_signal_connect(add_btn, "clicked", G_CALLBACK(transact_clicked), pall_hdls);
        gtk_box_pack_start(GTK_BOX(row3), add_btn, FALSE, FALSE, 0);

        GtkWidget *list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        GtkWidget *list_img = gtk_image_new_from_file("./resources/list_transactions_64x64.png");
        GtkWidget *list_lbl = gtk_label_new("Transaction List");
        gtk_widget_set_halign(list_img, GTK_ALIGN_CENTER);
        gtk_widget_set_vexpand(list_img, TRUE);
        gtk_widget_set_halign(list_lbl, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(list_lbl, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(list_box), list_img, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(list_box), list_lbl, FALSE, FALSE, 0);
        GtkWidget *list_btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(list_btn), list_box);
        gtk_widget_set_size_request(list_btn, 200, 200);
        gtk_widget_override_background_color(list_btn, GTK_STATE_FLAG_NORMAL, &dark_green);
        pall_hdls->vbx_hdls->hp_list_trans_btn = list_btn;
        g_object_set(list_btn, "tooltip-text", "Click to show all transactions", NULL);
        g_signal_connect(list_btn, "clicked", G_CALLBACK(list_transact_clicked), pall_hdls);
        gtk_box_pack_start(GTK_BOX(row3), list_btn, FALSE, FALSE, 0);

        GtkWidget *settings_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        GtkWidget *settings_img = gtk_image_new_from_file("./resources/settings.png");
        GtkWidget *settings_lbl = gtk_label_new("Settings");
        gtk_widget_set_halign(settings_img, GTK_ALIGN_CENTER);
        gtk_widget_set_vexpand(settings_img, TRUE);
        gtk_widget_set_halign(settings_lbl, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(settings_lbl, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(settings_box), settings_img, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(settings_box), settings_lbl, FALSE, FALSE, 0);
        GtkWidget *settings_btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(settings_btn), settings_box);
        gtk_widget_set_size_request(settings_btn, 200, 200);
        gtk_widget_override_background_color(settings_btn, GTK_STATE_FLAG_NORMAL, &dark_green);
        g_object_set(settings_btn, "tooltip-text", "Click to change app settings", NULL);
        g_signal_connect(settings_btn, "clicked", G_CALLBACK(on_settings_clicked), NULL);
        gtk_box_pack_start(GTK_BOX(row3), settings_btn, FALSE, FALSE, 0);


        gtk_box_pack_start(GTK_BOX(content), row3, FALSE, FALSE, 0);

        pall_hdls->vbox_home_page = center_in_page(content);
        gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_home_page);
        g_object_ref(pall_hdls->vbox_home_page);
    } else {
        gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_home_page);
    }

    gtk_widget_show_all(pall_hdls->vbox_home_page);
    return 0;
}
//   GtkWidget *hbox , *hbox2 , *hbox3, *hbox_spc;
//   GtkWidget *label, *blnk_label;
//   GtkWidget *button;

//   int rc = 0;

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

//   if ( pall_hdls->vbox_home_page == NULL ) {
//     pall_hdls->vbox_home_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

//     if ( pall_hdls->flg->dbg  ) printf( "    vbox = %p  , %s L%4d \n" , pall_hdls->vbox_home_page , __FILE__ , __LINE__  );

//     hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

//     label = gtk_label_new (" $3,261 ");

//     gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

//     gtk_widget_show_all (hbox);

//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox );

//     hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

//     // simple button , TODO make a text_view
//     button = gtk_button_new_with_label ("Show more details");
//     gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
//     gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox2 );

//     hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
//     blnk_label = gtk_label_new ("");
//     gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox_spc );

//     hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
//     blnk_label = gtk_label_new ("");
//     gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox_spc );

//     hbox3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

//     button = gtk_button_new_with_label ("");

//     GtkWidget *image = gtk_image_new_from_file("./resources/libreoffice-chart.png");
//     gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
//     gtk_button_set_image( GTK_BUTTON( button ) , image);

//     pall_hdls->vbx_hdls->hp_chart_btn = button;
//     g_object_set ( button , "tooltip-text", "Click to display Pie Chart", NULL);
//     g_signal_connect (button, "clicked",  G_CALLBACK ( chart_clicked ), (gpointer) pall_hdls );

//     gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

//     button = gtk_button_new_with_label ("");
//     image = gtk_image_new_from_file("./resources/transaction_64x64.png");
//     gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
//     gtk_button_set_image( GTK_BUTTON( button ) , image);

//     pall_hdls->vbx_hdls->hp_plus_trans_btn = button;

//     g_object_set ( button , "tooltip-text", "Click to add or delete a transaction", NULL);
//     g_signal_connect (button, "clicked",  G_CALLBACK ( transact_clicked ), (gpointer) pall_hdls );

//     gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

//     button = gtk_button_new_with_label ("");
//     image = gtk_image_new_from_file("./resources/list_transactions_64x64.png");
//     gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
//     gtk_button_set_image( GTK_BUTTON( button ) , image);

//     pall_hdls->vbx_hdls->hp_list_trans_btn = button;

//     g_object_set ( button , "tooltip-text", "Click to show all transactions", NULL);
//     g_signal_connect (button, "clicked",  G_CALLBACK ( list_transact_clicked ), (gpointer) pall_hdls );

//     gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

//     button = gtk_button_new_with_label ("");
//     image = gtk_image_new_from_file("./resources/settings.png");
//     gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);  // needed for GTK on macOS
//     gtk_button_set_image( GTK_BUTTON( button ) , image);

//     g_signal_connect(button, "clicked", G_CALLBACK(on_settings_clicked), NULL);
//     g_object_set ( button , "tooltip-text", "Click to change app settings", NULL);

//     gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

//     gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_home_page ), hbox3 );

    if ( pall_hdls->vbox_active == NULL ) gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_home_page );
  }  // if !all_hdls->vbox_home_page
  else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_home_page );
  }

//   g_object_ref ( pall_hdls->vbox_home_page );
//   gtk_widget_show_all ( pall_hdls->vbox_home_page );

  if ( pall_hdls->flg->dbg ) {
    LOG_BLOCK_END ( "  << Lv %s \n" , __FUNCTION__ );
  }

//   return rc;
// }  // create_home_screen

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