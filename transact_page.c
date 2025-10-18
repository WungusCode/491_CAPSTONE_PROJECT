#include <stdio.h>
#include <glib/gi18n.h>
#include<gtk/gtk.h>

#include <gdk/gdkkeysyms.h> // for key press

#include "base_defs.h"
#include "indent_print.h"

#include "my_time.h"
#include "link_list.h"
#include "transact_page.h"
#include "transactions.h"
#include "transaction_list_view.h"

static GtkWidget *entry1 = NULL;                 // TODO make stack passed
static GtkWidget *entry2 = NULL;

GtkTreeStore  *g_transact_list_treestore=NULL;   // TODO make stack passed
GtkWidget     *g_transact_list_treeview=NULL;    // TODO make stack passed

void check_button_clicked (GtkWidget *widget, gpointer *data) {
  phdl_grp all_hdls = (phdl_grp)data;

  int dbg=1;

  if ( dbg ) {
    g_print( "\n  %s data=%p *data=%p \n" , __FUNCTION__ , data , *data );
    g_print( "  all_hdls = %p \n" ,  all_hdls );
	}
}  // check_button_clicked

typedef struct _str_lib {
  char descrip [31];
  int  cat;
} str_lib, *pstr_lib;

#define NR_ENTRIES 30

str_lib cat_look_up[ NR_ENTRIES ] = {
  { "+-+-"            , NO_SELECTION_ID },
  { "wages"           , INCOME_ID },
  { "cal grant"       , INCOME_ID },
  { "fafsa grant"     , INCOME_ID },
  { "scholarship"     , INCOME_ID },
  { "gas"             , TRANSPORT_ID },
  { "car insurance"   , TRANSPORT_ID },
  { "smog test"       , TRANSPORT_ID },
  { "car tax"         , TRANSPORT_ID },
  { "license renewal" , TRANSPORT_ID },
  { "food"            , FOOD_ID },
  { "snacks"          , FOOD_ID },
  { "big mac"         , FOOD_ID },
  { "cheeseburger"    , FOOD_ID },
  { "eraser"          , WORK_ID },
  { "school supplies" , WORK_ID },
  { "rent"            , HOUSING_ID },
  { "cellphone"       , HOUSING_ID },
  { "electricity"     , HOUSING_ID },
  { "water bill "     , HOUSING_ID },
  // TODO : add more
  { ""                , MAX_ID }
};

int lookup_text( const char *txt_to_lookup ) {
  int ii=0;
  int cat_id=-1;
  int len;
  int dbg = 0;

  for ( ii=0; ii < NR_ENTRIES; ii++ ) {
    len = strlen( txt_to_lookup );

    if ( cat_look_up[ii].cat == MAX_ID ) goto end_of_list;

    if ( strncmp( cat_look_up[ ii ].descrip , txt_to_lookup , len ) == 0 ) {
      cat_id = cat_look_up[ ii ].cat;
      goto end_of_list;
    }
    else {
      if ( dbg ) printf( "  ii=%d , COULDN'T find match for %s \n" , ii, txt_to_lookup );
    }
  }

end_of_list:
  return cat_id;
}

void enter_amount( gpointer *data ) {
  phdl_grp all_hdls = (phdl_grp)data;

  LOG_BLOCK_START ( "  >> E  %s  all_hdls =%p \n" , __func__ , all_hdls );

  LOG_INDENTED ("    Sending acttivate to tp_w_amount ...\n");
  // show
  g_signal_emit_by_name( all_hdls->vbx_hdls->tp_w_amount , "activate", NULL);

  LOG_BLOCK_END ( "  << Lv %s  all_hdls =%p \n" , __func__ , all_hdls );
}

