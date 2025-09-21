#include <stdio.h>
#include <glib/gi18n.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "my_time.h"
#include "link_list.h"
#include "transact_page.h"
#include "transactions.h"
#include "transaction_list_page.h"

static GtkWidget *entry1 = NULL;                 // TODO make stack passed
static GtkWidget *entry2 = NULL;

GtkTreeStore  *g_transact_list_treestore=NULL;   // TODO make stack passed
GtkWidget     *g_transact_list_treeview=NULL;    // TODO make stack passed

GtkWidget     *g_addTodB=NULL;                   // TODO make stack passed

void check_button_clicked (GtkWidget *widget, gpointer *data) {
  phdl_grp all_hdls = (phdl_grp)data;

  int dbg=1;

  if ( dbg ) {
    g_print( "\n  %s data=%p *data=%p \n" , __FUNCTION__ , data , *data );
    g_print( "  all_hdls = %p \n" ,  all_hdls );
	}
}  // check_button_clicked

static void message_dialog_clicked (GtkButton *button, gpointer user_data) {

  GtkWidget *dialog;
  static gint i = 1;

  phdl_grp all_hdls = (phdl_grp)user_data;

  dialog = gtk_message_dialog_new ( GTK_WINDOW ( all_hdls->parentWin ),
                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK_CANCEL,
                                    "This message box has been popped up the following\n"
                                    "number of times:");

  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), "%d", i);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  i++;
}

static void add_record_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  const gchar *amount_txt;
  const gchar *description_txt;
  int   is_income=0;
  float amount = 0.0;
  static okane_grp this_entry;

  phdl_grp all_hdls = (phdl_grp)user_data;
  pokane_grp head  = (all_hdls->t_lst);

  printf( "  >> E    all_hdls =%p \n" , all_hdls );

  // get the data
  amount_txt      = gtk_entry_get_text( GTK_ENTRY( all_hdls->vbx_hdls->tp_w_amount ) );
  description_txt = gtk_entry_get_text( GTK_ENTRY( all_hdls->vbx_hdls->tp_w_description ) );

  is_income       = gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON( all_hdls->vbx_hdls->tp_w_is_income ) );
  printf( "   is_income = %d \n" , is_income );

  amount = atof ( amount_txt );

  this_entry.entry_nr  = -1;
  this_entry.is_income = is_income;
  this_entry.entry_ts  = get_unix_time_now();
  this_entry.amount    = amount;
  this_entry.in_dB     = 0;

  strcpy( this_entry.description , description_txt );

  printf( "   Got from entry boxes : \n" );
  printf( "     is_INCOME = %d  \n" , is_income );
  printf( "     amount    = %f  \n" , amount );
  printf( "     descr     = %s  \n" , description_txt );

  add_transaction ( &head , &this_entry, 1 );

  if ( 1 ) {
    linked_list_print_okane_grp ( head );
    //getchar();
  }
  // call update list transactions

  add_to_trans_list_treestore( all_hdls->t_lst );
  gtk_widget_show ( g_addTodB );  // TODO make stack passed var !

  printf( "  << Lv   pall_hdls =%p \n" , all_hdls );
} // add_record_clicked

static void cancel_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_transact_page );

  // Go back to home page screen
  gtk_container_add ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );
  gtk_widget_show_all ( all_hdls->parentWin );
}

static void save_db_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;
  pokane_grp head  = (all_hdls->t_lst);

  printf( "  >> E %s , all_hdls->vbox_transact_page = %p \n" , __func__ , all_hdls->vbox_transact_page );
  // SAVE TO DB !
  save_to_dB_transaction( head , 1 );
}


static void interactive_dialog_clicked (GtkButton *button,  gpointer   user_data) {
  GtkWidget *content_area;
  GtkWidget *dialog;
  GtkWidget *hbox;
  GtkWidget *image;
  GtkWidget *table;
  GtkWidget *local_entry1;
  GtkWidget *local_entry2;
  GtkWidget *label;
  GtkWidget *check_btn;

  gint response;

  phdl_grp all_hdls = (phdl_grp)user_data;

  dialog = gtk_dialog_new_with_buttons ("Interactive Dialog",
                                        GTK_WINDOW ( all_hdls->parentWin ),
                                        GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
                                        _("_OK"),
                                        GTK_RESPONSE_OK,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        NULL);

  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
  gtk_box_pack_start (GTK_BOX (content_area), hbox, FALSE, FALSE, 0);

  image = gtk_image_new_from_icon_name ("dialog-question", GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);

  table = gtk_grid_new ();
  gtk_grid_set_row_spacing (GTK_GRID (table), 4);
  gtk_grid_set_column_spacing (GTK_GRID (table), 4);
  gtk_box_pack_start (GTK_BOX (hbox), table, TRUE, TRUE, 0);

  //check_btn = gtk_check_button_new_with_label ( NULL, "_Is Income");
  check_btn = gtk_check_button_new_with_label ( "_Is Income" );
  gtk_grid_attach (GTK_GRID (table), check_btn, 0, 0, 1, 1);

  label = gtk_label_new_with_mnemonic ("A_mount ");
  gtk_grid_attach (GTK_GRID (table), label, 0, 1, 1, 1);
  local_entry1 = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (local_entry1), gtk_entry_get_text (GTK_ENTRY (entry1)));
  gtk_grid_attach (GTK_GRID (table), local_entry1, 1, 1, 1, 1);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), local_entry1);

  label = gtk_label_new_with_mnemonic ("De_scription");
  gtk_grid_attach (GTK_GRID (table), label, 0, 2, 1, 1);

  local_entry2 = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (local_entry2), gtk_entry_get_text (GTK_ENTRY (entry2)));
  gtk_grid_attach (GTK_GRID (table), local_entry2, 1, 2, 1, 1);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), local_entry2);

  gtk_widget_show_all (hbox);
  response = gtk_dialog_run (GTK_DIALOG (dialog));

  if (response == GTK_RESPONSE_OK)
    {
      gtk_entry_set_text (GTK_ENTRY (entry1), gtk_entry_get_text (GTK_ENTRY (local_entry1)));
      gtk_entry_set_text (GTK_ENTRY (entry2), gtk_entry_get_text (GTK_ENTRY (local_entry2)));
    }

  gtk_widget_destroy (dialog);

}  // interactive_dialog_clicked

