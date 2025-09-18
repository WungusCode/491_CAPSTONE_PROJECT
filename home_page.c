#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "custom_pie_widget.h"

// "Close" button
// Destroys the window that's passed in (closes it when button is clicked)
static void on_chart_close(GtkButton *btn, gpointer win) {
  gtk_widget_destroy(GTK_WIDGET(win));
}

// "Spending Chart" window
static void on_show_chart(GtkButton *btn, gpointer data) {
  phdl_grp h = (phdl_grp)data;

  GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "Spending Chart");
  gtk_window_set_default_size(GTK_WINDOW(win), 900, 700);
  gtk_window_set_transient_for(GTK_WINDOW(win), GTK_WINDOW(h->parentWin));
  gtk_window_set_modal(GTK_WINDOW(win), TRUE);

  GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width(GTK_CONTAINER(root), 24);
  gtk_container_add(GTK_CONTAINER(win), root);

  // Hard top spacer (prevents any clipping under the window decoration)
  GtkWidget *top_spacer = gtk_label_new("");
  gtk_widget_set_size_request(top_spacer, -1, 16);
  gtk_box_pack_start(GTK_BOX(root), top_spacer, FALSE, FALSE, 0);

  // Title label OUTSIDE the pie widget
  GtkWidget *title = gtk_label_new("Spending by Category");
  gtk_widget_set_margin_top(title, 10);
  gtk_widget_set_margin_bottom(title, 10);
  PangoFontDescription *font_desc = pango_font_description_from_string("Sans Bold 18");
  gtk_widget_override_font(title, font_desc);
  pango_font_description_free(font_desc);
  gtk_box_pack_start(GTK_BOX(root), title, FALSE, FALSE, 0);

  // Custom pie widget
  GtkWidget *pie = pie_widget_new();

  // IMPORTANT: clear the widget's INTERNAL title so it can't overlap the top
  pie_widget_set_text((PieWidget*)pie, PIE_TITLE_T, "");

  // Demo slices (will replace with real data later)
  pie_widget_add_slice_to_pie((PieWidget*)pie, 40, "#3478F6", "Housing");
  pie_widget_add_slice_to_pie((PieWidget*)pie, 20, "#34C759", "Groceries");
  pie_widget_add_slice_to_pie((PieWidget*)pie, 15, "#FF9500", "Transport");
  pie_widget_add_slice_to_pie((PieWidget*)pie, 10, "#AF52DE", "Utilities");
  pie_widget_add_slice_to_pie((PieWidget*)pie, 15, "#FF3B30", "Dining");

  // Let the pie expand and push it down a bit more
  gtk_widget_set_hexpand(pie, TRUE);
  gtk_widget_set_vexpand(pie, TRUE);
  gtk_widget_set_margin_top(pie, 20);
  gtk_box_pack_start(GTK_BOX(root), pie, TRUE, TRUE, 0);

  // Bottom row with Close
  GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  GtkWidget *close_btn = gtk_button_new_with_label("Close");
  g_signal_connect(close_btn, "clicked", G_CALLBACK(on_chart_close), win);
  gtk_box_pack_end(GTK_BOX(row), close_btn, FALSE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(root), row, FALSE, FALSE, 0);

  gtk_widget_show_all(win);
}

int create_home_screen ( phdl_grp all_hdls ) {

  GtkWidget *vbox, *hbox , *hbox2 , *hbox3, *hbox_spc;
  GtkWidget *label, *blnk_label;
  GtkWidget *button;

  int rc = 0;

  printf( "  >> E %s \n" , __FUNCTION__ );

  if ( all_hdls->flg->dbg ) {
    printf( "  >> E %s \n" , __FUNCTION__ );
    printf( "    flgs->dbg = %d \n" , all_hdls->flg->dbg );
    printf( "    parentWin = %p \n" , all_hdls->parentWin );
  }
  //actions = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER ( all_hdls->parentWin ), vbox);

  printf( "    vbox = %p \n" , vbox );

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

  label = gtk_label_new (" $3,261 ");

  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

  gtk_widget_show_all (hbox);

  gtk_container_add (GTK_CONTAINER ( vbox ), hbox );

  hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

  // simple button , TODO make a text_view
  button = gtk_button_new_with_label ("Show more details");
  gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
  gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( vbox ), hbox2 );

  hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  blnk_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( vbox ), hbox_spc );

  hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  blnk_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( vbox ), hbox_spc );

  hbox3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

  button = gtk_button_new_with_label ("Calendar");

  gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

  button = gtk_button_new_with_label ("Chart");

  gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

  button = gtk_button_new_with_label ("Calculate");

  gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( vbox ), hbox3 );

  gtk_widget_show_all ( vbox );

  if ( all_hdls->flg->dbg ) {
    printf( "  << Lv %s \n" , __FUNCTION__ );
  }

  printf( "  << Lv %s \n" , __FUNCTION__ );

  return rc;
}
