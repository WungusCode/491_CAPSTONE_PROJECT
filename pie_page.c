#include <stdio.h>
#include <glib/gi18n.h>
#include<gtk/gtk.h>
#include <glib.h>

#include "base_defs.h"
#include "custom_pie_widget.h"

static void cancel_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_chart_page );

  // Go back to home page screen
  gtk_container_add ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );
  gtk_widget_show_all ( all_hdls->parentWin );
}

int create_pie_chart_page( phdl_grp pall_hdls ) {

  int rc = 0;

  if ( pall_hdls != NULL ) {
    if ( pall_hdls->flg->dbg ) {
      printf( "  >> E %s \n" , __FUNCTION__ );
      printf( "    flgs->dbg = %d \n" , pall_hdls->flg->dbg );
      printf( "    parentWin = %p \n" , pall_hdls->parentWin );
    }
  }
  else {
     printf( "  >> E %s  , pall_hdls = NULL !!! \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_chart_page == NULL ) {
    typedef struct {
      const char *name;
      const char *color;
      int amount;
    } CategorySpending;
//issues with being able to print the title right about herem and other things
    CategorySpending categories[] = {
      {"Rent", "#F44336", 40},
      {"Food", "#2196F3", 25},
      {"Transport", "#4CAF50", 15},
      {"Entertainment", "#FFEB3B", 10},
      {"Others", "#9C27B0", 10}
    };
    int num_categories = sizeof(categories) / sizeof(CategorySpending);

    GtkWidget *piMy;
    GtkWidget *hbox , *button;
    GtkWidget *title_lbl;

    pall_hdls->vbox_chart_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    printf( "    vbox = %p  , %s L%4d \n" , pall_hdls->vbox_chart_page , __FILE__ , __LINE__  );

    title_lbl = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_lbl), "<b>Spending Categories</b>");
    gtk_widget_set_halign(title_lbl, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(title_lbl, 8);
    gtk_widget_set_margin_bottom(title_lbl, 4);
    gtk_box_pack_start(GTK_BOX(pall_hdls->vbox_chart_page), title_lbl, FALSE, FALSE, 0);

    piMy = pie_widget_new();
    pie_widget_set_text((PieWidget*)piMy, GLG_TITLE_T, "");
    gtk_widget_set_size_request(piMy, 520, 420);
    gtk_widget_set_halign(piMy, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(piMy, GTK_ALIGN_START);
    gtk_box_pack_start( GTK_BOX( pall_hdls->vbox_chart_page ), piMy, TRUE, TRUE, 0);

    for (int i = 0; i < num_categories; i++) {
      pie_widget_add_slice_to_pie((PieWidget *) piMy, categories[i].amount, categories[i].color, categories[i].name);
    }

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_end (GTK_BOX ( pall_hdls->vbox_chart_page ), hbox, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic ("_Done");
    pall_hdls->vbx_hdls->cp_pie_done_btn = button;

    g_signal_connect (button, "clicked", G_CALLBACK ( cancel_clicked ), (gpointer) pall_hdls  );

    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    // add ref to widget so it doesn't get destroyed on container_remove !
    g_object_ref ( pall_hdls->vbox_chart_page );

  }
 else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_chart_page );
  }
  gtk_widget_show_all ( pall_hdls->vbox_chart_page );

  if ( pall_hdls->flg->dbg ) printf( "  Lv  %s pall_hdls =%p \n" , __FUNCTION__ , pall_hdls );

  return rc;
} // create_pie_chart_pagehi

int create_pie_chart_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  printf( "  E  %s  ,  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  rc = create_pie_chart_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    printf( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_transact_page );

  *all_hdls = pall_hdls;

  if ( pall_hdls->flg->dbg ) printf( "  Lv %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  return rc;
}
