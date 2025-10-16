#include <stdio.h>
#include <string.h>

#include "base_defs.h"
#include "indent_print.h"

#include "link_list.h"
#include "my_time.h"

#include "database/my_sql.h"

#include "transactions.h"

// copied from database/test_main.c
int create_dummy_data( pokane_grp *head ) {

  okane_grp tmp;
  int lst_len;

  int dbg = 1; // TODO use passed in global dbg

  if ( dbg ) LOG_BLOCK_START ( "  >> E %s \n" , __FUNCTION__ );

  lst_len       = get_okane_grp_list_len( *head );
  if ( dbg ) LOG_INDENTED ( "      list len = %3d  \n" , lst_len );

  tmp.category  = INCOME_ID;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 100.0;
  tmp.in_dB     = 0;
  strcpy( tmp.description , "wages" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.category  = TRANSPORT_ID;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 20.0;
  tmp.in_dB     = 0;
  strcpy( tmp.description , "gas" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.category  = FOOD_ID;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 40.0;
  tmp.in_dB     = 0;
  strcpy( tmp.description , "food" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.category  = HOUSING_ID;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 30.0;
  tmp.in_dB     = 0;
  strcpy( tmp.description , "cellphone" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.category  = WORK_ID;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 15.0;
  tmp.in_dB     = 0;
  strcpy( tmp.description , "school supplies" );
  add_transaction( head , &tmp , 1 );
  lst_len = get_okane_grp_list_len( *head );
  if ( dbg ) LOG_INDENTED ( "      list len = %3d  \n" , lst_len );

  if ( dbg ) {
    linked_list_print_okane_grp ( *head );
  }

  if ( dbg ) LOG_BLOCK_END ( "  << Lv %s \n" , __FUNCTION__ );

  return 0;
}  // create_dummy_data

int get_data_from_db ( phdl_grp *all_hdls ) {
  int rc = 0;
  sqlite3 *db_hdl;
  phdl_grp pall_hdls;

  pall_hdls = *all_hdls;

  int dbg = pall_hdls->flg->dbg;

  if ( dbg ) LOG_BLOCK_START ( ">> E %s \n" , __func__ );

  // from database/main_test.c
  db_hdl = db_open( DB_FILE_0 , __FUNCTION__ , 0 ,dbg );
  if ( db_hdl == NULL ) {
    LOG_INDENTED ( "  DB doesn't exist, create it !!\n\t Ent to cont ! \n" );
    if ( pall_hdls->flg->dbg &&  pall_hdls->flg->autoTest == 0 ) getchar();
    create_new_db( DB_FILE_0 );
    db_hdl = db_open( DB_FILE_0 , __FUNCTION__ , 0 ,dbg );

    create_dummy_data( &pall_hdls->t_lst );
    linked_list_print_okane_grp ( pall_hdls->t_lst );
    if ( !pall_hdls->flg->autoTest ) {
      printf( "  Ent to cont ! \n" );
      getchar();
    }
    db_add_entry( db_hdl , pall_hdls->t_lst , dbg );
  }
  if ( pall_hdls->t_lst == NULL ) {
    db_read_in_all_transactions( db_hdl , &pall_hdls->t_lst, dbg );

    linked_list_print_okane_grp ( pall_hdls->t_lst );
    if ( dbg && pall_hdls->flg->autoTest == 0 ) getchar();
  }

  db_close( db_hdl, __FUNCTION__ , dbg  );

  *all_hdls = pall_hdls;

  if ( dbg ) LOG_BLOCK_END ( "<< Lv %s \n" , __func__ );

  return rc;
}


int del_transaction ( pokane_grp *t_lst , pokane_grp data_in, int dbg ) {

  return 0;
}

int add_transaction ( pokane_grp *t_lst , pokane_grp data_in, int dbg ) {

  int rc          = 0;
  int lst_len     = 0;
  pokane_grp tmp  = NULL;

  if ( data_in->entry_nr == -1 ) {
    // find nr of existing entries and +1 it !
    pokane_grp head = NULL;
    head = *t_lst;
    // find nr entries
    lst_len       = get_okane_grp_list_len( head );
    if ( dbg ) printf( "      list len = %3d  \n" , lst_len );

    data_in->entry_nr = lst_len;
  }

  tmp = malloc( sizeof( okane_grp ) );
  tmp->next      = NULL;
  tmp->entry_nr  = data_in->entry_nr;    // TODO auto generate this by counting entries in list so far
  tmp->category = data_in->category;
  tmp->entry_ts  = get_unix_time_now();
  tmp->amount    = data_in->amount;
  tmp->in_dB     = data_in->in_dB;
  strcpy( tmp->description , data_in->description );

  *t_lst = linked_list_add_okane_grp( *t_lst, tmp );

  return rc;
}

// if pass in a full list it will go through every entry
//   best to pass in pointer from where entries haven't been added to the dB
//   TODO, always pass in head ??
int save_to_dB_transaction ( pokane_grp t_lst , int dbg ) {
  int rc = 0;
  sqlite3 *db_hdl;

  db_hdl = db_open( DB_FILE_0 , __func__ , 0 ,dbg );
  if ( db_hdl != NULL ) {
    if ( dbg ) LOG_INDENTED ( "    %s  , L%4d   db_hdl=%p , call db_add_entry() !  \n" , __func__ , __LINE__ , db_hdl );
    db_add_entry( db_hdl , t_lst , dbg );
    db_close( db_hdl, __FUNCTION__ , dbg  );
  }

  return rc;
}
