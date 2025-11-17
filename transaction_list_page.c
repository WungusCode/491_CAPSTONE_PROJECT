#include <stdio.h>
#include <string.h>

#include "base_defs.h"
#include "indent_print.h"

#include "link_list.h"
#include "my_time.h"

#include "transaction_list_page.h"
#include "transaction_list_view.h"

static void done_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_t_history_page );

  // Go back to home page screen
  gtk_container_add ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );
  gtk_widget_show_all ( all_hdls->parentWin );
}

int create_transaction_history_page( phdl_grp pall_hdls ) {

  int rc = 0;
  int dbg;

  if ( pall_hdls != NULL ) {
    dbg = pall_hdls->flg->dbg;  // make it local

    dbg = 1;  // DEV override , TODO RM !
							//
    if ( dbg ) {
      LOG_BLOCK_START ( "  >> E %s \n" , __FUNCTION__ );
      LOG_INDENTED ( "    flgs->dbg     = %d \n" , pall_hdls->flg->dbg );
      LOG_INDENTED ( "    parentWin     = %p \n" , pall_hdls->parentWin );
      LOG_INDENTED ( "    vbox_transact = %p \n" , pall_hdls->vbox_t_history_page );
    }
  }
  else {
      LOG_BLOCK_START ( "  >> E %s pall_hdls = NULL  \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_t_history_page == NULL ) {
    GtkWidget *frame, *vbox;
    GtkWidget  *hbox, *button;
    GtkWidget  *scrolledwindow, *table;

    pall_hdls->vbox_t_history_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    LOG_INDENTED ( "    vbox_t_history_page = %p   , %s L%4d \n" , pall_hdls->vbox_t_history_page , __FILE__ , __LINE__ );

/* LAYOUT
   |-------------------------------------------|
   | vbox                                      |
   |   hbox                                    |
   |     frame                                 |
   |       table                               |
   |         scrolledwindow                    |
   |           view                            |
   |             sorted list                   |
   |   hbox                                    |
   |     button                                |
   |-------------------------------------------|
*/
    if ( pall_hdls->vbox_active == NULL ) {
      // only attach, if no vbox active !
      gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_t_history_page );
    }

    frame = gtk_frame_new ("Trans List");
    gtk_widget_set_size_request( frame, WIN_W - 200, WIN_H - 40 );
    gtk_container_set_border_width (GTK_CONTAINER ( frame ), 1);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_t_history_page ), frame);

    vbox   = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_add (GTK_CONTAINER (frame) , vbox);

    table = gtk_table_new( 20,1, FALSE /* TRUE */);        // 6x6 table to be put in frame
    gtk_widget_set_size_request( table, WIN_W - 220 , WIN_H - 60 );

    gtk_box_pack_start (GTK_BOX ( vbox ), table, FALSE, FALSE, 0);

    scrolledwindow = gtk_scrolled_window_new(NULL, NULL );
    gtk_widget_show( scrolledwindow );
    gtk_table_attach(GTK_TABLE(table), scrolledwindow, 0,2,0,12, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL ,0,0 );

    // make the tree list
    {
      void * tmpStore=NULL;
      tmpStore = create_trans_list_store ( pall_hdls , -1 , dbg );
      LOG_INDENTED ( "  tmpStore=%p vbx_hdls->tlp_t_lst_store = %p \n" , tmpStore , pall_hdls->vbx_hdls->tlp_t_lst_store );
    }

    pall_hdls->vbx_hdls->tlp_treeView  = GTK_WIDGET ( create_trans_listview ( pall_hdls , -1 , dbg ) );
    LOG_INDENTED ( "  vbx_hdls->tlp_treeStore = %p , vbx_hdls->tlp_treeView=%p \n" , pall_hdls->vbx_hdls->tlp_treeStore , pall_hdls->vbx_hdls->tlp_treeView );

    gtk_container_add(GTK_CONTAINER(scrolledwindow), pall_hdls->vbx_hdls->tlp_treeView );      // add view to scrolled window

    hbox   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    button = gtk_button_new_with_label ("DONE");
    g_signal_connect (button, "clicked", G_CALLBACK ( done_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX ( hbox ), button, FALSE, FALSE, 0);

    // for auto test
    pall_hdls->vbx_hdls->tlp_list_trans_done_btn = button;
    gtk_box_pack_end (GTK_BOX ( vbox ), hbox, FALSE, FALSE, 0);

    // add ref to widget so it doesn't get destroyed on container_remove !
    g_object_ref ( pall_hdls->vbox_t_history_page );
  } // !if ( pall_hdls->vbox_t_history_page == NULL
  else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_t_history_page );
  }
  gtk_widget_show_all ( pall_hdls->vbox_t_history_page );

  if ( dbg ) {
    LOG_BLOCK_END ( "  << Lv %s \n" , __FUNCTION__ );
  }
  return rc;
} // create_transaction_history_page

int create_transaction_history_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  LOG_BLOCK_START ( "  >> E  %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  rc = create_transaction_history_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    LOG_INDENTED ( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_t_history_page );
  }
  *all_hdls = pall_hdls;

  LOG_BLOCK_END ( "  << Lv %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  // TODO RM

  return rc;
}  // create_transaction_history_page_rtn

