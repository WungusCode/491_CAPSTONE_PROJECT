#ifndef _MY_SQL_H

#define _MY_SQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "../data_types.h"
#include "../link_list.h"

#define DB_FILE_0 "./dev_sekai_1.db"

void set_log_sql_access ( int logit );
int  get_log_sql_access ( void );

int create_new_db( char *dbName );

sqlite3 *db_open( const char * dbName , const char * from_where, int db_nr , int dbg );

int db_close( sqlite3 *db , const char * from_where , int dbg );

int db_add_entry( sqlite3 *db_hdl , pokane_grp data_lst , int dbg );

#endif