static gboolean key_press_cb(GtkWidget *w, GdkEvent *ev, gpointer data)
{
    GdkEventKey *key = (GdkEventKey*)ev;
    phdl_grp all_hdls = (phdl_grp)data;
    int key_dbg = 0;

    if(key) /* Extra check maybe redundant */
    {
      switch( key->keyval ) {
        case GDK_KEY_Tab:
          if ( key_dbg ) {
            printf(" Tab pressed\n");
            printf(" Enter pressed w=%p  tp_w_amount=%p \n" , w , all_hdls->vbx_hdls->tp_w_amount );
          }
          if ( w == all_hdls->vbx_hdls->tp_w_amount ) {
            // make save record visible, also need to check other items are populated and within range !!
            if ( gtk_widget_get_sensitive(  all_hdls->vbx_hdls->tp_add_record_btn ) == 0 ) {
              //set visible
              gtk_widget_set_sensitive(  all_hdls->vbx_hdls->tp_add_record_btn , TRUE);
              if ( key_dbg ) g_print("Button ENABLED.\n");
            }
          }  // if tp_w_amount
          break;
        case GDK_KEY_Return :
          if ( key_dbg ) {
            printf(" Enter pressed w=%p  tp_w_amount=%p \n" , w , all_hdls->vbx_hdls->tp_w_amount );
          }
          // set focus to amount widget
          if ( w == all_hdls->vbx_hdls->tp_w_amount ) {
            // make save record visible, also need to check other items are populated and within range !!
            if ( gtk_widget_get_sensitive(  all_hdls->vbx_hdls->tp_add_record_btn ) == 0 ) {
              //set visible
              gtk_widget_set_sensitive(  all_hdls->vbx_hdls->tp_add_record_btn , TRUE);
              if ( key_dbg ) g_print("Button ENABLED.\n");
            }
          }  // if tp_w_amount
          break;
        default:
          // dont care
          break;
      }  // switch
   }
    /* Default handling of "Tab" is change of focus.
     * If TRUE is passed here, the event will not be propagated
     */
    return FALSE;
}

static void clear_ALL ( gpointer *data )
{
  phdl_grp all_hdls = (phdl_grp)data;

  LOG_BLOCK_START ( "  >> E  %s  all_hdls =%p \n" , __func__ , all_hdls );

  gtk_entry_set_text ( GTK_ENTRY( all_hdls->vbx_hdls->tp_w_amount ) , "");
  gtk_entry_set_text ( GTK_ENTRY( all_hdls->vbx_hdls->tp_w_description ) , "");
  gtk_combo_box_set_active( GTK_COMBO_BOX ( all_hdls->vbx_hdls->tp_w_category ) , 0 );
  gtk_widget_set_sensitive(  all_hdls->vbx_hdls->tp_add_record_btn , FALSE );
  gtk_widget_set_sensitive(  all_hdls->vbx_hdls->tp_add_dB_btn     , FALSE );

  LOG_BLOCK_END ( "  << Lv %s  all_hdls =%p \n" , __func__ , all_hdls );
}

void on_description_key_press ( GtkWidget *widget, gpointer *data) {
  phdl_grp all_hdls = (phdl_grp)data;

  int dbg=0;
  int match_id = -1;

  if ( dbg ) {
    LOG_BLOCK_START ( " >> E %s data=%p *data=%p \n" , __FUNCTION__ , data , *data );
    LOG_INDENTED ( "  all_hdls = %p \n" ,  all_hdls );
  }
  // get text, lookup description
  const gchar *description_txt;
  description_txt = gtk_entry_get_text( GTK_ENTRY( widget ) );
  if( dbg ) LOG_INDENTED ( "   %s  got text : ->%s<- \n" , __func__ , description_txt);

  match_id = lookup_text( description_txt );
  if ( dbg ) LOG_INDENTED ( "  for str %s , match_id=%2d \n" , description_txt , match_id );
#if 0
	if ( match_id != -1 ) {
    // set category
    gtk_combo_box_set_active( GTK_COMBO_BOX ( all_hdls->vbx_hdls->tp_w_category ) , match_id );
    if ( match_id == INCOME_ID ) {
      gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON( all_hdls->vbx_hdls->tp_w_is_income ) , TRUE );
    }
  }
#endif
  if ( dbg ) LOG_BLOCK_END ( "  << Lv %s \n" , __func__ );
}  // on_description_key_press

