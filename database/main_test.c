#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "../data_types.h"

#include "my_sql.h"

#include "../link_list.h"
#include "../my_time.h"
#include "../transactions.h"

int dbg =1 ;

int create_dummy_data( pokane_grp *head ) {

  okane_grp tmp;
  int lst_len;

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

  if ( 0 /* dbg */ ) {
    linked_list_print_okane_grp ( *head );
    getchar();
  }

  if ( dbg ) printf( "  << Lv %s \n" , __FUNCTION__ );

  return 0;
}

int main(int argc, char* argv[]) {
  int rc  = 0;

  sqlite3 *db_hdl;
  pokane_grp t_act_lst = NULL;

  if ( dbg ) printf( "  >> E %s \n" , __FUNCTION__ );

  set_log_sql_access( 1 );
  // nothing for now
  db_hdl = db_open( DB_FILE_0 , __FUNCTION__ , 0 ,dbg );
  if ( db_hdl == NULL ) {
    printf( "  DB doesn't exist, create it !!\n\t Ent to cont ! \n" );
    getchar();
    create_new_db( DB_FILE_0 );
    db_hdl = db_open( DB_FILE_0 , __FUNCTION__ , 0 ,dbg );
  }

  create_dummy_data( &t_act_lst );
  linked_list_print_okane_grp ( t_act_lst );
  printf( "  Ent to cont ! \n" );
  getchar();

  db_add_entry( db_hdl , t_act_lst , dbg );

  db_close( db_hdl, __FUNCTION__ , dbg  );

  if ( dbg ) printf( "  << Lv %s \n" , __FUNCTION__ );

  return rc;
}
