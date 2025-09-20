#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "data_types.h"
#include "link_list.h"

#include "params.h"
#include "start_screen.h"
#include "create_screen.h"
#include "login_screen.h"
#include "home_page.h"
#include "start_screen.h"
#include "create_screen.h"
#include "login_screen.h"
#include "transact_page.h"
#include "custom_pie_widget.h"
//#include "pie_page.h"

#include "database/my_sql.h"
#include "transactions.h"
#include "transaction_list_page.h"

static app_flags flgs;

static void destroy( GtkWidget *widget,    gpointer   data ) {
  gtk_main_quit ();
}

int main(int argc, char* argv[]) {
  int rc = 0;

  rc = parse_cmdline( argc, argv , &flgs );
  if ( rc == 0 ) {
    GtkWidget* window;
    static hdl_grp   all_hdls;
    phdl_grp  pall_hdls = NULL;
    GtkWidget* window;
    hdl_grp   all_hdls;
    phdl_grp  pall_hdls = NULL;

    pall_hdls = &all_hdls;

    gtk_init(&argc, &argv);

    // init t_lst
    pall_hdls->t_lst = NULL;

    gtk_init(&argc, &argv);

    // code
    pall_hdls->vbox_active         = NULL;
    pall_hdls->vbox_start_page     = NULL;
    pall_hdls->vbox_create_page    = NULL;
    pall_hdls->vbox_login_page     = NULL;
    pall_hdls->vbox_home_page      = NULL;
    pall_hdls->vbox_start_page     = NULL;
    pall_hdls->vbox_create_page    = NULL;
    pall_hdls->vbox_login_page     = NULL;
    pall_hdls->vbox_transact_page  = NULL;
    pall_hdls->vbox_chart_page     = NULL;
    pall_hdls->vbox_t_history_page = NULL;

    pall_hdls->vbx_hdls = malloc ( sizeof ( uiHdl ) );
    printf( "  pall_hdls->vbx_hdls = %p \n" , pall_hdls->vbx_hdls );  // RM
    memset( pall_hdls->vbx_hdls, 0 , sizeof ( uiHdl ) );
    printf( "  pall_hdls->vbx_hdls = %p \n" , pall_hdls->vbx_hdls ); // RM

    pall_hdls->vbx_hdls->hp_chart_btn      = NULL;
    pall_hdls->vbx_hdls->hp_plus_trans_btn = NULL;
    pall_hdls->vbx_hdls->hp_list_trans_btn = NULL;
    pall_hdls->vbx_hdls->thp_xx = NULL;

    pall_hdls->vbx_hdls->tp_w_is_income   = NULL;
    pall_hdls->vbx_hdls->tp_w_amount      = NULL;
    pall_hdls->vbx_hdls->tp_w_description = NULL;

    pall_hdls->vbx_hdls->thp_xx = NULL;
    pall_hdls->vbx_hdls->cp_xx  = NULL;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    pall_hdls->parentWin = window;

    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), WIN_W, WIN_H );

    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);

    get_data_from_db ( &pall_hdls );

    create_start_screen_rtn             (  &pall_hdls );
    create_login_screen_rtn             (  &pall_hdls );
    create_create_screen_rtn            (  &pall_hdls );
    create_home_screen                  (  &pall_hdls );
    create_transaction_page_rtn         (  &pall_hdls );
    //create_pie_chart_page_rtn           (  &pall_hdls );

    printf( "  pall_hdls = %p pall_hdls->flg = %p pall_hdls->flg->dbg = %d \n" , pall_hdls, pall_hdls->flg, pall_hdls->flg->dbg );
    create_transaction_history_page_rtn (  &pall_hdls );

    //set_all_hdls_from ( &all_hdls, __FUNCTION__ , __LINE__  );

  gtk_widget_show_all ( window );

  gtk_main();  // blocks until GTK terminates
  return rc;
}