static void add_record_clicked ( GtkButton *button,  gpointer   user_data) {
  const gchar *amount_txt;
  const gchar *description_txt;
  int   category = -1;
  float amount   = 0.0;
  static okane_grp this_entry;

  phdl_grp all_hdls = (phdl_grp)user_data;
  pokane_grp head  = (all_hdls->t_lst);

  LOG_BLOCK_START ( "  >> E  %s L%4d   all_hdls =%p \n" , __func__ , __LINE__ , all_hdls );

  // get the data
  amount_txt      = gtk_entry_get_text( GTK_ENTRY( all_hdls->vbx_hdls->tp_w_amount ) );
  description_txt = gtk_entry_get_text( GTK_ENTRY( all_hdls->vbx_hdls->tp_w_description ) );

  category = gtk_combo_box_get_active ( GTK_COMBO_BOX ( all_hdls->vbx_hdls->tp_w_category ) );
  LOG_INDENTED ( "   category = %d \n" , category );

  amount = atof ( amount_txt );

  this_entry.entry_nr  = -1;
  this_entry.category  = category;
  this_entry.entry_ts  = get_unix_time_now();
  this_entry.amount    = amount;
  this_entry.in_dB     = 0;

  strcpy( this_entry.description , description_txt );

  LOG_INDENTED ( "   Got from entry boxes : \n" );
  LOG_INDENTED ( "     category  = %d  \n" , category );
  LOG_INDENTED ( "     amount    = %f  \n" , amount );
  LOG_INDENTED ( "     descr     = %s  \n" , description_txt );

  add_transaction ( &head , &this_entry, 1 );

  if ( 0 ) {
    linked_list_print_okane_grp ( head );
    //getchar();
  }
  // call update list transactions

  add_to_trans_list_treestore( all_hdls );
  gtk_widget_show ( all_hdls->vbx_hdls->tp_add_record_btn );
  gtk_widget_show ( all_hdls->vbx_hdls->tp_add_dB_btn );

  LOG_BLOCK_END ( "  << Lv %s L%4d ,  pall_hdls =%p \n" , __func__ , __LINE__ , all_hdls );
} // add_record_clicked

static void done_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;

  clear_ALL( user_data );

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_transact_page );

  // Go back to home page screen
  gtk_container_add ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );
  gtk_widget_show_all ( all_hdls->parentWin );
}

static void save_db_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;
  pokane_grp head  = (all_hdls->t_lst);

  LOG_BLOCK_START ( "  >> E %s , all_hdls->vbox_transact_page = %p \n" , __func__ , all_hdls->vbox_transact_page );
  // SAVE TO DB !
  save_to_dB_transaction( head , 1 );
  LOG_BLOCK_END ( "  << Lv %s , all_hdls->vbox_transact_page = %p \n" , __func__ , all_hdls->vbox_transact_page );
}

