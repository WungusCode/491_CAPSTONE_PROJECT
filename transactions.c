#include <stdio.h>
#include <string.h>

#include "base_defs.h"

#include "link_list.h"
#include "my_time.h"

#include "database/my_sql.h"

#include "transactions.h"

// copied from database/test_main.c
int create_dummy_data( pokane_grp *head ) {

  okane_grp tmp;
  int lst_len;

  int dbg = 1; // TODO use passed in global dbg

  if ( dbg ) printf( "  >> E %s \n" , __FUNCTION__ );

  lst_len       = get_okane_grp_list_len( *head );
  if ( dbg ) printf( "      list len = %3d  \n" , lst_len );

  tmp.is_income = 1;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 100.0;
  strcpy( tmp.description , "wages" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.is_income = 0;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 20.0;
  strcpy( tmp.description , "gas" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.is_income = 0;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 40.0;
  strcpy( tmp.description , "food" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.is_income = 0;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 30.0;
  strcpy( tmp.description , "cellphone" );
  add_transaction( head , &tmp , 1 );
  lst_len++;

  tmp.is_income = 0;
  tmp.entry_nr  = lst_len;
  tmp.entry_ts  = 0;
  tmp.amount    = 15.0;
  strcpy( tmp.description , "school supplies" );
  add_transaction( head , &tmp , 1 );
  lst_len = get_okane_grp_list_len( *head );
  if ( dbg ) printf( "      list len = %3d  \n" , lst_len );

  if ( dbg ) {
    linked_list_print_okane_grp ( *head );
    getchar();
  }

  if ( dbg ) printf( "  << Lv %s \n" , __FUNCTION__ );

  return 0;
}  // create_dummy_data

int get_data_from_db ( phdl_grp *all_hdls ) {
  int rc = 0;
  sqlite3 *db_hdl;
  phdl_grp pall_hdls;

  pall_hdls = *all_hdls;

  int dbg = pall_hdls->flg->dbg;

  // from database/main_test.c
  db_hdl = db_open( DB_FILE_0 , __FUNCTION__ , 0 ,dbg );
  if ( db_hdl == NULL ) {
    printf( "  DB doesn't exist, create it !!\n\t Ent to cont ! \n" );
    getchar();
    create_new_db( DB_FILE_0 );
    db_hdl = db_open( DB_FILE_0 , __FUNCTION__ , 0 ,dbg );

    create_dummy_data( &pall_hdls->t_lst );
    linked_list_print_okane_grp ( pall_hdls->t_lst );
    printf( "  Ent to cont ! \n" );
    getchar();

    db_add_entry( db_hdl , pall_hdls->t_lst , dbg );
  }
  if ( pall_hdls->t_lst == NULL ) {
    db_read_in_all_transactions( db_hdl , &pall_hdls->t_lst, dbg );

    linked_list_print_okane_grp ( pall_hdls->t_lst );
    if ( dbg ) getchar();
  }

  db_close( db_hdl, __FUNCTION__ , dbg  );

  *all_hdls = pall_hdls;
  return rc;
}


int del_transaction ( pokane_grp *t_lst , pokane_grp data_in, int dbg ) {

  return 0;
}

int add_transaction ( pokane_grp *t_lst , pokane_grp data_in, int dbg ) {

  int rc = 0;
  pokane_grp tmp = NULL;

  tmp = malloc( sizeof( okane_grp ) );
  tmp->next      = NULL;
  tmp->entry_nr  = data_in->entry_nr;    // TODO auto generate this by counting entries in list so far
  tmp->is_income = data_in->is_income;
  tmp->entry_ts  = get_unix_time_now();
  tmp->amount    = data_in->amount;
  strcpy( tmp->description , data_in->description );

  *t_lst = linked_list_add_okane_grp( *t_lst, tmp );

  return rc;
}
