#ifndef _TRANSACTIONS_H

#define _TRANSACTIONS_H 1

extern int get_data_from_db ( phdl_grp *all_hdls );
extern int del_transaction ( pokane_grp *t_lst , pokane_grp data_in, int dbg );
extern int add_transaction ( pokane_grp *t_lst , pokane_grp data_in, int dbg );

#endif