int create_transaction_page( phdl_grp pall_hdls ) {

  int rc = 0;

  if ( pall_hdls != NULL ) {
    if ( pall_hdls->flg->dbg ) {
      LOG_BLOCK_START ( "  >> E %s \n" , __FUNCTION__ );
      LOG_INDENTED ( "    flgs->dbg     = %d \n"   , pall_hdls->flg->dbg );
      LOG_INDENTED ( "    parentWin     = %p \n"   , pall_hdls->parentWin );
      LOG_INDENTED ( "    vbox_transact = %p \n\n" , pall_hdls->vbox_transact_page );
    }
  }
  else {
      LOG_BLOCK_START ( "  >> E %s pall_hdls = NULL  \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_transact_page == NULL ) {
    GtkWidget *frame;
    GtkWidget *vbox;
    GtkWidget *hbox, *hbox2;
    GtkWidget *button;
    GtkWidget *table;
    GtkWidget *label;
    GtkWidget *combo;

    pall_hdls->vbox_transact_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    if ( pall_hdls->vbox_active == NULL ) {
      // only attach, if no vbox active !
      gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_transact_page );
    }
    LOG_INDENTED ( "    vbox = %p   , %s L%4d \n" , pall_hdls->vbox_transact_page , __FILE__ , __LINE__ );

    frame = gtk_frame_new ("Dialogs");
    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_transact_page ), frame);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
    gtk_container_add (GTK_CONTAINER (frame), vbox);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX (vbox), hbox2, FALSE, FALSE, 0);

    table = gtk_grid_new ();
    gtk_grid_set_row_spacing (GTK_GRID (table), 4);
    gtk_grid_set_column_spacing (GTK_GRID (table), 4);
    gtk_box_pack_start (GTK_BOX (hbox), table, FALSE, FALSE, 0);

    /* A combobox with string IDs */
    combo = gtk_combo_box_text_new ();
    g_object_set ( combo, "tooltip-text", "Category of item added - will auto complete if previously entered", NULL);
    pall_hdls->vbx_hdls->tp_w_category = combo;

    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "never"       , "No selection");

    // UPDATE when CATOG_ID changes
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "+ Income");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Housing");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Utilities");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Food");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Transport");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Entertainment");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Health");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Work");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always" , "- Other");

    g_object_set ( combo, "tooltip-text", "Select category of item, eg wages = income, rent, electric = housing", NULL);

    gtk_grid_attach ( GTK_GRID ( table ), combo , 0, 2 , 1 , 1);

    label = gtk_label_new_with_mnemonic ("D_escription");
    gtk_grid_attach (GTK_GRID (table), label, 1, 2, 1, 1);

    entry1 = gtk_entry_new ();
    g_object_set ( entry1, "tooltip-text", "Enter a description of the expense you are entering", NULL);

    gtk_grid_attach (GTK_GRID (table), entry1, 2, 2, 1, 1);
    pall_hdls->vbx_hdls->tp_w_description = entry1;
    g_signal_connect( entry1, "changed", G_CALLBACK( on_description_key_press ), (gpointer)pall_hdls );
    g_signal_connect( entry1, "key-press-event", G_CALLBACK(key_press_cb), (gpointer)pall_hdls );

    entry2 = gtk_entry_new ();
    pall_hdls->vbx_hdls->tp_w_amount = entry2;

    g_signal_connect( entry2, "key-press-event", G_CALLBACK(key_press_cb), (gpointer)pall_hdls );

    gtk_grid_attach (GTK_GRID (table), entry2, 3, 2, 1, 1);
    g_object_set ( entry2, "tooltip-text", "Enter amount of expense, or money received", NULL);
//    gtk_widget_hide ( pall_hdls->vbx_hdls->tp_w_amount  );

    button = gtk_button_new_with_mnemonic ("D_one");
    pall_hdls->vbx_hdls->tp_cancel_btn = button;

    g_signal_connect (button, "clicked", G_CALLBACK ( done_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX (hbox2), button, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic ("Add Record");
    pall_hdls->vbx_hdls->tp_add_record_btn = button;
    gtk_widget_set_sensitive( button, FALSE);     // grey it out

    g_signal_connect ( pall_hdls->vbx_hdls->tp_add_record_btn, "clicked", G_CALLBACK ( add_record_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX (hbox2), button, FALSE, FALSE, 0);

    pall_hdls->vbx_hdls->tp_add_dB_btn = gtk_button_new_with_mnemonic ("SAVE to dB");
    gtk_widget_set_sensitive( pall_hdls->vbx_hdls->tp_add_dB_btn , FALSE);     // grey it out

    g_signal_connect ( pall_hdls->vbx_hdls->tp_add_dB_btn , "clicked", G_CALLBACK ( save_db_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX (hbox2), pall_hdls->vbx_hdls->tp_add_dB_btn, FALSE, FALSE, 0);

    // add ref to widget so it doesn't get destroyed on container_remove !
    g_object_ref ( pall_hdls->vbox_transact_page );

    gtk_widget_show_all ( pall_hdls->vbox_transact_page );

    // need an if on this
    //gtk_widget_hide ( pall_hdls->vbx_hdls->tp_w_amount  );
  } // !if ( pall_hdls->vbox_transact_page == NULL
  else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_transact_page );
    gtk_widget_show_all ( pall_hdls->vbox_transact_page );
  }

  if ( pall_hdls->flg->dbg ) {
    LOG_BLOCK_END ( "  << Lv %s \n" , __FUNCTION__ );
  }

  return rc;
}  //  create_transaction_page

int create_transaction_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  LOG_BLOCK_START ( "  E  %s *all_hdls = %p pall_hdls =%p \n" , __func__ , *all_hdls , pall_hdls );
  rc = create_transaction_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    LOG_INDENTED ( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_transact_page );
  }
  *all_hdls = pall_hdls;

  LOG_BLOCK_END ( "  Lv  %s *all_hdls = %p pall_hdls =%p \n" , __func__ , *all_hdls , pall_hdls );
  return rc;
}
