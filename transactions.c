#include <stdio.h>
#include <string.h>

#include "base_defs.h"

#include "link_list.h"
#include "my_time.h"

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