int create_transaction_page( phdl_grp pall_hdls ) {

  int rc = 0;

//  phdl_grp pall_hdls = *all_hdls;

  if ( pall_hdls != NULL ) {
    if ( pall_hdls->flg->dbg ) {
      printf( "  >> E %s \n" , __FUNCTION__ );
      printf( "    flgs->dbg     = %d \n" , pall_hdls->flg->dbg );
      printf( "    parentWin     = %p \n" , pall_hdls->parentWin );
      printf( "    vbox_transact = %p \n" , pall_hdls->vbox_transact_page );
    }
  }
  else {
      printf( "  >> E %s pall_hdls = NULL  \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_transact_page == NULL ) {
    GtkWidget *frame;
    GtkWidget *vbox;
    GtkWidget *vbox2;
    GtkWidget *hbox;
    GtkWidget *button;
    GtkWidget *table;
    GtkWidget *label;
    GtkWidget *check_btn;

    pall_hdls->vbox_transact_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    if ( pall_hdls->vbox_active != NULL ) {
      // only attach, if no vbox active !
			gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_transact_page );
    }
    printf( "    vbox = %p   , %s L%4d \n" , pall_hdls->vbox_transact_page , __FILE__ , __LINE__ );

    frame = gtk_frame_new ("Dialogs");
    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_transact_page ), frame);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
    gtk_container_add (GTK_CONTAINER (frame), vbox);

    /* Standard message dialog */
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    button = gtk_button_new_with_mnemonic ("_Message Dialog");
    g_signal_connect (button, "clicked",
                        G_CALLBACK (message_dialog_clicked), (gpointer) pall_hdls );
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    gtk_box_pack_start (GTK_BOX (vbox), gtk_separator_new (GTK_ORIENTATION_HORIZONTAL),
                          FALSE, FALSE, 0);

    /* Interactive dialog*/
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    vbox2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    button = gtk_button_new_with_mnemonic ("_Interactive Dialog");
    g_signal_connect (button, "clicked", G_CALLBACK (interactive_dialog_clicked), (gpointer) pall_hdls  );

    gtk_box_pack_start (GTK_BOX (hbox), vbox2, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);

    table = gtk_grid_new ();
    gtk_grid_set_row_spacing (GTK_GRID (table), 4);
    gtk_grid_set_column_spacing (GTK_GRID (table), 4);
    gtk_box_pack_start (GTK_BOX (hbox), table, FALSE, FALSE, 0);

    check_btn = gtk_check_button_new_with_label ( "Is Income");
    pall_hdls->vbx_hdls->tp_w_is_income = check_btn;

    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( check_btn), FALSE );

    g_object_set ( check_btn, "tooltip-text", "Click if item is moeny you RECEIVED", NULL);
    g_signal_connect ( check_btn, "toggled", G_CALLBACK ( check_button_clicked ), (gpointer)pall_hdls );
    gtk_grid_attach (GTK_GRID (table), check_btn, 0, 0, 1, 1);

    label = gtk_label_new_with_mnemonic ("_Amount ");
    gtk_grid_attach (GTK_GRID (table), label, 0, 1, 1, 1);

    entry1 = gtk_entry_new ();
    pall_hdls->vbx_hdls->tp_w_amount = entry1;

    gtk_grid_attach (GTK_GRID (table), entry1, 1, 1, 1, 1);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), entry1);

    label = gtk_label_new_with_mnemonic ("D_escription");
    gtk_grid_attach (GTK_GRID (table), label, 0, 2, 1, 1);

    entry2 = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (table), entry2, 1, 2, 1, 1);
    pall_hdls->vbx_hdls->tp_w_description = entry2;

    // final buttons
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic ("_Add Record");
    g_signal_connect (button, "clicked", G_CALLBACK ( add_record_clicked ), (gpointer) pall_hdls  );

    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic ("C_ancel");
    g_signal_connect (button, "clicked", G_CALLBACK ( cancel_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    g_addTodB = gtk_button_new_with_mnemonic ("SAVE to dB");
    g_signal_connect ( g_addTodB, "clicked", G_CALLBACK ( save_db_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX (hbox), g_addTodB, FALSE, FALSE, 0);

     // add ref to widget so it doesn't get destroyed on container_remove !
     g_object_ref ( pall_hdls->vbox_transact_page );
  } // !if ( pall_hdls->vbox_transact_page == NULL
  else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_transact_page );
  }
  gtk_widget_show_all ( pall_hdls->vbox_transact_page );

  if ( g_addTodB != NULL ) gtk_widget_hide ( g_addTodB ); // Hidden until have added a record

  if ( pall_hdls->flg->dbg ) {
    printf( "  << Lv %s \n" , __FUNCTION__ );
  }

  return rc;
}  //  create_transaction_page

int create_transaction_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  printf( "  E   *all_hdls = %p pall_hdls =%p \n" , *all_hdls , pall_hdls );
  rc = create_transaction_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    printf( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_transact_page );
	}
	*all_hdls = pall_hdls;

	printf( "  Lv   *all_hdls = %p pall_hdls =%p \n" , *all_hdls , pall_hdls );
  return rc;
}
