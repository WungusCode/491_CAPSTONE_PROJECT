#ifndef _MY_SQL_H

#define _MY_SQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "../data_types.h"
#include "../link_list.h"

#define DB_FILE_0 "./dev_sekai_1.db"

extern void     set_log_sql_access ( int logit );
extern int      get_log_sql_access ( void );

extern int      create_new_db( char *dbName );

extern sqlite3 *db_open( const char * dbName , const char * from_where, int db_nr , int dbg );

extern int      db_close( sqlite3 *db , const char * from_where , int dbg );

extern int      db_add_entry( sqlite3 *db_hdl , pokane_grp data_lst , int dbg );
extern int      db_read_in_all_transactions( sqlite3 *db_hdl , pokane_grp *data_lst, int dbg );
#endif
